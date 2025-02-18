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

    #define das_yyparse fmt_yyparse // We can't change flex prefix, since we have only one lexer file, seems like to define is the best way

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
    #include "../src/parser/lex.yy.h"

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
  YYSYMBOL_OPEN_BRACE = 163,               /* OPEN_BRACE  */
  YYSYMBOL_CLOSE_BRACE = 164,              /* CLOSE_BRACE  */
  YYSYMBOL_SEMICOLON = 165,                /* SEMICOLON  */
  YYSYMBOL_INTEGER = 166,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 167,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 168,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 169,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 170,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 171,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 172,                   /* "double constant"  */
  YYSYMBOL_NAME = 173,                     /* "name"  */
  YYSYMBOL_KEYWORD = 174,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 175,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 176,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 177,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 178,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 179,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 180,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 181,          /* "}"  */
  YYSYMBOL_END_OF_READ = 182,              /* "end of failed eader macro"  */
  YYSYMBOL_183_begin_of_code_block_ = 183, /* "begin of code block"  */
  YYSYMBOL_184_end_of_code_block_ = 184,   /* "end of code block"  */
  YYSYMBOL_185_end_of_expression_ = 185,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 186,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 187,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 188,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 189,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 190,            /* ",}]"  */
  YYSYMBOL_191_ = 191,                     /* ','  */
  YYSYMBOL_192_ = 192,                     /* '='  */
  YYSYMBOL_193_ = 193,                     /* '?'  */
  YYSYMBOL_194_ = 194,                     /* ':'  */
  YYSYMBOL_195_ = 195,                     /* '|'  */
  YYSYMBOL_196_ = 196,                     /* '^'  */
  YYSYMBOL_197_ = 197,                     /* '&'  */
  YYSYMBOL_198_ = 198,                     /* '<'  */
  YYSYMBOL_199_ = 199,                     /* '>'  */
  YYSYMBOL_200_ = 200,                     /* '-'  */
  YYSYMBOL_201_ = 201,                     /* '+'  */
  YYSYMBOL_202_ = 202,                     /* '*'  */
  YYSYMBOL_203_ = 203,                     /* '/'  */
  YYSYMBOL_204_ = 204,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 205,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 206,               /* UNARY_PLUS  */
  YYSYMBOL_207_ = 207,                     /* '~'  */
  YYSYMBOL_208_ = 208,                     /* '!'  */
  YYSYMBOL_PRE_INC = 209,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 210,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 211,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 212,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 213,                    /* DEREF  */
  YYSYMBOL_214_ = 214,                     /* '.'  */
  YYSYMBOL_215_ = 215,                     /* '['  */
  YYSYMBOL_216_ = 216,                     /* ']'  */
  YYSYMBOL_217_ = 217,                     /* '('  */
  YYSYMBOL_218_ = 218,                     /* ')'  */
  YYSYMBOL_219_ = 219,                     /* '$'  */
  YYSYMBOL_220_ = 220,                     /* '@'  */
  YYSYMBOL_221_ = 221,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 222,                 /* $accept  */
  YYSYMBOL_program = 223,                  /* program  */
  YYSYMBOL_semicolon = 224,                /* semicolon  */
  YYSYMBOL_top_level_reader_macro = 225,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 226, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 227,              /* module_name  */
  YYSYMBOL_module_declaration = 228,       /* module_declaration  */
  YYSYMBOL_character_sequence = 229,       /* character_sequence  */
  YYSYMBOL_string_constant = 230,          /* string_constant  */
  YYSYMBOL_string_builder_body = 231,      /* string_builder_body  */
  YYSYMBOL_string_builder = 232,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 233, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 234,              /* expr_reader  */
  YYSYMBOL_235_1 = 235,                    /* $@1  */
  YYSYMBOL_options_declaration = 236,      /* options_declaration  */
  YYSYMBOL_require_declaration = 237,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 238,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 239,      /* require_module_name  */
  YYSYMBOL_require_module = 240,           /* require_module  */
  YYSYMBOL_is_public_module = 241,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 242,       /* expect_declaration  */
  YYSYMBOL_expect_list = 243,              /* expect_list  */
  YYSYMBOL_expect_error = 244,             /* expect_error  */
  YYSYMBOL_expression_label = 245,         /* expression_label  */
  YYSYMBOL_expression_goto = 246,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 247,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 248,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 249,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 250, /* expression_else_one_liner  */
  YYSYMBOL_251_2 = 251,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 252,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 253,  /* expression_if_then_else  */
  YYSYMBOL_254_3 = 254,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 255,      /* expression_for_loop  */
  YYSYMBOL_256_4 = 256,                    /* $@4  */
  YYSYMBOL_257_5 = 257,                    /* $@5  */
  YYSYMBOL_expression_unsafe = 258,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 259,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 260,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 261,    /* expression_with_alias  */
  YYSYMBOL_262_6 = 262,                    /* $@6  */
  YYSYMBOL_annotation_argument_value = 263, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 264, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 265, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 266,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 267, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 268,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 269, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 270, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 271,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 272,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 273, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 274, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 275,   /* optional_function_type  */
  YYSYMBOL_function_name = 276,            /* function_name  */
  YYSYMBOL_global_function_declaration = 277, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 278, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 279, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 280,     /* function_declaration  */
  YYSYMBOL_281_7 = 281,                    /* $@7  */
  YYSYMBOL_open_block = 282,               /* open_block  */
  YYSYMBOL_close_block = 283,              /* close_block  */
  YYSYMBOL_expression_block = 284,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 285,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 286,           /* expression_any  */
  YYSYMBOL_expressions = 287,              /* expressions  */
  YYSYMBOL_expr_keyword = 288,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 289,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 290, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 291, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 292, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 293,       /* expression_keyword  */
  YYSYMBOL_294_8 = 294,                    /* $@8  */
  YYSYMBOL_295_9 = 295,                    /* $@9  */
  YYSYMBOL_296_10 = 296,                   /* $@10  */
  YYSYMBOL_297_11 = 297,                   /* $@11  */
  YYSYMBOL_expr_pipe = 298,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 299,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 300,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 301,     /* new_type_declaration  */
  YYSYMBOL_302_12 = 302,                   /* $@12  */
  YYSYMBOL_303_13 = 303,                   /* $@13  */
  YYSYMBOL_expr_new = 304,                 /* expr_new  */
  YYSYMBOL_expression_break = 305,         /* expression_break  */
  YYSYMBOL_expression_continue = 306,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 307, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 308,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 309, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 310,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 311,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 312,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 313,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 314,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 315,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 316, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 317,           /* expression_let  */
  YYSYMBOL_expr_cast = 318,                /* expr_cast  */
  YYSYMBOL_319_14 = 319,                   /* $@14  */
  YYSYMBOL_320_15 = 320,                   /* $@15  */
  YYSYMBOL_321_16 = 321,                   /* $@16  */
  YYSYMBOL_322_17 = 322,                   /* $@17  */
  YYSYMBOL_323_18 = 323,                   /* $@18  */
  YYSYMBOL_324_19 = 324,                   /* $@19  */
  YYSYMBOL_expr_type_decl = 325,           /* expr_type_decl  */
  YYSYMBOL_326_20 = 326,                   /* $@20  */
  YYSYMBOL_327_21 = 327,                   /* $@21  */
  YYSYMBOL_expr_type_info = 328,           /* expr_type_info  */
  YYSYMBOL_expr_list = 329,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 330,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 331,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 332,            /* capture_entry  */
  YYSYMBOL_capture_list = 333,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 334,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 335,               /* expr_block  */
  YYSYMBOL_expr_full_block = 336,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 337, /* expr_full_block_assumed_piped  */
  YYSYMBOL_338_22 = 338,                   /* $@22  */
  YYSYMBOL_expr_numeric_const = 339,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 340,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 341,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 342,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 343,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 344,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 345,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 346,           /* func_addr_expr  */
  YYSYMBOL_347_23 = 347,                   /* $@23  */
  YYSYMBOL_348_24 = 348,                   /* $@24  */
  YYSYMBOL_349_25 = 349,                   /* $@25  */
  YYSYMBOL_350_26 = 350,                   /* $@26  */
  YYSYMBOL_expr_field = 351,               /* expr_field  */
  YYSYMBOL_352_27 = 352,                   /* $@27  */
  YYSYMBOL_353_28 = 353,                   /* $@28  */
  YYSYMBOL_expr_call = 354,                /* expr_call  */
  YYSYMBOL_expr = 355,                     /* expr  */
  YYSYMBOL_356_29 = 356,                   /* $@29  */
  YYSYMBOL_357_30 = 357,                   /* $@30  */
  YYSYMBOL_358_31 = 358,                   /* $@31  */
  YYSYMBOL_359_32 = 359,                   /* $@32  */
  YYSYMBOL_360_33 = 360,                   /* $@33  */
  YYSYMBOL_361_34 = 361,                   /* $@34  */
  YYSYMBOL_expr_mtag = 362,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 363, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 364,        /* optional_override  */
  YYSYMBOL_optional_constant = 365,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 366, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 367, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 368, /* structure_variable_declaration  */
  YYSYMBOL_opt_sem = 369,                  /* opt_sem  */
  YYSYMBOL_struct_variable_declaration_list = 370, /* struct_variable_declaration_list  */
  YYSYMBOL_371_35 = 371,                   /* $@35  */
  YYSYMBOL_372_36 = 372,                   /* $@36  */
  YYSYMBOL_373_37 = 373,                   /* $@37  */
  YYSYMBOL_function_argument_declaration = 374, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 375,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 376,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 377,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 378,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 379,             /* variant_type  */
  YYSYMBOL_variant_type_list = 380,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 381,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 382,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 383,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 384,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 385,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 386, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 387, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 388, /* global_variable_declaration_list  */
  YYSYMBOL_389_38 = 389,                   /* $@38  */
  YYSYMBOL_optional_shared = 390,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 391, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 392,               /* global_let  */
  YYSYMBOL_393_39 = 393,                   /* $@39  */
  YYSYMBOL_enum_list = 394,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 395, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 396,             /* single_alias  */
  YYSYMBOL_397_40 = 397,                   /* $@40  */
  YYSYMBOL_alias_list = 398,               /* alias_list  */
  YYSYMBOL_alias_declaration = 399,        /* alias_declaration  */
  YYSYMBOL_400_41 = 400,                   /* $@41  */
  YYSYMBOL_optional_public_or_private_enum = 401, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 402,                /* enum_name  */
  YYSYMBOL_enum_declaration = 403,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 404, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 405,          /* optional_sealed  */
  YYSYMBOL_structure_name = 406,           /* structure_name  */
  YYSYMBOL_class_or_struct = 407,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 408, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 409, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 410,    /* structure_declaration  */
  YYSYMBOL_411_42 = 411,                   /* $@42  */
  YYSYMBOL_412_43 = 412,                   /* $@43  */
  YYSYMBOL_variable_name_with_pos_list = 413, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 414,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 415, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 416, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 417,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 418,            /* bitfield_bits  */
  YYSYMBOL_commas = 419,                   /* commas  */
  YYSYMBOL_bitfield_alias_bits = 420,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 421, /* bitfield_type_declaration  */
  YYSYMBOL_422_44 = 422,                   /* $@44  */
  YYSYMBOL_423_45 = 423,                   /* $@45  */
  YYSYMBOL_c_or_s = 424,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 425,          /* table_type_pair  */
  YYSYMBOL_dim_list = 426,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 427, /* type_declaration_no_options  */
  YYSYMBOL_428_46 = 428,                   /* $@46  */
  YYSYMBOL_429_47 = 429,                   /* $@47  */
  YYSYMBOL_430_48 = 430,                   /* $@48  */
  YYSYMBOL_431_49 = 431,                   /* $@49  */
  YYSYMBOL_432_50 = 432,                   /* $@50  */
  YYSYMBOL_433_51 = 433,                   /* $@51  */
  YYSYMBOL_434_52 = 434,                   /* $@52  */
  YYSYMBOL_435_53 = 435,                   /* $@53  */
  YYSYMBOL_436_54 = 436,                   /* $@54  */
  YYSYMBOL_437_55 = 437,                   /* $@55  */
  YYSYMBOL_438_56 = 438,                   /* $@56  */
  YYSYMBOL_439_57 = 439,                   /* $@57  */
  YYSYMBOL_440_58 = 440,                   /* $@58  */
  YYSYMBOL_441_59 = 441,                   /* $@59  */
  YYSYMBOL_442_60 = 442,                   /* $@60  */
  YYSYMBOL_443_61 = 443,                   /* $@61  */
  YYSYMBOL_444_62 = 444,                   /* $@62  */
  YYSYMBOL_445_63 = 445,                   /* $@63  */
  YYSYMBOL_446_64 = 446,                   /* $@64  */
  YYSYMBOL_447_65 = 447,                   /* $@65  */
  YYSYMBOL_448_66 = 448,                   /* $@66  */
  YYSYMBOL_449_67 = 449,                   /* $@67  */
  YYSYMBOL_450_68 = 450,                   /* $@68  */
  YYSYMBOL_451_69 = 451,                   /* $@69  */
  YYSYMBOL_452_70 = 452,                   /* $@70  */
  YYSYMBOL_453_71 = 453,                   /* $@71  */
  YYSYMBOL_454_72 = 454,                   /* $@72  */
  YYSYMBOL_type_declaration = 455,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 456,  /* tuple_alias_declaration  */
  YYSYMBOL_457_73 = 457,                   /* $@73  */
  YYSYMBOL_458_74 = 458,                   /* $@74  */
  YYSYMBOL_459_75 = 459,                   /* $@75  */
  YYSYMBOL_460_76 = 460,                   /* $@76  */
  YYSYMBOL_variant_alias_declaration = 461, /* variant_alias_declaration  */
  YYSYMBOL_462_77 = 462,                   /* $@77  */
  YYSYMBOL_463_78 = 463,                   /* $@78  */
  YYSYMBOL_464_79 = 464,                   /* $@79  */
  YYSYMBOL_465_80 = 465,                   /* $@80  */
  YYSYMBOL_bitfield_alias_declaration = 466, /* bitfield_alias_declaration  */
  YYSYMBOL_467_81 = 467,                   /* $@81  */
  YYSYMBOL_468_82 = 468,                   /* $@82  */
  YYSYMBOL_make_decl = 469,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 470,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 471,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 472,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 473,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 474,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 475,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 476, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 477,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 478, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 479, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 480, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 481, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 482, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 483,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 484,         /* make_struct_decl  */
  YYSYMBOL_485_83 = 485,                   /* $@83  */
  YYSYMBOL_486_84 = 486,                   /* $@84  */
  YYSYMBOL_487_85 = 487,                   /* $@85  */
  YYSYMBOL_488_86 = 488,                   /* $@86  */
  YYSYMBOL_489_87 = 489,                   /* $@87  */
  YYSYMBOL_490_88 = 490,                   /* $@88  */
  YYSYMBOL_491_89 = 491,                   /* $@89  */
  YYSYMBOL_492_90 = 492,                   /* $@90  */
  YYSYMBOL_make_tuple = 493,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 494,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 495,          /* make_tuple_call  */
  YYSYMBOL_496_91 = 496,                   /* $@91  */
  YYSYMBOL_497_92 = 497,                   /* $@92  */
  YYSYMBOL_make_dim = 498,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 499,            /* make_dim_decl  */
  YYSYMBOL_500_93 = 500,                   /* $@93  */
  YYSYMBOL_501_94 = 501,                   /* $@94  */
  YYSYMBOL_502_95 = 502,                   /* $@95  */
  YYSYMBOL_503_96 = 503,                   /* $@96  */
  YYSYMBOL_504_97 = 504,                   /* $@97  */
  YYSYMBOL_505_98 = 505,                   /* $@98  */
  YYSYMBOL_506_99 = 506,                   /* $@99  */
  YYSYMBOL_507_100 = 507,                  /* $@100  */
  YYSYMBOL_508_101 = 508,                  /* $@101  */
  YYSYMBOL_509_102 = 509,                  /* $@102  */
  YYSYMBOL_make_table = 510,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 511,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 512,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 513, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 514,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 515       /* array_comprehension  */
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
#define YYLAST   14956

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  294
/* YYNRULES -- Number of rules.  */
#define YYNRULES  904
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1746

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   449


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
       2,     2,     2,   208,     2,   221,   219,   204,   197,     2,
     217,   218,   202,   201,   191,   200,   214,   203,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   194,   185,
     198,   192,   199,   193,   220,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   215,     2,   216,   196,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   183,   195,   184,   207,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   186,   187,
     188,   189,   190,   205,   206,   209,   210,   211,   212,   213
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   564,   564,   565,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,   581,   582,   586,   589,   592,
     598,   599,   600,   604,   605,   609,   627,   628,   629,   630,
     634,   635,   639,   644,   653,   661,   677,   682,   690,   690,
     731,   761,   765,   766,   767,   771,   774,   778,   784,   793,
     796,   802,   803,   807,   811,   812,   816,   819,   825,   831,
     834,   840,   841,   845,   846,   847,   861,   862,   866,   867,
     867,   873,   874,   875,   876,   877,   881,   892,   892,   900,
     900,   904,   904,   913,   921,   933,   943,   943,   950,   951,
     952,   953,   954,   955,   959,   964,   972,   973,   974,   978,
     979,   980,   981,   982,   983,   984,   985,   991,   994,  1000,
    1003,  1006,  1012,  1013,  1014,  1015,  1019,  1032,  1050,  1053,
    1061,  1072,  1083,  1094,  1097,  1104,  1108,  1115,  1116,  1120,
    1121,  1122,  1126,  1129,  1136,  1140,  1141,  1142,  1143,  1144,
    1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,
    1155,  1156,  1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,
    1165,  1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,
    1175,  1176,  1177,  1178,  1179,  1180,  1181,  1182,  1183,  1184,
    1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,
    1195,  1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,
    1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,
    1215,  1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,  1228,
    1246,  1247,  1248,  1252,  1258,  1258,  1275,  1276,  1279,  1280,
    1283,  1290,  1314,  1323,  1332,  1338,  1339,  1340,  1341,  1342,
    1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
    1353,  1354,  1355,  1356,  1357,  1358,  1359,  1363,  1368,  1374,
    1380,  1391,  1392,  1396,  1397,  1401,  1402,  1406,  1410,  1417,
    1417,  1417,  1423,  1423,  1423,  1432,  1466,  1469,  1472,  1475,
    1481,  1482,  1493,  1497,  1500,  1508,  1508,  1508,  1511,  1517,
    1520,  1524,  1528,  1535,  1542,  1548,  1552,  1556,  1559,  1562,
    1570,  1573,  1576,  1584,  1587,  1595,  1598,  1601,  1609,  1621,
    1622,  1623,  1627,  1628,  1632,  1633,  1637,  1642,  1650,  1661,
    1667,  1682,  1694,  1697,  1703,  1703,  1703,  1706,  1706,  1706,
    1711,  1711,  1711,  1719,  1719,  1719,  1725,  1739,  1755,  1773,
    1783,  1794,  1809,  1812,  1818,  1819,  1826,  1837,  1838,  1839,
    1843,  1844,  1845,  1846,  1847,  1851,  1856,  1864,  1865,  1875,
    1879,  1889,  1896,  1903,  1903,  1912,  1913,  1914,  1915,  1916,
    1917,  1918,  1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,
    1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1938,  1939,
    1940,  1944,  1945,  1946,  1947,  1952,  1953,  1954,  1955,  1956,
    1957,  1958,  1959,  1960,  1961,  1962,  1963,  1964,  1965,  1966,
    1967,  1968,  1973,  1980,  1992,  1997,  2007,  2011,  2018,  2021,
    2021,  2021,  2026,  2026,  2026,  2039,  2043,  2047,  2052,  2059,
    2068,  2073,  2080,  2080,  2080,  2087,  2091,  2101,  2110,  2119,
    2123,  2126,  2132,  2133,  2134,  2135,  2136,  2137,  2138,  2139,
    2140,  2141,  2142,  2143,  2144,  2145,  2146,  2147,  2148,  2149,
    2150,  2151,  2152,  2153,  2154,  2155,  2156,  2157,  2158,  2159,
    2160,  2161,  2162,  2163,  2164,  2165,  2166,  2167,  2173,  2174,
    2175,  2176,  2177,  2191,  2192,  2193,  2194,  2195,  2196,  2197,
    2198,  2199,  2200,  2201,  2202,  2205,  2208,  2209,  2212,  2212,
    2212,  2215,  2220,  2224,  2228,  2228,  2228,  2233,  2236,  2240,
    2240,  2240,  2245,  2248,  2249,  2250,  2251,  2252,  2253,  2254,
    2255,  2256,  2258,  2262,  2270,  2275,  2279,  2280,  2281,  2282,
    2283,  2284,  2285,  2289,  2293,  2297,  2301,  2305,  2309,  2313,
    2317,  2321,  2328,  2329,  2338,  2342,  2343,  2344,  2348,  2349,
    2353,  2354,  2355,  2359,  2360,  2364,  2375,  2376,  2379,  2382,
    2382,  2401,  2400,  2416,  2415,  2429,  2438,  2447,  2457,  2458,
    2462,  2465,  2474,  2475,  2479,  2482,  2485,  2501,  2510,  2511,
    2515,  2518,  2521,  2535,  2536,  2540,  2546,  2552,  2555,  2559,
    2565,  2574,  2575,  2576,  2580,  2581,  2585,  2592,  2597,  2606,
    2612,  2623,  2626,  2631,  2636,  2644,  2655,  2658,  2658,  2678,
    2679,  2683,  2684,  2685,  2689,  2696,  2696,  2715,  2718,  2733,
    2752,  2753,  2754,  2759,  2759,  2789,  2792,  2799,  2809,  2809,
    2813,  2814,  2815,  2819,  2829,  2849,  2872,  2873,  2877,  2878,
    2882,  2888,  2889,  2890,  2891,  2895,  2896,  2897,  2901,  2904,
    2915,  2920,  2915,  2940,  2947,  2952,  2961,  2967,  2978,  2979,
    2980,  2981,  2982,  2983,  2984,  2985,  2986,  2987,  2988,  2989,
    2990,  2991,  2992,  2993,  2994,  2995,  2996,  2997,  2998,  2999,
    3000,  3001,  3002,  3003,  3004,  3008,  3009,  3010,  3011,  3012,
    3013,  3014,  3015,  3019,  3030,  3034,  3041,  3053,  3060,  3070,
    3071,  3076,  3081,  3084,  3099,  3112,  3112,  3112,  3125,  3126,
    3130,  3134,  3141,  3145,  3152,  3153,  3154,  3155,  3156,  3171,
    3177,  3177,  3177,  3181,  3186,  3193,  3193,  3200,  3204,  3208,
    3213,  3218,  3223,  3228,  3232,  3236,  3241,  3245,  3249,  3254,
    3254,  3254,  3260,  3267,  3267,  3267,  3272,  3272,  3272,  3278,
    3278,  3278,  3283,  3287,  3287,  3287,  3292,  3292,  3292,  3301,
    3305,  3305,  3305,  3310,  3310,  3310,  3319,  3323,  3323,  3323,
    3328,  3328,  3328,  3337,  3337,  3337,  3343,  3343,  3343,  3352,
    3355,  3366,  3382,  3382,  3387,  3396,  3382,  3425,  3425,  3430,
    3440,  3425,  3469,  3469,  3469,  3512,  3513,  3514,  3515,  3516,
    3520,  3527,  3534,  3540,  3546,  3553,  3560,  3566,  3575,  3581,
    3589,  3594,  3601,  3606,  3613,  3618,  3624,  3625,  3629,  3630,
    3635,  3636,  3640,  3641,  3645,  3646,  3650,  3651,  3652,  3656,
    3657,  3658,  3662,  3663,  3667,  3696,  3736,  3755,  3775,  3795,
    3816,  3816,  3816,  3824,  3824,  3824,  3831,  3831,  3831,  3838,
    3838,  3838,  3849,  3853,  3859,  3875,  3881,  3887,  3893,  3893,
    3893,  3903,  3908,  3915,  3924,  3948,  3972,  3972,  3972,  3982,
    3982,  3982,  3992,  3992,  3992,  4002,  4011,  4011,  4011,  4031,
    4038,  4038,  4038,  4048,  4053,  4060,  4063,  4069,  4077,  4096,
    4104,  4124,  4149,  4150,  4154,  4155,  4160,  4170,  4173,  4176,
    4179,  4187,  4196,  4208,  4218
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
  "\"[{\"", "\"{{\"", "OPEN_BRACE", "CLOSE_BRACE", "SEMICOLON",
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
  "'$'", "'@'", "'#'", "$accept", "program", "semicolon",
  "top_level_reader_macro", "optional_public_or_private_module",
  "module_name", "module_declaration", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@1", "options_declaration",
  "require_declaration", "keyword_or_name", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_else_one_liner", "$@2", "expression_if_one_liner",
  "expression_if_then_else", "$@3", "expression_for_loop", "$@4", "$@5",
  "expression_unsafe", "expression_while_loop", "expression_with",
  "expression_with_alias", "$@6", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@7", "open_block", "close_block",
  "expression_block", "expr_call_pipe", "expression_any", "expressions",
  "expr_keyword", "optional_expr_list", "optional_expr_list_in_braces",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "expression_keyword", "$@8", "$@9", "$@10", "$@11", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@12",
  "$@13", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@14", "$@15", "$@16", "$@17", "$@18",
  "$@19", "expr_type_decl", "$@20", "$@21", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@22", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe_right", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@23", "$@24", "$@25", "$@26", "expr_field", "$@27",
  "$@28", "expr_call", "expr", "$@29", "$@30", "$@31", "$@32", "$@33",
  "$@34", "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "opt_sem", "struct_variable_declaration_list", "$@35", "$@36", "$@37",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@38", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@39", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@40", "alias_list",
  "alias_declaration", "$@41", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "optional_structure_parent",
  "optional_sealed", "structure_name", "class_or_struct",
  "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@42", "$@43", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "commas",
  "bitfield_alias_bits", "bitfield_type_declaration", "$@44", "$@45",
  "c_or_s", "table_type_pair", "dim_list", "type_declaration_no_options",
  "$@46", "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53", "$@54",
  "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63",
  "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71", "$@72",
  "type_declaration", "tuple_alias_declaration", "$@73", "$@74", "$@75",
  "$@76", "variant_alias_declaration", "$@77", "$@78", "$@79", "$@80",
  "bitfield_alias_declaration", "$@81", "$@82", "make_decl",
  "make_struct_fields", "make_variant_dim", "make_struct_single",
  "make_struct_dim", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "optional_block",
  "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@83", "$@84", "$@85", "$@86", "$@87", "$@88", "$@89", "$@90",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@91", "$@92",
  "make_dim", "make_dim_decl", "$@93", "$@94", "$@95", "$@96", "$@97",
  "$@98", "$@99", "$@100", "$@101", "$@102", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1456)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-771)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1456,   120, -1456, -1456,    32,  -123,   297,   459, -1456,    84,
     561,   561,   561, -1456, -1456,   156,    83, -1456, -1456, -1456,
     397, -1456, -1456, -1456,   265, -1456,   -12, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456,  -133, -1456,   -92,
     -19,   -68, -1456, -1456, -1456, -1456,   297, -1456,    27, -1456,
   -1456, -1456,   561,   561, -1456, -1456,   -12, -1456, -1456, -1456,
   -1456, -1456,    40,    73, -1456, -1456, -1456, -1456,    83,    83,
      83,    10, -1456,   833,   176, -1456, -1456, -1456, -1456,   566,
     782,   856,   803, -1456,   806,    38,    32,   141,  -123,    25,
     101, -1456,   807,   807, -1456,   193,   397,    17,   397,   808,
     227,   291,   390, -1456,   413,   266, -1456, -1456,    -3,    32,
      83,    83,    83,    83, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456,   454, -1456, -1456, -1456, -1456, -1456, -1456, -1456,   459,
   -1456, -1456, -1456, -1456, -1456,   830,   142, -1456, -1456, -1456,
   -1456,   609, -1456, -1456, -1456,   351, -1456, -1456, -1456,   397,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,   513,
   -1456,   -82, -1456,   426,   574,   833, 14783, -1456,   244,   634,
   -1456,  -113, -1456, -1456, -1456,   844, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456,   319, -1456,   531, -1456, -1456,   351,   459,
     459,   459, -1456, -1456, 13609, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
     740,   745, -1456,   590,   459,   695, -1456, -1456,   636, -1456,
     476,    32,    32,   -87,   212, -1456, -1456, -1456,   142, -1456,
   10500, -1456, -1456, -1456, -1456,   700,   702, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456,   720,   633, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456,   873, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456,   725,   689, -1456, -1456,   216,   716,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
     459,   498,   723,   459, -1456,  -113,   240, -1456,    32, -1456,
     705,   881,   660, -1456, -1456,   744,   749,   756,   739,   776,
     788, -1456, -1456, -1456,   778, -1456, -1456, -1456, -1456, -1456,
     798, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456,   801, -1456, -1456, -1456,   812,   816, -1456, -1456,
   -1456, -1456,   819,   821,   794,   156, -1456, -1456, -1456, -1456,
   -1456,    31,   810,   612, -1456, -1456, -1456, -1456, -1456, -1456,
     851,   888, -1456,   809, -1456,    97, -1456,   131, 10500, -1456,
    2755, -1456,   361, -1456,   156, -1456, -1456, -1456,   212,   811,
   -1456,  9774,   855,   859, 10500, -1456,   -62, -1456, -1456, -1456,
    9774, -1456, -1456,   866, -1456,   585,   598,   607, -1456, -1456,
    9774,   179, -1456, -1456, -1456,    15, -1456, -1456, -1456,    39,
    5856, -1456,   814, 10248, -1456,   879,   517, 10351,   613, -1456,
   -1456,  9774, -1456, -1456,   276, -1456,   117,   810, -1456,   852,
     853,  9774, -1456, -1456,   832, -1456, -1456,   962,   -80,   854,
      36,  3582, -1456, -1456,   459,   570,  6062,   837,  9774,   884,
     860,   861,   843, -1456,   397,   864,   902,  6268,   106,   583,
     874, -1456,   614,   875,   876,  3788,  9774,  9774,   299,   299,
     299,   858,   862,   867,   869,   872,   878, -1456,  2307, 10145,
    6476, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,  6682,
     880, -1456,  1063, -1456,  9774,  9774,  9774,  9774,  9774,  5444,
    9774, -1456,   857, -1456, -1456,   397,   397,  9774,  1071, -1456,
   -1456, -1456, -1456, -1456, -1456,  6890,  1055, -1456, -1456, -1456,
     752, -1456,   102,   397, -1456,   397,   397,   397, -1456,   397,
   -1456, -1456,  1021, -1456, -1456, -1456, -1456,   883, -1456, -1456,
     237, -1456, -1456, -1456, -1456, -1456, -1456,  2108, -1456,   882,
   -1456, -1456, -1456, -1456, -1456, -1456,   498,  9774, -1456, -1456,
      34,   351, -1456,   885,   895,   898, -1456,  2490, -1456,  1060,
     428, -1456, -1456, -1456,  4200, 10500, 10500, 10500, 10843, 10500,
   10500,   889,   953, 10500,   590, 10500,   590, 10500,   590, 10603,
     954, 10954, -1456,  9774, -1456, -1456, -1456, -1456,   912, -1456,
   -1456, 13168,  9774, -1456,    31, -1456,   938, -1456, -1456,    86,
   -1456, -1456,   337, -1456,   810,   476,   936,   337, -1456,   476,
   10992,   916,  1091, -1456,   136, -1456, -1456, -1456, 13651,   276,
     919, -1456,   923, -1456, -1456,   156,   623, -1456,   943,   948,
     949, -1456,  9774,  4200, -1456,   957,  1017, 10808,  1136, 10500,
    9774,  9774, 14138,  9774, 13651,   960, -1456, -1456,  9774, -1456,
   -1456,   959,   988, 14138,  9774, -1456, -1456,  9774, -1456, -1456,
    9774, -1456, 10500,  4200, -1456, 10808,   557,   557,   937, -1456,
     883, -1456, -1456, -1456,  9774,  9774,  9774,  9774,  9774,  9774,
     276,  2961,   276,  3168,   276, 13837, -1456,   643, -1456, 13651,
   -1456,   657,   941,   557,   557,   -41,   557,   557,   -73,  1146,
     972, 14138,   972,   338, -1456, -1456, 13651, -1456,   276, -1456,
     974,   459, -1456, -1456, -1456,  4406, -1456, -1456, -1456, -1456,
   -1456, -1456,   -20,    42,   299, -1456, 14560, 14591,  4612,  4612,
    4612,  4612,  4612,  4612,  4612,  4612,  9774,  9774, -1456, -1456,
    9774,  4612,  4612,  9774,  9774,  9774,   991,  4612,  9774,    26,
    9774,  9774,  9774,  9774,  9774,  9774,  4612,  4612,  9774,  9774,
    9774,  4612,  4612,  4612,  9774,  4612,  7096,  9774,  9774,  9774,
    9774,  9774,  9774,  9774,  9774,  9774,  9774, 14721,  9774, -1456,
    7302, -1456, 13877,    83,  1158, -1456,  -113, -1456, 10500, -1456,
     995, -1456,  4200, -1456, 10687,   126,   164,   970,   494, -1456,
     270,   679, -1456, -1456,   468,   741,   716,   742,   716,   753,
     716, -1456,   344, -1456,   382, -1456, 10500,   952,   972, -1456,
   -1456, 13203, 10500, -1456, -1456, 10500, -1456, -1456, -1456,  9774,
     998, -1456,  1000, -1456, 10500, -1456,  4200, 10500, 10500, -1456,
      19,   276, 10500,  5650,  7508,  1002,  9774, 10500, -1456, -1456,
   -1456, 10500,   972, -1456,   957,  9774,  9774,  9774,  9774,  9774,
    9774,  9774,  9774,  9774,  9774,  9774,  9774,  9774,  9774,  9774,
    9774,  9774,  9774,   459,  1050,   958,   974, 14138, 11103, -1456,
   -1456, 10500, 10500, 11138, 10500, -1456, -1456, 11249, 10500,   972,
   10500, 10500,   972, 10500,   617, -1456, 10808, -1456,    42, 11287,
   11398, 11433, 11544, 11582, 11693,    46,   299,   963,    -9,  3375,
    4820,  7714, 13970,   990,     4,   342,   993,   185,    47,  7920,
       4,   541,    51,  9774,  1001, -1456,  9774, -1456, 10500, -1456,
   10500, -1456,  9774,   757,   276,   276,    53,   -38,  9774,   977,
     961,   978,   980,   626, -1456, -1456,   351,  9774,    56,  9774,
     476, -1456,   883,   108,  5028, -1456,   343,   990,   983,  1030,
    1030, -1456, -1456,   989,   149,   590, -1456,  1007,   992, -1456,
   -1456,  1009,   994, -1456, -1456,   299,   299,   299, -1456, -1456,
    1802, -1456,  1802, -1456,  1802, -1456,  1802, -1456,  1802, -1456,
    1802, -1456,  1802, -1456,  1802,   761,   761,  1161, -1456,  1802,
   -1456,  1802,  1161, 14415, 14415,   999, -1456,  1802,   290,  1004,
   -1456, 13314,   210,   210,   882, 14138,   761,   761, -1456,  1802,
   -1456,  1802, 14361, 10644, 14268, -1456,  1802, -1456,  1802, -1456,
    1802, 14231, -1456,  1802, 14690, 14007,   917, 14387,  2329,  1161,
    1161,  1502,  1502,   290,   290,   290,   348,  9774,  1006,  1011,
     350,  9774,  1207,  1012, 13352, -1456,   386, -1456,   378,   813,
    1143,   397,   870, -1456, -1456, 10687, -1456, -1456, -1456, -1456,
   10500, -1456, -1456, -1456,  1039, -1456,  1025, -1456,  1031, -1456,
    1032, -1456, 10603, -1456,   954,   389,   810, -1456, -1456, -1456,
     810,   810, 11728, -1456,  1189,   -71, -1456, 10808,  1366,  1628,
    9774,    57,   758,   500,   396,  1015,  1018,  1064, 11839,   469,
   11877,   760, 10500, 10500, 10500,  1883,  1020, 14138, 14138, 14138,
   14138, 14138, 14138, 14138, 14138, 14138, 14138, 14138, 14138, 14138,
   14138, 14138, 14138, 14138, 14138, -1456,  1022, 10500, -1456, -1456,
   -1456,  9774,  1982,  1991, -1456,  2144, -1456,  2444,  1024,  2449,
    2526,  1026,  4609,    42,   590, -1456, -1456, -1456, -1456, -1456,
    1023,  9774, -1456,  9774,  9774,  9774,  5236, 13168,     5,  9774,
     618,   500,   342, -1456, -1456,  1029, -1456,  9774, -1456,  1038,
    9774, -1456,  9774,   500,   669,  1041, -1456, -1456,  9774, 14138,
   -1456, -1456,   399,   446, 14100,    67,    70,  9774,   276,    74,
   -1456, -1456,  9774,  9774, 10500,   590,   818,  1205,  9774, -1456,
   -1456,  2755,    42,   -21, -1456,  1028,   385,  9980, -1456, -1456,
   -1456,   394,   417,   149,  1075,  1082,  1042,  1085,  1088, -1456,
     401,   716, -1456,  9774, -1456,  9774, -1456, -1456, -1456,  8126,
    9774, -1456,  1066,  1049, -1456, -1456,  9774,  1051, -1456, 13463,
    9774,  8332,  1052, -1456, 13498, -1456,  8538, -1456, -1456,   397,
   -1456, -1456,   692, -1456,    62,   351,    42, -1456, -1456, -1456,
   -1456,   810, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456,  1056, 10500, -1456,  1094,
    9774, -1456, -1456,   296,  9774, -1456,  1054, -1456, -1456, -1456,
     447, -1456,  1057,  1102, -1456, -1456,  4817,  5025,  5233, -1456,
   -1456,  9774,  6679, 13744, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456,   666,   716,  8744,   703, 11988, 14138, 14138,
       4,   342, 14138,  1059,   160,   990, -1456, -1456, 14138, -1456,
     993,   715,     4,  1065, -1456, -1456, -1456, -1456,   717, -1456,
   -1456, -1456,  9774,  9774,   722,    76,  9774, 12023, 12134, 10061,
     716,   459, -1456, -1456,  9774, -1456, -1456,   397,   724, -1456,
     590, -1456,  1067,  5028,  1109,  1068,    96,   456, -1456, -1456,
    1110, -1456, -1456,   149,  1077,   215, 10500, 12172, 10500, 12283,
   -1456,   481, 12318, -1456,  9774,  1615,  9774, -1456, 12429,  5028,
   -1456,   512,  9774, -1456, -1456, -1456,   528,   351,  1261,    62,
   -1456, -1456,   813, -1456,  1078, -1456, -1456, -1456,  9774,   810,
   -1456, 14138,  1079,  1084, -1456,   529, -1456, -1456,  9774,  1129,
    9774,  1104, -1456, -1456, -1456, -1456,  1087,  1089,  1093, -1456,
    9774,  9774,  9774,  1095,  1235,  1097,  1098,  8950, -1456,   215,
   -1456,   530,  9774,   183,   342, -1456,  9774,  9774,  9774,  9774,
     669, -1456,  9774,  9774,  1099,   538,   543,  9774,  9774,   734,
   -1456, -1456, -1456,  1107, -1456, 13651,  3994, -1456,  9774,   716,
   -1456,   425, -1456,   650, 10500,   -62, -1456,  1090, -1456, -1456,
    9156, -1456, -1456, 10084, -1456,   771, -1456, -1456, -1456, 10500,
   12467, 12578, -1456,   533, -1456, 12613, -1456, -1456, -1456, -1456,
    1261,   276,  1114,  1235,  1235,   459, 12724,  1112, 12762,  1103,
    1120,  1121,  1123,  9774, -1456,  9774,  1161,  1161,  1161,  9774,
   -1456, -1456,  1235,   500, -1456, 12873, -1456, -1456, 13744,  9774,
    9774, -1456, 12908, 14138, 14138, 13744, -1456,   397,  1161,  9774,
   -1456,  1124,  1142, 13744,   544,  9774,   262, -1456,   818,  9362,
    9568, -1456, -1456, -1456, -1456, -1456, 14138,   397,   459,  1127,
   10500,   -62,  1475,  9774, -1456,  9774, 14231, -1456, -1456,   774,
   -1456, -1456,  1128, -1456, 14783, -1456, -1456, -1456,   144,   144,
   -1456, -1456,  9774, -1456,  9774,  1235,  1235,   500,  1130,  1134,
     972,   144,   990,  1135, -1456,  1294,  1138, 14138, 14138,   211,
    1172,   476,  1166,  9774,  9774,  1150,  1173, 13744, -1456,   262,
   -1456,  9774,  9774, 14138,   476, -1456, -1456,  1475,  9774,  9774,
   13744,  1615, -1456, -1456, -1456, -1456,   397, 14783,   500,   990,
    1176, -1456,  1152,  1154, 13019, 13057,   144,   144,  1155, -1456,
   -1456,  1159,  1160, -1456,  9774,  1164,  9774,  9774,  1165,   476,
   -1456,  1167,   397, 13744, -1456,  9774,  1170, -1456, 14138, -1456,
    9774, 13744, 13744, -1456, -1456,   351,   459,   545,  1171, -1456,
   -1456, -1456, -1456, -1456,  1174,  1175, -1456, -1456, -1456, 14138,
   -1456, 14138, 14138, -1456, -1456, -1456,  1198,  1178, 13744, -1456,
   13744, -1456, -1456, -1456, -1456, -1456,   500, -1456, -1456, -1456,
   -1456,  1179, -1456,   547, -1456, -1456
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   127,     1,   312,     0,     0,     0,   628,   313,     0,
     620,   620,   620,    17,    18,     0,     0,    16,    15,     3,
       0,    10,     9,     8,     0,     7,   609,     6,    11,     5,
       4,    13,    12,    14,    97,    98,    96,   105,   107,    40,
      56,    53,    54,    42,    43,    44,     0,    45,    51,    41,
     227,   226,   620,   620,    24,    23,   609,   622,   621,   792,
     782,   787,     0,   280,    38,   113,   114,   115,     0,     0,
       0,   116,   118,   125,     0,   112,    19,   642,   641,   220,
     630,     0,   645,   610,   611,     0,     0,     0,     0,    46,
       0,    52,     0,     0,    49,     0,     0,   620,     0,    20,
       0,     0,     0,   282,     0,     0,   124,   119,     0,     0,
       0,     0,     0,     0,   128,   222,   221,   224,   219,   632,
     631,     0,   644,   643,   647,   646,   650,   613,   612,   615,
     103,   104,   101,   102,   100,     0,     0,    99,   108,    57,
      55,    51,    48,    47,   623,   556,   229,   228,   627,     0,
     629,    21,    22,    25,   793,   783,   788,   281,    36,    39,
     123,     0,   120,   121,   122,   126,     0,   633,     0,   638,
     606,   542,    26,    27,    31,     0,    92,    93,    90,    91,
      89,    88,    94,     0,    50,     0,   557,   625,   556,     0,
       0,     0,    37,   117,     0,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
       0,     0,   134,   129,     0,     0,   617,   639,     0,   651,
     607,     0,     0,   544,     0,    28,    29,    30,     0,   106,
       0,   626,   701,   784,   789,   188,   189,   186,   137,   138,
     140,   139,   141,   142,   143,   144,   170,   171,   168,   169,
     161,   172,   173,   162,   159,   160,   187,   181,     0,   185,
     174,   175,   176,   177,   148,   149,   150,   145,   146,   147,
     158,     0,   164,   165,   163,   156,   157,   152,   151,   153,
     154,   155,   136,   135,   180,     0,   166,   167,   542,   132,
     257,   225,   685,   688,   691,   692,   686,   689,   687,   690,
       0,     0,   636,   648,   614,   542,     0,   109,     0,   111,
       0,   596,   594,   616,    95,     0,     0,     0,     0,     0,
       0,   658,   678,   659,   694,   660,   664,   665,   666,   667,
     684,   671,   672,   673,   674,   675,   676,   677,   679,   680,
     681,   682,   752,   663,   670,   683,   759,   766,   661,   668,
     662,   669,     0,     0,     0,     0,   693,   714,   717,   715,
     716,   779,   624,   699,   574,   580,   190,   191,   184,   179,
     192,   182,   178,     0,   130,   311,   568,     0,     0,   223,
       0,   617,   556,   634,     0,   640,   558,   652,     0,     0,
     110,     0,     0,     0,     0,   595,     0,   720,   743,   746,
       0,   749,   739,     0,   705,   753,   760,   767,   773,   776,
       0,     0,   729,   734,   728,     0,   742,   738,   731,     0,
       0,   733,   718,     0,   702,   699,     0,   785,   790,   193,
     183,     0,   309,   310,     0,   131,   542,   133,   259,     0,
       0,     0,    66,    67,    79,   448,   449,     0,     0,     0,
       0,   297,   442,   295,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
       0,   684,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   531,     0,     0,
       0,   235,   365,   367,   366,   368,   369,   370,   371,     0,
       0,    32,   226,   236,     0,     0,     0,     0,     0,     0,
       0,   347,   348,   446,   445,     0,     0,     0,     0,   252,
     247,   244,   243,   245,   246,     0,   230,   279,   258,   238,
     525,   237,   443,     0,   516,    74,    75,    72,   250,    73,
     251,   253,   315,   242,   515,   514,   513,   127,   519,   444,
       0,   239,   518,   517,   489,   450,   490,   372,   451,     0,
     447,   795,   799,   796,   797,   798,     0,     0,   618,   637,
     559,   556,   543,     0,     0,     0,   525,     0,   598,   599,
       0,   592,   593,   591,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   129,     0,   129,     0,   129,     0,
       0,     0,   725,   261,   736,   737,   730,   732,     0,   735,
     719,     0,     0,   781,   780,   703,   704,   700,   794,   280,
     708,   709,     0,   575,   570,     0,     0,     0,   581,     0,
       0,     0,   653,   566,   585,   569,   840,   843,     0,     0,
       0,   285,   289,   288,   294,     0,     0,   333,     0,     0,
       0,   876,     0,     0,   301,   298,     0,   342,     0,     0,
     265,     0,   283,     0,     0,     0,   324,   327,     0,   256,
     330,     0,     0,    60,     0,    83,   880,     0,   849,   858,
       0,   846,     0,     0,   306,   303,   478,   479,   348,   360,
     127,   278,   276,   277,     0,     0,     0,     0,     0,     0,
       0,   818,     0,     0,     0,   856,   883,     0,   269,     0,
     272,     0,     0,   455,   454,   491,   453,   452,     0,     0,
     894,   342,   894,   349,   254,   255,     0,    77,     0,   885,
     894,     0,   363,   233,   523,     0,   241,   248,   249,   300,
     305,   314,     0,   357,     0,   240,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   480,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,     0,   232,
       0,   635,   556,     0,     0,   649,   542,   608,     0,   597,
       0,   601,     0,   605,   372,     0,     0,     0,   710,   723,
       0,     0,   695,   697,     0,     0,   132,     0,   132,     0,
     132,   572,     0,   578,     0,   696,     0,     0,   894,   727,
     712,     0,     0,   576,   786,     0,   582,   791,   567,     0,
       0,   584,     0,   583,     0,   586,     0,     0,     0,    84,
       0,     0,     0,   832,     0,     0,     0,     0,   866,   869,
     872,     0,   894,   302,   299,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   894,   284,     0,    85,
      86,     0,     0,     0,     0,    58,    59,     0,     0,   894,
       0,     0,   894,     0,     0,   307,   304,   349,   357,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   280,     0,
       0,     0,   852,   810,   818,     0,   861,     0,     0,     0,
     818,     0,     0,     0,     0,   821,     0,   888,     0,   260,
       0,    35,     0,    33,     0,     0,     0,     0,   895,     0,
       0,     0,     0,   419,   416,   418,   556,     0,     0,   895,
       0,   257,   127,     0,     0,   435,     0,   809,     0,     0,
       0,   323,   322,     0,     0,   129,   275,     0,     0,   502,
     501,     0,     0,   503,   507,     0,     0,     0,   394,   403,
     382,   404,   383,   406,   385,   405,   384,   407,   386,   397,
     376,   398,   377,   399,   378,   456,   457,   469,   408,   387,
     409,   388,   470,   467,   468,     0,   396,   374,   496,     0,
     487,     0,   520,   521,   522,   375,   458,   459,   410,   389,
     411,   390,   474,   475,   476,   400,   379,   401,   380,   402,
     381,   477,   395,   373,     0,     0,   472,   473,   471,   465,
     466,   461,   460,   462,   463,   464,     0,     0,     0,   425,
       0,     0,     0,     0,     0,   440,     0,   619,     0,   550,
     553,     0,     0,   600,   603,   372,   604,   721,   744,   747,
       0,   750,   740,   706,     0,   754,     0,   761,     0,   768,
       0,   774,     0,   777,     0,     0,   267,   724,   262,   713,
     571,   577,     0,   655,   656,   587,   590,   589,     0,     0,
       0,     0,     0,   833,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   382,   383,
     385,   384,   386,   376,   377,   378,   387,   388,   374,   389,
     390,   379,   380,   381,   373,   308,     0,     0,   889,   266,
     492,     0,     0,     0,   493,     0,   524,     0,     0,     0,
       0,     0,     0,   357,   129,   526,   527,   528,   529,   530,
       0,     0,   819,     0,     0,     0,     0,   342,   818,     0,
       0,     0,     0,   827,   828,     0,   835,     0,   825,     0,
       0,   864,     0,     0,     0,     0,   823,   865,     0,   855,
     820,   884,     0,     0,     0,     0,     0,     0,     0,     0,
     863,   482,     0,     0,     0,   129,    63,    68,     0,   886,
     887,     0,   357,     0,   436,     0,     0,     0,   439,   437,
     316,     0,     0,     0,     0,     0,     0,     0,     0,   355,
       0,   132,   498,     0,   504,     0,   393,   391,   392,     0,
       0,   485,     0,     0,   508,   512,     0,     0,   488,     0,
       0,     0,     0,   426,     0,   433,     0,   483,   441,   128,
     551,   552,   553,   554,   545,   556,   357,   602,   722,   745,
     748,   711,   751,   741,   707,   698,   755,   757,   762,   764,
     769,   771,   775,   573,   778,   579,     0,     0,   654,     0,
       0,   841,   844,     0,     0,   286,     0,   291,   292,   290,
       0,   336,     0,     0,   339,   334,     0,     0,     0,   877,
     875,   265,     0,     0,   325,   328,   331,   881,   879,   850,
     859,   857,   847,     0,   132,     0,     0,     0,   801,   800,
     818,     0,   853,     0,     0,   811,   834,   826,   854,   824,
     862,     0,   818,     0,   830,   831,   838,   822,     0,   270,
     273,    34,     0,     0,     0,     0,     0,     0,     0,     0,
     132,     0,    61,    62,     0,    76,    69,     0,     0,   231,
     129,   438,     0,     0,     0,     0,   594,     0,   352,   353,
       0,   351,   350,     0,     0,     0,     0,     0,     0,     0,
     414,     0,     0,   509,     0,   497,     0,   486,     0,     0,
     427,     0,     0,   484,   434,   430,     0,   556,   548,   545,
     546,   547,   550,   560,     0,   758,   765,   772,   261,   268,
     657,   588,     0,     0,    80,     0,   287,   293,     0,     0,
       0,     0,   335,   867,   870,   873,     0,     0,     0,    87,
       0,     0,     0,     0,   832,     0,     0,     0,   234,     0,
     532,     0,     0,     0,     0,   836,     0,     0,     0,     0,
       0,   829,     0,     0,   263,     0,     0,     0,     0,     0,
     417,   541,   420,     0,    64,     0,     0,    78,     0,   132,
     412,     0,   317,   594,     0,     0,   359,     0,   356,   358,
       0,   344,   362,     0,   540,     0,   538,   415,   535,     0,
       0,     0,   534,     0,   428,     0,   431,   565,   549,   561,
     548,     0,     0,   832,   832,     0,     0,     0,     0,     0,
       0,     0,     0,   261,   890,   265,   326,   329,   332,     0,
     833,   851,   832,     0,   494,     0,   361,   533,   892,     0,
       0,   837,     0,   803,   802,   892,   839,   892,   271,   261,
     274,     0,     0,   892,     0,     0,     0,   423,    63,   297,
       0,    70,    74,    75,    72,    73,    71,   892,     0,     0,
       0,     0,     0,     0,   354,     0,   345,   499,   505,     0,
     539,   537,     0,   536,     0,   563,   555,   726,   817,   817,
      82,   337,     0,   340,     0,   832,   832,     0,     0,     0,
     894,   817,   808,     0,   495,     0,     0,   805,   804,     0,
       0,     0,   894,     0,     0,     0,     0,   892,   421,     0,
      65,     0,     0,   303,     0,   364,   413,     0,     0,     0,
       0,   346,   500,   506,   510,   429,     0,     0,     0,   814,
     894,   816,     0,     0,     0,     0,   817,   817,     0,   878,
     891,     0,     0,   848,     0,     0,     0,     0,     0,     0,
     895,     0,   892,   892,   896,     0,     0,   424,   304,   903,
       0,     0,     0,   321,   511,   556,     0,     0,   895,   815,
     842,   845,   338,   341,     0,     0,   874,   882,   860,   893,
     901,   807,   806,   902,   904,   264,     0,     0,   892,   900,
       0,   320,   319,   562,   564,   812,     0,   868,   871,   899,
     897,     0,   318,     0,   898,   813
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1456, -1456,    -1, -1456, -1456, -1456, -1456,   662,  1304, -1456,
   -1456, -1456,  1389, -1456, -1456, -1456,   899,  1345, -1456,  1256,
   -1456, -1456,  1322, -1456, -1456, -1456,  -175, -1456, -1456, -1456,
     -91, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456,  1183, -1456, -1456,   -50,   -54, -1456, -1456, -1456,   642,
     601,  -535,  -578,  -815, -1456, -1456, -1456, -1389, -1456, -1456,
      -4,  -229,  -219,  -359, -1456,   436, -1456, -1387, -1456, -1304,
    -809,   676, -1456, -1456, -1456, -1456,  -399,     0, -1456, -1456,
   -1456, -1456, -1456,   -84,   -78,   -76, -1456,   -75, -1456, -1456,
   -1456,  1423, -1456,   442, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456,  -130,   -46,  1083,
      29,   190,  -899,  -456, -1456,  -527, -1456, -1456,  -383,   427,
   -1456, -1456, -1456, -1455, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456,  1092, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456,  -153,     6,   -96,     8,   159, -1456,  -186, -1456, -1456,
   -1456, -1456,  1008, -1456,  -592, -1456, -1456,  -600, -1456, -1456,
    -633,   -89,  -586, -1346, -1456,  -378, -1456, -1456,  1399, -1456,
   -1456, -1456,  1070,   973,   235, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456,  -609,  -216, -1456,  1013, -1456, -1456,  1037, -1456, -1456,
   -1456, -1456,  -429, -1456, -1456,  -154, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456,  -155, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456,  1016,  -630,  -171,
    -838,  -686, -1456, -1456, -1180,  -906, -1456, -1456, -1456, -1170,
     -33, -1132, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,
   -1456,   248,  -488, -1456, -1456, -1456,   751, -1456, -1456, -1456,
   -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456, -1456,   930,
   -1456, -1215,  -726, -1456
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   631,    18,   153,    56,    19,   175,   181,   721,
     523,   159,   524,   105,    21,    22,    47,    48,    49,    94,
      23,    41,    42,   525,   526,  1394,  1395,   527,  1397,  1506,
     528,   529,   977,   530,   649,   650,   531,   532,   533,   534,
    1171,   182,   183,    37,    38,    39,   233,    71,    72,    73,
      74,    24,   299,   389,   223,    25,   117,   224,   118,   166,
     535,   148,   699,  1008,   538,   390,   539,   847,  1580,   905,
    1115,   586,   958,  1493,   960,  1494,   541,   542,   543,   652,
     872,  1456,   544,   545,   546,   547,   548,   549,   550,   551,
     444,   552,   752,  1251,   991,   553,   554,   911,  1470,   912,
    1471,   914,  1472,   555,   877,  1462,   556,   730,  1522,   557,
    1259,  1260,   995,   701,   558,   809,   982,   559,   666,  1009,
     561,   562,   563,   975,   564,  1234,  1586,  1235,  1649,   565,
    1082,  1434,   566,   731,  1416,  1662,  1418,  1663,  1529,  1704,
     568,   385,  1442,  1539,  1292,  1294,  1091,   187,   580,   816,
    1614,  1667,   386,   387,   632,   842,   437,   637,   844,   438,
    1195,   643,   594,   406,   322,   323,   230,   315,    84,   129,
      27,   171,   311,    95,    96,   185,    97,    28,    53,   121,
     168,    29,   395,   228,   229,    82,   126,   397,    30,   169,
     313,   644,   569,   310,   368,   369,   834,   436,   373,   370,
     602,  1304,  1317,   827,   432,   371,   595,  1298,   846,   600,
    1303,   596,  1299,   597,  1300,   599,  1302,   603,  1306,   604,
    1445,   605,  1308,   606,  1446,   607,  1310,   608,  1447,   609,
    1312,   610,  1314,   634,    31,   101,   190,   374,   635,    32,
     102,   191,   375,   639,    33,   100,   189,   570,  1669,  1633,
     988,   944,  1670,  1671,  1672,   945,   957,  1217,  1211,  1206,
    1376,  1136,   571,   867,  1452,   868,  1453,   923,  1476,   920,
    1474,   946,   739,   572,   921,  1475,   947,   573,  1142,  1550,
    1143,  1551,  1144,  1552,   881,  1466,   918,  1473,   717,   906,
     574,  1636,   969,   575
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   314,   241,    52,   822,   301,   970,   560,   633,   638,
     843,   866,   716,   743,   980,    64,    75,   841,   234,    76,
     581,  1106,   753,  1108,   367,  1110,   836,   950,   838,  1184,
     840,   537,  1366,   702,   703,  1135,   138,  1467,  1202,  1130,
     870,   658,  -127,    40,  1214,   814,   993,   231,   614,    90,
      83,    34,    35,   130,   131,   161,   861,   936,   936,    85,
    1515,  1542,   664,    62,   422,   591,  1191,  1212,    75,    75,
      75,  1218,   616,  1227,    57,   592,  1238,  1324,    13,   641,
      58,   943,   393,   943,    91,   372,   694,  1382,   779,   780,
    1383,   423,   424,    63,  1386,   145,  1498,   150,    14,    86,
     642,   935,   537,   948,   442,   952,   861,   232,  1440,    86,
      75,    75,    75,    75,   641,   987,  1194,    65,   861,   966,
       2,   863,  1118,    88,   433,   170,   537,     3,  1194,   978,
     593,  1648,   320,   318,   104,   642,   193,   655,   659,   660,
     989,   110,   111,   112,   965,  1245,    66,   443,   188,  1222,
       4,  1223,     5,   321,     6,   425,  1146,   176,   177,   426,
       7,   536,   398,   937,   226,   928,  1628,  1601,  1441,    13,
       8,   863,   367,   807,   808,    87,     9,   316,  1243,  1228,
    1169,   146,   317,   863,  1039,   242,   243,   244,   367,    14,
      67,  1485,  1642,  1178,  1697,   823,  1181,   990,   146,  1040,
      10,   147,   994,   853,   132,    36,   578,   628,   856,   133,
     862,   134,   615,   103,   135,   160,   104,   367,   147,    68,
     300,   367,    11,    12,   427,  1666,    62,   109,   428,   104,
      92,   429,   319,   447,   661,   537,   617,   862,   862,    93,
     366,    93,   862,   987,   862,   668,   430,   862,   862,   813,
     590,  1629,   431,   662,   618,   136,    63,    54,   862,   685,
     619,   862,  1131,   861,   883,   862,   937,   862,   400,    50,
      77,    78,   383,    79,   141,   744,  1254,   231,  1706,   624,
     852,  1243,   367,   367,  1353,    13,  1255,   861,    98,    51,
    1514,    69,  1361,   405,   925,  1326,    13,  1487,   996,    62,
      70,    80,   937,    55,   537,    14,   391,   139,   178,   396,
     861,   756,   757,   179,  1571,   180,    14,  1243,   135,   745,
    1569,   433,  1256,   684,    15,  1097,  1244,   862,   863,    63,
     864,   665,   149,   865,   537,    16,   776,   232,   861,   779,
     780,  1257,  1561,  1400,   711,   713,  1258,   811,  1686,   445,
      13,   815,   863,   987,  1246,  1225,  1226,  1520,  1229,   433,
    1640,  1668,  1641,  1098,   320,   421,   144,   113,  1645,   754,
      14,   383,    81,  1208,   992,   863,   231,   612,    50,   367,
     367,   367,  1654,   367,   367,   321,   446,   367,   366,   367,
     732,   367,   114,   367,   579,   817,   613,  1444,    51,  1100,
     154,  1209,    13,   863,   366,    62,   854,    50,   768,   769,
     857,  1618,  1619,  1112,   971,  1114,   776,  1261,   778,   779,
     780,   781,    14,  1094,   807,   808,   782,    51,   641,   869,
    1631,    86,  1696,   366,   384,    63,   232,   366,   225,  1673,
     825,   826,   828,   158,   830,   831,  1415,  1242,   835,   642,
     837,  1682,   839,   367,  1484,   909,   399,   366,   656,    50,
     300,   422,    50,   537,   155,   433,  1490,  1126,  1221,  1101,
      43,    44,    45,   679,   300,  1167,   367,  1726,  1727,    51,
    1192,    62,    51,   848,   300,   300,   300,   885,   423,   424,
     971,  1239,  1320,  1676,  1677,   972,  1714,  1715,   366,   366,
     959,    46,    13,   987,   807,   808,  1277,   537,  1282,    13,
     238,    63,  1360,  1741,  1315,   904,   186,   976,   521,   698,
    1313,  1278,    14,  1283,   734,   735,   186,  1372,   630,    14,
    1203,  1204,   882,   884,  1247,   630,   973,   239,   924,  1479,
    1000,  1004,   746,  1111,   747,   748,   749,    13,   750,  1266,
    1267,  1268,   425,   577,    13,   591,   426,   919,  1205,   755,
     922,  1248,    13,   156,    13,   592,  1044,    14,   943,   113,
     110,  1365,   112,   630,    14,  1503,  1200,   885,   756,   757,
     630,  1113,    14,   943,    14,  1404,   157,   885,  1316,   821,
     630,  1083,  1413,    13,  1289,   366,   366,   366,  1379,   366,
     366,  1402,   367,   366,  1288,   366,  1354,   366,  1404,   366,
    1405,    13,    13,    14,  1327,   986,  1200,  1414,    57,  1385,
     593,   427,    50,   115,    58,   428,  1087,   167,   429,   116,
     367,    14,    14,    13,    13,  1406,   367,   630,   630,   367,
     146,  1599,    51,   430,   300,  1380,  1458,  1413,   367,   431,
     422,   367,   367,    14,    14,   874,   367,  1390,   937,    13,
     147,   367,   146,  1090,  1092,   367,    91,  1103,  1332,   366,
     300,   392,   885,  1243,  1516,   768,   769,   423,   424,    14,
    1086,   146,   147,   776,  1165,   630,   779,   780,   781,   433,
     192,  1116,   366,   782,  1598,   367,   367,  1120,   367,  1527,
    1121,   147,   367,   885,   367,   367,    13,   367,   627,  1125,
     106,   107,   108,  1128,  1129,   300,   956,  1132,   110,   885,
     885,   885,  1141,   240,  1200,  1215,    14,  1145,  1216,   885,
    1534,  1488,   300,   974,   885,   885,  1200,   981,  1200,  1438,
     227,   425,   367,  1134,   367,   426,  1536,  1545,  1567,  1612,
     300,  1240,   162,   163,   164,   165,  1581,  1172,  1173,  1293,
    1175,  1582,  1646,  1735,  1177,  1745,  1179,  1180,   669,  1182,
     302,   807,   808,  1511,   303,   296,  1363,   434,    13,  -756,
     297,   686,   756,   757,  -756,   435,   636,   670,   304,   305,
    1236,  1364,  -763,   306,   307,   308,   309,  -763,    14,  1533,
     687,  -770,  -756,  1116,   630,  1116,  -770,   298,    13,   312,
     427,   732,   689,    75,   428,  -763,  1183,   429,   366,   732,
    -422,   875,  1509,  1096,  -770,  -422,  1478,   954,    14,   955,
    1391,   690,   430,  1104,   172,   173,   961,   962,   431,   119,
     876,  1392,  1393,  -422,  1600,   120,   366,   405,  1275,   379,
    1570,   403,   366,  1373,   404,   366,  1374,   405,   560,  1375,
     124,   122,   123,   127,   366,   151,   125,   366,   366,   128,
    1290,   152,   366,   376,   433,   377,  1291,   366,  1102,   768,
     769,   366,   537,  1477,   367,   521,   698,   776,  1482,   778,
     779,   780,   781,   378,   885,   380,   367,   782,   381,   300,
    1489,  1459,  1492,   422,  1681,   382,   885,  1497,   885,  1508,
     388,   366,   366,   885,   366,   885,  1691,   394,   366,  1585,
     366,   366,   401,   366,   402,   885,   367,   367,   367,  1603,
     423,   424,   300,  1632,   235,   236,   433,   433,   756,   757,
    1105,  1107,   407,  1201,  1709,  1301,  1210,   408,   433,  1201,
    1210,   367,  1109,   433,   409,   433,   410,  1325,   366,  1335,
     366,   802,   803,   804,   805,   806,   433,   449,   450,   433,
    1608,   521,   698,  1664,   411,   807,   808,   110,   111,   112,
      43,    44,    45,    59,    60,    61,   412,   460,  1336,  1337,
    1338,   142,   143,   465,   425,   413,   414,  1632,   426,   415,
    1323,   300,   300,   300,  1577,   433,  1687,   172,   173,   174,
     416,   420,  1399,  1342,   417,  1658,  1659,   418,   367,   419,
    1597,   235,   236,   237,   439,   440,   441,   582,   588,   622,
     479,   480,   589,   766,   767,   768,   769,   770,  1707,   601,
     773,   774,   775,   776,   625,   778,   779,   780,   781,   -81,
     646,   647,   657,   782,   673,   784,   785,   675,   676,   677,
     678,  1356,   680,   427,   482,   483,   540,   428,   681,  1296,
     429,  1700,   688,   691,   692,   704,   722,   733,   720,   705,
    1389,   737,  1371,   422,   706,   430,   707,   751,  1378,   708,
    1295,   431,   741,   818,  1297,   709,   718,  1384,    16,   810,
     366,   367,   684,   820,  1454,    62,  1743,   832,  1398,  1443,
     423,   424,   366,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   498,   499,   500,    50,   833,   636,   849,   627,
     855,   807,   808,   859,   860,    63,   871,   540,  1333,  1421,
     873,   878,   366,   366,   366,   512,   879,   880,   885,   754,
     903,  1431,   910,   915,   916,  1692,  1436,   927,   964,   967,
     651,   540,  1449,   968,  1035,   979,  1089,   366,  1093,  1099,
    1117,  1123,  1504,  1124,   425,  1139,  1168,   519,   426,  1231,
    1193,  1200,   756,   757,  1207,  1220,  1011,  1013,  1015,  1017,
    1019,  1021,  1023,  1230,  1455,  1232,  1521,  1233,  1028,  1030,
     367,  1249,   367,  1250,  1036,  1262,  1253,  1264,  1285,  1263,
    1293,  1265,  1305,  1048,  1050,    13,  1269,  1396,  1055,  1057,
    1059,  1270,  1062,  1280,  1307,  1481,   756,   757,  1281,  1286,
    1309,  1311,  1319,  1328,   366,    14,  1329,  1330,  1340,  1341,
    1355,   630,  1348,   427,  1351,  1367,  1401,   428,  1408,  1166,
     429,  1537,  1495,  1496,  1369,  1409,  1499,  1377,  1411,  1410,
    1521,  1412,  1523,  1525,  1423,   430,  1424,  1450,  1426,  1432,
     540,   431,  1457,  1448,  1460,  1461,  1486,   766,   767,   768,
     769,  1491,  1512,  1517,  1513,  1510,  1588,   776,  1437,   778,
     779,   780,   781,  1519,  1538,  1477,  1543,   782,   367,   784,
     785,  1544,  1547,  1549,  1553,  1560,  1587,  1554,  1604,  1643,
    1555,  1622,  1559,   367,  1562,  1563,  1579,   366,   848,   300,
    1624,   766,   767,   768,   769,   770,  1620,  1644,   773,   774,
     775,   776,  1617,   778,   779,   780,   781,  1625,  1626,   540,
    1627,   782,  1469,   784,   785,  1656,  1665,  1684,  1679,   788,
     789,   790,  1680,  1683,  1685,   794,  1688,  1690,  1695,  1201,
    1602,   802,   803,   804,   805,   806,  1694,  1708,  1584,   540,
    1710,  1201,  1711,  1716,  1609,   807,   808,  1717,  1718,  1655,
    1720,  1723,  1739,   963,   367,  1725,  1729,   300,  1736,   137,
      20,    89,  1737,  1738,  1740,  1744,  1507,   184,   796,   422,
     797,   798,   799,   800,   801,   802,   803,   804,   805,   806,
     140,   300,  1689,  1650,  1088,  1591,   366,  1241,   366,   807,
     808,   324,  1592,   848,    26,  1699,   423,   424,  1593,  1630,
    1594,  1595,  1252,  1566,   540,   540,   540,   540,   540,   540,
     540,   540,  1518,  1407,  1615,  1540,  1657,   540,   540,   848,
    1541,  1439,  1616,   540,   645,    99,  1678,  1576,  1370,   665,
    1724,   576,   540,   540,   951,   740,     0,   540,   540,   540,
     653,   540,   626,   654,     0,   300,     0,     0,     0,     0,
       0,     0,   567,     0,     0,     0,     0,  1734,     0,     0,
     425,     0,     0,   587,   426,     0,     0,     0,   540,     0,
       0,   300,   598,     0,     0,     0,     0,     0,   422,     0,
       0,     0,   611,     0,   366,     0,     0,     0,     0,  1733,
       0,   884,   621,   756,   757,     0,     0,     0,     0,   366,
       0,     0,     0,   640,     0,   423,   424,     0,     0,     0,
       0,   300,   540,   648,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   667,     0,     0,     0,     0,   672,   427,
     674,     0,     0,   428,     0,  1321,   429,  1635,     0,   683,
       0,   700,   700,   700,  1635,     0,  1635,   695,   696,   697,
       0,   430,  1635,     0,     0,     0,   974,   431,     0,     0,
       0,     0,   715,     0,   300,     0,  1635,     0,     0,   425,
     366,   719,   591,   426,     0,     0,   723,   724,   725,   726,
     727,     0,   592,     0,     0,     0,     0,     0,     0,   736,
     768,   769,     0,   742,     0,     0,     0,   715,   776,     0,
     778,   779,   780,   781,     0,     0,   756,   757,   782,     0,
       0,     0,     0,     0,     0,     0,  1635,     0,     0,   974,
     742,     0,     0,     0,     0,     0,     0,     0,     0,  1703,
       0,   422,     0,     0,     0,  1705,     0,   593,   427,   812,
       0,     0,   428,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   824,     0,   423,   424,
     430,  1635,  1635,     0,     0,     0,   431,     0,     0,     0,
    1731,  1732,   300,     0,   804,   805,   806,     0,     0,     0,
       0,     0,     0,     0,   851,     0,   807,   808,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1635,     0,  1742,
       0,   766,   767,   768,   769,   770,     0,     0,   773,   774,
     775,   776,     0,   778,   779,   780,   781,     0,     0,     0,
     742,   782,   425,   784,   785,   667,   426,     0,     0,   788,
     789,   790,   715,   907,     0,   908,     0,     0,     0,     0,
     913,     0,     0,     0,     0,     0,   917,     0,   742,     0,
       0,     0,     0,     0,     0,   926,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   929,   930,   931,   932,
     933,   934,     0,   942,     0,   942,     0,     0,   796,     0,
     797,   798,   799,   800,   801,   802,   803,   804,   805,   806,
       0,   427,     0,   756,   757,   428,     0,  1322,   429,   807,
     808,     0,     0,     0,     0,     0,     0,   700,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,   431,
    1010,  1012,  1014,  1016,  1018,  1020,  1022,  1024,  1025,  1026,
       0,     0,  1027,  1029,  1031,  1032,  1033,  1034,     0,  1037,
    1038,     0,  1041,  1042,  1043,  1045,  1046,  1047,  1049,  1051,
    1052,  1053,  1054,  1056,  1058,  1060,  1061,  1063,  1065,  1066,
    1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,     0,
    1084,     0,     0,     0,     0,     0,     0,   742,     0,     0,
       0,     0,     0,     0,  1095,     0,   422,   540,   766,   767,
     768,   769,   770,     0,     0,   773,   774,   775,   776,     0,
     778,   779,   780,   781,     0,     0,     0,     0,   782,     0,
     784,   785,     0,   423,   424,     0,   788,   789,   790,     0,
       0,  1122,   794,     0,     0,     0,     0,     0,  1127,     0,
       0,     0,     0,     0,     0,     0,  1138,     0,  1140,     0,
       0,     0,     0,     0,     0,     0,     0,  1147,  1148,  1149,
    1150,  1151,  1152,  1153,  1154,  1155,  1156,  1157,  1158,  1159,
    1160,  1161,  1162,  1163,  1164,   796,     0,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   425,     0,   742,
       0,   426,     0,     0,     0,   422,   807,   808,     0,   700,
       0,   521,   698,     0,   422,     0,     0,     0,     0,     0,
       0,   723,  1197,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   423,   424,     0,  1219,     0,     0,   715,     0,
       0,   423,   424,     0,  1224,     0,     0,     0,     0,     0,
    1147,     0,     0,     0,     0,     0,     0,     0,     0,  1237,
       0,   715,     0,     0,     0,     0,   427,     0,     0,     0,
     428,     0,  1339,   429,     0,     0,     0,     0,   700,   700,
     700,     0,     0,   742,     0,   742,     0,   742,   430,   742,
       0,   742,     0,   742,   431,   742,   425,   742,     0,     0,
     426,     0,   742,     0,   742,   425,     0,     0,   -71,   426,
     742,     0,     0,     0,     0,     0,     0,     0,     0,   756,
     757,     0,   742,     0,   742,     0,     0,     0,     0,   742,
       0,   742,     0,   742,     0,     0,   742,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1279,
       0,     0,     0,  1284,     0,   427,     0,   422,   742,   428,
       0,  1344,   429,     0,   427,     0,     0,     0,   428,     0,
    1345,   429,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,   431,   423,   424,   430,     0,     0,     0,
     742,     0,   431,     0,     0,     0,   758,   759,   760,   761,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
     772,   773,   774,   775,   776,   777,   778,   779,   780,   781,
       0,     0,     0,     0,   782,   783,   784,   785,   786,   787,
       0,     0,   788,   789,   790,   791,   792,   793,   794,     0,
       0,     0,     0,  1343,     0,     0,     0,     0,   425,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1357,  1358,  1359,     0,     0,
       0,  1362,     0,     0,     0,     0,     0,     0,     0,  1368,
     795,   796,   942,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,     0,     0,     0,     0,     0,     0,     0,
     710,     0,   807,   808,  1387,  1388,   325,   521,   698,     0,
       0,     0,   326,   567,     0,     0,     0,   427,   327,  1147,
       0,   428,     0,  1346,   429,     0,     0,     0,   328,     0,
     756,   757,     0,     0,     0,  1417,   329,  1419,     0,   430,
       0,     0,  1422,     0,     0,   431,     0,     0,  1425,     0,
       0,   330,  1428,     0,     0,     0,     0,     0,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,     0,  1451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   715,     0,     0,   742,     0,     0,     0,
       0,     0,     0,     0,     0,   766,   767,   768,   769,   770,
      62,     0,   773,   774,   775,   776,     0,   778,   779,   780,
     781,     0,     0,   364,     0,   782,     0,   784,   785,     0,
       0,     0,     0,     0,     0,     0,     0,   422,     0,     0,
      63,     0,   422,     0,     0,     0,  1505,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   423,   424,     0,     0,     0,   423,
     424,   756,   757,     0,     0,     0,  1530,     0,  1531,     0,
       0,     0,     0,     0,  1535,     0,   365,   800,   801,   802,
     803,   804,   805,   806,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   807,   808,     0,     0,     0,     0,     0,
    1546,     0,  1548,     0,     0,     0,     0,     0,     0,   422,
       0,     0,  1556,  1557,  1558,     0,     0,     0,   425,  1565,
       0,     0,   426,   425,  1568,     0,     0,   426,  1572,  1573,
    1574,  1575,     0,     0,   715,  1578,   423,   424,     0,  1583,
       0,     0,     0,     0,     0,     0,     0,     0,  1596,     0,
     715,     0,     0,     0,     0,     0,   766,   767,   768,   769,
     770,     0,  1606,   773,   774,   775,   776,     0,   778,   779,
     780,   781,     0,     0,     0,     0,   782,     0,   784,   785,
       0,     0,     0,     0,   788,   789,   790,   427,     0,     0,
     794,   428,   427,  1347,   429,     0,   428,   715,  1349,   429,
     425,     0,     0,     0,   426,     0,     0,     0,     0,   430,
       0,  1637,  1638,     0,   430,   431,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,  1647,     0,     0,
       0,     0,  1653,   796,     0,   797,   798,   799,   800,   801,
     802,   803,   804,   805,   806,  1660,     0,  1661,     0,     0,
       0,     0,     0,     0,   807,   808,     0,     0,   819,     0,
       0,     0,     0,     0,  1674,     0,  1675,     0,     0,   427,
       0,     0,     0,   428,     0,  1350,   429,     0,     0,     0,
       0,     0,     0,     0,     0,   715,  1693,     0,     0,     0,
       0,   430,     0,     0,  1698,     0,     0,   431,     0,     0,
    1701,  1702,     0,     0,     0,     0,   448,     0,     0,     0,
     449,   450,     3,     0,   451,   452,   453,     0,   454,     0,
     455,   456,   457,   458,   459,     0,  1719,     0,  1721,  1722,
     460,   461,   462,   463,   464,     0,   465,  1728,     0,     0,
       0,     0,  1730,   466,   467,     0,     0,   468,     0,   469,
     470,     0,     0,   471,     0,     8,   472,   473,     0,   474,
     475,     0,     0,   476,   477,     0,     0,     0,     0,     0,
     478,     0,     0,   479,   480,     0,   331,   332,   333,     0,
     335,   336,   337,   338,   339,   481,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,   353,   354,
     355,     0,     0,   358,   359,   360,   361,   482,   483,   484,
     485,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,   487,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   488,   489,
     490,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   491,   492,   493,   494,
     495,     0,   496,     0,   497,   498,   499,   500,    50,   146,
     501,   502,   503,   504,   505,   506,   507,   508,    63,   509,
     510,   511,     0,     0,     0,     0,     0,     0,   512,   147,
     513,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   514,   515,   516,     0,    15,
       0,     0,   517,   518,     0,     0,   449,   450,     0,     0,
     519,     0,   520,     0,   521,   522,   455,   456,   457,   458,
     459,     0,     0,     0,     0,     0,   460,     0,   462,     0,
       0,     0,   465,     0,   422,     0,     0,     0,     0,     0,
     467,     0,     0,     0,     0,     0,   470,     0,     0,   471,
       0,     0,   472,     0,   936,     0,   475,     0,     0,     0,
       0,   423,   424,     0,     0,     0,   583,     0,     0,   479,
     480,     0,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   481,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,     0,   358,
     359,   360,   361,   482,   483,   584,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   486,
     487,     0,     0,     0,     0,   425,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   491,   492,   493,   494,   495,     0,   496,   937,
     497,   498,   499,   500,    50,     0,     0,   502,   503,   504,
     505,   506,   507,   508,   938,   585,   510,   511,     0,     0,
       0,     0,     0,     0,   512,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,   428,     0,
       0,   939,   515,   516,     0,    15,     0,     0,   517,   518,
       0,     0,     0,   449,   450,     0,   940,     0,   941,     0,
     521,   522,   431,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,   422,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,   423,   424,
       0,     0,     0,   583,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   584,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,   425,     0,     0,     0,   426,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,   937,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,   938,   585,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,   428,     0,     0,   939,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,     0,
     449,   450,     0,   940,     0,   949,     0,   521,   522,   431,
     455,   456,   457,   458,   459,     0,     0,     0,     0,     0,
     460,     0,   462,     0,     0,     0,   465,     0,   616,     0,
       0,     0,     0,     0,   467,     0,     0,     0,     0,     0,
     470,     0,     0,   471,     0,     0,   472,     0,     0,     0,
     475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     583,     0,     0,   479,   480,     0,   331,   332,   333,     0,
     335,   336,   337,   338,   339,   481,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,   353,   354,
     355,     0,     0,   358,   359,   360,   361,   482,   483,   584,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,   487,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   491,   492,   493,   494,
     495,     0,   496,     0,   497,   498,   499,   500,    50,     0,
       0,   502,   503,   504,   505,   506,   507,   508,    63,   585,
     510,   511,     0,     0,     0,     0,     0,     0,   512,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   617,     0,     0,   514,   515,   516,     0,    15,
       0,     0,   517,   518,     0,     0,     0,   449,   450,     0,
    1196,     0,   520,     0,   521,   522,   619,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   583,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   484,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,   663,
       0,     0,     0,     0,     0,   488,   489,   490,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   585,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
       0,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   583,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   484,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,   693,     0,     0,     0,     0,
       0,   488,   489,   490,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   585,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,     0,   520,     0,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
    1589,   462,   463,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,   473,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   583,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   584,  1590,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   585,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,     0,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   583,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   484,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   585,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,     0,   521,
     522,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   583,     0,     0,   479,   480,   983,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,   937,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,   938,
     585,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   984,     0,   520,   985,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,   422,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,   423,
     424,     0,     0,     0,     0,     0,     0,   583,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   484,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,   425,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,  1005,  1006,  1007,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   585,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,   428,     0,  1352,   429,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,     0,   430,   449,   450,   519,     0,   520,
     431,   521,   522,   728,     0,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
     422,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,   729,
       0,   472,     0,     0,     0,   475,     0,   423,   424,     0,
       0,     0,     0,     0,     0,   583,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   584,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,   425,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   585,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,   428,     0,  1463,   429,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,     0,   430,   449,   450,   519,   620,   520,   431,   521,
     522,   728,     0,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,   422,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,   729,     0,   472,
       0,     0,     0,   475,     0,   423,   424,     0,     0,     0,
       0,     0,     0,   583,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   584,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,   425,
       0,     0,     0,   426,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,   937,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,   938,   585,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,   428,     0,  1464,   429,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,     0,
     430,   449,   450,   519,     0,   520,   431,   521,   522,   728,
       0,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,   422,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,   729,     0,   472,     0,     0,
       0,   475,     0,   423,   424,     0,     0,     0,     0,     0,
       0,   583,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     585,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
     428,     0,  1465,   429,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,     0,   430,   449,
     450,   519,   849,   520,   431,   521,   522,   728,     0,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,   729,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   583,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   584,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   585,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,     0,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   583,     0,     0,   479,   480,
    1133,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   584,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,   937,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,   938,   585,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,     0,   521,
     522,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   583,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     585,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,   620,   520,     0,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,   671,     0,     0,     0,   583,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   584,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   585,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
       0,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,   682,     0,     0,     0,
       0,     0,     0,   583,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   584,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   585,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,     0,
       0,   449,   450,   519,     0,   520,     0,   521,   522,   714,
       0,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   583,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     585,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,     0,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,   422,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,   423,
     424,     0,     0,     0,     0,     0,     0,   583,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   584,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,   425,     0,     0,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   585,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,   428,     0,  1468,   429,
     718,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,     0,   430,   449,   450,   519,     0,   520,
     431,   521,   522,   738,     0,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   583,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   584,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   585,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,     0,   521,
     522,   455,   456,   457,   458,   459,     0,     0,  1064,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   583,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     585,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,     0,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   583,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   584,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   585,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
    1085,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   583,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   584,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   585,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1137,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,     0,   520,     0,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   583,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   584,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   585,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,  1198,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   583,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   584,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   585,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,  1213,   521,
     522,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   583,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     585,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,  1420,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   583,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   584,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   585,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,  1429,     0,   520,
    1430,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   583,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   584,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   585,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,     0,   520,  1435,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   583,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   584,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   585,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,  1480,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   583,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   584,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   585,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,  1564,   521,
     522,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   583,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     584,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,  1605,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     585,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,     0,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   583,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   584,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,  1651,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   585,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
       0,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   583,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   584,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,  1652,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   585,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,     0,   520,     0,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   583,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   584,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   585,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,     0,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   583,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   584,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   422,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   422,     0,     0,
       0,   423,   424,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,   423,   424,   502,   503,   504,   505,
     506,   507,   508,    63,   585,   510,   511,     0,   712,     0,
       0,     0,     0,   512,   325,     0,     0,     0,     0,     0,
     326,     0,     0,     0,     0,     0,   327,     0,     0,     0,
     514,   515,   516,     0,    15,   425,   328,   517,   518,   426,
       0,     0,     0,     0,   329,  1403,     0,   520,     0,   521,
     522,     0,     0,     0,     0,     0,     0,     0,   425,   330,
       0,     0,   426,     0,     0,     0,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,     0,
       0,     0,     0,     0,   427,     0,     0,     0,   428,     0,
    1502,   429,     0,     0,     0,     0,     0,   325,     0,     0,
       0,     0,     0,   326,     0,     0,   430,   427,     0,   327,
       0,   428,   431,  1607,   429,     0,     0,     0,    62,   328,
       0,     0,     0,     0,     0,     0,     0,   329,     0,   430,
       0,   364,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,   330,     0,     0,     0,     0,     0,    63,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,     0,     0,     0,     0,     0,
     325,     0,     0,     0,     0,     0,   326,     0,     0,     0,
       0,     0,   327,     0,     0,     0,     0,     0,     0,     0,
       0,    62,   328,     0,     0,     0,     0,     0,     0,     0,
     329,     0,     0,     0,   364,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   330,     0,     0,     0,     0,
       0,    63,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,   623,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   364,     0,     0,
       0,     0,     0,     0,     0,     0,    13,     0,     0,   325,
       0,     0,     0,     0,   629,   326,     0,     0,     0,     0,
       0,   327,     0,     0,     0,     0,    14,     0,     0,     0,
       0,   328,   630,     0,     0,     0,     0,     0,     0,   329,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   330,     0,     0,     0,     0,     0,
     365,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,     0,     0,     0,     0,     0,   326,     0,
       0,     0,     0,     0,   327,     0,     0,     0,     0,     0,
       0,     0,     0,    62,   328,     0,     0,     0,     0,     0,
       0,     0,   329,     0,     0,     0,   364,     0,     0,     0,
       0,     0,     0,     0,     0,   756,   757,   330,     0,     0,
       0,     0,     0,    63,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,     0,   756,   757,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   364,
     766,   767,   768,   769,   770,     0,     0,   773,   774,   775,
     776,     0,   778,   779,   780,   781,   629,     0,     0,     0,
     782,     0,   784,   785,     0,     0,     0,     0,   788,     0,
     790,     0,     0,     0,     0,   886,   887,   888,   889,   890,
     891,   892,   893,   766,   767,   768,   769,   770,   894,   895,
     773,   774,   775,   776,   896,   778,   779,   780,   781,     0,
       0,     0,   365,   782,   783,   784,   785,   897,   898,   756,
     757,   788,   789,   790,   899,   900,   901,   794,     0,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,     0,
       0,     0,    13,     0,     0,     0,     0,     0,   807,   808,
       0,     0,     0,     0,   756,   757,     0,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,   902,
     796,     0,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   807,   808,     0,     0,     0,   521,   698,     0,     0,
       0,     0,     0,     0,     0,     0,   886,   887,   888,   889,
     890,   891,   892,   893,   766,   767,   768,   769,   770,   894,
     895,   773,   774,   775,   776,   896,   778,   779,   780,   781,
    -372,     0,     0,     0,   782,   783,   784,   785,   897,   898,
       0,     0,   788,   789,   790,   899,   900,   901,   794,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,   756,   757,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,     0,     0,     0,     0,     0,     0,
     902,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   756,   757,     0,     0,     0,     0,     0,
       0,     0,   807,   808,     0,     0,     0,   521,   698,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   807,   808,     0,
       0,   829,     0,     0,     0,     0,     0,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   773,   774,   775,
     776,     0,   778,   779,   780,   781,     0,     0,     0,     0,
     782,     0,   784,   785,     0,     0,     0,     0,   788,   789,
     790,     0,     0,     0,   794,     0,     0,     0,   766,   767,
     768,   769,   770,     0,     0,   773,   774,   775,   776,     0,
     778,   779,   780,   781,   756,   757,     0,     0,   782,     0,
     784,   785,     0,     0,     0,     0,   788,   789,   790,     0,
       0,     0,   794,     0,     0,     0,     0,   796,     0,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   756,
     757,     0,     0,     0,     0,     0,     0,     0,   807,   808,
       0,     0,   845,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   796,     0,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   807,   808,     0,     0,
     858,     0,     0,     0,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,     0,     0,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
     756,   757,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,     0,   794,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   756,   757,
       0,     0,     0,     0,     0,     0,     0,   807,   808,     0,
       0,  1170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   807,   808,     0,     0,  1174,     0,     0,     0,
       0,     0,     0,     0,     0,   766,   767,   768,   769,   770,
       0,     0,   773,   774,   775,   776,     0,   778,   779,   780,
     781,     0,     0,     0,     0,   782,     0,   784,   785,     0,
       0,     0,     0,   788,   789,   790,     0,     0,     0,   794,
       0,     0,     0,   766,   767,   768,   769,   770,     0,     0,
     773,   774,   775,   776,     0,   778,   779,   780,   781,   756,
     757,     0,     0,   782,     0,   784,   785,     0,     0,     0,
       0,   788,   789,   790,     0,     0,     0,   794,     0,     0,
       0,     0,   796,     0,   797,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   756,   757,     0,     0,     0,     0,
       0,     0,     0,   807,   808,     0,     0,  1176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     796,     0,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   807,   808,     0,     0,  1185,     0,     0,     0,     0,
       0,     0,     0,     0,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
       0,     0,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,     0,   794,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,   756,   757,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   756,   757,     0,     0,     0,     0,     0,
       0,     0,   807,   808,     0,     0,  1186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   807,   808,     0,
       0,  1187,     0,     0,     0,     0,     0,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   773,   774,   775,
     776,     0,   778,   779,   780,   781,     0,     0,     0,     0,
     782,     0,   784,   785,     0,     0,     0,     0,   788,   789,
     790,     0,     0,     0,   794,     0,     0,     0,   766,   767,
     768,   769,   770,     0,     0,   773,   774,   775,   776,     0,
     778,   779,   780,   781,   756,   757,     0,     0,   782,     0,
     784,   785,     0,     0,     0,     0,   788,   789,   790,     0,
       0,     0,   794,     0,     0,     0,     0,   796,     0,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   756,
     757,     0,     0,     0,     0,     0,     0,     0,   807,   808,
       0,     0,  1188,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   796,     0,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   807,   808,     0,     0,
    1189,     0,     0,     0,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,     0,     0,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
     756,   757,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,     0,   794,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   756,   757,
       0,     0,     0,     0,     0,     0,     0,   807,   808,     0,
       0,  1190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   807,   808,     0,     0,  1318,     0,     0,     0,
       0,     0,     0,     0,     0,   766,   767,   768,   769,   770,
       0,     0,   773,   774,   775,   776,     0,   778,   779,   780,
     781,     0,     0,     0,     0,   782,     0,   784,   785,     0,
       0,     0,     0,   788,   789,   790,     0,     0,     0,   794,
       0,     0,     0,   766,   767,   768,   769,   770,     0,     0,
     773,   774,   775,   776,     0,   778,   779,   780,   781,   756,
     757,     0,     0,   782,     0,   784,   785,     0,     0,     0,
       0,   788,   789,   790,     0,     0,     0,   794,     0,     0,
       0,     0,   796,     0,   797,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   756,   757,     0,     0,     0,     0,
       0,     0,     0,   807,   808,     0,     0,  1331,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     796,     0,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   807,   808,     0,     0,  1334,     0,     0,     0,     0,
       0,     0,     0,     0,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
       0,     0,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,     0,   794,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,   756,   757,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   756,   757,     0,     0,     0,     0,     0,
       0,     0,   807,   808,     0,     0,  1483,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   807,   808,     0,
       0,  1500,     0,     0,     0,     0,     0,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   773,   774,   775,
     776,     0,   778,   779,   780,   781,     0,     0,     0,     0,
     782,     0,   784,   785,     0,     0,     0,     0,   788,   789,
     790,     0,     0,     0,   794,     0,     0,     0,   766,   767,
     768,   769,   770,     0,     0,   773,   774,   775,   776,     0,
     778,   779,   780,   781,   756,   757,     0,     0,   782,     0,
     784,   785,     0,     0,     0,     0,   788,   789,   790,     0,
       0,     0,   794,     0,     0,     0,     0,   796,     0,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   756,
     757,     0,     0,     0,     0,     0,     0,     0,   807,   808,
       0,     0,  1501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   796,     0,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   807,   808,     0,     0,
    1524,     0,     0,     0,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,     0,     0,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
     756,   757,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,     0,   794,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   756,   757,
       0,     0,     0,     0,     0,     0,     0,   807,   808,     0,
       0,  1526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   807,   808,     0,     0,  1528,     0,     0,     0,
       0,     0,     0,     0,     0,   766,   767,   768,   769,   770,
       0,     0,   773,   774,   775,   776,     0,   778,   779,   780,
     781,     0,     0,     0,     0,   782,     0,   784,   785,     0,
       0,     0,     0,   788,   789,   790,     0,     0,     0,   794,
       0,     0,     0,   766,   767,   768,   769,   770,     0,     0,
     773,   774,   775,   776,     0,   778,   779,   780,   781,   756,
     757,     0,     0,   782,     0,   784,   785,     0,     0,     0,
       0,   788,   789,   790,     0,     0,     0,   794,     0,     0,
       0,     0,   796,     0,   797,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   756,   757,     0,     0,     0,     0,
       0,     0,     0,   807,   808,     0,     0,  1532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     796,     0,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   807,   808,     0,     0,  1610,     0,     0,     0,     0,
       0,     0,     0,     0,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
       0,     0,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,     0,   794,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,   756,   757,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   756,   757,     0,     0,     0,     0,     0,
       0,     0,   807,   808,     0,     0,  1611,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   807,   808,     0,
       0,  1613,     0,     0,     0,     0,     0,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   773,   774,   775,
     776,     0,   778,   779,   780,   781,     0,     0,     0,     0,
     782,     0,   784,   785,     0,     0,     0,     0,   788,   789,
     790,     0,     0,     0,   794,     0,     0,     0,   766,   767,
     768,   769,   770,     0,     0,   773,   774,   775,   776,     0,
     778,   779,   780,   781,   756,   757,     0,     0,   782,     0,
     784,   785,     0,     0,     0,     0,   788,   789,   790,     0,
       0,     0,   794,     0,     0,     0,     0,   796,     0,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   756,
     757,     0,     0,     0,     0,     0,     0,     0,   807,   808,
       0,     0,  1621,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   796,     0,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   807,   808,     0,     0,
    1623,     0,     0,     0,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,     0,     0,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
     756,   757,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,     0,   794,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   756,   757,
       0,     0,     0,     0,     0,     0,     0,   807,   808,     0,
       0,  1634,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   807,   808,     0,     0,  1639,     0,     0,     0,
       0,     0,     0,     0,     0,   766,   767,   768,   769,   770,
       0,     0,   773,   774,   775,   776,     0,   778,   779,   780,
     781,     0,     0,     0,     0,   782,     0,   784,   785,     0,
       0,     0,     0,   788,   789,   790,     0,     0,     0,   794,
       0,     0,     0,   766,   767,   768,   769,   770,     0,     0,
     773,   774,   775,   776,     0,   778,   779,   780,   781,   756,
     757,     0,     0,   782,     0,   784,   785,     0,     0,     0,
       0,   788,   789,   790,     0,     0,     0,   794,     0,     0,
       0,     0,   796,     0,   797,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   756,   757,     0,     0,     0,     0,
       0,     0,     0,   807,   808,     0,     0,  1712,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     796,     0,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   807,   808,     0,     0,  1713,     0,     0,     0,     0,
       0,     0,     0,     0,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
       0,     0,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,     0,   794,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,   756,   757,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   756,   757,     0,     0,     0,     0,     0,
       0,     0,   807,   808,   850,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   807,   808,  1119,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   773,   774,   775,
     776,     0,   778,   779,   780,   781,     0,     0,     0,     0,
     782,     0,   784,   785,     0,     0,     0,     0,   788,   789,
     790,     0,     0,     0,   794,     0,     0,     0,   766,   767,
     768,   769,   770,     0,     0,   773,   774,   775,   776,     0,
     778,   779,   780,   781,   756,   757,     0,     0,   782,     0,
     784,   785,     0,     0,     0,     0,   788,   789,   790,     0,
       0,     0,   794,     0,     0,     0,     0,   796,     0,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   756,
     757,     0,     0,     0,     0,     0,     0,     0,   807,   808,
    1271,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   796,     0,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   806,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   807,   808,  1287,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   773,   774,   775,   776,
       0,   778,   779,   780,   781,     0,     0,     0,     0,   782,
       0,   784,   785,     0,     0,     0,     0,   788,   789,   790,
       0,     0,     0,   794,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
     245,   246,     0,     0,   782,     0,   784,   785,     0,     0,
       0,     0,   788,   789,   790,     0,     0,   247,   794,     0,
       0,     0,     0,     0,     0,     0,   796,     0,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,     0,     0,
       0,     0,   756,   757,     0,     0,     0,   807,   808,  1427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   807,   808,  1433,     0,     0,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,     0,     0,   266,   267,   268,
       0,     0,     0,     0,     0,     0,   269,   270,   271,   272,
     273,     0,     0,   274,   275,   276,   277,   278,   279,   280,
       0,     0,     0,     0,     0,   756,   757,   766,   767,   768,
     769,   770,     0,     0,   773,   774,   775,   776,     0,   778,
     779,   780,   781,     0,     0,     0,     0,   782,     0,   784,
     785,     0,     0,     0,     0,   788,   789,   790,     0,     0,
       0,   794,   281,     0,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,    50,     0,   292,   293,     0,     0,
       0,     0,     0,   294,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   796,     0,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   806,     0,     0,   756,   757,
     766,   767,   768,   769,   770,   807,   808,   773,   774,   775,
     776,     0,   778,   779,   780,   781,     0,     0,     0,     0,
     782,     0,   784,   785,     0,     0,     0,     0,   788,   789,
     790,     0,     0,     0,   794,     0,     0,     0,   756,   757,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    13,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,   796,     0,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,     0,
       0,     0,     0,   766,   767,   768,   769,   770,   807,   808,
     773,   774,   775,   776,     0,   778,   779,   780,   781,     0,
       0,     0,     0,   782,     0,   784,   785,     0,     0,   953,
       0,   788,   789,   790,     0,     0,     0,   794,     0,     0,
       0,   756,   757,   766,   767,   768,   769,   770,     0,     0,
     773,   774,   775,   776,     0,   778,   779,   780,   781,     0,
       0,     0,     0,   782,     0,   784,   785,     0,     0,     0,
       0,   788,   789,   790,     0,     0,     0,   794,   756,   757,
     796,     0,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   186,     0,     0,     0,     0,     0,     0,     0,
       0,   807,   808,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     796,     0,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,     0,     0,     0,     0,   766,   767,   768,   769,
     770,   807,   808,   773,   774,   775,   776,     0,   778,   779,
     780,   781,     0,     0,     0,     0,   782,     0,   784,   785,
       0,     0,  1199,     0,   788,   789,   790,     0,     0,     0,
     794,   756,   757,   766,   767,   768,   769,   770,     0,     0,
     773,   774,   775,   776,     0,   778,   779,   780,   781,     0,
       0,     0,     0,   782,     0,   784,   785,     0,     0,     0,
       0,   788,   789,   790,     0,     0,     0,   794,     0,   756,
     757,     0,     0,   796,     0,   797,   798,   799,   800,   801,
     802,   803,   804,   805,   806,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   807,   808,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     796,  1276,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,     0,     0,     0,     0,   766,   767,   768,   769,
     770,   807,   808,   773,   774,   775,   776,     0,   778,   779,
     780,   781,     0,     0,     0,     0,   782,     0,   784,   785,
       0,     0,     0,     0,   788,   789,   790,     0,     0,     0,
     794,     0,   756,   757,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
       0,     0,     0,     0,   782,     0,   784,   785,     0,     0,
       0,  1381,   788,   789,   790,     0,     0,     0,   794,   756,
     757,     0,     0,   796,     0,   797,   798,   799,   800,   801,
     802,   803,   804,   805,   806,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   807,   808,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   796,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,     0,     0,     0,     0,   766,   767,   768,
     769,   770,   807,   808,   773,   774,   775,   776,     0,   778,
     779,   780,   781,     0,     0,     0,     0,   782,     0,   784,
     785,     0,     0,     0,     0,   788,   789,   790,     0,     0,
       0,  -771,   756,   757,   766,   767,   768,   769,   770,     0,
       0,   773,   774,   775,   776,     0,   778,   779,   780,   781,
       0,     0,     0,     0,   782,     0,   784,   785,   756,   757,
       0,     0,   788,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   796,     0,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   756,   757,     0,     0,
       0,     0,     0,     0,     0,   807,   808,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,     0,     0,     0,     0,   766,   767,   768,
     769,   770,   807,   808,   773,   774,   775,   776,     0,   778,
     779,   780,   781,     0,     0,     0,     0,   782,     0,   784,
     785,     0,     0,   766,   767,   768,   769,   770,     0,     0,
     773,   774,   775,   776,     0,   778,   779,   780,   781,     0,
       0,     0,     0,   782,     0,   784,   785,     0,     0,     0,
       0,   766,   767,   768,   769,   770,     0,     0,   773,     0,
       0,   776,     0,   778,   779,   780,   781,     0,     0,     0,
       0,   782,     0,   784,   785,     0,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   806,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   807,   808,     0,     0,   997,
       0,     0,     0,     0,   799,   800,   801,   802,   803,   804,
     805,   806,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   807,   808,     0,     0,     0,     0,     0,     0,     0,
    1001,     0,     0,   800,   801,   802,   803,   804,   805,   806,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   807,
     808,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   481,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,     0,   358,
     359,   360,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1272,
       0,     0,     0,     0,     0,     0,     0,     0,   998,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   999,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1002,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   331,   332,   333,  1003,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   481,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,   194,   358,
     359,   360,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1273,     0,
    1076,  1077,     0,     0,   195,     0,   196,     0,   197,   198,
     199,   200,   201,  1274,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,     0,   213,   214,   215,  1078,
       0,   216,   217,   218,   219,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1079,     0,     0,     0,     0,     0,
       0,   220,   221,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1080,  1081,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222
};

static const yytype_int16 yycheck[] =
{
       1,   230,   188,     7,   590,   224,   732,   390,   437,   438,
     610,   644,   500,   540,   740,    15,    16,   609,   171,    20,
     398,   836,   557,   838,   240,   840,   604,   713,   606,   928,
     608,   390,  1202,   489,   490,   873,    86,  1341,   944,    20,
     649,     5,     8,   166,   950,   580,     4,   160,    33,    22,
      62,    19,    20,    15,    16,   109,   127,    53,    53,   192,
    1406,  1448,   461,   143,    33,   127,    20,    20,    68,    69,
      70,    20,    33,    20,    57,   137,    20,    20,   165,   152,
      63,   711,   311,   713,    57,   240,   485,    20,   129,   130,
      20,    60,    61,   173,    20,    96,    20,    98,   185,   191,
     173,   710,   461,   712,     7,   714,   127,   220,    46,   191,
     110,   111,   112,   113,   152,   745,   137,    34,   127,   728,
       0,   192,   848,   191,   195,   129,   485,     7,   137,   738,
     192,  1586,   152,   220,   143,   173,   218,   217,   102,   103,
     160,   144,   145,   146,   217,   983,    63,    50,   149,   958,
      30,   960,    32,   173,    34,   124,   882,    15,    16,   128,
      40,   390,   315,   158,   168,   700,  1553,  1513,   106,   165,
      50,   192,   388,   214,   215,   194,    56,   231,   173,   217,
     906,   164,   232,   192,   158,   189,   190,   191,   404,   185,
     107,  1361,  1579,   919,  1649,   594,   922,   217,   164,   173,
      80,   184,   160,   632,   166,   173,   392,   436,   637,   171,
     191,   173,   197,   173,   176,   218,   143,   433,   184,   136,
     224,   437,   102,   103,   193,  1614,   143,   217,   197,   143,
     203,   200,   233,   388,   198,   594,   197,   191,   191,   214,
     240,   214,   191,   873,   191,   464,   215,   191,   191,   215,
     404,  1555,   221,   217,   215,   217,   173,   173,   191,   478,
     221,   191,   871,   127,   663,   191,   158,   191,   318,   163,
       5,     6,   155,     8,   173,   173,   127,   160,  1667,   433,
     194,   173,   498,   499,  1183,   165,   137,   127,    53,   183,
     194,   208,  1198,   197,   693,  1133,   165,   137,   754,   143,
     217,    36,   158,   219,   663,   185,   310,   166,   166,   313,
     127,    21,    22,   171,  1484,   173,   185,   173,   176,   217,
     137,   195,   173,   217,   204,   199,   218,   191,   192,   173,
     194,   461,    97,   197,   693,   215,   126,   220,   127,   129,
     130,   192,  1474,  1242,   498,   499,   197,   576,   137,   218,
     165,   580,   192,   983,   984,   964,   965,   142,   967,   195,
    1575,   217,  1577,   199,   152,   365,   173,   191,  1583,   132,
     185,   155,   107,   188,   752,   192,   160,   198,   163,   595,
     596,   597,  1597,   599,   600,   173,   387,   603,   388,   605,
     520,   607,   216,   609,   394,   581,   217,  1296,   183,   828,
     173,   216,   165,   192,   404,   143,   635,   163,   118,   119,
     639,  1543,  1544,   842,   152,   844,   126,   995,   128,   129,
     130,   131,   185,   822,   214,   215,   136,   183,   152,   648,
    1562,   191,  1647,   433,   218,   173,   220,   437,   194,  1619,
     595,   596,   597,   177,   599,   600,  1261,   982,   603,   173,
     605,  1631,   607,   669,  1360,   674,   216,   457,   458,   163,
     464,    33,   163,   822,   173,   195,  1372,   866,   956,   199,
     173,   174,   175,   474,   478,   904,   692,  1692,  1693,   183,
     936,   143,   183,   613,   488,   489,   490,   191,    60,    61,
     152,   979,  1125,  1625,  1626,   157,  1676,  1677,   498,   499,
     719,   204,   165,  1133,   214,   215,   158,   866,   158,   165,
     191,   173,  1198,  1728,  1114,   669,   165,   736,   219,   220,
    1112,   173,   185,   173,   525,   526,   165,  1213,   191,   185,
     188,   189,   662,   663,   191,   191,   198,   218,   692,  1354,
     756,   757,   543,   199,   545,   546,   547,   165,   549,  1005,
    1006,  1007,   124,   192,   165,   127,   128,   687,   216,   560,
     690,   218,   165,   173,   165,   137,   782,   185,  1198,   191,
     144,  1201,   146,   191,   185,  1390,   191,   191,    21,    22,
     191,   199,   185,  1213,   185,   191,   173,   191,   199,   590,
     191,   807,   191,   165,   216,   595,   596,   597,   199,   599,
     600,   216,   818,   603,   218,   605,  1184,   607,   191,   609,
     216,   165,   165,   185,   218,   745,   191,   216,    57,  1228,
     192,   193,   163,    57,    63,   197,   812,   173,   200,    63,
     846,   185,   185,   165,   165,   218,   852,   191,   191,   855,
     164,   216,   183,   215,   648,   199,   199,   191,   864,   221,
      33,   867,   868,   185,   185,   655,   872,  1235,   158,   165,
     184,   877,   164,   816,   818,   881,    57,   199,   199,   669,
     674,   173,   191,   173,   218,   118,   119,    60,    61,   185,
     810,   164,   184,   126,   903,   191,   129,   130,   131,   195,
     177,   846,   692,   136,  1509,   911,   912,   852,   914,   218,
     855,   184,   918,   191,   920,   921,   165,   923,   191,   864,
      68,    69,    70,   867,   868,   719,   717,   872,   144,   191,
     191,   191,   877,   192,   191,   184,   185,   881,   187,   191,
     218,  1364,   736,   733,   191,   191,   191,   741,   191,    47,
     106,   124,   958,   873,   960,   128,   218,   218,   218,   216,
     754,   980,   110,   111,   112,   113,   218,   911,   912,    67,
     914,   218,   218,   218,   918,   218,   920,   921,   198,   923,
      75,   214,   215,  1403,    79,    35,   158,   165,   165,   194,
      35,   198,    21,    22,   199,   173,   173,   217,    93,    94,
     976,   173,   194,    98,    99,   100,   101,   199,   185,  1429,
     217,   194,   217,   958,   191,   960,   199,   217,   165,   173,
     193,   941,   198,   813,   197,   217,   199,   200,   818,   949,
     194,   198,  1400,   824,   217,   199,  1353,   184,   185,   186,
      12,   217,   215,   834,   177,   178,   179,   180,   221,    57,
     217,    23,    24,   217,   194,    63,   846,   197,  1064,   216,
    1483,   191,   852,   184,   194,   855,   187,   197,  1241,   190,
      57,     5,     6,    57,   864,    57,    63,   867,   868,    63,
      57,    63,   872,   173,   195,   173,    63,   877,   199,   118,
     119,   881,  1241,   217,  1100,   219,   220,   126,   185,   128,
     129,   130,   131,   173,   191,    22,  1112,   136,   173,   903,
     185,  1330,   185,    33,  1630,   216,   191,   185,   191,   185,
     194,   911,   912,   191,   914,   191,  1642,   194,   918,   185,
     920,   921,   217,   923,    43,   191,  1142,  1143,  1144,  1515,
      60,    61,   936,  1563,   177,   178,   195,   195,    21,    22,
     199,   199,   198,   944,  1670,  1100,   947,   198,   195,   950,
     951,  1167,   199,   195,   198,   195,   217,   199,   958,   199,
     960,   200,   201,   202,   203,   204,   195,     5,     6,   195,
     199,   219,   220,   199,   198,   214,   215,   144,   145,   146,
     173,   174,   175,    10,    11,    12,   198,    25,  1142,  1143,
    1144,    92,    93,    31,   124,   217,   198,  1627,   128,   198,
    1130,  1005,  1006,  1007,  1492,   195,  1639,   177,   178,   179,
     198,   217,  1241,  1167,   198,  1601,  1602,   198,  1234,   198,
    1508,   177,   178,   179,   173,   137,   217,   216,   173,   215,
      68,    69,   173,   116,   117,   118,   119,   120,  1668,   173,
     123,   124,   125,   126,   165,   128,   129,   130,   131,   217,
     198,   198,   198,   136,   217,   138,   139,   173,   198,   198,
     217,  1191,   198,   193,   102,   103,   390,   197,   166,   199,
     200,  1657,   198,   198,   198,   217,    13,   220,   198,   217,
    1234,    10,  1212,    33,   217,   215,   217,    66,  1218,   217,
    1091,   221,    37,   198,  1095,   217,   198,  1227,   215,   217,
    1100,  1317,   217,    43,  1323,   143,  1736,   218,  1238,  1295,
      60,    61,  1112,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   160,   161,   162,   163,   173,   173,   216,   191,
     194,   214,   215,   217,    43,   173,   217,   461,  1139,  1269,
     217,   198,  1142,  1143,  1144,   183,   198,   198,   191,   132,
      14,  1281,   192,   194,   166,  1643,  1286,   220,   217,    13,
     198,   485,  1317,   191,   173,   191,     8,  1167,   173,   199,
     218,   173,  1391,   173,   124,   173,   218,   215,   128,   218,
     217,   191,    21,    22,   191,   184,   759,   760,   761,   762,
     763,   764,   765,   216,  1324,   217,  1415,   217,   771,   772,
    1416,   218,  1418,   173,   777,   198,   217,   198,     1,   217,
      67,   217,   173,   786,   787,   165,   217,    12,   791,   792,
     793,   217,   795,   217,   199,  1355,    21,    22,   217,   217,
     199,   199,    43,   218,  1234,   185,   218,   173,   218,   217,
     217,   191,   218,   193,   218,   216,   218,   197,   173,   199,
     200,  1437,  1382,  1383,   216,   173,  1386,   216,   173,   217,
    1479,   173,  1416,  1418,   198,   215,   217,   173,   217,   217,
     594,   221,   218,   217,   217,   173,   217,   116,   117,   118,
     119,   216,   173,   173,   216,   218,  1505,   126,  1289,   128,
     129,   130,   131,   216,    33,   217,   217,   136,  1514,   138,
     139,   217,   173,   199,   217,    70,   199,   218,   218,   185,
     217,   199,   217,  1529,   217,   217,   217,  1317,  1448,  1323,
     217,   116,   117,   118,   119,   120,  1545,   185,   123,   124,
     125,   126,   218,   128,   129,   130,   131,   217,   217,   663,
     217,   136,  1343,   138,   139,   218,   218,    53,   218,   144,
     145,   146,   218,   218,   216,   150,   184,   191,   185,  1360,
    1514,   200,   201,   202,   203,   204,   216,   191,  1498,   693,
     218,  1372,   218,   218,  1529,   214,   215,   218,   218,  1598,
     216,   216,   184,   721,  1600,   218,   216,  1391,   217,    85,
       1,    46,   218,   218,   216,   216,  1397,   141,   193,    33,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      88,  1415,  1641,  1588,   813,  1506,  1416,   981,  1418,   214,
     215,   238,  1506,  1553,     1,  1654,    60,    61,  1506,  1559,
    1506,  1506,   990,  1479,   758,   759,   760,   761,   762,   763,
     764,   765,  1413,  1253,  1540,  1439,  1600,   771,   772,  1579,
    1442,  1292,  1541,   777,   446,    56,  1627,  1490,  1210,  1589,
    1689,   391,   786,   787,   713,   535,    -1,   791,   792,   793,
     457,   795,   435,   457,    -1,  1479,    -1,    -1,    -1,    -1,
      -1,    -1,   390,    -1,    -1,    -1,    -1,  1706,    -1,    -1,
     124,    -1,    -1,   401,   128,    -1,    -1,    -1,   822,    -1,
      -1,  1505,   410,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,   420,    -1,  1514,    -1,    -1,    -1,    -1,  1705,
      -1,  1651,   430,    21,    22,    -1,    -1,    -1,    -1,  1529,
      -1,    -1,    -1,   441,    -1,    60,    61,    -1,    -1,    -1,
      -1,  1545,   866,   451,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   461,    -1,    -1,    -1,    -1,   466,   193,
     468,    -1,    -1,   197,    -1,   199,   200,  1568,    -1,   477,
      -1,   488,   489,   490,  1575,    -1,  1577,   485,   486,   487,
      -1,   215,  1583,    -1,    -1,    -1,  1586,   221,    -1,    -1,
      -1,    -1,   500,    -1,  1598,    -1,  1597,    -1,    -1,   124,
    1600,   509,   127,   128,    -1,    -1,   514,   515,   516,   517,
     518,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,   527,
     118,   119,    -1,   540,    -1,    -1,    -1,   535,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    21,    22,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1647,    -1,    -1,  1649,
     567,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1660,
      -1,    33,    -1,    -1,    -1,  1666,    -1,   192,   193,   577,
      -1,    -1,   197,    -1,    -1,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   594,    -1,    60,    61,
     215,  1692,  1693,    -1,    -1,    -1,   221,    -1,    -1,    -1,
    1701,  1702,  1706,    -1,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   622,    -1,   214,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1728,    -1,  1730,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
     667,   136,   124,   138,   139,   663,   128,    -1,    -1,   144,
     145,   146,   670,   671,    -1,   673,    -1,    -1,    -1,    -1,
     678,    -1,    -1,    -1,    -1,    -1,   684,    -1,   695,    -1,
      -1,    -1,    -1,    -1,    -1,   693,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   704,   705,   706,   707,
     708,   709,    -1,   711,    -1,   713,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,   193,    -1,    21,    22,   197,    -1,   199,   200,   214,
     215,    -1,    -1,    -1,    -1,    -1,    -1,   754,    -1,    -1,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,   221,
     758,   759,   760,   761,   762,   763,   764,   765,   766,   767,
      -1,    -1,   770,   771,   772,   773,   774,   775,    -1,   777,
     778,    -1,   780,   781,   782,   783,   784,   785,   786,   787,
     788,   789,   790,   791,   792,   793,   794,   795,   796,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   806,    -1,
     808,    -1,    -1,    -1,    -1,    -1,    -1,   824,    -1,    -1,
      -1,    -1,    -1,    -1,   822,    -1,    33,  1241,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    60,    61,    -1,   144,   145,   146,    -1,
      -1,   859,   150,    -1,    -1,    -1,    -1,    -1,   866,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   874,    -1,   876,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   885,   886,   887,
     888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
     898,   899,   900,   901,   902,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   124,    -1,   926,
      -1,   128,    -1,    -1,    -1,    33,   214,   215,    -1,   936,
      -1,   219,   220,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,   939,   940,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    -1,   953,    -1,    -1,   956,    -1,
      -1,    60,    61,    -1,   962,    -1,    -1,    -1,    -1,    -1,
     968,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   977,
      -1,   979,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,
     197,    -1,   199,   200,    -1,    -1,    -1,    -1,  1005,  1006,
    1007,    -1,    -1,  1010,    -1,  1012,    -1,  1014,   215,  1016,
      -1,  1018,    -1,  1020,   221,  1022,   124,  1024,    -1,    -1,
     128,    -1,  1029,    -1,  1031,   124,    -1,    -1,    10,   128,
    1037,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,  1049,    -1,  1051,    -1,    -1,    -1,    -1,  1056,
      -1,  1058,    -1,  1060,    -1,    -1,  1063,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1077,
      -1,    -1,    -1,  1081,    -1,   193,    -1,    33,  1095,   197,
      -1,   199,   200,    -1,   193,    -1,    -1,    -1,   197,    -1,
     199,   200,    -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,   221,    60,    61,   215,    -1,    -1,    -1,
    1127,    -1,   221,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    -1,    -1,  1171,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1193,  1194,  1195,    -1,    -1,
      -1,  1199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1207,
     192,   193,  1210,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,   214,   215,  1232,  1233,    19,   219,   220,    -1,
      -1,    -1,    25,  1241,    -1,    -1,    -1,   193,    31,  1247,
      -1,   197,    -1,   199,   200,    -1,    -1,    -1,    41,    -1,
      21,    22,    -1,    -1,    -1,  1263,    49,  1265,    -1,   215,
      -1,    -1,  1270,    -1,    -1,   221,    -1,    -1,  1276,    -1,
      -1,    64,  1280,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,  1320,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1341,    -1,    -1,  1353,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     143,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,   156,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
     173,    -1,    33,    -1,    -1,    -1,  1394,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    60,
      61,    21,    22,    -1,    -1,    -1,  1424,    -1,  1426,    -1,
      -1,    -1,    -1,    -1,  1432,    -1,   219,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,
    1458,    -1,  1460,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,  1470,  1471,  1472,    -1,    -1,    -1,   124,  1477,
      -1,    -1,   128,   124,  1482,    -1,    -1,   128,  1486,  1487,
    1488,  1489,    -1,    -1,  1492,  1493,    60,    61,    -1,  1497,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1506,    -1,
    1508,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,  1520,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,   193,    -1,    -1,
     150,   197,   193,   199,   200,    -1,   197,  1555,   199,   200,
     124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,   215,
      -1,  1569,  1570,    -1,   215,   221,    -1,    -1,    -1,    -1,
     221,    -1,    -1,    -1,    -1,    -1,    -1,  1585,    -1,    -1,
      -1,    -1,  1590,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,  1603,    -1,  1605,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,  1622,    -1,  1624,    -1,    -1,   193,
      -1,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1643,  1644,    -1,    -1,    -1,
      -1,   215,    -1,    -1,  1652,    -1,    -1,   221,    -1,    -1,
    1658,  1659,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,  1684,    -1,  1686,  1687,
      25,    26,    27,    28,    29,    -1,    31,  1695,    -1,    -1,
      -1,    -1,  1700,    38,    39,    -1,    -1,    42,    -1,    44,
      45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,    -1,   219,   220,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,    -1,     5,     6,    -1,   215,    -1,   217,    -1,
     219,   220,   221,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    60,    61,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,    -1,    -1,   197,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,
       5,     6,    -1,   215,    -1,   217,    -1,   219,   220,   221,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,    -1,     5,     6,    -1,
     215,    -1,   217,    -1,   219,   220,   221,    15,    16,    17,
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
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,   218,   219,   220,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,   215,     5,     6,   215,    -1,   217,
     221,   219,   220,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      33,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,    -1,   215,     5,     6,   215,   216,   217,   221,   219,
     220,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
      -1,    -1,   197,    -1,   199,   200,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,
     215,     5,     6,   215,    -1,   217,   221,   219,   220,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    33,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,
     197,    -1,   199,   200,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,    -1,   215,     5,
       6,   215,   216,   217,   221,   219,   220,    13,    -1,    15,
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
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      70,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,   216,   217,    -1,   219,   220,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    58,    -1,    -1,    -1,
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
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,
      -1,     5,     6,   215,    -1,   217,    -1,   219,   220,    13,
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
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,    -1,   219,   220,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,
     198,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,   215,     5,     6,   215,    -1,   217,
     221,   219,   220,    13,    -1,    15,    16,    17,    18,    19,
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
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    22,    -1,
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
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,    -1,   219,   220,    15,    16,    17,
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
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
     218,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
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
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,   218,   219,   220,    15,    16,    17,    18,    19,
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
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,   218,   219,   220,    15,    16,    17,
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
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
     218,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,   218,   219,   220,    15,
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
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,   218,   219,   220,    15,    16,    17,    18,    19,
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
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,    -1,   219,   220,    15,    16,    17,
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
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
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
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,
      -1,   217,    -1,   219,   220,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    60,    61,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,   163,    60,    61,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    13,    -1,
      -1,    -1,    -1,   183,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,   124,    41,   207,   208,   128,
      -1,    -1,    -1,    -1,    49,   215,    -1,   217,    -1,   219,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    64,
      -1,    -1,   128,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,
     199,   200,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,   215,   193,    -1,    31,
      -1,   197,   221,   199,   200,    -1,    -1,    -1,   143,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,   215,
      -1,   156,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   173,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   173,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   219,    -1,   221,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    19,
      -1,    -1,    -1,    -1,   173,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,
      -1,    41,   191,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
     219,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    64,    -1,    -1,
      -1,    -1,    -1,   173,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,   173,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,    -1,
     146,    -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,    -1,   219,   136,   137,   138,   139,   140,   141,    21,
      22,   144,   145,   146,   147,   148,   149,   150,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,    -1,   219,   220,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,    -1,   136,   137,   138,   139,   140,   141,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
     192,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,   219,   220,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,   216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    38,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,   214,   215,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,   216,    -1,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,   128,   129,   130,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,
     141,    -1,    -1,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,    -1,    -1,    21,    22,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   163,    -1,   207,   208,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    21,    22,
     116,   117,   118,   119,   120,   214,   215,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,   214,   215,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,   142,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    21,    22,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    21,    22,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,   142,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    21,
      22,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    21,    22,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,   181,   144,   145,   146,    -1,    -1,    -1,   150,    21,
      22,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   214,   215,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    21,    22,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    21,    22,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   214,   215,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,    -1,
      -1,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    19,
      -1,    -1,    -1,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,   173,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    35,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
     129,   130,    -1,    -1,    71,    -1,    73,    -1,    75,    76,
      77,    78,    79,   173,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,   158,
      -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   225,   228,
     234,   236,   237,   242,   273,   277,   313,   392,   399,   403,
     410,   456,   461,   466,    19,    20,   173,   265,   266,   267,
     166,   243,   244,   173,   174,   175,   204,   238,   239,   240,
     163,   183,   282,   400,   173,   219,   227,    57,    63,   395,
     395,   395,   143,   173,   299,    34,    63,   107,   136,   208,
     217,   269,   270,   271,   272,   299,   224,     5,     6,     8,
      36,   107,   407,    62,   390,   192,   191,   194,   191,   239,
      22,    57,   203,   214,   241,   395,   396,   398,   396,   390,
     467,   457,   462,   173,   143,   235,   271,   271,   271,   217,
     144,   145,   146,   191,   216,    57,    63,   278,   280,    57,
      63,   401,     5,     6,    57,    63,   408,    57,    63,   391,
      15,    16,   166,   171,   173,   176,   217,   230,   266,   166,
     244,   173,   238,   238,   173,   224,   164,   184,   283,   396,
     224,    57,    63,   226,   173,   173,   173,   173,   177,   233,
     218,   267,   271,   271,   271,   271,   281,   173,   402,   411,
     282,   393,   177,   178,   179,   229,    15,    16,   166,   171,
     173,   230,   263,   264,   241,   397,   165,   369,   224,   468,
     458,   463,   177,   218,    35,    71,    73,    75,    76,    77,
      78,    79,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    93,    94,    95,    98,    99,   100,   101,
     118,   119,   173,   276,   279,   194,   282,   106,   405,   406,
     388,   160,   220,   268,   363,   177,   178,   179,   191,   218,
     192,   369,   282,   282,   282,    21,    22,    38,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   128,   129,   130,   137,
     138,   139,   140,   141,   144,   145,   146,   147,   148,   149,
     150,   193,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   207,   208,   214,   215,    35,    35,   217,   274,
     282,   284,    75,    79,    93,    94,    98,    99,   100,   101,
     415,   394,   173,   412,   283,   389,   267,   266,   220,   224,
     152,   173,   386,   387,   263,    19,    25,    31,    41,    49,
      64,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   156,   219,   299,   414,   416,   417,
     421,   427,   455,   420,   459,   464,   173,   173,   173,   216,
      22,   173,   216,   155,   218,   363,   374,   375,   194,   275,
     287,   282,   173,   283,   194,   404,   282,   409,   363,   216,
     266,   217,    43,   191,   194,   197,   385,   198,   198,   198,
     217,   198,   198,   217,   198,   198,   198,   198,   198,   198,
     217,   299,    33,    60,    61,   124,   128,   193,   197,   200,
     215,   221,   426,   195,   165,   173,   419,   378,   381,   173,
     137,   217,     7,    50,   312,   218,   224,   455,     1,     5,
       6,     9,    10,    11,    13,    15,    16,    17,    18,    19,
      25,    26,    27,    28,    29,    31,    38,    39,    42,    44,
      45,    48,    51,    52,    54,    55,    58,    59,    65,    68,
      69,    80,   102,   103,   104,   105,   118,   119,   133,   134,
     135,   151,   152,   153,   154,   155,   157,   159,   160,   161,
     162,   165,   166,   167,   168,   169,   170,   171,   172,   174,
     175,   176,   183,   185,   200,   201,   202,   207,   208,   215,
     217,   219,   220,   232,   234,   245,   246,   249,   252,   253,
     255,   258,   259,   260,   261,   282,   283,   285,   286,   288,
     293,   298,   299,   300,   304,   305,   306,   307,   308,   309,
     310,   311,   313,   317,   318,   325,   328,   331,   336,   339,
     340,   342,   343,   344,   346,   351,   354,   355,   362,   414,
     469,   484,   495,   499,   512,   515,   394,   192,   369,   299,
     370,   387,   216,    65,   104,   174,   293,   355,   173,   173,
     427,   127,   137,   192,   384,   428,   433,   435,   355,   437,
     431,   173,   422,   439,   441,   443,   445,   447,   449,   451,
     453,   355,   198,   217,    33,   197,    33,   197,   215,   221,
     216,   355,   215,   221,   427,   165,   419,   191,   283,   173,
     191,   224,   376,   424,   455,   460,   173,   379,   424,   465,
     355,   152,   173,   383,   413,   374,   198,   198,   355,   256,
     257,   198,   301,   416,   469,   217,   299,   198,     5,   102,
     103,   198,   217,   127,   298,   329,   340,   355,   284,   198,
     217,    61,   355,   217,   355,   173,   198,   198,   217,   224,
     198,   166,    58,   355,   217,   284,   198,   217,   198,   198,
     217,   198,   198,   127,   298,   355,   355,   355,   220,   284,
     331,   335,   335,   335,   217,   217,   217,   217,   217,   217,
      13,   427,    13,   427,    13,   355,   494,   510,   198,   355,
     198,   231,    13,   355,   355,   355,   355,   355,    13,    49,
     329,   355,   329,   220,   224,   224,   355,    10,    13,   494,
     511,    37,   331,   337,   173,   217,   224,   224,   224,   224,
     224,    66,   314,   273,   132,   224,    21,    22,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   136,   137,   138,   139,   140,   141,   144,   145,
     146,   147,   148,   149,   150,   192,   193,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   214,   215,   337,
     217,   283,   355,   215,   273,   283,   371,   369,   198,   218,
      43,   224,   384,   298,   355,   455,   455,   425,   455,   218,
     455,   455,   218,   173,   418,   455,   274,   455,   274,   455,
     274,   376,   377,   379,   380,   218,   430,   289,   329,   216,
     216,   355,   194,   424,   283,   194,   424,   283,   218,   217,
      43,   127,   191,   192,   194,   197,   382,   485,   487,   284,
     413,   217,   302,   217,   299,   198,   217,   326,   198,   198,
     198,   506,   329,   298,   329,   191,   108,   109,   110,   111,
     112,   113,   114,   115,   121,   122,   127,   140,   141,   147,
     148,   149,   192,    14,   427,   291,   511,   355,   355,   284,
     192,   319,   321,   355,   323,   194,   166,   355,   508,   329,
     491,   496,   329,   489,   427,   298,   355,   220,   273,   355,
     355,   355,   355,   355,   355,   413,    53,   158,   173,   200,
     215,   217,   355,   470,   473,   477,   493,   498,   413,   217,
     473,   498,   413,   142,   184,   186,   224,   478,   294,   284,
     296,   179,   180,   229,   217,   217,   413,    13,   191,   514,
     514,   152,   157,   198,   299,   345,   284,   254,   413,   191,
     514,   282,   338,    70,   215,   218,   329,   470,   472,   160,
     217,   316,   387,     4,   160,   334,   335,    19,   158,   173,
     414,    19,   158,   173,   414,   133,   134,   135,   285,   341,
     355,   341,   355,   341,   355,   341,   355,   341,   355,   341,
     355,   341,   355,   341,   355,   355,   355,   355,   341,   355,
     341,   355,   355,   355,   355,   173,   341,   355,   355,   158,
     173,   355,   355,   355,   414,   355,   355,   355,   341,   355,
     341,   355,   355,   355,   355,   341,   355,   341,   355,   341,
     355,   355,   341,   355,    22,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   129,   130,   158,   173,
     214,   215,   352,   414,   355,   218,   329,   369,   272,     8,
     363,   368,   427,   173,   298,   355,   224,   199,   199,   199,
     424,   199,   199,   199,   224,   199,   275,   199,   275,   199,
     275,   199,   424,   199,   424,   292,   455,   218,   514,   216,
     455,   455,   355,   173,   173,   455,   298,   355,   427,   427,
      20,   413,   455,    70,   329,   472,   483,   198,   355,   173,
     355,   455,   500,   502,   504,   427,   514,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   284,   199,   424,   218,   514,
     218,   262,   427,   427,   218,   427,   218,   427,   514,   427,
     427,   514,   427,   199,   334,   218,   218,   218,   218,   218,
     218,    20,   335,   217,   137,   382,   215,   355,   218,   142,
     191,   224,   477,   188,   189,   216,   481,   191,   188,   216,
     224,   480,    20,   218,   477,   184,   187,   479,    20,   355,
     184,   494,   292,   292,   355,   413,   413,    20,   217,   413,
     216,   218,   217,   217,   347,   349,   369,   355,    20,   494,
     283,   287,   273,   173,   218,   472,   470,   191,   218,   218,
     173,   315,   315,   217,   127,   137,   173,   192,   197,   332,
     333,   274,   198,   217,   198,   217,   335,   335,   335,   217,
     217,   216,    19,   158,   173,   414,   194,   158,   173,   355,
     217,   217,   158,   173,   355,     1,   217,   216,   218,   216,
      57,    63,   366,    67,   367,   224,   199,   224,   429,   434,
     436,   455,   438,   432,   423,   173,   440,   199,   444,   199,
     448,   199,   452,   376,   454,   379,   199,   424,   218,    43,
     382,   199,   199,   329,    20,   199,   472,   218,   218,   218,
     173,   218,   199,   224,   218,   199,   427,   427,   427,   199,
     218,   217,   427,   355,   199,   199,   199,   199,   218,   199,
     199,   218,   199,   334,   274,   217,   329,   355,   355,   355,
     473,   477,   355,   158,   173,   470,   481,   216,   355,   216,
     493,   329,   473,   184,   187,   190,   482,   216,   329,   199,
     199,   181,    20,    20,   329,   413,    20,   355,   355,   427,
     274,    12,    23,    24,   247,   248,    12,   250,   329,   283,
     334,   218,   216,   215,   191,   216,   218,   333,   173,   173,
     217,   173,   173,   191,   216,   275,   356,   355,   358,   355,
     218,   329,   355,   198,   217,   355,   217,   216,   355,   215,
     218,   329,   217,   216,   353,   218,   329,   224,    47,   367,
      46,   106,   364,   369,   334,   442,   446,   450,   217,   455,
     173,   355,   486,   488,   284,   329,   303,   218,   199,   424,
     217,   173,   327,   199,   199,   199,   507,   291,   199,   224,
     320,   322,   324,   509,   492,   497,   490,   217,   337,   275,
     218,   329,   185,   218,   477,   481,   217,   137,   382,   185,
     477,   216,   185,   295,   297,   329,   329,   185,    20,   329,
     218,   218,   199,   275,   284,   355,   251,   224,   185,   274,
     218,   470,   173,   216,   194,   385,   218,   173,   332,   216,
     142,   284,   330,   427,   218,   455,   218,   218,   218,   360,
     355,   355,   218,   470,   218,   355,   218,   369,    33,   365,
     364,   366,   289,   217,   217,   218,   355,   173,   355,   199,
     501,   503,   505,   217,   218,   217,   355,   355,   355,   217,
      70,   483,   217,   217,   218,   355,   330,   218,   355,   137,
     382,   481,   355,   355,   355,   355,   482,   494,   355,   217,
     290,   218,   218,   355,   329,   185,   348,   199,   284,    26,
     105,   252,   305,   306,   307,   309,   355,   494,   275,   216,
     194,   385,   427,   384,   218,   127,   355,   199,   199,   455,
     218,   218,   216,   218,   372,   365,   383,   218,   483,   483,
     284,   218,   199,   218,   217,   217,   217,   217,   289,   291,
     329,   483,   470,   471,   218,   224,   513,   355,   355,   218,
     513,   513,   289,   185,   185,   513,   218,   355,   345,   350,
     248,   127,   127,   355,   513,   284,   218,   427,   384,   384,
     355,   355,   357,   359,   199,   218,   279,   373,   217,   470,
     474,   475,   476,   476,   355,   355,   483,   483,   471,   218,
     218,   514,   476,   218,    53,   216,   137,   382,   184,   283,
     191,   514,   494,   355,   216,   185,   513,   345,   355,   283,
     384,   355,   355,   224,   361,   224,   279,   470,   191,   514,
     218,   218,   218,   218,   476,   476,   218,   218,   218,   355,
     216,   355,   355,   216,   283,   218,   513,   513,   355,   216,
     355,   224,   224,   369,   284,   218,   217,   218,   218,   184,
     216,   513,   224,   470,   216,   218
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   222,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   224,   224,   225,
     226,   226,   226,   227,   227,   228,   229,   229,   229,   229,
     230,   230,   231,   231,   231,   232,   233,   233,   235,   234,
     236,   237,   238,   238,   238,   239,   239,   239,   239,   240,
     240,   241,   241,   242,   243,   243,   244,   244,   245,   246,
     246,   247,   247,   248,   248,   248,   249,   249,   250,   251,
     250,   252,   252,   252,   252,   252,   253,   254,   253,   256,
     255,   257,   255,   258,   259,   260,   262,   261,   263,   263,
     263,   263,   263,   263,   264,   264,   265,   265,   265,   266,
     266,   266,   266,   266,   266,   266,   266,   267,   267,   268,
     268,   268,   269,   269,   269,   269,   270,   270,   271,   271,
     271,   271,   271,   271,   271,   272,   272,   273,   273,   274,
     274,   274,   275,   275,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   277,
     278,   278,   278,   279,   281,   280,   282,   282,   283,   283,
     284,   284,   285,   285,   285,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   287,   287,   287,
     288,   289,   289,   290,   290,   291,   291,   292,   292,   294,
     295,   293,   296,   297,   293,   298,   298,   298,   298,   298,
     299,   299,   299,   300,   300,   302,   303,   301,   301,   304,
     304,   304,   304,   304,   304,   305,   306,   307,   307,   307,
     308,   308,   308,   309,   309,   310,   310,   310,   311,   312,
     312,   312,   313,   313,   314,   314,   315,   315,   316,   316,
     316,   316,   317,   317,   319,   320,   318,   321,   322,   318,
     323,   324,   318,   326,   327,   325,   328,   328,   328,   328,
     328,   328,   329,   329,   330,   330,   330,   331,   331,   331,
     332,   332,   332,   332,   332,   333,   333,   334,   334,   334,
     335,   335,   336,   338,   337,   339,   339,   339,   339,   339,
     339,   339,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   341,   341,   341,   341,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   343,   343,   344,   344,   345,   345,   346,   347,
     348,   346,   349,   350,   346,   351,   351,   351,   351,   351,
     351,   351,   352,   353,   351,   354,   354,   354,   354,   354,
     354,   354,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   356,   357,
     355,   355,   355,   355,   358,   359,   355,   355,   355,   360,
     361,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   363,   363,   363,   364,   364,   364,   365,   365,
     366,   366,   366,   367,   367,   368,   369,   369,   370,   371,
     370,   372,   370,   373,   370,   370,   374,   374,   375,   375,
     376,   376,   377,   377,   378,   378,   378,   379,   380,   380,
     381,   381,   381,   382,   382,   383,   383,   383,   383,   383,
     383,   384,   384,   384,   385,   385,   386,   386,   386,   386,
     386,   387,   387,   387,   387,   387,   388,   389,   388,   390,
     390,   391,   391,   391,   392,   393,   392,   394,   394,   394,
     395,   395,   395,   397,   396,   398,   398,   399,   400,   399,
     401,   401,   401,   402,   403,   403,   404,   404,   405,   405,
     406,   407,   407,   407,   407,   408,   408,   408,   409,   409,
     411,   412,   410,   413,   413,   413,   413,   413,   414,   414,
     414,   414,   414,   414,   414,   414,   414,   414,   414,   414,
     414,   414,   414,   414,   414,   414,   414,   414,   414,   414,
     414,   414,   414,   414,   414,   415,   415,   415,   415,   415,
     415,   415,   415,   416,   417,   417,   417,   418,   418,   419,
     419,   420,   420,   420,   420,   422,   423,   421,   424,   424,
     425,   425,   426,   426,   427,   427,   427,   427,   427,   427,
     428,   429,   427,   427,   427,   430,   427,   427,   427,   427,
     427,   427,   427,   427,   427,   427,   427,   427,   427,   431,
     432,   427,   427,   433,   434,   427,   435,   436,   427,   437,
     438,   427,   427,   439,   440,   427,   441,   442,   427,   427,
     443,   444,   427,   445,   446,   427,   427,   447,   448,   427,
     449,   450,   427,   451,   452,   427,   453,   454,   427,   455,
     455,   455,   457,   458,   459,   460,   456,   462,   463,   464,
     465,   461,   467,   468,   466,   469,   469,   469,   469,   469,
     470,   470,   470,   470,   470,   470,   470,   470,   471,   472,
     473,   473,   474,   474,   475,   475,   476,   476,   477,   477,
     478,   478,   479,   479,   480,   480,   481,   481,   481,   482,
     482,   482,   483,   483,   484,   484,   484,   484,   484,   484,
     485,   486,   484,   487,   488,   484,   489,   490,   484,   491,
     492,   484,   493,   493,   493,   494,   494,   495,   496,   497,
     495,   498,   498,   499,   499,   499,   500,   501,   499,   502,
     503,   499,   504,   505,   499,   499,   506,   507,   499,   499,
     508,   509,   499,   510,   510,   511,   511,   512,   512,   512,
     512,   512,   513,   513,   514,   514,   515,   515,   515,   515,
     515,   515,   515,   515,   515
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     2,
       0,     1,     1,     1,     1,     4,     1,     1,     2,     2,
       3,     2,     0,     2,     4,     3,     1,     2,     0,     4,
       2,     2,     1,     1,     1,     1,     2,     3,     3,     2,
       4,     0,     1,     2,     1,     3,     1,     3,     3,     3,
       2,     1,     1,     0,     2,     4,     1,     1,     0,     0,
       3,     1,     1,     1,     1,     1,     5,     0,     6,     0,
       6,     0,     8,     2,     3,     3,     0,     6,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     2,
       3,     2,     1,     1,     1,     1,     1,     4,     1,     2,
       3,     3,     3,     3,     2,     1,     3,     0,     3,     0,
       2,     3,     0,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       2,     2,     3,     4,     3,     2,     2,     2,     2,     2,
       3,     3,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     3,     0,     4,     1,     1,     1,     1,
       3,     7,     2,     2,     6,     1,     1,     1,     1,     1,
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
       0,     1,     1,     0,     1,     5,     0,     1,     0,     0,
       5,     0,    10,     0,    10,     6,     3,     4,     1,     3,
       1,     3,     1,     3,     0,     2,     3,     3,     1,     3,
       0,     2,     3,     1,     1,     1,     2,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     1,     4,     3,     3,
       5,     4,     6,     5,     5,     4,     0,     0,     5,     0,
       1,     0,     1,     1,     6,     0,     6,     0,     3,     5,
       0,     1,     1,     0,     5,     3,     4,     4,     0,     4,
       0,     1,     1,     1,     7,     9,     0,     2,     0,     1,
       3,     1,     1,     2,     2,     0,     1,     1,     0,     3,
       0,     0,     7,     1,     4,     3,     3,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     1,     3,     0,
       2,     0,     2,     3,     3,     0,     0,     6,     1,     1,
       1,     3,     3,     4,     1,     1,     1,     1,     2,     3,
       0,     0,     6,     4,     5,     0,     9,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     0,     0,    10,     0,     0,     0,
       0,    10,     0,     0,     9,     1,     1,     1,     1,     1,
       3,     3,     5,     5,     6,     6,     8,     8,     1,     1,
       1,     3,     3,     5,     1,     2,     1,     0,     0,     2,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     2,
       1,     1,     0,     1,     5,     4,     6,     7,     5,     7,
       0,     0,    10,     0,     0,    10,     0,     0,     9,     0,
       0,     7,     1,     3,     3,     3,     1,     5,     0,     0,
      10,     1,     3,     4,     4,     4,     0,     0,    11,     0,
       0,    11,     0,     0,    10,     5,     0,     0,     9,     5,
       0,     0,    10,     1,     3,     1,     3,     4,     3,     4,
       7,     9,     0,     3,     0,     1,     9,    11,    12,    11,
      10,    10,    10,     9,    10
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

    case YYSYMBOL_alias_list: /* alias_list  */
            { delete ((*yyvaluep).positions); }
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

  case 17: /* semicolon: SEMICOLON  */
                       {
        format::try_semicolon_at_eol(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
    }
    break;

  case 18: /* semicolon: "end of expression"  */
          {}
    break;

  case 19: /* top_level_reader_macro: expr_reader semicolon  */
                                   {
        delete (yyvsp[-1].pExpression);    // we do nothing, we don't even attemp to 'visit'
    }
    break;

  case 20: /* optional_public_or_private_module: %empty  */
                        { (yyval.b) = yyextra->g_Program->policies.default_module_public; }
    break;

  case 21: /* optional_public_or_private_module: "public"  */
                        { (yyval.b) = true; }
    break;

  case 22: /* optional_public_or_private_module: "private"  */
                        { (yyval.b) = false; }
    break;

  case 23: /* module_name: '$'  */
                    { (yyval.s) = new string("$"); }
    break;

  case 24: /* module_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 25: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module  */
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

  case 26: /* character_sequence: STRING_CHARACTER  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 27: /* character_sequence: STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 28: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 29: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 30: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 31: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 32: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 33: /* string_builder_body: string_builder_body character_sequence  */
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

  case 34: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                                        {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 35: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 36: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 37: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 38: /* $@1: %empty  */
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

  case 39: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
                                     {
        yyextra->g_ReaderExpr->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[0]));
        (yyval.pExpression) = yyextra->g_ReaderExpr;
        int thisLine = 0;
        FileInfo * info = nullptr;
        if (!format::is_formatter_enabled()) {
        if ( auto seqt = yyextra->g_ReaderMacro->suffix(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, thisLine, info, tokAt(scanner,(yylsp[0]))) ) {
            das_accept_sequence(scanner,seqt,strlen(seqt),thisLine,info);
            yylloc.first_column = (yylsp[0]).first_column;
            yylloc.first_line = (yylsp[0]).first_line;
            yylloc.last_column = (yylsp[0]).last_column;
            yylloc.last_line = (yylsp[0]).last_line;
        }
        }
        delete (yyvsp[-2].s);
        yyextra->g_ReaderMacro = nullptr;
        yyextra->g_ReaderExpr = nullptr;
    }
    break;

  case 40: /* options_declaration: "options" annotation_argument_list  */
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

  case 42: /* keyword_or_name: "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 43: /* keyword_or_name: "keyword"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 44: /* keyword_or_name: "type function"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 45: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 46: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 47: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 48: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 49: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 50: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 51: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 52: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 56: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 57: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 58: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 59: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 60: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 61: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 62: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 63: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 64: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 65: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-2])), (yyvsp[-2].pExpression)->at);
        if (!format::is_else_newline() && (yyvsp[0].pExpression) != nullptr) {
            format::skip_spaces_or_print(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), yyextra->das_tab_size);
        }

        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 66: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 67: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 68: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 69: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 70: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 71: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 72: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 73: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 74: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 75: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 76: /* expression_if_then_else: if_or_static_if expr expression_block opt_sem expression_else  */
                                                                                              {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-3])), (yyvsp[-3].pExpression)->at);
        if (!format::is_else_newline() && (yyvsp[0].pExpression) != nullptr) {
            format::skip_spaces_or_print(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-2])), tokAt(scanner,(yylsp[0])), yyextra->das_tab_size);
        }

        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-3].pExpression)),
            ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-4].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 77: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 78: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner semicolon  */
                                                                                                                                                         {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-2])), (yyvsp[-2].pExpression)->at);

        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 79: /* $@4: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 80: /* expression_for_loop: "for" $@4 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        format::wrap_par_expr(tokRangeAt(scanner, (yylsp[-3]), (yylsp[-1])), tokRangeAt(scanner, (yylsp[-3]), (yylsp[-1])));
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 81: /* $@5: %empty  */
                     { // Had to add to successfully convert to v2 syntax, just copied from ds2_parser
                             yyextra->das_keyword = true;
     }
    break;

  case 82: /* expression_for_loop: "for" $@5 '(' variable_name_with_pos_list "in" expr_list ')' expression_block  */
                                                                                                  {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 83: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 84: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at);

        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 85: /* expression_with: "with" expr expression_block  */
                                                         {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at);
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 86: /* $@6: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 87: /* expression_with_alias: "assume" "name" '=' $@6 expr semicolon  */
                                                                                                         {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-5])), *(yyvsp[-4].s), (yyvsp[-1].pExpression) );
        delete (yyvsp[-4].s);
    }
    break;

  case 88: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 89: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 90: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 91: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 92: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 93: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 94: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 95: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 96: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 97: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 98: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 99: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 100: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 101: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 102: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 103: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 104: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 105: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 106: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 107: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 108: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 109: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 110: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 111: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 112: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 113: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 114: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 115: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 116: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 117: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 118: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 119: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 120: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 121: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 122: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 123: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 124: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 125: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 126: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 127: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 128: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 129: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 130: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 131: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 132: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 133: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 134: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 135: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 136: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 137: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 138: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 139: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 140: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 141: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 142: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 143: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 144: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 145: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 146: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 147: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 148: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 149: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 150: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 151: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 152: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 153: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 154: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 155: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 156: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 157: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 158: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 159: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 160: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 161: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 162: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 163: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 164: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 165: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 166: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 167: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 168: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 169: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 170: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 171: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 172: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 173: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 174: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 175: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 176: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 177: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 178: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 179: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 180: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 181: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 182: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 183: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 184: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 185: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 186: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 187: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 188: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 189: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 190: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 191: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 192: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 193: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 194: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 195: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 196: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 197: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 198: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 199: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 200: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 201: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 202: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 203: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 204: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 205: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 206: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 207: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 208: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 209: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 210: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 211: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 212: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 213: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 214: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 215: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 216: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 217: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 218: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 219: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 220: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 221: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 222: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 223: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 224: /* $@7: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 225: /* function_declaration: optional_public_or_private_function $@7 function_declaration_header expression_block  */
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

  case 226: /* open_block: "begin of code block"  */
          { (yyval.i) = 0xdeadbeef; }
    break;

  case 227: /* open_block: OPEN_BRACE  */
                        { (yyval.i) = (yyvsp[0].i); }
    break;

  case 228: /* close_block: "end of code block"  */
          { (yyval.i) = 0xdeadbeef; }
    break;

  case 229: /* close_block: CLOSE_BRACE  */
                         { (yyval.i) = (yyvsp[0].i); }
    break;

  case 230: /* expression_block: open_block expressions close_block  */
                                                                  {
        auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].i), format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size, format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 231: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                                  {
        auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-6])));
        if (format::is_replace_braces() && (yyvsp[-6].i) != 0xdeadbeef && format::prepare_rule(prev_loc)) {
            handle_brace(prev_loc, (yyvsp[-6].i), format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-5])))),
                         yyextra->das_tab_size, format::Pos::from_last(tokAt(scanner,(yylsp[-5]))));
            auto prev_loc_f = format::Pos::from(tokAt(scanner,(yylsp[-2])));
            assert((yyvsp[-2].i) != 0xdeadbeef);
            {
                const auto internal_f = format::get_substring(prev_loc_f, format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
                format::get_writer() << " finally {" << internal_f << "\n" << string((yyvsp[-2].i) * yyextra->das_tab_size, ' ') + "}";
                format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
            }
        }

        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 232: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 233: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 234: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 235: /* expression_any: SEMICOLON  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 236: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 237: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 238: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 239: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 240: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 241: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 242: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 244: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 245: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 246: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 247: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 249: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 250: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 251: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 252: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 254: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 255: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 256: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 257: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 258: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 259: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 260: /* expr_keyword: "keyword" expr expression_block  */
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

  case 261: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 262: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 263: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 264: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 265: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 266: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 267: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 268: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 269: /* $@8: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 270: /* $@9: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 271: /* expression_keyword: "keyword" '<' $@8 type_declaration_no_options_list '>' $@9 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 272: /* $@10: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 273: /* $@11: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 274: /* expression_keyword: "type function" '<' $@10 type_declaration_no_options_list '>' $@11 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 275: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 276: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 277: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 278: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 279: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 280: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 281: /* name_in_namespace: "name" "::" "name"  */
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

  case 282: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 283: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 284: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 285: /* $@12: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 286: /* $@13: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 287: /* new_type_declaration: '<' $@12 type_declaration '>' $@13  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 288: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 289: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 290: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 291: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 292: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 293: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 294: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 295: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 296: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 297: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 298: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 299: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 300: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 301: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 302: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 303: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 304: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 305: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 306: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 307: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 308: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                              {
        const auto end_block = format::Pos::from_last(tokAt(scanner, (yylsp[-2])));
        const auto start = format::Pos::from(tokAt(scanner, (yylsp[-3])));
        if (format::is_replace_braces()) {
            format::skip_spaces_or_print(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])), yyextra->das_tab_size);
        }

        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 309: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 310: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 311: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 312: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 313: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 314: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 315: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 316: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 317: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 318: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                                                            {
        // std::cout << "case11" << std::endl;
        format::replace_with(false,
                             format::Pos::from(tokAt(scanner,(yylsp[-8]))),
                             format::substring_between(tokAt(scanner, (yylsp[-8])), tokAt(scanner, (yylsp[-6]))),
                             format::Pos::from_last(tokAt(scanner,(yylsp[-5]))), "(", ")");
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 319: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 320: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                                                {
        // std::cout << "case12" << std::endl;
        format::replace_with(false,
                              format::Pos::from(tokAt(scanner,(yylsp[-7]))),
                              format::substring_between(tokAt(scanner, (yylsp[-7])), tokAt(scanner, (yylsp[-5]))),
                              format::Pos::from_last(tokAt(scanner,(yylsp[-4]))), "(", ")");

        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-6]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 321: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 322: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 323: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 324: /* $@14: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 325: /* $@15: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 326: /* expr_cast: "cast" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 327: /* $@16: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 328: /* $@17: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 329: /* expr_cast: "upcast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 330: /* $@18: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 331: /* $@19: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 332: /* expr_cast: "reinterpret" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 333: /* $@20: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 334: /* $@21: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 335: /* expr_type_decl: "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 336: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
                                                                                {
            format::replace_with(false,
                                 format::Pos::from(tokAt(scanner,(yylsp[-3]))),
                                 format::get_substring(tokAt(scanner,(yylsp[-2]))),
                                 format::Pos::from(tokAt(scanner,(yylsp[-1]))), " ", "(");
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

  case 337: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
                                                                                                             {
            format::replace_with(false,
                                 format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                 format::get_substring(tokRangeAt(scanner,(yylsp[-5]),(yylsp[-2]))),
                                 format::Pos::from(tokAt(scanner,(yylsp[-1]))), " ", "(");

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

  case 338: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
                                                                                                                              {
            format::replace_with(false,
                                 format::Pos::from(tokAt(scanner,(yylsp[-8]))),
                                 format::get_substring(tokRangeAt(scanner,(yylsp[-7]),(yylsp[-1]))),
                                 format::Pos::from(tokAt(scanner,(yylsp[-1]))), " ", "(");

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

  case 339: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 340: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 341: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" semicolon "name" '>' '(' expr ')'  */
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

  case 342: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 343: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 344: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 345: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 346: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 347: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 348: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 349: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 350: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 351: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 352: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 353: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 354: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 355: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 356: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 357: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 358: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                                              {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-3]))))) {
            format::get_writer() << format::substring_between(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])))
                      << "capture(" << format::get_substring(tokAt(scanner, (yylsp[-2]))) << ")"
                      << format::substring_between(tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[0]))));
        }

         ; (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 359: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 360: /* expr_block: expression_block  */
                                            {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << "$() ";
            format::finish_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))));
        }

        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 361: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 362: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 363: /* $@22: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 364: /* expr_full_block_assumed_piped: block_or_lambda $@22 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 365: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 366: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 367: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 368: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 369: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 370: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 371: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 372: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 373: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 379: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 380: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 381: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 382: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 392: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 393: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 394: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 395: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 400: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 401: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 402: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 403: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 404: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 405: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 406: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 407: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 408: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 409: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 410: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 411: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 412: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 413: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 414: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 415: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 416: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 417: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 418: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 419: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 420: /* $@24: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 421: /* func_addr_expr: '@' '@' '<' $@23 type_declaration_no_options '>' $@24 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 422: /* $@25: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 423: /* $@26: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 424: /* func_addr_expr: '@' '@' '<' $@25 optional_function_argument_list optional_function_type '>' $@26 func_addr_name  */
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

  case 425: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 426: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 427: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 428: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 429: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 430: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 431: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 432: /* $@27: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 433: /* $@28: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 434: /* expr_field: expr '.' $@27 error $@28  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 435: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 436: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 437: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 438: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 439: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 440: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 441: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 442: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 443: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 444: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 445: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 446: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 447: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 448: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 449: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 450: /* expr: expr_field  */
                                              { (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 451: /* expr: expr_mtag  */
                                              { (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 452: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 455: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 456: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 457: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 459: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 460: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 461: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 462: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 463: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 464: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 465: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 466: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 467: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 468: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 469: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 470: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 471: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 472: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 473: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 474: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 475: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 476: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 477: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 478: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 479: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 480: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 481: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 482: /* expr: '(' expr_list optional_comma ')'  */
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
                (yyvsp[-2].pExpression)->at = tokAt(scanner, (yylsp[-2]));
                (yyval.pExpression) = (yyvsp[-2].pExpression);
            }
        }
    break;

  case 483: /* expr: expr '[' expr ']'  */
                                                      { (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 484: /* expr: expr '.' '[' expr ']'  */
                                                          { (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 485: /* expr: expr "?[" expr ']'  */
                                                      { (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 486: /* expr: expr '.' "?[" expr ']'  */
                                                          { (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 487: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 488: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 489: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 490: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 491: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 492: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 493: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 494: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 495: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 496: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 497: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 498: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 499: /* $@30: %empty  */
                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 500: /* expr: expr "is" "type" '<' $@29 type_declaration_no_options '>' $@30  */
                                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 501: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 502: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 503: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 504: /* $@31: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 505: /* $@32: %empty  */
                                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 506: /* expr: expr "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                                 {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 507: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 508: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 509: /* $@33: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 510: /* $@34: %empty  */
                                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 511: /* expr: expr '?' "as" "type" '<' $@33 type_declaration '>' $@34  */
                                                                                                                                                     {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-8]), (yylsp[-1])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 512: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 513: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 514: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 515: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 516: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 517: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); (yyval.pExpression)->at = tokAt(scanner, (yylsp[0])); }
    break;

  case 518: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 519: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 520: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); }
    break;

  case 521: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]), (yylsp[0]))); (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])); }
    break;

  case 522: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
    }
    break;

  case 523: /* expr: name_in_namespace "name"  */
                                                  {
        if (format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << "." << format::get_substring(tokAt(scanner,(yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 524: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 525: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 526: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 527: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 528: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 529: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 530: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 531: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 532: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 533: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 534: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 535: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 536: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 537: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 538: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 539: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 540: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 541: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 542: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 543: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-3]))))) {
            for (const auto &arg: *(yyvsp[-2].aaList)) {
                format::get_writer() << "@" << format::get_substring(arg.at);
            }
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[0]))));
        }
        (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/
    }
    break;

  case 544: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 545: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 546: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 547: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 548: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 549: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 550: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 551: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 552: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 553: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 554: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 555: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 558: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 559: /* $@35: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 560: /* struct_variable_declaration_list: struct_variable_declaration_list $@35 structure_variable_declaration semicolon opt_sem  */
                                                             {
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
        if ( (yyvsp[-2].pVarDecl) ) (yyvsp[-4].pVarDeclList)->push_back((yyvsp[-2].pVarDecl));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) {
                for ( const auto & nl : *((yyvsp[-2].pVarDecl)->pNameList) ) {
                    crd->afterStructureField(nl.name.c_str(), nl.at);
                }
            }
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterStructureFields(tak);
        }
    }
    break;

  case 561: /* $@36: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 562: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@36 function_declaration_header semicolon opt_sem  */
                                                                  {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-6].b),(yyvsp[-4].b), (yyvsp[-2].pFuncDecl));
            }
    break;

  case 563: /* $@37: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 564: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@37 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 565: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon opt_sem  */
                                                                                               {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-3])), CompilationError::syntax_error);
        delete (yyvsp[-3].faList);
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
    }
    break;

  case 566: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 567: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 568: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 569: /* function_argument_list: function_argument_list semicolon function_argument_declaration  */
                                                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 570: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 571: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 572: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 573: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 574: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 575: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 576: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 577: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 578: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 579: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 580: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 581: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 582: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 583: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 584: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 585: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 586: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 587: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 588: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 589: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 590: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 591: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 592: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 593: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 594: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 595: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 596: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 597: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 598: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 599: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 600: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 601: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 602: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 603: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 604: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 605: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 606: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 607: /* $@38: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 608: /* global_variable_declaration_list: global_variable_declaration_list $@38 optional_field_annotation let_variable_declaration opt_sem  */
                                                                              {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders )
                for ( auto & nl : *((yyvsp[-1].pVarDecl)->pNameList) )
                    crd->afterGlobalVariable(nl.name.c_str(),tak);
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterGlobalVariables(tak);
        }
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
        (yyvsp[-1].pVarDecl)->annotation = (yyvsp[-2].aaList);
        (yyvsp[-4].pVarDeclList)->push_back((yyvsp[-1].pVarDecl));
    }
    break;

  case 609: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 610: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 611: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 612: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 613: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 614: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                                   {
        handle_brace(format::Pos::from(tokAt(scanner, (yylsp[-2]))), (yyvsp[-2].i),
                     format::get_substring(tokRangeAt(scanner, (yylsp[-2]), (yylsp[0]))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 615: /* $@39: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 616: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@39 optional_field_annotation let_variable_declaration  */
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

  case 617: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 618: /* enum_list: enum_list "name" opt_sem  */
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

  case 619: /* enum_list: enum_list "name" '=' expr opt_sem  */
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

  case 620: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 621: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 622: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 623: /* $@40: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 624: /* single_alias: optional_public_or_private_alias "name" $@40 '=' type_declaration  */
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
        if ((yylsp[-4]).first_column == (yylsp[-4]).last_column) {
            (yyloc).first_line = (yylsp[-3]).first_line;
            (yyloc).first_column = (yylsp[-3]).first_column;
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 625: /* alias_list: single_alias semicolon opt_sem  */
                                                   {
        (yyval.positions) = new vector<LineInfo>(1, tokAt(scanner, (yylsp[-2])));
    }
    break;

  case 626: /* alias_list: alias_list single_alias semicolon opt_sem  */
                                                                      {
        (yyvsp[-3].positions)->emplace_back(tokAt(scanner, (yylsp[-2])));
        (yyval.positions) = (yyvsp[-3].positions);
    }
    break;

  case 627: /* alias_declaration: "typedef" open_block alias_list close_block  */
                                                                           {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-3]))))) {
            // todo: comments here and all such places, same rule
            for (const auto single: *(yyvsp[-1].positions)) {
                format::get_writer() << "typedef " << format::get_substring(single) << '\n';
            }
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[0]))));
        }

    }
    break;

  case 628: /* $@41: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 630: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 631: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 632: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 633: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 634: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block enum_list close_block  */
                                                                                                                                                              {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        const auto first_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(first_loc, (yyvsp[-2].i),
                     format::get_substring(first_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-3].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-6].faList),tokAt(scanner,(yylsp[-6])),(yyvsp[-4].b),(yyvsp[-3].pEnum),(yyvsp[-1].pEnum),Type::tInt);
    }
    break;

  case 635: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block enum_list close_block  */
                                                                                                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
        const auto first_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(first_loc, (yyvsp[-2].i),
                     format::get_substring(first_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-1].pEnum),(yyvsp[-3].type));
    }
    break;

  case 636: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 637: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 638: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 639: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 640: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 641: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 642: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 643: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 644: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 645: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 646: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 647: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 648: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 649: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                                      {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].i),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 650: /* $@42: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 651: /* $@43: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 652: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@42 structure_name $@43 optional_struct_variable_declaration_list  */
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

  case 653: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 654: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 655: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 656: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 657: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 658: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 659: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 660: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 661: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 662: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 663: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 664: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 665: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 666: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 667: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 668: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 669: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 670: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 671: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 672: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 673: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 674: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 675: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 676: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 677: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 678: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 679: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 680: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 681: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 682: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 683: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 684: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 685: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 686: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 687: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 688: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 689: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 690: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 691: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 692: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 693: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 694: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 695: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 696: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 697: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 698: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 701: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 702: /* bitfield_alias_bits: bitfield_alias_bits SEMICOLON  */
                                            {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 703: /* bitfield_alias_bits: bitfield_alias_bits "name" SEMICOLON  */
                                                       {
        if (format::enum_bitfield_with_comma() && format::is_replace_braces() && format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << ",";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        }

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

  case 704: /* bitfield_alias_bits: bitfield_alias_bits "name" commas  */
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

  case 705: /* $@44: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 706: /* $@45: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 707: /* bitfield_type_declaration: "bitfield" '<' $@44 bitfield_bits '>' $@45  */
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

  case 710: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 711: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 712: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 713: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 714: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 715: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 716: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 717: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 718: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 719: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 720: /* $@46: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 721: /* $@47: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 722: /* type_declaration_no_options: "type" '<' $@46 type_declaration '>' $@47  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 723: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 724: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 725: /* $@48: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 726: /* type_declaration_no_options: '$' name_in_namespace '<' $@48 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 727: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 728: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 729: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 730: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 731: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 732: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 733: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 734: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 735: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 736: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 737: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 738: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 739: /* $@49: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* $@50: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 741: /* type_declaration_no_options: "smart_ptr" '<' $@49 type_declaration '>' $@50  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 742: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 743: /* $@51: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 744: /* $@52: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 745: /* type_declaration_no_options: "array" '<' $@51 type_declaration '>' $@52  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 746: /* $@53: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 747: /* $@54: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 748: /* type_declaration_no_options: "table" '<' $@53 table_type_pair '>' $@54  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 749: /* $@55: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 750: /* $@56: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 751: /* type_declaration_no_options: "iterator" '<' $@55 type_declaration '>' $@56  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 752: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 753: /* $@57: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 754: /* $@58: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 755: /* type_declaration_no_options: "block" '<' $@57 type_declaration '>' $@58  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 756: /* $@59: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 757: /* $@60: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 758: /* type_declaration_no_options: "block" '<' $@59 optional_function_argument_list optional_function_type '>' $@60  */
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

  case 759: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 760: /* $@61: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 761: /* $@62: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 762: /* type_declaration_no_options: "function" '<' $@61 type_declaration '>' $@62  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 763: /* $@63: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 764: /* $@64: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 765: /* type_declaration_no_options: "function" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 766: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 767: /* $@65: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 768: /* $@66: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 769: /* type_declaration_no_options: "lambda" '<' $@65 type_declaration '>' $@66  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 770: /* $@67: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 771: /* $@68: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 772: /* type_declaration_no_options: "lambda" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 773: /* $@69: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 774: /* $@70: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 775: /* type_declaration_no_options: "tuple" '<' $@69 tuple_type_list '>' $@70  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 776: /* $@71: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 777: /* $@72: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 778: /* type_declaration_no_options: "variant" '<' $@71 variant_type_list '>' $@72  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 779: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 780: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 781: /* type_declaration: type_declaration '|' '#'  */
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

  case 782: /* $@73: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 783: /* $@74: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 784: /* $@75: %empty  */
                      {
        if (format::is_replace_braces() && (yyvsp[0].i) != 0xdeadbeef && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << " {";
            format::finish_rule(format::Pos::from(tokAt(scanner,(yylsp[0]))));
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 785: /* $@76: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 786: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@73 "name" $@74 open_block $@75 tuple_alias_type_list $@76 close_block  */
                         {
        if (format::is_replace_braces() && (yyvsp[-4].i) != 0xdeadbeef && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << "\n" << string((yyvsp[0].i) * yyextra->das_tab_size, ' ') + "}";
            format::finish_rule(format::Pos::from(tokAt(scanner,(yylsp[0]))));
        }
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

  case 787: /* $@77: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 788: /* $@78: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 789: /* $@79: %empty  */
                      {
        if (format::is_replace_braces() && (yyvsp[0].i) != 0xdeadbeef && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << " {";
            format::finish_rule(format::Pos::from(tokAt(scanner,(yylsp[0]))));
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 790: /* $@80: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 791: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@77 "name" $@78 open_block $@79 variant_alias_type_list $@80 close_block  */
                         {
        if (format::is_replace_braces() && (yyvsp[0].i) != 0xdeadbeef && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << "\n" << string((yyvsp[0].i) * yyextra->das_tab_size, ' ') + "}";
            format::finish_rule(format::Pos::from(tokAt(scanner,(yylsp[0]))));
        }
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

  case 792: /* $@81: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 793: /* $@82: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 794: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@81 "name" $@82 open_block bitfield_alias_bits commas close_block  */
                                                                          {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-3])));
        handle_brace(prev_loc, (yyvsp[-3].i),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-2])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-2]))));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-5].s);
        btype->at = tokAt(scanner,(yylsp[-5]));
        btype->argNames = *(yyvsp[-2].pNameList);
        btype->isPrivateAlias = !(yyvsp[-7].b);
        if ( btype->argNames.size()>32 ) {
            das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-5].s),tokAt(scanner,(yylsp[-5])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-5].s)->c_str(),atvname);
        }
        delete (yyvsp[-5].s);
        delete (yyvsp[-2].pNameList);
    }
    break;

  case 795: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 796: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 797: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 798: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 799: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 800: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 801: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 802: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 803: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 804: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 805: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner, (yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 806: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                                       {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 807: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                              {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 808: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 809: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 810: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 811: /* make_struct_dim: make_struct_dim semicolon make_struct_fields  */
                                                               {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 812: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 813: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 814: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 815: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 816: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 817: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 818: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 819: /* optional_block: "where" expr_block  */
                                  { (yyvsp[0].pExpression)->at = tokAt(scanner, (yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 832: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 833: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 834: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                     {
        // std::cout << "case1" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-4]))))) {
            auto type = format::type_to_string((yyvsp[-3].pTypeDecl), tokAt(scanner,(yylsp[-3]))).value_or("");
            const bool is_initialized = false;
            const auto before = format::substring_between(tokAt(scanner,(yylsp[-4])), tokAt(scanner, (yylsp[-3])));
            const auto internal = format::handle_msd(tokAt(scanner, (yylsp[-3])),
                                                     static_cast<ExprMakeStruct*>((yyvsp[-2].pExpression)),
                                                     tokAt(scanner, (yyvsp[-1].pExpression) != nullptr ? (yylsp[-1]) : (yylsp[0])),
                                                     type, is_initialized);
            if (static_cast<ExprMakeStruct*>((yyvsp[-2].pExpression))->structs.size() == 1) {
                // single struct
                format::get_writer() << before << internal;
                if ((yyvsp[-1].pExpression) != nullptr) {
                    format::get_writer() << " <| " << format::get_substring((yyvsp[-1].pExpression)->at);
                }
            } else {
                // array of structs
    //            const auto internal = format::get_substring(format::Pos::from(tokAt(scanner,@msd)),
    //                                                          format::Pos::from(tokAt(scanner,@end)));
                format::get_writer() << "[" << internal << "]";
            }
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 835: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                {
        // // std::cout << "case2" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-3]))))) {
            auto type = format::type_to_string((yyvsp[-2].pTypeDecl), tokAt(scanner,(yylsp[-2])));
            das_hash_set<string> always_init = {
                // Is there a method to describe all this types?
                "struct<array<",
                "array<",
                "bitfield",
                "string",
                "int",
                "float",
                "variant",
                "tuple",
            };
            const auto before = format::get_substring(format::Pos::from_last(tokAt(scanner,(yylsp[-3]))),
                                                      format::Pos::from(tokAt(scanner, (yylsp[-2]))));
            const auto after = format::get_substring(format::Pos::from_last(tokAt(scanner,(yylsp[-2]))),
                                                      format::Pos::from(tokAt(scanner, (yylsp[0]))));
            format::get_writer() << before;
            string suffix = ")";
            if ((yyvsp[-2].pTypeDecl)->isPointer()) {
                format::get_writer() << "default<" << type.value() << ">";
                suffix.clear();
            } else if (any_of(always_init.begin(), always_init.end(),
                            [&type](auto t){ return type.value_or("").find(t) == 0; })) {
                format::get_writer() << type.value_or("") << "(";
            } else {
                format::get_writer() << type.value_or("") << "("; // Possible uninitialized
            }
            format::get_writer() << after << suffix;
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 836: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                            {
        // std::cout << "case3" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-5]))))) {
            const auto type_name = format::type_to_string((yyvsp[-4].pTypeDecl), tokAt(scanner,(yylsp[-4])));
            auto maybe_init = (format::can_default_init(type_name.value_or(""))) ? "" : ""; // Possible uninitialized
            format::get_writer() << format::substring_between(tokAt(scanner,(yylsp[-5])), tokAt(scanner, (yylsp[-4])))
                                 << type_name.value_or("") << "("
                                 << format::substring_between(tokAt(scanner,(yylsp[-3])), tokAt(scanner, (yylsp[-2])))
                                 << maybe_init << ")"
                                 << format::substring_between(tokAt(scanner,(yylsp[-2])), tokAt(scanner, (yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 837: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                                                {
        // std::cout << "case4" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            auto type_name = format::type_to_string((yyvsp[-5].pTypeDecl), tokAt(scanner,(yylsp[-5]))).value_or("");
            const auto internal = format::handle_msd(tokAt(scanner, (yylsp[-3])),
                                                     static_cast<ExprMakeStruct*>((yyvsp[-2].pExpression)),
                                                     tokAt(scanner, (yylsp[0])),
                                                     type_name);
            format::get_writer() << format::substring_between(tokAt(scanner, (yylsp[-6])), tokAt(scanner, (yylsp[-5])))
                                 << format::substring_between(tokAt(scanner, (yylsp[-4])), tokAt(scanner, (yylsp[-3])))
                                 << internal;
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 838: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                          {
        // std::cout << "case6" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-4]))))) {
            auto type_name = format::type_to_string((yyvsp[-3].pTypeDecl), tokAt(scanner,(yylsp[-3]))).value_or("");
            format::get_writer() << format::substring_between(tokAt(scanner, (yylsp[-4])), tokAt(scanner, (yylsp[-3]))) << "["
                                     << format::handle_msd(tokAt(scanner, (yylsp[-3])),
                                                              static_cast<ExprMakeStruct*>((yyvsp[-2].pExpression)),
                                                              tokAt(scanner, (yylsp[0])),
                                                              type_name)
                                 << "]";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 839: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                                          {
        // std::cout << "case7" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            auto type_name = format::type_to_string((yyvsp[-5].pTypeDecl), tokAt(scanner,(yylsp[-5]))).value_or("");
            format::get_writer() << format::substring_between(tokAt(scanner, (yylsp[-6])), tokAt(scanner, (yylsp[-5]))) << "["
                                   << format::handle_msd(tokAt(scanner, (yylsp[-3])),
                                                         static_cast<ExprMakeStruct*>((yyvsp[-2].pExpression)),
                                                         tokAt(scanner, (yylsp[0])),
                                                         type_name)
                                   << "]";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 840: /* $@83: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 841: /* $@84: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 842: /* make_struct_decl: "struct" '<' $@83 type_declaration_no_options '>' $@84 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 843: /* $@85: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 844: /* $@86: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 845: /* make_struct_decl: "class" '<' $@85 type_declaration_no_options '>' $@86 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 846: /* $@87: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 847: /* $@88: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 848: /* make_struct_decl: "variant" '<' $@87 type_declaration_no_options '>' $@88 '(' make_variant_dim ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 849: /* $@89: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 850: /* $@90: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 851: /* make_struct_decl: "default" '<' $@89 type_declaration_no_options '>' $@90 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 852: /* make_tuple: expr  */
                  {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 853: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 854: /* make_tuple: make_tuple ',' expr  */
                                      {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        }
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        mt->at = format::concat(mt->at, tokAt(scanner, (yylsp[0])));
        (yyval.pExpression) = mt;
    }
    break;

  case 855: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 856: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 857: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 858: /* $@91: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 859: /* $@92: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 860: /* make_tuple_call: "tuple" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 861: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 862: /* make_dim: make_dim semicolon make_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 863: /* make_dim_decl: '[' expr_list optional_comma ']'  */
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

  case 864: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                                   {
        // std::cout << "case13" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-3]))))) {
            auto type_name = format::type_to_string((yyvsp[-2].pTypeDecl), tokAt(scanner,(yylsp[-2])));
            auto internal = format::handle_mka(tokAt(scanner, (yylsp[-2])),
                                               static_cast<ExprMakeArray*>((yyvsp[-1].pExpression)),
                                               tokAt(scanner, (yylsp[0])));
            format::get_writer() << format::substring_between(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])));
            if (static_cast<ExprMakeArray*>((yyvsp[-1].pExpression))->values.size() == 1) {
                // single element
                format::get_writer() << internal;
            } else {
                format::get_writer() << "fixed_array";
                if (!(yyvsp[-2].pTypeDecl)->isAuto()) {
                    format::get_writer() << "<" << type_name.value().substr(0, type_name.value().find('[')) << ">";
                }
                format::get_writer() << "(" << internal << ")";
            }
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 865: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                                   {
        // std::cout << "case8" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-3]))))) {
            string prefix = "[";
            string suffix = "]";
            if (!(yyvsp[-2].pTypeDecl)->isAuto()) {
                prefix = "array<" + format::get_substring(tokAt(scanner,(yylsp[-2]))) + ">(";
                suffix = ")";
            }
            format::get_writer() << format::substring_between(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])))
                                 << prefix
                                 << format::handle_mka(tokAt(scanner, (yylsp[-2])),
                                                                 static_cast<ExprMakeArray*>((yyvsp[-1].pExpression)),
                                                                 tokAt(scanner, (yylsp[0])))
                                 << suffix;
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 866: /* $@93: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 867: /* $@94: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 868: /* make_dim_decl: "array" "struct" '<' $@93 type_declaration_no_options '>' $@94 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 869: /* $@95: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 870: /* $@96: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 871: /* make_dim_decl: "array" "tuple" '<' $@95 type_declaration_no_options '>' $@96 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 872: /* $@97: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 873: /* $@98: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 874: /* make_dim_decl: "array" "variant" '<' $@97 type_declaration_no_options '>' $@98 '(' make_variant_dim ')'  */
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

  case 875: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 876: /* $@99: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 877: /* $@100: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 878: /* make_dim_decl: "array" '<' $@99 type_declaration_no_options '>' $@100 '(' optional_expr_list ')'  */
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

  case 879: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 880: /* $@101: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 881: /* $@102: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 882: /* make_dim_decl: "fixed_array" '<' $@101 type_declaration_no_options '>' $@102 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 883: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 884: /* make_table: make_table semicolon make_map_tuple  */
                                                      {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 885: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 886: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 887: /* make_table_decl: open_block expr_map_tuple_list optional_comma close_block  */
                                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 888: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
                                                                                {
        // std::cout << "case10" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-2]))))) {
            format::get_writer() << "{"
                                 << format::substring_between(tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])))
                                 << format::convert_to_string(((ExprMakeArray *)(yyvsp[-1].pExpression))->values, ",", ";")
                                 << format::substring_between(tokAt(scanner, (yylsp[-1])), tokAt(scanner, (yylsp[0])))
                                 << "}";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = mkt;
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-2]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = ttm;
    }
    break;

  case 889: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 890: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 891: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 892: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 893: /* array_comprehension_where: semicolon "where" expr  */
                                          { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 894: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 895: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 896: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 897: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                           {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 898: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                        {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 899: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 900: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 901: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                                        {
        // std::cout << "case5" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-9]))))) {
            auto internal = format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-8]))), format::Pos::from_last(tokAt(scanner,(yylsp[-2]))));
            format::get_writer() << "[iterator " << internal << "]";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 902: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                                                {
        // std::cout << "case9" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-9]))))) {
            auto internal = format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-8]))), format::Pos::from_last(tokAt(scanner,(yylsp[-2]))));
            format::get_writer() << "[" << format::substring_between(tokAt(scanner, (yylsp[-9])), tokAt(scanner, (yylsp[-8])))
                                 << internal
                                 << format::substring_between(tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1]))) << "]";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 903: /* array_comprehension: open_block "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block  */
                                                                                                                                                                                        {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::substring_between(tokAt(scanner, (yylsp[-8])), tokAt(scanner, (yylsp[-7])))
                                 << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4]))))
                                 << format::substring_between(tokAt(scanner, (yylsp[-1])), tokAt(scanner, (yylsp[0]))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 904: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block close_block  */
                                                                                                                                                                                                       {
        // std::cout << "case12" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-9]))))) {
            auto internal = format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-8]))), format::Pos::from_last(tokAt(scanner,(yylsp[-2]))));
            format::get_writer() << "[" << format::substring_between(tokAt(scanner, (yylsp[-9])), tokAt(scanner, (yylsp[-8])))
                                 << internal
                                 << format::substring_between(tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1]))) << "]";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
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


