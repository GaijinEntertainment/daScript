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
    void das_strfmt ( yyscan_t yyscanner );

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
  YYSYMBOL_OPEN_BRACE = 163,               /* "new scope"  */
  YYSYMBOL_CLOSE_BRACE = 164,              /* "close scope"  */
  YYSYMBOL_SEMICOLON = 165,                /* SEMICOLON  */
  YYSYMBOL_INTEGER = 166,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 167,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 168,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 169,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 170,            /* "unsigned int8 constant"  */
  YYSYMBOL_DAS_FLOAT = 171,                /* "floating point constant"  */
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
  YYSYMBOL_END_OF_EXPR = 191,              /* END_OF_EXPR  */
  YYSYMBOL_EMPTY = 192,                    /* EMPTY  */
  YYSYMBOL_193_ = 193,                     /* ','  */
  YYSYMBOL_194_ = 194,                     /* '='  */
  YYSYMBOL_195_ = 195,                     /* '?'  */
  YYSYMBOL_196_ = 196,                     /* ':'  */
  YYSYMBOL_197_ = 197,                     /* '|'  */
  YYSYMBOL_198_ = 198,                     /* '^'  */
  YYSYMBOL_199_ = 199,                     /* '&'  */
  YYSYMBOL_200_ = 200,                     /* '<'  */
  YYSYMBOL_201_ = 201,                     /* '>'  */
  YYSYMBOL_202_ = 202,                     /* '-'  */
  YYSYMBOL_203_ = 203,                     /* '+'  */
  YYSYMBOL_204_ = 204,                     /* '*'  */
  YYSYMBOL_205_ = 205,                     /* '/'  */
  YYSYMBOL_206_ = 206,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 207,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 208,               /* UNARY_PLUS  */
  YYSYMBOL_209_ = 209,                     /* '~'  */
  YYSYMBOL_210_ = 210,                     /* '!'  */
  YYSYMBOL_PRE_INC = 211,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 212,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 213,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 214,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 215,                    /* DEREF  */
  YYSYMBOL_216_ = 216,                     /* '.'  */
  YYSYMBOL_217_ = 217,                     /* '['  */
  YYSYMBOL_218_ = 218,                     /* ']'  */
  YYSYMBOL_219_ = 219,                     /* '('  */
  YYSYMBOL_220_ = 220,                     /* ')'  */
  YYSYMBOL_221_ = 221,                     /* '$'  */
  YYSYMBOL_222_ = 222,                     /* '@'  */
  YYSYMBOL_223_ = 223,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 224,                 /* $accept  */
  YYSYMBOL_program = 225,                  /* program  */
  YYSYMBOL_semicolon = 226,                /* semicolon  */
  YYSYMBOL_top_level_reader_macro = 227,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 228, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 229,              /* module_name  */
  YYSYMBOL_module_declaration = 230,       /* module_declaration  */
  YYSYMBOL_character_sequence = 231,       /* character_sequence  */
  YYSYMBOL_string_constant = 232,          /* string_constant  */
  YYSYMBOL_format_string = 233,            /* format_string  */
  YYSYMBOL_optional_format_string = 234,   /* optional_format_string  */
  YYSYMBOL_235_1 = 235,                    /* $@1  */
  YYSYMBOL_string_builder_body = 236,      /* string_builder_body  */
  YYSYMBOL_string_builder = 237,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 238, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 239,              /* expr_reader  */
  YYSYMBOL_240_2 = 240,                    /* $@2  */
  YYSYMBOL_options_declaration = 241,      /* options_declaration  */
  YYSYMBOL_require_declaration = 242,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 243,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 244,      /* require_module_name  */
  YYSYMBOL_require_module = 245,           /* require_module  */
  YYSYMBOL_is_public_module = 246,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 247,       /* expect_declaration  */
  YYSYMBOL_expect_list = 248,              /* expect_list  */
  YYSYMBOL_expect_error = 249,             /* expect_error  */
  YYSYMBOL_expression_label = 250,         /* expression_label  */
  YYSYMBOL_expression_goto = 251,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 252,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 253,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 254,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 255, /* expression_else_one_liner  */
  YYSYMBOL_256_3 = 256,                    /* $@3  */
  YYSYMBOL_expression_if_one_liner = 257,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 258,  /* expression_if_then_else  */
  YYSYMBOL_259_4 = 259,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 260,      /* expression_for_loop  */
  YYSYMBOL_261_5 = 261,                    /* $@5  */
  YYSYMBOL_262_6 = 262,                    /* $@6  */
  YYSYMBOL_expression_unsafe = 263,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 264,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 265,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 266,    /* expression_with_alias  */
  YYSYMBOL_267_7 = 267,                    /* $@7  */
  YYSYMBOL_annotation_argument_value = 268, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 269, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 270, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 271,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 272, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 273,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 274, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 275, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 276,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 277,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 278, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 279, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 280,   /* optional_function_type  */
  YYSYMBOL_function_name = 281,            /* function_name  */
  YYSYMBOL_global_function_declaration = 282, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 283, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 284, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 285,     /* function_declaration  */
  YYSYMBOL_286_8 = 286,                    /* $@8  */
  YYSYMBOL_open_block = 287,               /* open_block  */
  YYSYMBOL_close_block = 288,              /* close_block  */
  YYSYMBOL_expression_block = 289,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 290,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 291,           /* expression_any  */
  YYSYMBOL_expressions = 292,              /* expressions  */
  YYSYMBOL_expr_keyword = 293,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 294,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 295, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 296, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 297, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 298,       /* expression_keyword  */
  YYSYMBOL_299_9 = 299,                    /* $@9  */
  YYSYMBOL_300_10 = 300,                   /* $@10  */
  YYSYMBOL_301_11 = 301,                   /* $@11  */
  YYSYMBOL_302_12 = 302,                   /* $@12  */
  YYSYMBOL_expr_pipe = 303,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 304,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 305,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 306,     /* new_type_declaration  */
  YYSYMBOL_307_13 = 307,                   /* $@13  */
  YYSYMBOL_308_14 = 308,                   /* $@14  */
  YYSYMBOL_expr_new = 309,                 /* expr_new  */
  YYSYMBOL_expression_break = 310,         /* expression_break  */
  YYSYMBOL_expression_continue = 311,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 312, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 313,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 314, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 315,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 316,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 317,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 318,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 319,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 320,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 321, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 322,           /* expression_let  */
  YYSYMBOL_expr_cast = 323,                /* expr_cast  */
  YYSYMBOL_324_15 = 324,                   /* $@15  */
  YYSYMBOL_325_16 = 325,                   /* $@16  */
  YYSYMBOL_326_17 = 326,                   /* $@17  */
  YYSYMBOL_327_18 = 327,                   /* $@18  */
  YYSYMBOL_328_19 = 328,                   /* $@19  */
  YYSYMBOL_329_20 = 329,                   /* $@20  */
  YYSYMBOL_expr_type_decl = 330,           /* expr_type_decl  */
  YYSYMBOL_331_21 = 331,                   /* $@21  */
  YYSYMBOL_332_22 = 332,                   /* $@22  */
  YYSYMBOL_expr_type_info = 333,           /* expr_type_info  */
  YYSYMBOL_expr_list = 334,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 335,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 336,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 337,            /* capture_entry  */
  YYSYMBOL_capture_list = 338,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 339,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 340,               /* expr_block  */
  YYSYMBOL_expr_full_block = 341,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 342, /* expr_full_block_assumed_piped  */
  YYSYMBOL_343_23 = 343,                   /* $@23  */
  YYSYMBOL_expr_numeric_const = 344,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 345,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 346,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 347,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 348,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 349,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 350,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 351,           /* func_addr_expr  */
  YYSYMBOL_352_24 = 352,                   /* $@24  */
  YYSYMBOL_353_25 = 353,                   /* $@25  */
  YYSYMBOL_354_26 = 354,                   /* $@26  */
  YYSYMBOL_355_27 = 355,                   /* $@27  */
  YYSYMBOL_expr_field = 356,               /* expr_field  */
  YYSYMBOL_357_28 = 357,                   /* $@28  */
  YYSYMBOL_358_29 = 358,                   /* $@29  */
  YYSYMBOL_expr_call = 359,                /* expr_call  */
  YYSYMBOL_expr2 = 360,                    /* expr2  */
  YYSYMBOL_361_30 = 361,                   /* $@30  */
  YYSYMBOL_362_31 = 362,                   /* $@31  */
  YYSYMBOL_363_32 = 363,                   /* $@32  */
  YYSYMBOL_364_33 = 364,                   /* $@33  */
  YYSYMBOL_365_34 = 365,                   /* $@34  */
  YYSYMBOL_366_35 = 366,                   /* $@35  */
  YYSYMBOL_expr = 367,                     /* expr  */
  YYSYMBOL_expr_mtag = 368,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 369, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 370,        /* optional_override  */
  YYSYMBOL_optional_constant = 371,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 372, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 373, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 374, /* structure_variable_declaration  */
  YYSYMBOL_opt_sem = 375,                  /* opt_sem  */
  YYSYMBOL_struct_variable_declaration_list = 376, /* struct_variable_declaration_list  */
  YYSYMBOL_377_36 = 377,                   /* $@36  */
  YYSYMBOL_378_37 = 378,                   /* $@37  */
  YYSYMBOL_379_38 = 379,                   /* $@38  */
  YYSYMBOL_function_argument_declaration_no_type = 380, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 381, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 382,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 383,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 384,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 385,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 386,             /* variant_type  */
  YYSYMBOL_variant_type_list = 387,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 388,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 389,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 390, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 391, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 392,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 393,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 394,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 395, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 396, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 397, /* global_variable_declaration_list  */
  YYSYMBOL_398_39 = 398,                   /* $@39  */
  YYSYMBOL_optional_shared = 399,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 400, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 401,               /* global_let  */
  YYSYMBOL_402_40 = 402,                   /* $@40  */
  YYSYMBOL_enum_list = 403,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 404, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 405,             /* single_alias  */
  YYSYMBOL_406_41 = 406,                   /* $@41  */
  YYSYMBOL_alias_list = 407,               /* alias_list  */
  YYSYMBOL_alias_declaration = 408,        /* alias_declaration  */
  YYSYMBOL_409_42 = 409,                   /* $@42  */
  YYSYMBOL_optional_public_or_private_enum = 410, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 411,                /* enum_name  */
  YYSYMBOL_enum_declaration = 412,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 413, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 414,          /* optional_sealed  */
  YYSYMBOL_structure_name = 415,           /* structure_name  */
  YYSYMBOL_class_or_struct = 416,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 417, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 418, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 419,    /* structure_declaration  */
  YYSYMBOL_420_43 = 420,                   /* $@43  */
  YYSYMBOL_421_44 = 421,                   /* $@44  */
  YYSYMBOL_variable_name_with_pos_list = 422, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 423,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 424, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 425, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 426,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 427,            /* bitfield_bits  */
  YYSYMBOL_commas = 428,                   /* commas  */
  YYSYMBOL_bitfield_alias_bits = 429,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 430, /* bitfield_type_declaration  */
  YYSYMBOL_431_45 = 431,                   /* $@45  */
  YYSYMBOL_432_46 = 432,                   /* $@46  */
  YYSYMBOL_c_or_s = 433,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 434,          /* table_type_pair  */
  YYSYMBOL_dim_list = 435,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 436, /* type_declaration_no_options  */
  YYSYMBOL_437_47 = 437,                   /* $@47  */
  YYSYMBOL_438_48 = 438,                   /* $@48  */
  YYSYMBOL_439_49 = 439,                   /* $@49  */
  YYSYMBOL_440_50 = 440,                   /* $@50  */
  YYSYMBOL_441_51 = 441,                   /* $@51  */
  YYSYMBOL_442_52 = 442,                   /* $@52  */
  YYSYMBOL_443_53 = 443,                   /* $@53  */
  YYSYMBOL_444_54 = 444,                   /* $@54  */
  YYSYMBOL_445_55 = 445,                   /* $@55  */
  YYSYMBOL_446_56 = 446,                   /* $@56  */
  YYSYMBOL_447_57 = 447,                   /* $@57  */
  YYSYMBOL_448_58 = 448,                   /* $@58  */
  YYSYMBOL_449_59 = 449,                   /* $@59  */
  YYSYMBOL_450_60 = 450,                   /* $@60  */
  YYSYMBOL_451_61 = 451,                   /* $@61  */
  YYSYMBOL_452_62 = 452,                   /* $@62  */
  YYSYMBOL_453_63 = 453,                   /* $@63  */
  YYSYMBOL_454_64 = 454,                   /* $@64  */
  YYSYMBOL_455_65 = 455,                   /* $@65  */
  YYSYMBOL_456_66 = 456,                   /* $@66  */
  YYSYMBOL_457_67 = 457,                   /* $@67  */
  YYSYMBOL_458_68 = 458,                   /* $@68  */
  YYSYMBOL_459_69 = 459,                   /* $@69  */
  YYSYMBOL_460_70 = 460,                   /* $@70  */
  YYSYMBOL_461_71 = 461,                   /* $@71  */
  YYSYMBOL_462_72 = 462,                   /* $@72  */
  YYSYMBOL_463_73 = 463,                   /* $@73  */
  YYSYMBOL_type_declaration = 464,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 465,  /* tuple_alias_declaration  */
  YYSYMBOL_466_74 = 466,                   /* $@74  */
  YYSYMBOL_467_75 = 467,                   /* $@75  */
  YYSYMBOL_468_76 = 468,                   /* $@76  */
  YYSYMBOL_469_77 = 469,                   /* $@77  */
  YYSYMBOL_variant_alias_declaration = 470, /* variant_alias_declaration  */
  YYSYMBOL_471_78 = 471,                   /* $@78  */
  YYSYMBOL_472_79 = 472,                   /* $@79  */
  YYSYMBOL_473_80 = 473,                   /* $@80  */
  YYSYMBOL_474_81 = 474,                   /* $@81  */
  YYSYMBOL_bitfield_alias_declaration = 475, /* bitfield_alias_declaration  */
  YYSYMBOL_476_82 = 476,                   /* $@82  */
  YYSYMBOL_477_83 = 477,                   /* $@83  */
  YYSYMBOL_make_decl = 478,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 479,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 480,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 481,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 482,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 483,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 484,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 485, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 486,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 487, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 488, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 489, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 490, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 491, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 492,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 493,         /* make_struct_decl  */
  YYSYMBOL_494_84 = 494,                   /* $@84  */
  YYSYMBOL_495_85 = 495,                   /* $@85  */
  YYSYMBOL_496_86 = 496,                   /* $@86  */
  YYSYMBOL_497_87 = 497,                   /* $@87  */
  YYSYMBOL_498_88 = 498,                   /* $@88  */
  YYSYMBOL_499_89 = 499,                   /* $@89  */
  YYSYMBOL_500_90 = 500,                   /* $@90  */
  YYSYMBOL_501_91 = 501,                   /* $@91  */
  YYSYMBOL_make_tuple = 502,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 503,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 504,          /* make_tuple_call  */
  YYSYMBOL_505_92 = 505,                   /* $@92  */
  YYSYMBOL_506_93 = 506,                   /* $@93  */
  YYSYMBOL_make_dim = 507,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 508,            /* make_dim_decl  */
  YYSYMBOL_509_94 = 509,                   /* $@94  */
  YYSYMBOL_510_95 = 510,                   /* $@95  */
  YYSYMBOL_511_96 = 511,                   /* $@96  */
  YYSYMBOL_512_97 = 512,                   /* $@97  */
  YYSYMBOL_513_98 = 513,                   /* $@98  */
  YYSYMBOL_514_99 = 514,                   /* $@99  */
  YYSYMBOL_515_100 = 515,                  /* $@100  */
  YYSYMBOL_516_101 = 516,                  /* $@101  */
  YYSYMBOL_517_102 = 517,                  /* $@102  */
  YYSYMBOL_518_103 = 518,                  /* $@103  */
  YYSYMBOL_make_table = 519,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 520,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 521,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 522, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 523,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 524       /* array_comprehension  */
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
#define YYLAST   14190

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  224
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  301
/* YYNRULES -- Number of rules.  */
#define YYNRULES  932
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1775

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   451


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
       2,     2,     2,   210,     2,   223,   221,   206,   199,     2,
     219,   220,   204,   203,   193,   202,   216,   205,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   196,   185,
     200,   194,   201,   195,   222,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   217,     2,   218,   198,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   183,   197,   184,   209,     2,     2,     2,
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
     188,   189,   190,   191,   192,   207,   208,   211,   212,   213,
     214,   215
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   578,   578,   579,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   600,   603,   606,
     612,   613,   614,   618,   619,   623,   641,   642,   643,   644,
     648,   649,   653,   654,   658,   659,   659,   663,   668,   677,
     692,   708,   713,   721,   721,   762,   792,   796,   797,   798,
     802,   805,   809,   815,   824,   827,   833,   834,   838,   842,
     843,   847,   850,   856,   862,   865,   871,   872,   876,   877,
     878,   888,   901,   902,   906,   907,   907,   913,   914,   915,
     916,   917,   921,   931,   941,   941,   949,   949,   953,   953,
     962,   970,   982,   992,   992,   999,  1000,  1001,  1002,  1003,
    1004,  1008,  1013,  1021,  1022,  1023,  1027,  1028,  1029,  1030,
    1031,  1032,  1033,  1034,  1040,  1043,  1049,  1052,  1055,  1061,
    1062,  1063,  1064,  1068,  1081,  1099,  1102,  1110,  1121,  1132,
    1143,  1146,  1153,  1157,  1164,  1165,  1169,  1170,  1171,  1175,
    1178,  1185,  1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,
    1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,
    1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,
    1217,  1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,
    1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,
    1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,
    1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,
    1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,
    1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,
    1277,  1278,  1279,  1280,  1281,  1282,  1283,  1288,  1306,  1307,
    1308,  1312,  1318,  1318,  1335,  1336,  1339,  1340,  1343,  1350,
    1374,  1392,  1401,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,
    1424,  1425,  1426,  1427,  1428,  1432,  1437,  1443,  1449,  1461,
    1462,  1466,  1467,  1471,  1472,  1476,  1480,  1487,  1487,  1487,
    1493,  1493,  1493,  1502,  1536,  1544,  1551,  1558,  1564,  1565,
    1576,  1580,  1583,  1591,  1591,  1591,  1594,  1600,  1603,  1607,
    1611,  1618,  1625,  1631,  1635,  1639,  1642,  1645,  1653,  1656,
    1659,  1667,  1670,  1678,  1681,  1684,  1692,  1704,  1705,  1706,
    1710,  1711,  1715,  1716,  1720,  1725,  1733,  1744,  1750,  1765,
    1777,  1780,  1786,  1786,  1786,  1789,  1789,  1789,  1794,  1794,
    1794,  1802,  1802,  1802,  1808,  1822,  1838,  1856,  1866,  1877,
    1892,  1895,  1901,  1902,  1909,  1920,  1921,  1922,  1926,  1927,
    1928,  1929,  1930,  1934,  1939,  1947,  1948,  1961,  1965,  1975,
    1982,  1989,  1989,  1998,  1999,  2000,  2001,  2002,  2003,  2004,
    2008,  2009,  2010,  2011,  2012,  2013,  2014,  2015,  2016,  2017,
    2018,  2019,  2020,  2021,  2022,  2023,  2024,  2025,  2026,  2030,
    2040,  2049,  2058,  2063,  2064,  2065,  2066,  2067,  2068,  2069,
    2070,  2071,  2072,  2073,  2074,  2075,  2076,  2077,  2078,  2079,
    2084,  2091,  2103,  2108,  2118,  2122,  2129,  2132,  2132,  2132,
    2137,  2137,  2137,  2150,  2154,  2158,  2163,  2170,  2179,  2184,
    2191,  2191,  2191,  2198,  2202,  2212,  2221,  2230,  2234,  2237,
    2243,  2244,  2245,  2246,  2247,  2248,  2249,  2250,  2251,  2252,
    2253,  2254,  2255,  2256,  2257,  2258,  2259,  2260,  2261,  2262,
    2263,  2264,  2265,  2266,  2267,  2268,  2269,  2270,  2271,  2278,
    2279,  2280,  2281,  2282,  2283,  2284,  2285,  2286,  2287,  2288,
    2289,  2296,  2297,  2298,  2299,  2300,  2301,  2302,  2303,  2317,
    2318,  2319,  2320,  2321,  2324,  2327,  2328,  2328,  2328,  2331,
    2336,  2340,  2344,  2344,  2344,  2349,  2352,  2356,  2356,  2356,
    2361,  2364,  2365,  2366,  2367,  2368,  2369,  2370,  2371,  2372,
    2374,  2378,  2386,  2391,  2395,  2404,  2405,  2406,  2407,  2408,
    2409,  2410,  2414,  2418,  2422,  2426,  2430,  2434,  2438,  2442,
    2446,  2453,  2454,  2463,  2467,  2468,  2469,  2473,  2474,  2478,
    2479,  2480,  2484,  2485,  2489,  2500,  2501,  2502,  2503,  2508,
    2511,  2511,  2530,  2529,  2545,  2544,  2558,  2567,  2579,  2588,
    2598,  2599,  2600,  2601,  2602,  2606,  2609,  2618,  2619,  2623,
    2626,  2629,  2645,  2654,  2655,  2659,  2662,  2665,  2679,  2680,
    2684,  2690,  2696,  2705,  2708,  2715,  2718,  2724,  2725,  2726,
    2730,  2731,  2735,  2742,  2747,  2756,  2762,  2773,  2776,  2781,
    2786,  2794,  2804,  2815,  2818,  2818,  2838,  2839,  2843,  2844,
    2845,  2849,  2856,  2856,  2875,  2878,  2894,  2914,  2915,  2916,
    2921,  2921,  2951,  2954,  2961,  2971,  2971,  2975,  2976,  2977,
    2981,  2991,  3011,  3034,  3035,  3039,  3040,  3044,  3050,  3051,
    3052,  3053,  3057,  3058,  3059,  3063,  3066,  3077,  3082,  3077,
    3102,  3109,  3114,  3123,  3129,  3140,  3141,  3142,  3143,  3144,
    3145,  3146,  3147,  3148,  3149,  3150,  3151,  3152,  3153,  3154,
    3155,  3156,  3157,  3158,  3159,  3160,  3161,  3162,  3163,  3164,
    3165,  3166,  3170,  3171,  3172,  3173,  3174,  3175,  3176,  3177,
    3181,  3192,  3196,  3203,  3215,  3222,  3232,  3233,  3238,  3243,
    3258,  3268,  3281,  3281,  3281,  3294,  3295,  3299,  3303,  3310,
    3314,  3321,  3322,  3323,  3324,  3325,  3340,  3346,  3346,  3346,
    3350,  3355,  3362,  3362,  3369,  3373,  3377,  3382,  3387,  3392,
    3397,  3401,  3405,  3410,  3414,  3418,  3423,  3423,  3423,  3429,
    3436,  3436,  3436,  3441,  3441,  3441,  3447,  3447,  3447,  3452,
    3456,  3456,  3456,  3461,  3461,  3461,  3470,  3474,  3474,  3474,
    3479,  3479,  3479,  3488,  3492,  3492,  3492,  3497,  3497,  3497,
    3506,  3506,  3506,  3512,  3512,  3512,  3521,  3524,  3535,  3551,
    3551,  3556,  3565,  3551,  3594,  3594,  3599,  3609,  3594,  3638,
    3638,  3638,  3681,  3682,  3683,  3684,  3685,  3689,  3696,  3703,
    3709,  3715,  3722,  3729,  3735,  3744,  3747,  3753,  3761,  3766,
    3773,  3778,  3785,  3790,  3796,  3797,  3801,  3802,  3807,  3808,
    3812,  3813,  3817,  3818,  3822,  3823,  3824,  3828,  3829,  3830,
    3834,  3835,  3839,  3872,  3911,  3930,  3950,  3970,  3991,  3991,
    3991,  3999,  3999,  3999,  4006,  4006,  4006,  4017,  4017,  4017,
    4028,  4032,  4038,  4054,  4060,  4066,  4072,  4072,  4072,  4086,
    4091,  4098,  4118,  4146,  4170,  4170,  4170,  4180,  4180,  4180,
    4194,  4194,  4194,  4208,  4217,  4217,  4217,  4237,  4244,  4244,
    4244,  4254,  4259,  4266,  4269,  4275,  4295,  4314,  4322,  4342,
    4367,  4368,  4372,  4373,  4378,  4388,  4391,  4394,  4397,  4405,
    4414,  4426,  4436
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
  "\"[{\"", "\"{{\"", "\"new scope\"", "\"close scope\"", "SEMICOLON",
  "\"integer constant\"", "\"long integer constant\"",
  "\"unsigned integer constant\"", "\"unsigned long integer constant\"",
  "\"unsigned int8 constant\"", "\"floating point constant\"",
  "\"double constant\"", "\"name\"", "\"keyword\"", "\"type function\"",
  "\"start of the string\"", "STRING_CHARACTER", "STRING_CHARACTER_ESC",
  "\"end of the string\"", "\"{\"", "\"}\"",
  "\"end of failed eader macro\"", "\"begin of code block\"",
  "\"end of code block\"", "\"end of expression\"", "\";}}\"", "\";}]\"",
  "\";]]\"", "\",]]\"", "\",}]\"", "END_OF_EXPR", "EMPTY", "','", "'='",
  "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'", "'-'", "'+'", "'*'",
  "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'", "'!'", "PRE_INC",
  "PRE_DEC", "POST_INC", "POST_DEC", "DEREF", "'.'", "'['", "']'", "'('",
  "')'", "'$'", "'@'", "'#'", "$accept", "program", "semicolon",
  "top_level_reader_macro", "optional_public_or_private_module",
  "module_name", "module_declaration", "character_sequence",
  "string_constant", "format_string", "optional_format_string", "$@1",
  "string_builder_body", "string_builder", "reader_character_sequence",
  "expr_reader", "$@2", "options_declaration", "require_declaration",
  "keyword_or_name", "require_module_name", "require_module",
  "is_public_module", "expect_declaration", "expect_list", "expect_error",
  "expression_label", "expression_goto", "elif_or_static_elif",
  "expression_else", "if_or_static_if", "expression_else_one_liner", "$@3",
  "expression_if_one_liner", "expression_if_then_else", "$@4",
  "expression_for_loop", "$@5", "$@6", "expression_unsafe",
  "expression_while_loop", "expression_with", "expression_with_alias",
  "$@7", "annotation_argument_value", "annotation_argument_value_list",
  "annotation_argument_name", "annotation_argument",
  "annotation_argument_list", "metadata_argument_list",
  "annotation_declaration_name", "annotation_declaration_basic",
  "annotation_declaration", "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@8", "open_block", "close_block",
  "expression_block", "expr_call_pipe", "expression_any", "expressions",
  "expr_keyword", "optional_expr_list", "optional_expr_list_in_braces",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "expression_keyword", "$@9", "$@10", "$@11", "$@12", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@13",
  "$@14", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@15", "$@16", "$@17", "$@18", "$@19",
  "$@20", "expr_type_decl", "$@21", "$@22", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@23", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe_right", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@24", "$@25", "$@26", "$@27", "expr_field", "$@28",
  "$@29", "expr_call", "expr2", "$@30", "$@31", "$@32", "$@33", "$@34",
  "$@35", "expr", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "opt_sem", "struct_variable_declaration_list", "$@36", "$@37", "$@38",
  "function_argument_declaration_no_type",
  "function_argument_declaration_type", "function_argument_list",
  "tuple_type", "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration_no_type", "variable_declaration_type",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@39", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@40", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@41", "alias_list",
  "alias_declaration", "$@42", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "optional_structure_parent",
  "optional_sealed", "structure_name", "class_or_struct",
  "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@43", "$@44", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "commas",
  "bitfield_alias_bits", "bitfield_type_declaration", "$@45", "$@46",
  "c_or_s", "table_type_pair", "dim_list", "type_declaration_no_options",
  "$@47", "$@48", "$@49", "$@50", "$@51", "$@52", "$@53", "$@54", "$@55",
  "$@56", "$@57", "$@58", "$@59", "$@60", "$@61", "$@62", "$@63", "$@64",
  "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71", "$@72", "$@73",
  "type_declaration", "tuple_alias_declaration", "$@74", "$@75", "$@76",
  "$@77", "variant_alias_declaration", "$@78", "$@79", "$@80", "$@81",
  "bitfield_alias_declaration", "$@82", "$@83", "make_decl",
  "make_struct_fields", "make_variant_dim", "make_struct_single",
  "make_struct_dim", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "optional_block",
  "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@84", "$@85", "$@86", "$@87", "$@88", "$@89", "$@90", "$@91",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@92", "$@93",
  "make_dim", "make_dim_decl", "$@94", "$@95", "$@96", "$@97", "$@98",
  "$@99", "$@100", "$@101", "$@102", "$@103", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1555)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-798)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1555,   175, -1555, -1555,    41,   -30,   258,   597, -1555,   -80,
     127,   127,   127, -1555, -1555,    27,    20, -1555, -1555, -1555,
     630, -1555, -1555, -1555,    95, -1555,   106, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555,    66, -1555,   -41,
       5,   121, -1555, -1555, -1555, -1555,   258, -1555,    31, -1555,
   -1555, -1555,   127,   127, -1555, -1555,   106, -1555, -1555, -1555,
   -1555, -1555,    19,   237, -1555, -1555, -1555, -1555,    20,    20,
      20,   148, -1555,   690,   118, -1555, -1555, -1555, -1555,   404,
     463,    90,   517, -1555,   836,    51,    41,   314,   -30,   238,
     337, -1555,   848,   848, -1555,   349,   642,    14,   630,   840,
     403,   443,   524, -1555,   611,   338, -1555, -1555,   -65,    41,
      20,    20,    20,    20, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555,   616, -1555, -1555, -1555, -1555, -1555, -1555, -1555,   597,
   -1555, -1555, -1555, -1555, -1555,   861,   124, -1555, -1555, -1555,
   -1555,   601, -1555, -1555, -1555, -1555,   582, -1555, -1555, -1555,
   -1555,   642, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555,   668, -1555,  -111, -1555,   117,   678,   690, 13872, -1555,
     386,   778, -1555,   -87, -1555, -1555, -1555,   867, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555,   223, -1555,   715, -1555, -1555,
     597,   597,   597, -1555, -1555, 12958, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555,   884,   900, -1555,   738,   597,  1018, -1555, -1555,   776,
   -1555,   679,    41,    41,   261,   160, -1555, -1555, -1555,   124,
   -1555, 10666, -1555, -1555, -1555,   789,   792, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555,   793,   751, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555,   953, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555,   804,   766, -1555, -1555,   174,   784,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
     597,   550,   790,   597, -1555,   -87,   298, -1555,    41, -1555,
     769,   952,   589, -1555, -1555,   802,   809,   812,   799,   824,
     830, -1555, -1555, -1555,   815, -1555, -1555, -1555, -1555, -1555,
     842, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555,   853, -1555, -1555, -1555,   854,   856, -1555, -1555,
   -1555, -1555,   857,   858,   817,    27, -1555, -1555, -1555, -1555,
   -1555,   624,   807,   682, -1555, -1555, -1555, -1555, -1555, -1555,
     860,   653, -1555,   846, -1555,   273,   874,   125,   841, 10666,
   -1555,  3061, -1555,   453, -1555,    27, -1555, -1555, -1555,   160,
     852, -1555, 10148,   898,   903, 10666, -1555,    24, -1555, -1555,
   -1555, 10148, -1555, -1555,   904, -1555,   535,   557,   574, -1555,
   -1555, 10148,   154, -1555, -1555, -1555,    17, -1555, -1555, -1555,
      29,  6192, -1555,   862,  2902,   192, -1555, 10515,   531, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, 10148, -1555, -1555,   262,   141,   141,   141, -1555,
     807, -1555,   880,   887, 10148, -1555, -1555,   865, -1555, -1555,
    1298,   -69,   888,    40,  3896, -1555, -1555,   597,   598,  6400,
     870, 10148,   919,   894,   899,   879, -1555,   630,   901,   934,
    6608,   -61,   669,   902, -1555,   670,   905,   906,  4104, 10148,
   10148,   230,   230,   230,   890,   896,   907,   909,   910,   911,
   -1555,  1936,  2546,  6818, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555,  7026,   908, -1555,  1090, -1555, 10148, 10148, 10148,
   10148, 10148,  5776, 10148, -1555,   885, -1555, -1555,   630,   630,
   10148,  1103, -1555, -1555, -1555, -1555, -1555, -1555,  7236,  1084,
   -1555, -1555, -1555,   785, -1555,   -47,   630, -1555,   630,   630,
     630, -1555,   630, -1555, -1555,  1057, -1555, -1555, -1555, -1555,
     918, -1555, -1555,   150, -1555, -1555, -1555, -1555, -1555, -1555,
   13148,  1152, -1555,   928, -1555, -1555, -1555, -1555, -1555, -1555,
     550, 10148, -1555, -1555,    39,   642, -1555,   936,   957,   958,
   -1555,  2328, -1555,  1116,   659, -1555, -1555, -1555,  4520, 10666,
   10666, 10666,  2645, 10666, 10666,   943,   991, 10666,   738, 10666,
     738, 10666,   738, 10769,   992,  2686, -1555, 10148, -1555, -1555,
   -1555, -1555,   948, -1555, -1555, 12510, 10148, -1555,   624, -1555,
   -1555,   975,   -25, -1555, -1555,   300, -1555,   807,   679,   973,
     300, -1555,   679, 10760,   976,  1129, -1555, -1555,   165, -1555,
   -1555, -1555, -1555, -1555,   597,   262,   977, -1555,   978, -1555,
   -1555,    27,   673, -1555,   994,   998,   999, -1555, 10148,  4520,
   -1555,  1007,  1069,  2159, 13968,  1188, 10666, 10148, 10148, -1555,
   10148,   597,  1012, -1555, -1555, 10148, -1555, -1555,  1011,  1042,
   -1555, 10148, -1555, -1555, 10148, -1555, -1555, 10148, -1555, 10666,
    4520, -1555, 13925,   155,   155,   987, -1555,   918, -1555, -1555,
   -1555, 10148, 10148, 10148, 10148, 10148, 10148,   262,  3269,   262,
    3478,   262,  1068, -1555,   645, -1555,   597, -1555,   814,   993,
     155,   155,     3,   155,   155,     7,  1201,   997,  1023, 13148,
    1023,   317, -1555, -1555,   597, -1555,   262,   679, -1555,  1024,
     597, -1555, -1555, -1555,  4728, -1555, -1555, -1555, -1555, -1555,
   -1555,   -45,    82,   230, -1555, 13649, 13748, 10148, 10148, -1555,
   -1555, 10148, 10148, 10148, 10148,  1045, 10148,   356, 10148, 10148,
   10148, 10148, 10148, 10148, 10148, 10148, 10148,  7444, 10148, 10148,
   10148, 10148, 10148, 10148, 10148, 10148, 10148, 10148, 13810, 10148,
    4936,  4936,  4936,  4936,  4936,  4936,  4936,  4936,  4936,  4936,
    4936, 10148,  4936,  4936,  4936,  4936,  4936,  4936, -1555,  7652,
   -1555,   642,    20,  1213, -1555,   -87, -1555, 10666, -1555,  1049,
   -1555,  4520, -1555, 13925,   120,   663,  1022,   402, -1555,   713,
     720, -1555, -1555,   294,   723,   784,   730,   784,   743,   784,
   -1555,   377, -1555,   378, -1555, 10666,  1004, -1555, -1555, 12550,
   -1555, 10666, -1555, -1555, 10666, -1555, -1555, -1555, 10148,  1052,
   -1555,  1053, -1555, 10666, -1555, 10148, 10666, 10666, -1555,    36,
     262, 10666,  5984,  7860,  1054, 10148, 10666, -1555, -1555, -1555,
   10666,  1023, -1555,  1007, 10148, 10148, 10148, 10148, 10148, 10148,
   10148, 10148, 10148, 10148, 10148, 10148, 10148, 10148, 10148, 10148,
   10148, 10148,   597,   788,  1008, -1555, 10879, -1555, -1555, 10666,
   10666, 10914, 10666, -1555, -1555, 11025, 10666,  1023, 10666, 10769,
    1023,   992,   818, -1555, 13925, -1555,    82, 11065, 11176, 11211,
   11322, 11362, 11473,    44,   230,  1013,    10,  3687,  5146,  8068,
    1088,  1038,     4,   222,  1041,   342,    45,  8276,     4,   607,
      56, 10148,  1051, -1555, 10148, -1555, 10666, -1555, 10666, -1555,
   10148,   717,   262,   262,    58,   204, -1555, 10148, -1555,  1016,
    1014,  1019,   578, -1555, -1555,    93, 10148,    64, -1555, 10148,
   -1555, -1555,   918,   149,  5356, -1555,   228,  1044,  1021,  1066,
    1066, -1555, -1555,  1025,   189,   738, -1555,  1043,  1026, -1555,
   -1555,  1046,  1028, -1555, -1555,   523,   523,  2891,  2891,  1633,
    1633,  1030,   213,  1032, -1555, 12661,    -6,    -6,   928,   523,
     523, 13556, 13408, 13445, 13259, 13779, 13111, 13580,  1932, 10516,
    2891,  2891,   272,   272,   213,   213,   213,   417, 10148,  1033,
    1034,   464, 10148,  1254,  1039, 12696,   230,   230,   230, -1555,
   -1555,   785, -1555,   785, -1555,   785, -1555,   785, -1555,   785,
   -1555,   785, -1555,   785, -1555,   785, -1555,   785, -1555,   785,
   -1555,   785, -1555, -1555,   785, -1555,   785, -1555,   785, -1555,
     785, -1555,   785, -1555,   785, -1555,   229, -1555,   310,   850,
    1192,   630,  1159, -1555, -1555, 13866, -1555, -1555, -1555, -1555,
   10666, -1555, -1555, -1555,  1095, -1555,  1070, -1555,  1071, -1555,
    1074, -1555, 10769, -1555,   992,   388,   807, -1555, -1555,   807,
     807, 11508, -1555,  1226,   105, -1555,  1196,  1221, 10148,    69,
     744,   518,   299,  1050,  1056,  1105,  1064,   367,  1065,   746,
   10666, 10769,   992,  1329,  1075, 13148, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555,  1067, 10666, -1555, -1555, 10148,  1434,
    1445, -1555,  1543, -1555,  1664,  1076,  2287,   400,  1077,   401,
      82,   738, -1555, -1555, -1555, -1555, -1555,  1072, 10148, -1555,
   10148, 10148, 10148,  5566, 12510,     6, 10148,   586,   518,   222,
   -1555, -1555,  1080, -1555, 10148, -1555,  1087, 10148, -1555, 10148,
     518,   721,  1089, -1555, -1555, 10148, -1555, -1555, -1555,   427,
     442,  1092,    71,    78, 10148,   262,    79, -1555, 10148, 10148,
   10666,   738,   597, -1555, -1555,   622, 10148, -1555,  1278, 10148,
   -1555,  3061,    82,   161, -1555,  1082,   316, 10356, -1555,   586,
   -1555, -1555, -1555,   447,   315,   189,  1121,  1133,  1093,  1136,
    1138, -1555,   480,   784, -1555, 10148, -1555, 10148,  8484, 10148,
   -1555,  1113,  1098, -1555, -1555, 10148,  1099, -1555, 12807, 10148,
    8692,  1100, -1555, 12847, -1555,  8900, -1555, -1555, -1555, -1555,
   -1555,   630, -1555, -1555,   810, -1555,   102,   642,    82, -1555,
   -1555, -1555, -1555,   807, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,  1102, 10666,
   -1555,  1141, 10148, -1555, -1555,   500, 10148, -1555,  1106, -1555,
   -1555, -1555,   479, -1555,  1109,  1149, -1555, -1555,  2341,   509,
     510, -1555, -1555, 10148,  2367,   630, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555,   777,   784,  9108,   224, 11619,
   -1555, -1555,     4,   222, -1555,  1111,   193,  1038, -1555, -1555,
   -1555, -1555,  1041,   588,     4,  1107, -1555, -1555, -1555, -1555,
     635, -1555, -1555, -1555,  1150, 10148, 10148,   674,    86, 10148,
   11659, 11770,  2461,   784, -1555, -1555,   597, -1555,   630,   687,
   -1555,   738, -1555,  1112,  5356,  1160,  1119,    -2,   371, -1555,
   -1555,  1165, -1555, -1555,   189,  1122,     2, 10666, 11805, 10666,
   11916, -1555,   395, 11956, -1555, 10148, 13297, 10148, -1555, 12067,
    5356, -1555,   411, 10148, -1555, -1555, -1555,   413,   642,  1309,
     102, -1555, -1555,   850, -1555,  1124, -1555, -1555, -1555, 10148,
     807, -1555, -1555,  1128,  1131, -1555,   457, -1555, -1555, 10148,
    1175, 10148,  1151, -1555, -1555, -1555, -1555,  1132,  1137,  1140,
   -1555, 10148, 10148, 10148,  1146,  1299,  1153,  1156,  9316, -1555,
       2, -1555,   469, 10148,   210,   222, -1555, 10148, 10148, 10148,
   10148,   721, -1555, 10148, 10148,  1158, -1555, -1555,   485,   497,
   10148, 10148,   705, -1555, -1555, -1555,  1170,   122,  4312, -1555,
   10148,   784, -1555,   519, -1555,   729, 10666,    24, -1555,  1163,
   -1555, -1555,  9524, -1555, -1555,  2739, -1555,   757, -1555, -1555,
   -1555, 10666, 12102, 12213, -1555,   528, -1555, 12253, -1555, -1555,
   -1555, -1555,  1309,   262,  1164,  1299,  1299,   597,  1168,  1177,
    1174,  1166,  1183,  1184,  1185, 10148, -1555, 10148, -1555, -1555,
   -1555, 10148, -1555, -1555,  1299,  1299, -1555, 12364, -1555, -1555,
   13000, 10148, 10148, -1555, 12399, -1555, -1555, 13000, -1555,   630,
   -1555, 10148, -1555,  1219,  1220,  1222, 13000,   522, 10148,   527,
   -1555,   622, -1555,  9732,  9940, -1555, -1555, -1555, -1555, -1555,
   -1555,   630,   597,  1186, 10666,    24,   873, 10148, -1555, 10148,
   -1555, -1555, -1555,   762, -1555, -1555,  1189, -1555, 13872, -1555,
   -1555, -1555, -1555, -1555,   180,   180, -1555, -1555, 10148, -1555,
   10148,  1299,  1299,   518,  1190,  1195,  1023,   180,   518, -1555,
    1355,  1200, -1555, -1555,   288,  1237,   679,  1231, -1555, 10148,
   10148,  1207,  1241, 13000, -1555,   527, -1555, 10148, 10148, -1555,
     679, -1555, -1555,   873, 10148, 10148,   630, -1555, -1555, -1555,
   -1555, -1555,   630, 13872,   518,  1038,  1234, -1555,  1208,  1210,
    1211,  1214,   180,   180,  1038,  1215, -1555, -1555,  1216,  1223,
    1227, 10148,  1228, 10148, 10148,  1230,   679, -1555,  1229,   630,
   13000, -1555, 10148,  1232, -1555, -1555, -1555, 10148,   630,   630,
   -1555, -1555,   642,   597,   529,  1235, -1555, -1555, -1555, -1555,
   -1555,  1242,  1243, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555,  1249,  1233, 13000, -1555,   630, -1555,
   -1555, -1555,   642, -1555,   518, -1555, -1555, -1555, -1555,  1246,
   -1555, -1555,   537, -1555, -1555
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   134,     1,   330,     0,     0,     0,   655,   331,     0,
     647,   647,   647,    17,    18,     0,     0,    16,    15,     3,
       0,    10,     9,     8,     0,     7,   636,     6,    11,     5,
       4,    13,    12,    14,   104,   105,   103,   112,   114,    45,
      61,    58,    59,    47,    48,    49,     0,    50,    56,    46,
     245,   244,   647,   647,    24,    23,   636,   649,   648,   819,
     809,   814,     0,   298,    43,   120,   121,   122,     0,     0,
       0,   123,   125,   132,     0,   119,    19,   669,   668,   238,
     657,     0,   672,   637,   638,     0,     0,     0,     0,    51,
       0,    57,     0,     0,    54,     0,   578,   647,     0,    20,
       0,     0,     0,   300,     0,     0,   131,   126,     0,     0,
       0,     0,     0,     0,   135,   240,   239,   242,   237,   659,
     658,     0,   671,   670,   674,   673,   677,   640,   639,   642,
     110,   111,   108,   109,   107,     0,     0,   106,   115,    62,
      60,    56,    53,    52,   650,   575,   576,   652,   247,   246,
     654,   578,   656,    21,    22,    25,   820,   810,   815,   299,
      41,    44,   130,     0,   127,   128,   129,   133,     0,   660,
       0,   665,   633,   561,    26,    27,    31,     0,    99,   100,
      97,    98,    96,    95,   101,     0,    55,     0,   577,   653,
       0,     0,     0,    42,   124,     0,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,     0,     0,   141,   136,     0,     0,   644,   666,     0,
     678,   634,     0,     0,   563,     0,    28,    29,    30,     0,
     113,     0,   728,   811,   816,   206,   207,   204,   144,   145,
     147,   146,   148,   149,   150,   151,   177,   178,   175,   176,
     168,   179,   180,   169,   166,   167,   205,   188,     0,   203,
     181,   182,   183,   184,   155,   156,   157,   152,   153,   154,
     165,     0,   171,   172,   170,   163,   164,   159,   158,   160,
     161,   162,   143,   142,   187,     0,   173,   174,   561,   139,
     275,   243,   712,   715,   718,   719,   713,   716,   714,   717,
       0,     0,   663,   675,   641,   561,     0,   116,     0,   118,
       0,   622,   620,   643,   102,     0,     0,     0,     0,     0,
       0,   685,   705,   686,   721,   687,   691,   692,   693,   694,
     711,   698,   699,   700,   701,   702,   703,   704,   706,   707,
     708,   709,   779,   690,   697,   710,   786,   793,   688,   695,
     689,   696,     0,     0,     0,     0,   720,   741,   744,   742,
     743,   806,   651,     0,   599,   605,   208,   209,   202,   186,
     210,   189,   185,     0,   137,   329,   590,   591,     0,     0,
     241,     0,   644,   578,   661,     0,   667,   579,   679,     0,
       0,   117,     0,     0,     0,     0,   621,     0,   747,   770,
     773,     0,   776,   766,     0,   732,   780,   787,   794,   800,
     803,     0,     0,   756,   761,   755,     0,   769,   765,   758,
       0,     0,   760,   745,     0,   731,   821,   812,   817,   211,
     191,   192,   194,   193,   195,   196,   197,   198,   190,   199,
     200,   201,     0,   327,   328,     0,   561,   561,   561,   138,
     140,   277,     0,     0,     0,    72,    73,    86,   506,   507,
       0,     0,     0,     0,   315,   501,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,   711,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     550,     0,     0,     0,   253,   383,   385,   384,   386,   387,
     388,   389,     0,     0,    37,   244,   254,     0,     0,     0,
       0,     0,   279,     0,   365,   366,   504,   503,     0,     0,
       0,     0,   270,   265,   262,   261,   263,   264,   283,   248,
     297,   276,   256,   543,   255,   460,     0,   534,    80,    81,
      78,   268,    79,   269,   271,   333,   260,   533,   532,   531,
     134,   537,   502,     0,   257,   536,   535,   509,   461,   510,
     544,   390,   462,     0,   505,   822,   826,   823,   824,   825,
       0,     0,   645,   664,   580,   578,   562,     0,     0,     0,
     543,     0,   624,   625,     0,   618,   619,   617,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   136,     0,
     136,     0,   136,     0,     0,     0,   752,   279,   763,   764,
     757,   759,     0,   762,   746,     0,     0,   808,   807,   729,
     726,   730,   298,   735,   736,     0,   600,   595,     0,     0,
       0,   606,     0,     0,     0,   680,   587,   588,   610,   592,
     593,   594,   868,   871,     0,     0,     0,   303,   307,   306,
     312,     0,     0,   351,     0,     0,     0,   904,     0,     0,
     319,   316,     0,   544,   390,     0,     0,   283,     0,   301,
       0,     0,     0,   342,   345,     0,   274,   348,     0,     0,
      65,     0,    90,   908,     0,   877,   886,     0,   874,     0,
       0,   324,   321,   489,   490,   366,   378,   134,   296,   294,
     295,     0,     0,     0,     0,     0,     0,     0,   846,     0,
       0,     0,   884,   911,     0,   287,     0,   290,     0,     0,
     466,   465,   499,   464,   463,     0,     0,     0,   922,   360,
     922,   367,   272,   273,     0,    84,     0,     0,   913,   922,
       0,   381,   251,   541,     0,   259,   266,   267,   318,   323,
     332,     0,   375,     0,   258,     0,     0,     0,     0,   491,
     492,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   250,     0,
     662,   578,     0,     0,   676,   561,   635,     0,   623,     0,
     627,     0,   632,   631,     0,     0,     0,   737,   750,     0,
       0,   722,   724,     0,     0,   139,     0,   139,     0,   139,
     597,     0,   603,     0,   723,     0,     0,   754,   739,     0,
     727,     0,   601,   813,     0,   607,   818,   589,     0,     0,
     609,     0,   608,     0,   611,     0,     0,     0,    91,     0,
       0,     0,   860,     0,     0,     0,     0,   894,   897,   900,
       0,   922,   320,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,    92,    93,     0,
       0,     0,     0,    63,    64,     0,     0,   922,     0,     0,
     922,     0,     0,   325,   322,   367,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
     880,   838,   846,     0,   889,     0,     0,     0,   846,     0,
       0,     0,     0,   849,     0,   916,     0,   278,     0,    40,
       0,    38,     0,     0,     0,     0,   891,   923,   280,     0,
       0,     0,   437,   434,   436,    68,     0,     0,   915,   923,
     284,   275,   134,     0,   279,   453,     0,   922,     0,     0,
       0,   341,   340,     0,     0,   136,   293,     0,     0,   520,
     519,     0,     0,   521,   525,   467,   468,   480,   481,   478,
     479,     0,   515,     0,   497,     0,   538,   539,   540,   469,
     470,   485,   486,   487,   488,     0,     0,   483,   484,   482,
     476,   477,   472,   471,   473,   474,   475,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,   412,
     421,   400,   422,   401,   424,   403,   423,   402,   425,   404,
     415,   394,   416,   395,   417,   396,   426,   405,   427,   406,
     414,   392,   393,   428,   407,   429,   408,   418,   397,   419,
     398,   420,   399,   413,   391,   458,     0,   646,     0,   569,
     572,     0,     0,   626,   629,   390,   630,   748,   771,   774,
       0,   777,   767,   733,     0,   781,     0,   788,     0,   795,
       0,   801,     0,   804,     0,     0,   285,   751,   740,   596,
     602,     0,   682,   683,   613,   612,     0,     0,     0,     0,
       0,   861,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   400,   401,   403,   402,
     404,   394,   395,   396,   405,   406,   392,   407,   408,   397,
     398,   399,   391,   326,     0,     0,   917,   511,     0,     0,
       0,   512,     0,   542,     0,     0,     0,     0,     0,     0,
     375,   136,   545,   546,   547,   548,   549,     0,     0,   847,
       0,     0,     0,     0,   360,   846,     0,     0,     0,     0,
     855,   856,     0,   863,     0,   853,     0,     0,   892,     0,
       0,     0,     0,   851,   893,     0,   883,   848,   912,     0,
       0,    34,     0,     0,     0,     0,     0,   508,     0,     0,
       0,   136,     0,    66,    67,    68,     0,    83,    74,     0,
     914,     0,   375,     0,   454,     0,     0,     0,   457,   923,
     837,   455,   334,     0,     0,     0,     0,     0,     0,     0,
       0,   373,     0,   139,   516,     0,   522,     0,     0,     0,
     495,     0,     0,   526,   530,     0,     0,   498,     0,     0,
       0,     0,   444,     0,   451,     0,   493,   411,   409,   410,
     459,   135,   570,   571,   572,   573,   564,   578,   375,   628,
     749,   772,   775,   738,   778,   768,   734,   725,   782,   784,
     789,   791,   796,   798,   802,   598,   805,   604,     0,     0,
     681,     0,     0,   869,   872,     0,     0,   304,     0,   309,
     310,   308,     0,   354,     0,     0,   357,   352,     0,     0,
       0,   905,   903,   283,     0,     0,   343,   346,   349,   909,
     907,   878,   887,   885,   875,     0,   139,     0,     0,     0,
     828,   827,   846,     0,   881,     0,     0,   839,   862,   854,
     882,   852,   890,     0,   846,     0,   858,   859,   866,   850,
       0,   288,   291,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,    69,    82,     0,    75,     0,     0,
     249,   136,   456,     0,   279,     0,     0,   620,     0,   370,
     371,     0,   369,   368,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,   527,     0,   500,     0,   496,     0,
     279,   445,     0,     0,   494,   452,   448,     0,   578,   567,
     564,   565,   566,   569,   581,     0,   785,   792,   799,   279,
     286,   684,   614,     0,     0,    87,     0,   305,   311,     0,
       0,     0,     0,   353,   895,   898,   901,     0,     0,     0,
      94,     0,     0,     0,     0,   860,     0,     0,     0,   252,
       0,   551,     0,     0,     0,     0,   864,     0,     0,     0,
       0,     0,   857,     0,     0,   281,    32,    39,     0,     0,
       0,     0,     0,   435,   560,   438,     0,    68,     0,    85,
       0,   139,   430,     0,   335,   620,     0,     0,   377,     0,
     374,   376,     0,   362,   380,     0,   559,     0,   557,   433,
     554,     0,     0,     0,   553,     0,   446,     0,   449,   586,
     568,   582,   567,     0,     0,   860,   860,     0,     0,     0,
       0,     0,     0,     0,     0,   279,   918,   283,   344,   347,
     350,     0,   861,   879,   860,   860,   513,     0,   379,   552,
     920,     0,     0,   865,     0,   830,   829,   920,   867,   920,
     289,   279,   292,    36,     0,     0,   920,     0,     0,     0,
     441,    68,    70,   315,     0,    76,    80,    81,    78,    79,
      77,   920,     0,     0,     0,     0,     0,     0,   372,     0,
     363,   517,   523,     0,   558,   556,     0,   555,     0,   584,
     616,   615,   574,   753,   845,   845,    89,   355,     0,   358,
       0,   860,   860,   835,     0,     0,   922,   845,   835,   514,
       0,     0,   832,   831,     0,     0,     0,   922,    33,     0,
       0,     0,     0,   920,   439,     0,    71,     0,     0,   321,
       0,   382,   431,     0,     0,     0,     0,   364,   518,   524,
     528,   447,     0,     0,     0,   842,   922,   844,     0,     0,
       0,     0,   845,   845,   836,     0,   906,   919,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   923,     0,   920,
     920,   924,     0,     0,   442,   322,   931,     0,     0,     0,
     339,   529,   578,     0,     0,   923,   843,   870,   873,   356,
     359,     0,     0,   902,   910,   888,   876,   921,   929,   834,
     833,   930,   932,   282,     0,     0,   920,   928,     0,   338,
     337,   583,   578,   840,     0,   896,   899,   927,   925,     0,
     336,   585,     0,   926,   841
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1555, -1555,    -1, -1555, -1555, -1555, -1555,   702,  1370, -1555,
   -1555, -1555, -1555, -1555, -1555,  1464, -1555, -1555, -1555,   935,
    1420, -1555,  1327, -1555, -1555,  1381, -1555, -1555, -1555, -1214,
   -1555, -1555, -1555,   -58, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555,  1236, -1555, -1555,   -42,   -37, -1555,
   -1555, -1555,   556,   640,  -536,  -596,  -830, -1555, -1555, -1555,
   -1426, -1555, -1555,    -4,  -224,  -223,  -360, -1555,   473, -1555,
    -619, -1555,  -675,  -165,   364, -1555, -1555, -1555, -1555,  -435,
       0, -1555, -1555, -1555, -1555, -1555,   -52,   -51,   -46, -1555,
     -44, -1555, -1555, -1555,  1482, -1555,   475, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
     244,   -14,   833,    53,   214,  -911,  -470, -1555,  -542, -1555,
   -1555,  -386,   326, -1555, -1555, -1555, -1554, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555,  1451, -1555, -1555, -1555,
   -1555, -1555, -1555,   889, -1555,  -153,    30,   -71,    37,   179,
   -1555,  -150, -1555, -1555, -1555, -1555, -1555, -1555,   591,  -424,
    -896, -1555,  -421,  -893, -1555,  -648,   -64,   -62, -1555,  -600,
   -1352, -1555,  -359, -1555, -1555,  1446, -1555, -1555, -1555,  1115,
    1040,   255, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,  -637,   195,
   -1555,  1048, -1555, -1555, -1555, -1555, -1555, -1555, -1555,  -400,
   -1555, -1555,  -382, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
    -235, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555,  1055,  -679,  -154,  -846,  -709, -1555,
   -1555, -1201,  -932, -1555, -1555, -1555, -1177,    -3, -1127, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555,   282,  -499,
   -1555, -1555, -1555,   781, -1555, -1555, -1555, -1555, -1555, -1555,
   -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1555, -1179,  -741,
   -1555
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   644,    18,   155,    56,    19,   177,   183,  1603,
    1404,  1516,   738,   536,   161,   537,   105,    21,    22,    47,
      48,    49,    94,    23,    41,    42,   538,   539,  1256,  1257,
     540,  1418,  1528,   541,   542,   996,   543,   665,   666,   544,
     545,   546,   547,  1188,   184,   185,    37,    38,    39,   234,
      71,    72,    73,    74,    24,   299,   390,   224,    25,   117,
     225,   118,   168,   548,   150,   716,  1069,   551,   391,   552,
     747,  1602,   757,  1135,   600,   976,  1514,   978,  1515,   554,
     555,   556,   668,   891,  1477,   557,   558,   559,   560,   561,
     562,   563,   564,   455,   565,   771,  1273,  1011,   566,   567,
     929,  1491,   930,  1492,   932,  1493,   568,   896,  1483,   569,
     748,  1544,   570,  1281,  1282,  1015,   718,   571,   828,  1002,
     572,   682,  1070,   574,   575,   576,   994,   577,  1250,  1609,
    1251,  1675,   578,  1063,  1455,   579,   580,  1437,  1688,  1439,
    1689,  1551,  1731,   732,   582,   385,  1463,  1561,  1314,  1316,
    1111,   147,   594,   835,  1638,  1693,   386,   387,   388,   860,
     861,   437,   862,   863,   438,  1212,   656,   657,  1642,   608,
     407,   322,   323,   231,   315,    84,   129,    27,   173,   311,
      95,    96,   187,    97,    28,    53,   121,   170,    29,   396,
     229,   230,    82,   126,   398,    30,   171,   313,   658,   583,
     310,   368,   369,   853,   641,   373,   370,   616,  1326,  1132,
     846,   433,   371,   609,  1320,   865,   614,  1325,   610,  1321,
     611,  1322,   613,  1324,   617,  1328,   618,  1466,   619,  1330,
     620,  1467,   621,  1332,   622,  1468,   623,  1334,   624,  1336,
     647,    31,   101,   191,   374,   648,    32,   102,   192,   375,
     652,    33,   100,   190,   584,  1695,  1705,  1008,   962,  1696,
    1697,  1698,   963,   975,  1234,  1228,  1223,  1398,  1154,   585,
     886,  1473,   887,  1474,   941,  1497,   938,  1495,   964,   758,
     586,   939,  1496,   965,   587,  1160,  1572,  1161,  1573,  1162,
    1574,   900,  1487,   936,  1494,   734,   759,   588,  1661,   988,
     589
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   189,   301,    52,   841,   573,   372,   314,   866,   989,
     885,   762,   924,   645,   733,    64,    75,   650,  1000,    76,
     235,   968,   855,   604,   857,  1126,   859,  1128,   889,  1130,
    1219,   550,   719,   720,   772,  1201,  1231,   646,   651,   680,
     595,  1415,  1388,  1197,   138,   674,  1153,  -134,  1199,   961,
     628,   961,   638,    90,    65,  1674,  1148,   954,   833,   954,
      34,    35,   630,   711,  1208,  1229,   130,   131,    75,    75,
      75,    57,   163,   232,    62,  1537,  1235,    58,  1244,   110,
     111,   112,    86,    66,  1259,  1007,  1013,   394,    91,  1346,
     953,  1405,   966,    54,   970,   122,   123,   152,  1406,  1409,
      77,    78,    50,    79,    63,  1252,  1521,   320,   984,   194,
      75,    75,    75,    75,   550,  1009,  1253,  1254,   104,   997,
     785,  1724,    51,   787,   788,   172,   763,    67,   321,   728,
     730,    80,   787,   788,  1252,   233,    40,   880,   550,   178,
     179,    55,   675,   676,  1542,  1253,  1254,  1211,  1461,   436,
     671,   605,    86,   104,   460,   162,    68,  1265,   701,   654,
    1164,   606,   399,    62,   955,    50,   227,   549,    83,    13,
      62,   871,   764,   842,  1010,     2,   775,   776,   148,  1263,
     655,   946,     3,  1625,    57,    51,   242,   243,   244,    14,
      58,   317,   103,    63,  1536,   316,  1195,   406,   149,  1198,
      63,    87,    81,   148,   882,     4,  1506,     5,  1462,     6,
     808,   809,  1692,  1007,    36,     7,   629,   132,   607,   808,
     809,   300,   133,   149,   134,     8,   983,   135,   631,   881,
      69,     9,   880,   319,   775,   776,    92,   881,   881,    70,
     677,   366,  1014,   592,   902,   872,   632,    93,   550,   881,
     875,   881,   633,  1149,   685,    10,   832,   881,  1255,   678,
      85,   110,   881,   112,   881,  1359,  1270,  1733,   702,  1360,
     136,   881,   881,   779,   780,   943,   401,    11,    12,   881,
     453,   785,   773,  1383,   787,   788,   789,  1611,   880,  1375,
     180,   790,   880,   775,   776,   181,   383,   182,  1211,   882,
     135,   232,   434,  1016,   923,  1348,   392,   955,    98,   397,
     457,   113,   320,  1612,    88,    13,  1276,   434,   458,   550,
     880,  1117,  1263,   454,  1007,  1266,  1277,   942,  1593,   383,
    1508,   779,   780,   321,   232,    14,   114,   880,   955,   785,
      13,   786,   787,   788,   789,  1242,  1243,  1591,  1246,   790,
     550,  1421,   151,  1263,   626,   882,   654,   639,   881,   882,
      14,   883,  1278,   233,   884,   422,   830,   109,  1583,  1264,
     834,   808,   809,   627,   844,   845,   847,   655,   849,   850,
     104,    15,   854,  1279,   856,   640,   858,   882,  1280,   366,
     779,   780,    16,    50,   384,   593,   233,  1676,   785,  1694,
     786,   787,   788,   789,   882,   366,  1114,  1465,   790,  1503,
    1220,  1221,  1012,    51,   654,   880,   239,   904,  1665,  1283,
    1666,  1267,   904,  1245,   873,  1713,    13,  1671,   876,   808,
     809,    43,    44,    45,   366,   655,   367,   366,  1644,  1645,
    1222,   888,  1680,   240,  1699,   836,    14,  1120,  1268,  1310,
    1505,   534,   715,  1436,    93,  1112,  1709,  1657,  1658,    13,
      62,   115,  1511,  1134,    46,    13,  1262,   116,   927,   990,
     366,   672,  1007,   300,   991,  1238,   805,   806,   807,    14,
     139,   550,   882,   318,  1209,    14,   696,   300,   808,   809,
      63,    86,   904,   643,  1723,  1123,  1342,   300,   300,   300,
    1260,  1741,  1742,   113,  1146,  1147,  1382,    13,  1425,  1217,
     141,   366,   366,   977,  1033,   160,   400,   992,  1163,  1349,
     119,  1394,   144,  1185,  1702,  1703,   120,    14,  1311,  1034,
    1225,   995,    13,   998,  1423,  1427,   961,   752,   753,  1387,
    1754,  1755,    13,    13,   775,   776,  1500,  1189,  1190,    50,
    1192,   961,    14,    13,  1194,   765,  1196,   766,   767,   768,
    1226,   769,    14,    14,  1434,    13,    13,    13,  1354,    51,
     643,   643,   774,    14,   124,  1296,   156,  1769,  1131,  1133,
     125,   643,   226,  1526,   367,    14,    14,    14,   904,  1338,
    1297,  1538,    13,   643,   643,   643,  1307,  1308,  1309,   434,
     367,  1372,  1374,   840,   904,  1376,   904,    13,  1408,   366,
     366,   366,    14,   366,   366,  1549,   157,   366,   145,   366,
     643,   366,  1301,   366,   106,   107,   108,    14,  1401,   367,
    1136,  1556,   367,  1558,  1252,   643,  1139,  1302,   146,  1140,
    1425,   779,   780,  1402,    13,  1253,  1254,   591,  1144,   785,
     904,   786,   787,   788,   789,  1413,  1150,   423,    91,   790,
     300,  1159,   904,    50,    14,  1426,   164,   165,   166,   167,
      62,   893,   643,  1434,    13,    13,   955,  1567,   904,   990,
    1479,  1107,  1110,    51,   424,   425,   366,   300,  1488,  1589,
     904,  1263,   423,   904,    14,    14,    13,   158,  1435,  1183,
      63,  1622,   643,   643,   649,  1604,   367,   367,  1335,   366,
    1485,  1486,  1217,  1337,   148,   904,    14,  1605,   681,   424,
     425,  1217,  1217,   393,   643,   803,   804,   805,   806,   807,
    1217,  -783,   300,   974,   149,  1339,  -783,  1623,  1509,   808,
     809,  1136,  1672,  1136,  1385,  1533,  1636,   188,   426,  1763,
     300,   993,   427,  -790,  -783,   553,  1001,  1774,  -790,  1386,
      50,   440,   441,   442,   443,   444,   445,   446,   447,   300,
    -797,  1555,    13,  1510,  -440,  -797,  -790,   750,  1358,  -440,
      51,   904,   404,   426,   159,   405,   605,   427,   406,   169,
     448,  1232,    14,  -797,  1233,    13,   606,  -440,   686,  1134,
     449,   450,   451,  1364,   367,   367,   367,   145,   367,   367,
      13,  1239,   367,  1240,   367,    14,   367,   687,   367,   428,
    1513,   423,   110,   429,    13,  1531,   430,   146,   904,   972,
      14,   973,    75,  1499,   110,   111,   112,   366,   553,  1339,
    1339,   431,  1116,   148,    14,   193,   148,   432,   424,   425,
    1564,   423,  1124,   607,   428,   435,  1592,  1459,   429,  1520,
     434,   430,   553,   149,  1118,   366,   149,   904,  1412,   703,
     706,   366,  1530,   894,   366,   573,   431,  1315,   424,   425,
     904,   367,   432,   366,   228,  1323,   366,   366,   704,   707,
    1608,   366,   895,   127,   236,   237,   366,   153,   904,   128,
     366,   550,  1655,   154,   367,  1395,   423,  1312,  1396,   241,
     434,  1397,   426,  1313,  1121,  1708,   427,   434,   300,   296,
     434,  1122,   901,   903,  1125,  1624,  1718,   434,   406,   366,
     366,  1127,   366,   424,   425,   297,   366,  1627,   366,   366,
     434,   434,   426,   434,  1129,  1347,   427,  1357,   937,   312,
     300,   940,  1480,    13,   434,  1736,  1654,   298,  1632,   434,
    1134,  1218,   376,  1690,  1227,   377,   378,  1218,  1227,   379,
    1020,  1024,   553,    14,  1704,   380,   366,   381,   366,  1704,
     389,   643,  1667,   428,   382,  1038,   395,   429,   402,  1184,
     430,   174,   175,   979,   980,   403,  1498,   426,   534,   715,
     605,   427,   408,  1064,   434,   431,   534,   715,  1006,   409,
     606,   432,   410,   428,  1599,  1734,  1714,   429,   411,  1200,
     430,    43,    44,    45,   412,  1684,  1685,   142,   143,  1414,
     413,  1621,   367,   439,   414,   431,   421,  1420,   174,   175,
     176,   432,   415,   553,   236,   237,   238,   659,   660,   661,
      59,    60,    61,   416,   417,  1545,   418,   419,   420,   456,
     367,   459,   300,   300,   300,   452,   367,   607,   428,   367,
     596,   602,   429,  1106,   553,   430,   603,   615,   367,   636,
     662,   367,   367,  1727,   -88,  1772,   367,   663,   673,   690,
     431,   367,   692,   302,   693,   367,   432,   303,   695,   694,
     698,   697,   705,   739,  1470,   708,   709,   751,   737,   721,
    1317,   304,   305,   755,  1319,   722,   306,   307,   308,   309,
     366,   760,  1475,   770,   367,   367,   723,   367,   724,   725,
     726,   367,   366,   367,   367,    16,  1152,  1072,  1074,  1076,
    1078,  1080,  1082,  1084,  1086,  1088,  1090,   829,  1093,  1095,
    1097,  1099,  1101,  1103,  1626,   701,  1355,   837,   735,   839,
     366,   366,   -77,   851,   852,   649,   867,  1464,   870,   874,
    1719,   367,   879,   367,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   366,   553,   553,   553,   553,
     553,   553,   423,  1527,   897,   878,   890,   892,   898,   899,
     904,   773,   922,   750,  1547,   553,   928,   933,   934,   945,
     971,   750,   982,  1543,   985,   986,   987,   999,  1031,   424,
     425,  1109,  1113,  1119,  1137,  1142,  1143,  1157,  1186,   423,
    1216,  1217,  1210,  1248,  1224,  1237,  1247,  1269,  1249,  1272,
    1294,  1271,  1683,  1284,  1275,  1285,  1286,  1287,   300,  1288,
     366,  1289,  1299,  1300,   423,  1304,   424,   425,  1305,  1315,
     810,   811,   812,   813,   814,   815,   816,   817,  1327,  1341,
    1350,  1329,  1331,   818,   819,  1333,  1351,  1543,  1352,   820,
     581,   424,   425,   426,  1353,  1356,  1363,   427,  1403,   821,
    1417,  1377,   822,   823,  1429,  1362,  1370,  1373,  1389,   824,
     825,   826,  1422,   462,   463,  1391,  1430,  1399,  1559,  1432,
    1458,  1433,  1431,  1444,  1471,   367,  1633,  1445,  1447,  1453,
     426,  1469,  1482,   473,   427,  1512,  1478,   367,  1481,   478,
    1507,  1517,  1532,  1534,   717,   717,   717,  1535,  1539,   366,
    1541,   300,  1560,  1498,  1646,   426,   827,  1565,  1569,   427,
    1566,  1575,  1571,   664,   428,   367,   367,  1576,   429,  1577,
    1318,   430,   423,   684,  1490,  1581,   492,   493,   689,  1582,
     691,  1610,  1584,   534,   715,  1585,   431,  1601,  1648,   700,
     367,  1218,   432,  1628,  1643,  1650,   761,   712,  1647,   424,
     425,   428,  1345,  1218,  1649,   429,  1668,  1343,   430,  1681,
     495,   496,  1651,  1652,  1653,  1669,  1682,  1670,  1711,  1691,
    1706,   736,   300,   431,   761,  1707,   428,  1529,  1712,   432,
     429,  1715,  1344,   430,  1717,  1721,  1722,  1735,  1737,   754,
    1738,  1739,   300,  1767,  1740,  1743,  1744,   366,   431,   366,
     981,    62,  1716,  1745,   432,   367,  1748,  1746,  1751,  1753,
    1757,  1768,  1378,   426,  1764,   137,  1726,   427,   511,   512,
     513,    50,  1765,  1766,  1773,    20,    89,   423,   186,   140,
    1615,    63,  1108,  1393,  1261,   324,  1616,  1617,   423,  1400,
     831,   525,  1618,    26,  1619,  1274,  1588,  1540,  1407,  1428,
    1562,  1639,  1752,  1460,   424,   425,   300,   843,   667,  1640,
    1563,  1641,    99,  1419,  1710,   424,   425,   590,  1598,  1392,
    1762,   969,     0,     0,     0,   532,     0,   761,   669,     0,
       0,     0,     0,     0,   428,   670,     0,     0,   429,     0,
    1361,   430,  1442,     0,   367,     0,   366,     0,     0,     0,
       0,     0,     0,     0,  1452,   761,   431,     0,     0,  1457,
       0,   366,   432,     0,     0,     0,     0,     0,   426,     0,
       0,     0,   427,   300,     0,     0,     0,     0,   684,   426,
       0,     0,     0,   427,     0,     0,   423,   925,     0,     0,
       0,     0,  1761,     0,     0,     0,     0,     0,     0,  1660,
    1476,     0,     0,     0,     0,     0,  1660,     0,  1660,   944,
       0,     0,     0,   424,   425,  1660,   717,     0,     0,   993,
       0,     0,  1771,     0,     0,     0,     0,   960,   300,   960,
    1660,  1502,     0,     0,   366,   553,     0,     0,     0,   428,
       0,     0,   367,   429,   367,  1366,   430,     0,     0,     0,
     428,     0,     0,     0,   429,     0,  1367,   430,     0,  1518,
    1519,   431,     0,  1522,   775,   776,     0,   432,     0,     0,
       0,     0,   431,     0,     0,     0,     0,   426,   432,     0,
       0,   427,  1660,     0,     0,   993,   761,     0,     0,     0,
       0,     0,     0,     0,     0,  1730,     0,     0,     0,     0,
       0,  1732,     0,     0,     0,     0,     0,   423,     0,  1071,
    1073,  1075,  1077,  1079,  1081,  1083,  1085,  1087,  1089,  1091,
    1092,  1094,  1096,  1098,  1100,  1102,  1104,     0,  1660,  1660,
       0,     0,     0,     0,   424,   425,     0,  1759,  1760,   300,
    1115,   367,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,   429,     0,  1368,   430,   367,     0,     0,   777,
     778,   779,   780,   781,     0,  1660,   782,  1770,     0,   785,
     431,   786,   787,   788,   789,  1607,   432,     0,     0,   790,
       0,   791,   792,     0,  1145,     0,     0,   761,     0,     0,
       0,     0,  1156,     0,  1158,     0,     0,   717,   426,     0,
       0,     0,   427,     0,  1166,  1167,  1168,  1169,  1170,  1171,
    1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,     0,     0,     0,     0,  1656,     0,     0,     0,     0,
       0,     0,     0,   801,   802,   803,   804,   805,   806,   807,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   808,
     809,     0,     0,   601,     0,     0,     0,   681,     0,   428,
    1236,     0,   612,   429,     0,  1369,   430,     0,     0,  1241,
       0,     0,   625,     0,     0,     0,     0,     0,     0,     0,
       0,   431,   635,     0,     0,  1258,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   717,
     717,   717,     0,   653,   761,     0,   761,     0,   761,     0,
     761,     0,   761,     0,   761,     0,   761,     0,   761,     0,
     761,   903,   761,     0,   761,   683,     0,   761,     0,   761,
       0,   761,     0,   761,     0,   761,     0,   761,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,   727,
     713,   714,     0,   775,   776,   325,     0,     0,     0,     0,
       0,   326,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   328,   740,   741,
     742,   743,   744,   749,   749,   329,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     330,     0,     0,     0,     0,     0,     0,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
       0,     0,     0,     0,     0,     0,     0,     0,   777,   778,
     779,   780,   781,     0,     0,   782,   783,   784,   785,     0,
     786,   787,   788,   789,     0,     0,     0,     0,   790,     0,
     791,   792,     0,     0,     0,     0,     0,  1365,   749,    62,
       0,     0,     0,     0,     0,     0,     0,   869,     0,     0,
       0,     0,   364,     0,     0,     0,     0,     0,     0,     0,
    1380,  1381,     0,     0,     0,  1384,     0,     0,     0,    63,
       0,     0,     0,  1390,     0,     0,   960,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   749,
     683,   800,   801,   802,   803,   804,   805,   806,   807,     0,
       0,   926,     0,     0,     0,  1416,   931,     0,   808,   809,
     581,     0,   935,     0,     0,   749,     0,   365,   749,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -360,
       0,     0,   947,   948,   949,   950,   951,   952,     0,     0,
     775,   776,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,     0,
       0,     0,     0,     0,     0,   749,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1025,  1026,
       0,  1472,  1027,  1028,  1029,  1030,     0,  1032,     0,  1035,
    1036,  1037,  1039,  1040,  1041,  1042,  1043,  1044,  1046,  1047,
    1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,     0,
    1065,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   777,   778,   779,   780,   781,
     749,     0,   782,   783,   784,   785,     0,   786,   787,   788,
     789,     0,     0,     0,     0,   790,     0,   791,   792,     0,
       0,     0,     0,   793,   794,   795,     0,     0,     0,   796,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     423,     0,     0,     0,  -360,     0,     0,     0,     0,  1141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   749,  -360,     0,     0,   424,   425,   775,
     776,     0,  -360,     0,   797,  1165,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,     0,     0,  1568,     0,
    1570,     0,     0,     0,   423,   808,   809,     0,     0,     0,
    1578,  1579,  1580,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1595,  1596,     0,
     423,   424,   425,  1600,     0,     0,     0,     0,   740,  1214,
     749,   426,     0,     0,     0,   427,     0,  1620,   749,     0,
       0,     0,     0,     0,     0,     0,     0,   424,   425,     0,
       0,  1630,     0,     0,     0,     0,     0,     0,  1165,     0,
       0,     0,     0,     0,   777,   778,   779,   780,   781,     0,
       0,   782,   783,   784,   785,   749,   786,   787,   788,   789,
       0,     0,     0,     0,   790,   426,   791,   792,     0,   427,
       0,     0,   793,   794,   795,     0,     0,     0,   796,     0,
    1662,  1663,   428,     0,     0,     0,   429,     0,  1371,   430,
       0,   426,     0,     0,   423,   427,     0,     0,     0,     0,
       0,     0,     0,  1679,   431,     0,     0,     0,     0,  1298,
     432,     0,     0,  1303,     0,     0,  1686,     0,  1687,     0,
       0,   424,   425,   797,     0,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   807,     0,   428,  1700,     0,  1701,
     429,     0,  1484,   430,   808,   809,     0,     0,   838,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,   729,
       0,     0,   428,     0,   432,   325,   429,  1725,  1489,   430,
       0,   326,     0,  1728,  1729,     0,     0,   327,     0,     0,
       0,     0,     0,     0,   431,   426,     0,   328,     0,   427,
     432,     0,     0,     0,     0,   329,     0,     0,     0,   749,
    1747,     0,  1749,  1750,     0,     0,     0,     0,     0,     0,
     330,     0,     0,     0,     0,     0,  1758,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
       0,     0,     0,     0,     0,     0,   428,     0,     0,   749,
     429,  1379,  1525,   430,   749,     0,   775,   776,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
     749,     0,     0,     0,   432,     0,   749,     0,     0,    62,
       0,     0,     0,     0,     0,   749,     0,     0,     0,  1410,
    1411,     0,   364,     0,     0,     0,     0,   775,   776,     0,
     749,     0,     0,     0,     0,     0,     0,     0,  1165,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1438,     0,  1440,   749,
    1443,     0,     0,     0,     0,     0,  1446,     0,     0,     0,
    1449,   749,     0,     0,     0,     0,   749,     0,     0,     0,
       0,   777,   778,   779,   780,   781,     0,   365,   782,   783,
     784,   785,   423,   786,   787,   788,   789,     0,     0,     0,
       0,   790,     0,   791,   792,     0,     0,     0,     0,   793,
     794,   795,     0,     0,     0,   796,     0,   749,     0,   424,
     425,     0,   777,   778,   779,   780,   781,     0,     0,   782,
     783,   784,   785,     0,   786,   787,   788,   789,     0,     0,
       0,     0,   790,     0,   791,   792,     0,     0,   749,     0,
     793,   794,   795,     0,     0,     0,   796,     0,     0,     0,
     797,     0,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,     0,     0,     0,     0,   749,   749,     0,     0,
     749,   808,   809,   426,     0,   848,     0,   427,     0,     0,
       0,     0,     0,     0,     0,   749,     0,     0,     0,     0,
       0,   797,     0,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   807,     0,     0,     0,  1552,     0,  1553,     0,
       0,   749,   808,   809,  1557,     0,   864,     0,     0,     0,
       0,     0,   775,   776,     0,     0,     0,     0,     0,     0,
     749,   325,     0,     0,     0,     0,     0,   326,     0,     0,
       0,     0,     0,   327,   428,     0,     0,     0,   429,     0,
    1631,   430,     0,   328,     0,     0,     0,     0,     0,  1587,
       0,   329,     0,     0,  1590,     0,   431,     0,  1594,     0,
       0,  1597,   432,     0,     0,     0,   330,     0,     0,     0,
       0,  1606,   749,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,     0,   777,   778,   779,
     780,     0,     0,     0,     0,     0,     0,   785,     0,   786,
     787,   788,   789,     0,     0,     0,   749,   790,     0,   791,
     792,     0,   749,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,   749,     0,     0,     0,     0,     0,   364,  1673,
       0,     0,   461,     0,   749,     0,   462,   463,     3,     0,
     464,   465,   466,     0,   467,    63,   468,   469,   470,   471,
     472,     0,     0,     0,     0,     0,   473,   474,   475,   476,
     477,     0,   478,   803,   804,   805,   806,   807,     0,   479,
     480,     0,     0,   481,     0,   482,   483,   808,   809,   484,
       0,     8,   485,   486,     0,   487,   488,     0,     0,   489,
     490,  1720,     0,   365,     0,   637,   491,     0,   749,   492,
     493,     0,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   494,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,     0,   358,
     359,   360,   361,   495,   496,   497,   498,     0,     0,     0,
       0,     0,     0,  1756,     0,     0,     0,     0,     0,   499,
     500,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   501,   502,   503,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   504,   505,   506,   507,   508,     0,   509,     0,
     510,   511,   512,   513,    50,   148,   514,   515,   516,   517,
     518,   519,   520,   521,    63,   522,   523,   524,     0,     0,
       0,     0,     0,     0,   525,   149,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,     0,    15,     0,     0,
     530,   531,     0,     0,   462,   463,     0,     0,   532,     0,
     533,     0,   534,   535,   468,   469,   470,   471,   472,     0,
       0,     0,     0,     0,   473,     0,   475,     0,     0,     0,
     478,     0,   423,     0,     0,     0,     0,     0,   480,     0,
       0,     0,     0,     0,   483,     0,     0,   484,     0,     0,
     485,     0,   954,     0,   488,     0,     0,     0,     0,   424,
     425,     0,     0,     0,   597,     0,     0,   492,   493,     0,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   494,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   495,   496,   598,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   499,   500,     0,
       0,     0,     0,   426,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     504,   505,   506,   507,   508,     0,   509,   955,   510,   511,
     512,   513,    50,     0,     0,   515,   516,   517,   518,   519,
     520,   521,   956,   599,   523,   524,     0,     0,     0,     0,
       0,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,   429,     0,
       0,   957,   528,   529,     0,    15,     0,     0,   530,   531,
       0,     0,     0,   462,   463,     0,   958,     0,   959,     0,
     534,   535,   432,   468,   469,   470,   471,   472,     0,     0,
       0,     0,     0,   473,     0,   475,     0,     0,     0,   478,
       0,   423,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,   483,     0,     0,   484,     0,     0,   485,
       0,     0,     0,   488,     0,     0,     0,     0,   424,   425,
       0,     0,     0,   597,     0,     0,   492,   493,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   494,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     495,   496,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   500,     0,     0,
       0,     0,   426,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   504,
     505,   506,   507,   508,     0,   509,   955,   510,   511,   512,
     513,    50,     0,     0,   515,   516,   517,   518,   519,   520,
     521,   956,   599,   523,   524,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   428,     0,     0,     0,   429,     0,     0,
     957,   528,   529,     0,    15,     0,     0,   530,   531,     0,
       0,     0,   462,   463,     0,   958,     0,   967,     0,   534,
     535,   432,   468,   469,   470,   471,   472,     0,     0,     0,
       0,     0,   473,     0,   475,     0,     0,     0,   478,     0,
     630,     0,     0,     0,     0,     0,   480,     0,     0,     0,
       0,     0,   483,     0,     0,   484,     0,     0,   485,     0,
       0,     0,   488,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   597,     0,     0,   492,   493,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   494,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   495,
     496,   598,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   499,   500,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   504,   505,
     506,   507,   508,     0,   509,     0,   510,   511,   512,   513,
      50,     0,     0,   515,   516,   517,   518,   519,   520,   521,
      63,   599,   523,   524,     0,     0,     0,     0,     0,     0,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   631,     0,     0,   527,
     528,   529,     0,    15,     0,     0,   530,   531,     0,     0,
       0,   462,   463,     0,  1213,     0,   533,     0,   534,   535,
     633,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,     0,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,   679,     0,     0,     0,     0,     0,   501,
     502,   503,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,   462,
     463,     0,     0,   532,     0,   533,     0,   534,   535,   468,
     469,   470,   471,   472,     0,     0,     0,     0,     0,   473,
       0,   475,     0,     0,     0,   478,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,   483,
       0,     0,   484,     0,     0,   485,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   597,
       0,     0,   492,   493,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   494,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   495,   496,   497,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   499,   500,     0,     0,     0,     0,     0,     0,
       0,   710,     0,     0,     0,     0,     0,   501,   502,   503,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   504,   505,   506,   507,   508,
       0,   509,     0,   510,   511,   512,   513,    50,     0,     0,
     515,   516,   517,   518,   519,   520,   521,    63,   599,   523,
     524,     0,     0,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,   528,   529,     0,
      15,     0,     0,   530,   531,     0,     0,   462,   463,     0,
       0,   532,     0,   533,     0,   534,   535,   468,   469,   470,
     471,   472,     0,     0,     0,     0,     0,   473,  1613,   475,
     476,     0,     0,   478,     0,     0,     0,     0,     0,     0,
       0,   480,     0,     0,     0,     0,     0,   483,     0,     0,
     484,     0,     0,   485,   486,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   597,     0,     0,
     492,   493,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   494,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   495,   496,   598,  1614,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,   500,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   506,   507,   508,     0,   509,
       0,   510,   511,   512,   513,    50,     0,     0,   515,   516,
     517,   518,   519,   520,   521,    63,   599,   523,   524,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,   528,   529,     0,    15,     0,
       0,   530,   531,     0,     0,   462,   463,     0,     0,   532,
       0,   533,     0,   534,   535,   468,   469,   470,   471,   472,
       0,     0,     0,     0,     0,   473,     0,   475,     0,     0,
       0,   478,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,   483,     0,     0,   484,     0,
       0,   485,     0,     0,     0,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   597,     0,     0,   492,   493,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     494,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   495,   496,   497,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   499,   500,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   501,   502,   503,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   504,   505,   506,   507,   508,     0,   509,     0,   510,
     511,   512,   513,    50,     0,     0,   515,   516,   517,   518,
     519,   520,   521,    63,   599,   523,   524,     0,     0,     0,
       0,     0,     0,   525,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,   528,   529,     0,    15,     0,     0,   530,
     531,     0,     0,   462,   463,     0,     0,   532,     0,   533,
       0,   534,   535,   468,   469,   470,   471,   472,     0,     0,
       0,     0,     0,   473,     0,   475,     0,     0,     0,   478,
       0,     0,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,   483,     0,     0,   484,     0,     0,   485,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   597,     0,     0,   492,   493,  1003,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   494,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     495,   496,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   504,
     505,   506,   507,   508,     0,   509,   955,   510,   511,   512,
     513,    50,     0,     0,   515,   516,   517,   518,   519,   520,
     521,   956,   599,   523,   524,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,   528,   529,     0,    15,     0,     0,   530,   531,     0,
       0,   462,   463,     0,     0,  1004,     0,   533,  1005,   534,
     535,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,     0,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1066,
    1067,  1068,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,     0,
       0,   462,   463,   532,     0,   533,     0,   534,   535,   745,
       0,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,   746,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,     0,
       0,   462,   463,   532,   634,   533,     0,   534,   535,   745,
       0,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,   746,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,   955,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,   956,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,     0,
       0,   462,   463,   532,     0,   533,     0,   534,   535,   745,
       0,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,   746,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,     0,
       0,   462,   463,   532,   867,   533,     0,   534,   535,   745,
       0,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,   746,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,   462,
     463,     0,     0,   532,     0,   533,     0,   534,   535,   468,
     469,   470,   471,   472,     0,     0,     0,     0,     0,   473,
       0,   475,     0,     0,     0,   478,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,   483,
       0,     0,   484,     0,     0,   485,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   597,
       0,     0,   492,   493,  1151,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   494,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   495,   496,   598,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   499,   500,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   504,   505,   506,   507,   508,
       0,   509,   955,   510,   511,   512,   513,    50,     0,     0,
     515,   516,   517,   518,   519,   520,   521,   956,   599,   523,
     524,     0,     0,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,   528,   529,     0,
      15,     0,     0,   530,   531,     0,     0,   462,   463,     0,
       0,   532,     0,   533,     0,   534,   535,   468,   469,   470,
     471,   472,     0,     0,     0,     0,     0,   473,     0,   475,
       0,     0,     0,   478,     0,     0,     0,     0,     0,     0,
       0,   480,     0,     0,     0,     0,     0,   483,     0,     0,
     484,     0,     0,   485,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   597,     0,     0,
     492,   493,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   494,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   495,   496,   598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,   500,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   506,   507,   508,     0,   509,
       0,   510,   511,   512,   513,    50,     0,     0,   515,   516,
     517,   518,   519,   520,   521,    63,   599,   523,   524,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,   528,   529,     0,    15,     0,
       0,   530,   531,     0,     0,   462,   463,     0,     0,   532,
     634,   533,     0,   534,   535,   468,   469,   470,   471,   472,
       0,     0,     0,     0,     0,   473,     0,   475,     0,     0,
       0,   478,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,   483,     0,     0,   484,     0,
       0,   485,     0,     0,     0,   488,     0,     0,     0,     0,
       0,   688,     0,     0,     0,   597,     0,     0,   492,   493,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     494,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   495,   496,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   499,   500,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   504,   505,   506,   507,   508,     0,   509,     0,   510,
     511,   512,   513,    50,     0,     0,   515,   516,   517,   518,
     519,   520,   521,    63,   599,   523,   524,     0,     0,     0,
       0,     0,     0,   525,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,   528,   529,     0,    15,     0,     0,   530,
     531,     0,     0,   462,   463,     0,     0,   532,     0,   533,
       0,   534,   535,   468,   469,   470,   471,   472,     0,     0,
       0,     0,     0,   473,     0,   475,     0,     0,     0,   478,
       0,     0,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,   483,     0,     0,   484,     0,     0,   485,
       0,     0,     0,   488,     0,     0,   699,     0,     0,     0,
       0,     0,     0,   597,     0,     0,   492,   493,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   494,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     495,   496,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   504,
     505,   506,   507,   508,     0,   509,     0,   510,   511,   512,
     513,    50,     0,     0,   515,   516,   517,   518,   519,   520,
     521,    63,   599,   523,   524,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,   528,   529,     0,    15,     0,     0,   530,   531,     0,
       0,     0,     0,   462,   463,   532,     0,   533,     0,   534,
     535,   731,     0,   468,   469,   470,   471,   472,     0,     0,
       0,     0,     0,   473,     0,   475,     0,     0,     0,   478,
       0,     0,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,   483,     0,     0,   484,     0,     0,   485,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   597,     0,     0,   492,   493,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   494,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     495,   496,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   504,
     505,   506,   507,   508,     0,   509,     0,   510,   511,   512,
     513,    50,     0,     0,   515,   516,   517,   518,   519,   520,
     521,    63,   599,   523,   524,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,   528,   529,     0,    15,     0,     0,   530,   531,     0,
       0,   462,   463,     0,     0,   532,     0,   533,     0,   534,
     535,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,     0,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   735,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,     0,
       0,   462,   463,   532,     0,   533,     0,   534,   535,   756,
       0,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,     0,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,   462,
     463,     0,     0,   532,     0,   533,     0,   534,   535,   468,
     469,   470,   471,   472,     0,     0,  1045,     0,     0,   473,
       0,   475,     0,     0,     0,   478,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,   483,
       0,     0,   484,     0,     0,   485,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   597,
       0,     0,   492,   493,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   494,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   495,   496,   598,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   499,   500,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   504,   505,   506,   507,   508,
       0,   509,     0,   510,   511,   512,   513,    50,     0,     0,
     515,   516,   517,   518,   519,   520,   521,    63,   599,   523,
     524,     0,     0,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,   528,   529,     0,
      15,     0,     0,   530,   531,     0,     0,   462,   463,     0,
       0,   532,     0,   533,     0,   534,   535,   468,   469,   470,
     471,   472,     0,     0,     0,     0,     0,   473,     0,   475,
       0,     0,     0,   478,     0,     0,     0,     0,     0,     0,
       0,   480,     0,     0,     0,     0,     0,   483,     0,     0,
     484,     0,     0,   485,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   597,     0,     0,
     492,   493,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   494,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   495,   496,   598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,   500,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   506,   507,   508,     0,   509,
       0,   510,   511,   512,   513,    50,     0,     0,   515,   516,
     517,   518,   519,   520,   521,    63,   599,   523,   524,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,   528,   529,     0,    15,     0,
       0,   530,   531,     0,     0,   462,   463,     0,     0,   532,
       0,   533,  1105,   534,   535,   468,   469,   470,   471,   472,
       0,     0,     0,     0,     0,   473,     0,   475,     0,     0,
       0,   478,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,   483,     0,     0,   484,     0,
       0,   485,     0,     0,     0,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   597,     0,     0,   492,   493,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     494,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   495,   496,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   499,   500,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   504,   505,   506,   507,   508,     0,   509,     0,   510,
     511,   512,   513,    50,     0,     0,   515,   516,   517,   518,
     519,   520,   521,    63,   599,   523,   524,     0,     0,     0,
       0,     0,     0,   525,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1155,     0,   527,   528,   529,     0,    15,     0,     0,   530,
     531,     0,     0,   462,   463,     0,     0,   532,     0,   533,
       0,   534,   535,   468,   469,   470,   471,   472,     0,     0,
       0,     0,     0,   473,     0,   475,     0,     0,     0,   478,
       0,     0,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,   483,     0,     0,   484,     0,     0,   485,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   597,     0,     0,   492,   493,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   494,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     495,   496,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   504,
     505,   506,   507,   508,     0,   509,     0,   510,   511,   512,
     513,    50,     0,     0,   515,   516,   517,   518,   519,   520,
     521,    63,   599,   523,   524,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,   528,   529,     0,    15,     0,     0,   530,   531,     0,
       0,   462,   463,     0,     0,   532,     0,   533,  1215,   534,
     535,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,     0,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,   462,
     463,     0,     0,   532,     0,   533,  1230,   534,   535,   468,
     469,   470,   471,   472,     0,     0,     0,     0,     0,   473,
       0,   475,     0,     0,     0,   478,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,   483,
       0,     0,   484,     0,     0,   485,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   597,
       0,     0,   492,   493,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   494,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   495,   496,   598,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   499,   500,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   504,   505,   506,   507,   508,
       0,   509,     0,   510,   511,   512,   513,    50,     0,     0,
     515,   516,   517,   518,   519,   520,   521,    63,   599,   523,
     524,     0,     0,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,   528,   529,     0,
      15,     0,     0,   530,   531,     0,     0,   462,   463,     0,
       0,   532,     0,   533,  1441,   534,   535,   468,   469,   470,
     471,   472,     0,     0,     0,     0,     0,   473,     0,   475,
       0,     0,     0,   478,     0,     0,     0,     0,     0,     0,
       0,   480,     0,     0,     0,     0,     0,   483,     0,     0,
     484,     0,     0,   485,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   597,     0,     0,
     492,   493,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   494,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   495,   496,   598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,   500,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   506,   507,   508,     0,   509,
       0,   510,   511,   512,   513,    50,     0,     0,   515,   516,
     517,   518,   519,   520,   521,    63,   599,   523,   524,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,   528,   529,     0,    15,     0,
       0,   530,   531,     0,     0,   462,   463,     0,     0,  1450,
       0,   533,  1451,   534,   535,   468,   469,   470,   471,   472,
       0,     0,     0,     0,     0,   473,     0,   475,     0,     0,
       0,   478,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,   483,     0,     0,   484,     0,
       0,   485,     0,     0,     0,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   597,     0,     0,   492,   493,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     494,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   495,   496,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   499,   500,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   504,   505,   506,   507,   508,     0,   509,     0,   510,
     511,   512,   513,    50,     0,     0,   515,   516,   517,   518,
     519,   520,   521,    63,   599,   523,   524,     0,     0,     0,
       0,     0,     0,   525,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,   528,   529,     0,    15,     0,     0,   530,
     531,     0,     0,   462,   463,     0,     0,   532,     0,   533,
    1456,   534,   535,   468,   469,   470,   471,   472,     0,     0,
       0,     0,     0,   473,     0,   475,     0,     0,     0,   478,
       0,     0,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,   483,     0,     0,   484,     0,     0,   485,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   597,     0,     0,   492,   493,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   494,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     495,   496,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   504,
     505,   506,   507,   508,     0,   509,     0,   510,   511,   512,
     513,    50,     0,     0,   515,   516,   517,   518,   519,   520,
     521,    63,   599,   523,   524,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,   528,   529,     0,    15,     0,     0,   530,   531,     0,
       0,   462,   463,     0,     0,   532,     0,   533,  1501,   534,
     535,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,     0,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,     0,    15,     0,     0,   530,   531,     0,     0,   462,
     463,     0,     0,   532,     0,   533,  1586,   534,   535,   468,
     469,   470,   471,   472,     0,     0,     0,     0,     0,   473,
       0,   475,     0,     0,     0,   478,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,   483,
       0,     0,   484,     0,     0,   485,     0,     0,     0,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   597,
       0,     0,   492,   493,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   494,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   495,   496,   598,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   499,   500,     0,     0,     0,     0,     0,     0,
       0,  1629,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   504,   505,   506,   507,   508,
       0,   509,     0,   510,   511,   512,   513,    50,     0,     0,
     515,   516,   517,   518,   519,   520,   521,    63,   599,   523,
     524,     0,     0,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,   528,   529,     0,
      15,     0,     0,   530,   531,     0,     0,   462,   463,     0,
       0,   532,     0,   533,     0,   534,   535,   468,   469,   470,
     471,   472,     0,     0,     0,     0,     0,   473,     0,   475,
       0,     0,     0,   478,     0,     0,     0,     0,     0,     0,
       0,   480,     0,     0,     0,     0,     0,   483,     0,     0,
     484,     0,     0,   485,     0,     0,     0,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   597,     0,     0,
     492,   493,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   494,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   495,   496,   598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,   500,     0,     0,     0,     0,     0,     0,     0,  1677,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   506,   507,   508,     0,   509,
       0,   510,   511,   512,   513,    50,     0,     0,   515,   516,
     517,   518,   519,   520,   521,    63,   599,   523,   524,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,   528,   529,     0,    15,     0,
       0,   530,   531,     0,     0,   462,   463,     0,     0,   532,
       0,   533,     0,   534,   535,   468,   469,   470,   471,   472,
       0,     0,     0,     0,     0,   473,     0,   475,     0,     0,
       0,   478,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,   483,     0,     0,   484,     0,
       0,   485,     0,     0,     0,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   597,     0,     0,   492,   493,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     494,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   495,   496,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   499,   500,
       0,     0,     0,     0,     0,     0,     0,  1678,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   504,   505,   506,   507,   508,     0,   509,     0,   510,
     511,   512,   513,    50,     0,     0,   515,   516,   517,   518,
     519,   520,   521,    63,   599,   523,   524,     0,     0,     0,
       0,     0,     0,   525,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,   528,   529,     0,    15,     0,     0,   530,
     531,     0,     0,   462,   463,     0,     0,   532,     0,   533,
       0,   534,   535,   468,   469,   470,   471,   472,     0,     0,
       0,     0,     0,   473,     0,   475,     0,     0,     0,   478,
       0,     0,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,   483,     0,     0,   484,     0,     0,   485,
       0,     0,     0,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   597,     0,     0,   492,   493,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   494,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     495,   496,   598,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   504,
     505,   506,   507,   508,     0,   509,     0,   510,   511,   512,
     513,    50,     0,     0,   515,   516,   517,   518,   519,   520,
     521,    63,   599,   523,   524,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,   528,   529,     0,    15,     0,     0,   530,   531,     0,
       0,   462,   463,     0,     0,   532,     0,   533,     0,   534,
     535,   468,   469,   470,   471,   472,     0,     0,     0,     0,
       0,   473,     0,   475,     0,     0,     0,   478,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   483,     0,     0,   484,     0,     0,   485,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   597,     0,     0,   492,   493,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   494,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   495,   496,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   504,   505,   506,
     507,   508,     0,   509,     0,   510,   511,   512,   513,    50,
       0,     0,   515,   516,   517,   518,   519,   520,   521,    63,
     599,   523,   524,     0,   325,     0,     0,   775,   776,   525,
     326,     0,     0,     0,     0,     0,   327,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   328,     0,   527,   528,
     529,     0,    15,     0,   329,   530,   531,     0,     0,     0,
       0,     0,     0,  1424,     0,   533,     0,   534,   535,   330,
       0,     0,     0,     0,     0,     0,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   777,   778,   779,   780,   781,     0,     0,   782,
     783,   784,   785,     0,   786,   787,   788,   789,     0,     0,
       0,     0,   790,     0,   791,   792,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   364,     0,     0,     0,     0,     0,     0,     0,     0,
      13,     0,     0,     0,     0,   325,     0,     0,   642,     0,
       0,   326,     0,     0,     0,     0,     0,   327,     0,     0,
      14,     0,     0,     0,     0,     0,     0,   328,   643,     0,
       0,     0,     0,     0,     0,   329,   801,   802,   803,   804,
     805,   806,   807,     0,     0,     0,     0,     0,     0,     0,
     330,     0,   808,   809,     0,     0,   365,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,   776,     0,     0,     0,     0,     0,   325,     0,
       0,     0,     0,     0,   326,     0,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,     0,     0,    62,
     328,     0,     0,     0,     0,     0,     0,     0,   329,     0,
       0,     0,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   330,     0,     0,     0,     0,     0,    63,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,     0,     0,     0,   777,   778,   779,   780,
     781,     0,     0,   782,   783,   784,   785,   365,   786,   787,
     788,   789,     0,     0,     0,     0,   790,     0,   791,   792,
     775,   776,     0,     0,   793,   794,   795,     0,     0,     0,
     796,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   364,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   775,   776,     0,     0,     0,
       0,     0,   642,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,     0,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   808,   809,     0,     0,
     877,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     365,     0,     0,     0,     0,   777,   778,   779,   780,   781,
       0,     0,   782,   783,   784,   785,     0,   786,   787,   788,
     789,     0,     0,     0,     0,   790,     0,   791,   792,     0,
       0,     0,     0,   793,   794,   795,     0,     0,     0,   796,
     777,   778,   779,   780,   781,     0,     0,   782,   783,   784,
     785,     0,   786,   787,   788,   789,   775,   776,     0,     0,
     790,     0,   791,   792,     0,     0,     0,     0,   793,   794,
     795,     0,     0,     0,   796,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   797,     0,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,   775,   776,     0,     0,
       0,     0,     0,     0,     0,   808,   809,     0,     0,  1187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   797,
       0,   798,   799,   800,   801,   802,   803,   804,   805,   806,
     807,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     808,   809,     0,     0,  1191,     0,     0,     0,     0,     0,
       0,   777,   778,   779,   780,   781,     0,     0,   782,   783,
     784,   785,     0,   786,   787,   788,   789,     0,     0,     0,
       0,   790,     0,   791,   792,     0,     0,     0,     0,   793,
     794,   795,     0,     0,     0,   796,     0,     0,     0,     0,
       0,   777,   778,   779,   780,   781,     0,     0,   782,   783,
     784,   785,     0,   786,   787,   788,   789,   775,   776,     0,
       0,   790,     0,   791,   792,     0,     0,     0,     0,   793,
     794,   795,     0,     0,     0,   796,     0,     0,     0,     0,
     797,     0,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,   775,   776,     0,     0,     0,     0,     0,     0,
       0,   808,   809,     0,     0,  1193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,     0,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   808,   809,     0,     0,  1202,     0,     0,     0,     0,
       0,     0,   777,   778,   779,   780,   781,     0,     0,   782,
     783,   784,   785,     0,   786,   787,   788,   789,     0,     0,
       0,     0,   790,     0,   791,   792,     0,     0,     0,     0,
     793,   794,   795,     0,     0,     0,   796,   777,   778,   779,
     780,   781,     0,     0,   782,   783,   784,   785,     0,   786,
     787,   788,   789,   775,   776,     0,     0,   790,     0,   791,
     792,     0,     0,     0,     0,   793,   794,   795,     0,     0,
       0,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,     0,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   807,   775,   776,     0,     0,     0,     0,     0,
       0,     0,   808,   809,     0,     0,  1203,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,     0,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   808,   809,     0,
       0,  1204,     0,     0,     0,     0,     0,     0,   777,   778,
     779,   780,   781,     0,     0,   782,   783,   784,   785,     0,
     786,   787,   788,   789,     0,     0,     0,     0,   790,     0,
     791,   792,     0,     0,     0,     0,   793,   794,   795,     0,
       0,     0,   796,     0,     0,     0,     0,     0,   777,   778,
     779,   780,   781,     0,     0,   782,   783,   784,   785,     0,
     786,   787,   788,   789,   775,   776,     0,     0,   790,     0,
     791,   792,     0,     0,     0,     0,   793,   794,   795,     0,
       0,     0,   796,     0,     0,     0,     0,   797,     0,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   807,   775,
     776,     0,     0,     0,     0,     0,     0,     0,   808,   809,
       0,     0,  1205,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   797,     0,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   807,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   808,   809,
       0,     0,  1206,     0,     0,     0,     0,     0,     0,   777,
     778,   779,   780,   781,     0,     0,   782,   783,   784,   785,
       0,   786,   787,   788,   789,     0,     0,     0,     0,   790,
       0,   791,   792,     0,     0,     0,     0,   793,   794,   795,
       0,     0,     0,   796,   777,   778,   779,   780,   781,     0,
       0,   782,   783,   784,   785,     0,   786,   787,   788,   789,
     775,   776,     0,     0,   790,     0,   791,   792,     0,     0,
       0,     0,   793,   794,   795,     0,     0,     0,   796,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   797,     0,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   807,
     775,   776,     0,     0,     0,     0,     0,     0,     0,   808,
     809,     0,     0,  1207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   797,     0,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   807,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   808,   809,     0,     0,  1340,     0,
       0,     0,     0,     0,     0,   777,   778,   779,   780,   781,
       0,     0,   782,   783,   784,   785,     0,   786,   787,   788,
     789,     0,     0,     0,     0,   790,     0,   791,   792,     0,
       0,     0,     0,   793,   794,   795,     0,     0,     0,   796,
       0,     0,     0,     0,     0,   777,   778,   779,   780,   781,
       0,     0,   782,   783,   784,   785,     0,   786,   787,   788,
     789,   775,   776,     0,     0,   790,     0,   791,   792,     0,
       0,     0,     0,   793,   794,   795,     0,     0,     0,   796,
       0,     0,     0,     0,   797,     0,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,   775,   776,     0,     0,
       0,     0,     0,     0,     0,   808,   809,     0,     0,  1504,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   797,     0,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   808,   809,     0,     0,  1523,
       0,     0,     0,     0,     0,     0,   777,   778,   779,   780,
     781,     0,     0,   782,   783,   784,   785,     0,   786,   787,
     788,   789,     0,     0,     0,     0,   790,     0,   791,   792,
       0,     0,     0,     0,   793,   794,   795,     0,     0,     0,
     796,   777,   778,   779,   780,   781,     0,     0,   782,   783,
     784,   785,     0,   786,   787,   788,   789,   775,   776,     0,
       0,   790,     0,   791,   792,     0,     0,     0,     0,   793,
     794,   795,     0,     0,     0,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,     0,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   775,   776,     0,
       0,     0,     0,     0,     0,     0,   808,   809,     0,     0,
    1524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,     0,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   808,   809,     0,     0,  1546,     0,     0,     0,     0,
       0,     0,   777,   778,   779,   780,   781,     0,     0,   782,
     783,   784,   785,     0,   786,   787,   788,   789,     0,     0,
       0,     0,   790,     0,   791,   792,     0,     0,     0,     0,
     793,   794,   795,     0,     0,     0,   796,     0,     0,     0,
       0,     0,   777,   778,   779,   780,   781,     0,     0,   782,
     783,   784,   785,     0,   786,   787,   788,   789,   775,   776,
       0,     0,   790,     0,   791,   792,     0,     0,     0,     0,
     793,   794,   795,     0,     0,     0,   796,     0,     0,     0,
       0,   797,     0,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   807,   775,   776,     0,     0,     0,     0,     0,
       0,     0,   808,   809,     0,     0,  1548,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,     0,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   807,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   808,   809,     0,     0,  1550,     0,     0,     0,
       0,     0,     0,   777,   778,   779,   780,   781,     0,     0,
     782,   783,   784,   785,     0,   786,   787,   788,   789,     0,
       0,     0,     0,   790,     0,   791,   792,     0,     0,     0,
       0,   793,   794,   795,     0,     0,     0,   796,   777,   778,
     779,   780,   781,     0,     0,   782,   783,   784,   785,     0,
     786,   787,   788,   789,   775,   776,     0,     0,   790,     0,
     791,   792,     0,     0,     0,     0,   793,   794,   795,     0,
       0,     0,   796,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   797,     0,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   807,   775,   776,     0,     0,     0,     0,
       0,     0,     0,   808,   809,     0,     0,  1554,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   797,     0,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   807,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   808,   809,
       0,     0,  1634,     0,     0,     0,     0,     0,     0,   777,
     778,   779,   780,   781,     0,     0,   782,   783,   784,   785,
       0,   786,   787,   788,   789,     0,     0,     0,     0,   790,
       0,   791,   792,     0,     0,     0,     0,   793,   794,   795,
       0,     0,     0,   796,     0,     0,     0,     0,     0,   777,
     778,   779,   780,   781,     0,     0,   782,   783,   784,   785,
       0,   786,   787,   788,   789,   775,   776,     0,     0,   790,
       0,   791,   792,     0,     0,     0,     0,   793,   794,   795,
       0,     0,     0,   796,     0,     0,     0,     0,   797,     0,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   807,
     775,   776,     0,     0,     0,     0,     0,     0,     0,   808,
     809,     0,     0,  1635,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   797,     0,
     798,   799,   800,   801,   802,   803,   804,   805,   806,   807,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   808,
     809,     0,     0,  1637,     0,     0,     0,     0,     0,     0,
     777,   778,   779,   780,   781,     0,     0,   782,   783,   784,
     785,     0,   786,   787,   788,   789,     0,     0,     0,     0,
     790,     0,   791,   792,     0,     0,     0,     0,   793,   794,
     795,     0,     0,     0,   796,   777,   778,   779,   780,   781,
       0,     0,   782,   783,   784,   785,     0,   786,   787,   788,
     789,   775,   776,     0,     0,   790,     0,   791,   792,     0,
       0,     0,     0,   793,   794,   795,     0,     0,     0,   796,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   797,
       0,   798,   799,   800,   801,   802,   803,   804,   805,   806,
     807,   775,   776,     0,     0,     0,     0,     0,     0,     0,
     808,   809,     0,     0,  1659,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   797,     0,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   808,   809,     0,     0,  1664,
       0,     0,     0,     0,     0,     0,   777,   778,   779,   780,
     781,     0,     0,   782,   783,   784,   785,     0,   786,   787,
     788,   789,     0,     0,     0,     0,   790,     0,   791,   792,
       0,     0,     0,     0,   793,   794,   795,     0,     0,     0,
     796,     0,     0,     0,     0,     0,   777,   778,   779,   780,
     781,     0,     0,   782,   783,   784,   785,     0,   786,   787,
     788,   789,   775,   776,     0,     0,   790,     0,   791,   792,
       0,     0,     0,     0,   793,   794,   795,     0,     0,     0,
     796,     0,     0,     0,     0,   797,     0,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   775,   776,     0,
       0,     0,     0,     0,     0,     0,   808,   809,   868,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,     0,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   808,   809,  1138,     0,
       0,     0,     0,     0,     0,     0,     0,   777,   778,   779,
     780,   781,     0,     0,   782,   783,   784,   785,     0,   786,
     787,   788,   789,     0,     0,     0,     0,   790,     0,   791,
     792,     0,     0,     0,     0,   793,   794,   795,     0,     0,
       0,   796,   777,   778,   779,   780,   781,     0,     0,   782,
     783,   784,   785,     0,   786,   787,   788,   789,   775,   776,
       0,     0,   790,     0,   791,   792,     0,     0,     0,     0,
     793,   794,   795,     0,     0,     0,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,     0,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   775,   776,
       0,     0,     0,     0,     0,     0,     0,   808,   809,  1290,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,     0,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   807,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   808,   809,  1306,     0,     0,     0,     0,     0,
       0,     0,     0,   777,   778,   779,   780,   781,     0,     0,
     782,   783,   784,   785,     0,   786,   787,   788,   789,     0,
       0,     0,     0,   790,     0,   791,   792,     0,     0,     0,
       0,   793,   794,   795,     0,     0,     0,   796,     0,     0,
       0,     0,     0,   777,   778,   779,   780,   781,     0,     0,
     782,   783,   784,   785,     0,   786,   787,   788,   789,   245,
     246,     0,     0,   790,     0,   791,   792,     0,     0,     0,
       0,   793,   794,   795,     0,     0,   247,   796,     0,     0,
       0,     0,   797,     0,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   807,     0,     0,     0,     0,     0,     0,
       0,   775,   776,   808,   809,  1448,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   797,     0,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   807,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   808,   809,  1454,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,     0,     0,   266,   267,   268,     0,
       0,     0,     0,     0,     0,   269,   270,   271,   272,   273,
       0,     0,   274,   275,   276,   277,   278,   279,   280,     0,
       0,     0,     0,     0,     0,     0,   777,   778,   779,   780,
     781,     0,     0,   782,   783,   784,   785,     0,   786,   787,
     788,   789,   775,   776,     0,     0,   790,     0,   791,   792,
       0,     0,     0,     0,   793,   794,   795,     0,     0,     0,
     796,     0,     0,   281,     0,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,    13,     0,   292,   293,   775,
     776,     0,     0,     0,   294,   295,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,     0,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   777,   778,   779,
     780,   781,     0,     0,   782,   783,   784,   785,     0,   786,
     787,   788,   789,     0,     0,     0,     0,   790,     0,   791,
     792,     0,     0,     0,     0,   793,   794,   795,     0,     0,
       0,   796,     0,     0,   777,   778,   779,   780,   781,     0,
       0,   782,   783,   784,   785,     0,   786,   787,   788,   789,
     775,   776,     0,     0,   790,     0,   791,   792,     0,     0,
       0,     0,   793,   794,   795,     0,     0,     0,   796,     0,
       0,     0,     0,     0,     0,     0,   797,  1295,   798,   799,
     800,   801,   802,   803,   804,   805,   806,   807,   775,   776,
       0,     0,     0,     0,     0,     0,     0,   808,   809,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   797,     0,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   807,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   808,   809,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   777,   778,   779,   780,   781,
       0,     0,   782,   783,   784,   785,     0,   786,   787,   788,
     789,     0,     0,     0,     0,   790,     0,   791,   792,     0,
       0,     0,     0,   793,   794,   795,     0,     0,     0,  -798,
       0,     0,     0,   777,   778,   779,   780,   781,     0,     0,
     782,   783,   784,   785,     0,   786,   787,   788,   789,   775,
     776,     0,     0,   790,     0,   791,   792,     0,     0,     0,
       0,   793,   794,   795,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   797,     0,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,   775,   776,     0,     0,
       0,     0,     0,     0,     0,   808,   809,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   797,     0,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   807,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   808,   809,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   777,   778,   779,   780,   781,     0,
       0,   782,   783,   784,   785,     0,   786,   787,   788,   789,
       0,     0,     0,     0,   790,     0,   791,   792,     0,     0,
       0,     0,   793,     0,   795,     0,     0,     0,     0,     0,
       0,   777,   778,   779,   780,   781,     0,     0,   782,   783,
     784,   785,     0,   786,   787,   788,   789,   775,   776,     0,
       0,   790,     0,   791,   792,     0,     0,     0,     0,   793,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,   776,     0,     0,   798,   799,   800,   801,   802,
     803,   804,   805,   806,   807,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   808,   809,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   808,   809,     0,     0,     0,     0,     0,  1017,     0,
       0,     0,   777,   778,   779,   780,   781,     0,     0,   782,
     783,   784,   785,     0,   786,   787,   788,   789,     0,     0,
       0,     0,   790,     0,   791,   792,   777,   778,   779,   780,
     781,     0,     0,   782,   783,   784,   785,     0,   786,   787,
     788,   789,     0,     0,     0,     0,   790,     0,   791,   792,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   494,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,     0,     0,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   807,     0,     0,     0,     0,  1021,     0,     0,
       0,     0,   808,   809,     0,     0,     0,     0,   799,   800,
     801,   802,   803,   804,   805,   806,   807,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   808,   809,  1291,     0,
       0,     0,     0,     0,     0,     0,     0,  1018,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   331,
     332,   333,  1019,   335,   336,   337,   338,   339,   494,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   494,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     494,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,  1022,   195,   358,   359,
     360,   361,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1023,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1292,     0,  1057,
    1058,     0,     0,   196,     0,   197,     0,   198,   199,   200,
     201,   202,  1293,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,     0,   214,   215,   216,  1059,     0,
     217,   218,   219,   220,   905,   906,   907,   908,   909,   910,
     911,   912,     0,  1060,     0,     0,     0,   913,   914,     0,
     221,   222,     0,   915,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   821,     0,     0,   916,   917,     0,     0,
       0,     0,     0,   918,   919,   920,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1061,  1062,     0,     0,
       0,    13,     0,   905,   906,   907,   908,   909,   910,   911,
     912,     0,     0,     0,     0,   223,   913,   914,     0,     0,
       0,    14,   915,     0,     0,     0,     0,  -390,     0,     0,
     921,     0,   821,     0,     0,   916,   917,     0,     0,     0,
       0,     0,   918,   919,   920,     0,   905,   906,   907,   908,
     909,   910,   911,   912,     0,     0,     0,   534,   715,   913,
     914,     0,     0,     0,     0,   915,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   821,     0,     0,   916,   917,
       0,     0,     0,     0,     0,   918,   919,   920,     0,   921,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   715,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   921,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     715
};

static const yytype_int16 yycheck[] =
{
       1,   151,   225,     7,   604,   391,   241,   231,   627,   750,
     658,   553,   687,   437,   513,    15,    16,   438,   759,    20,
     173,   730,   618,   405,   620,   855,   622,   857,   665,   859,
     962,   391,   502,   503,   570,   946,   968,   437,   438,   474,
     399,  1255,  1219,   939,    86,     5,   892,     8,   941,   728,
      33,   730,   434,    22,    34,  1609,    20,    53,   594,    53,
      19,    20,    33,   498,    20,    20,    15,    16,    68,    69,
      70,    57,   109,   160,   143,  1427,    20,    63,    20,   144,
     145,   146,   193,    63,    20,   764,     4,   311,    57,    20,
     727,    20,   729,   173,   731,     5,     6,    98,    20,    20,
       5,     6,   163,     8,   173,    12,    20,   152,   745,   220,
     110,   111,   112,   113,   474,   160,    23,    24,   143,   756,
     126,  1675,   183,   129,   130,   129,   173,   107,   173,   511,
     512,    36,   129,   130,    12,   222,   166,   127,   498,    15,
      16,   221,   102,   103,   142,    23,    24,   137,    46,   373,
     219,   127,   193,   143,   389,   220,   136,  1003,   219,   152,
     901,   137,   315,   143,   158,   163,   170,   391,    62,   165,
     143,   196,   219,   608,   219,     0,    21,    22,   164,   173,
     173,   717,     7,  1535,    57,   183,   190,   191,   192,   185,
      63,   233,   173,   173,   196,   232,   937,   199,   184,   940,
     173,   196,   107,   164,   194,    30,  1383,    32,   106,    34,
     216,   217,  1638,   892,   173,    40,   199,   166,   194,   216,
     217,   225,   171,   184,   173,    50,   219,   176,   199,   193,
     210,    56,   127,   234,    21,    22,   205,   193,   193,   219,
     200,   241,   160,   393,   679,   645,   217,   216,   608,   193,
     650,   193,   223,   890,   477,    80,   217,   193,   165,   219,
     194,   144,   193,   146,   193,  1161,  1007,  1693,   491,  1162,
     219,   193,   193,   118,   119,   710,   318,   102,   103,   193,
       7,   126,   132,  1215,   129,   130,   131,   165,   127,  1200,
     166,   136,   127,    21,    22,   171,   155,   173,   137,   194,
     176,   160,   197,   773,   686,  1151,   310,   158,    53,   313,
     185,   193,   152,  1527,   193,   165,   127,   197,   193,   679,
     127,   201,   173,    50,  1003,  1004,   137,   709,  1505,   155,
     137,   118,   119,   173,   160,   185,   218,   127,   158,   126,
     165,   128,   129,   130,   131,   982,   983,   137,   985,   136,
     710,  1262,    97,   173,   200,   194,   152,   165,   193,   194,
     185,   196,   173,   222,   199,   365,   590,   219,  1495,   220,
     594,   216,   217,   219,   609,   610,   611,   173,   613,   614,
     143,   206,   617,   194,   619,   193,   621,   194,   199,   389,
     118,   119,   217,   163,   220,   395,   222,  1611,   126,   219,
     128,   129,   130,   131,   194,   405,   841,  1318,   136,   185,
     188,   189,   771,   183,   152,   127,   193,   193,  1597,  1015,
    1599,   193,   193,   219,   648,   137,   165,  1606,   652,   216,
     217,   173,   174,   175,   434,   173,   241,   437,  1565,  1566,
     218,   664,  1621,   220,  1645,   595,   185,   847,   220,   220,
    1382,   221,   222,  1283,   216,   837,  1657,  1584,  1585,   165,
     143,    57,  1394,   863,   206,   165,  1002,    63,   691,   152,
     470,   471,  1151,   477,   157,   974,   204,   205,   206,   185,
     166,   841,   194,   222,   954,   185,   487,   491,   216,   217,
     173,   193,   193,   193,  1673,   201,  1144,   501,   502,   503,
     999,  1702,  1703,   193,   886,   887,  1215,   165,   193,   193,
     173,   511,   512,   736,   158,   177,   218,   200,   900,   220,
      57,  1230,   173,   923,  1651,  1652,    63,   185,   218,   173,
     188,   754,   165,   757,   218,   220,  1215,   538,   539,  1218,
    1719,  1720,   165,   165,    21,    22,  1376,   929,   930,   163,
     932,  1230,   185,   165,   936,   556,   938,   558,   559,   560,
     218,   562,   185,   185,   193,   165,   165,   165,   201,   183,
     193,   193,   573,   185,    57,   158,   173,  1756,   201,   201,
      63,   193,   196,  1413,   389,   185,   185,   185,   193,   201,
     173,   220,   165,   193,   193,   193,  1066,  1067,  1068,   197,
     405,   201,   201,   604,   193,  1201,   193,   165,  1245,   609,
     610,   611,   185,   613,   614,   220,   173,   617,   165,   619,
     193,   621,   158,   623,    68,    69,    70,   185,   201,   434,
     865,   220,   437,   220,    12,   193,   871,   173,   185,   874,
     193,   118,   119,   201,   165,    23,    24,   194,   883,   126,
     193,   128,   129,   130,   131,  1251,   891,    33,    57,   136,
     664,   896,   193,   163,   185,   218,   110,   111,   112,   113,
     143,   671,   193,   193,   165,   165,   158,   220,   193,   152,
     201,   831,   835,   183,    60,    61,   686,   691,  1363,   220,
     193,   173,    33,   193,   185,   185,   165,   173,   218,   922,
     173,  1531,   193,   193,   173,   220,   511,   512,  1132,   709,
     201,   201,   193,  1134,   164,   193,   185,   220,   474,    60,
      61,   193,   193,   173,   193,   202,   203,   204,   205,   206,
     193,   196,   736,   734,   184,  1135,   201,   218,  1386,   216,
     217,   976,   220,   978,   158,  1424,   218,   165,   124,   220,
     754,   751,   128,   196,   219,   391,   760,   220,   201,   173,
     163,   108,   109,   110,   111,   112,   113,   114,   115,   773,
     196,  1450,   165,   185,   196,   201,   219,   533,  1160,   201,
     183,   193,   193,   124,   173,   196,   127,   128,   199,   173,
     137,   184,   185,   219,   187,   165,   137,   219,   200,  1199,
     147,   148,   149,  1185,   609,   610,   611,   165,   613,   614,
     165,   976,   617,   978,   619,   185,   621,   219,   623,   195,
     185,    33,   144,   199,   165,  1421,   202,   185,   193,   184,
     185,   186,   832,  1375,   144,   145,   146,   837,   474,  1239,
    1240,   217,   843,   164,   185,   177,   164,   223,    60,    61,
    1469,    33,   853,   194,   195,   173,  1504,    47,   199,   185,
     197,   202,   498,   184,   201,   865,   184,   193,  1250,   200,
     200,   871,   185,   200,   874,  1261,   217,    67,    60,    61,
     193,   686,   223,   883,   106,  1120,   886,   887,   219,   219,
     185,   891,   219,    57,   177,   178,   896,    57,   193,    63,
     900,  1261,  1577,    63,   709,   184,    33,    57,   187,   194,
     197,   190,   124,    63,   201,  1656,   128,   197,   922,    35,
     197,   201,   678,   679,   201,   196,  1667,   197,   199,   929,
     930,   201,   932,    60,    61,    35,   936,  1537,   938,   939,
     197,   197,   124,   197,   201,   201,   128,   201,   704,   173,
     954,   707,  1352,   165,   197,  1696,  1575,   219,   201,   197,
    1360,   962,   173,   201,   965,   173,   173,   968,   969,   218,
     775,   776,   608,   185,  1653,    22,   976,   173,   978,  1658,
     196,   193,  1601,   195,   218,   790,   196,   199,   219,   201,
     202,   177,   178,   179,   180,    43,   219,   124,   221,   222,
     127,   128,   200,   808,   197,   217,   221,   222,   764,   200,
     137,   223,   200,   195,  1513,  1694,  1664,   199,   219,   201,
     202,   173,   174,   175,   200,  1625,  1626,    92,    93,  1252,
     200,  1530,   837,   173,   219,   217,   219,  1261,   177,   178,
     179,   223,   200,   679,   177,   178,   179,   456,   457,   458,
      10,    11,    12,   200,   200,  1437,   200,   200,   200,   185,
     865,   220,  1066,  1067,  1068,   219,   871,   194,   195,   874,
     218,   173,   199,   829,   710,   202,   173,   173,   883,   217,
     200,   886,   887,  1683,   219,  1764,   891,   200,   200,   219,
     217,   896,   173,    75,   200,   900,   223,    79,   219,   200,
     166,   200,   200,    13,  1339,   200,   200,   222,   200,   219,
    1111,    93,    94,    10,  1115,   219,    98,    99,   100,   101,
    1120,    37,  1345,    66,   929,   930,   219,   932,   219,   219,
     219,   936,  1132,   938,   939,   217,   892,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,   219,   822,   823,
     824,   825,   826,   827,  1536,   219,  1157,   200,   200,    43,
    1160,  1161,    10,   220,   173,   173,   218,  1317,   193,   196,
    1669,   976,    43,   978,   810,   811,   812,   813,   814,   815,
     816,   817,   818,   819,   820,  1185,   822,   823,   824,   825,
     826,   827,    33,  1416,   200,   219,   219,   219,   200,   200,
     193,   132,    14,   959,  1439,   841,   194,   196,   166,   222,
     142,   967,   219,  1436,    13,   218,   193,   193,   173,    60,
      61,     8,   173,   201,   220,   173,   173,   173,   220,    33,
     142,   193,   219,   219,   193,   184,   220,   193,   219,   173,
    1045,   220,  1624,   200,   219,   219,   200,   219,  1252,   219,
    1250,   219,   219,   219,    33,     1,    60,    61,   219,    67,
     108,   109,   110,   111,   112,   113,   114,   115,   173,    43,
     220,   201,   201,   121,   122,   201,   220,  1500,   173,   127,
     391,    60,    61,   124,   220,   220,   219,   128,   196,   137,
      12,   219,   140,   141,   173,   220,   220,   220,   218,   147,
     148,   149,   220,     5,     6,   218,   173,   218,  1458,   173,
    1311,   173,   219,   200,   173,  1120,  1551,   219,   219,   219,
     124,   219,   173,    25,   128,   218,   220,  1132,   219,    31,
     219,   181,   220,   173,   501,   502,   503,   218,   173,  1339,
     218,  1345,    33,   219,  1567,   124,   194,   219,   173,   128,
     219,   219,   201,   464,   195,  1160,  1161,   220,   199,   219,
     201,   202,    33,   474,  1365,   219,    68,    69,   479,    70,
     481,   201,   219,   221,   222,   219,   217,   219,   201,   490,
    1185,  1382,   223,   220,   220,   219,   553,   498,   220,    60,
      61,   195,  1148,  1394,   220,   199,   177,   201,   202,  1622,
     102,   103,   219,   219,   219,   185,   220,   185,    53,   220,
     220,   522,  1416,   217,   581,   220,   195,  1418,   218,   223,
     199,   184,   201,   202,   193,   218,   185,   193,   220,   540,
     220,   220,  1436,   184,   220,   220,   220,  1437,   217,  1439,
     738,   143,  1666,   220,   223,  1250,   218,   220,   218,   220,
     218,   218,  1208,   124,   219,    85,  1680,   128,   160,   161,
     162,   163,   220,   220,   218,     1,    46,    33,   141,    88,
    1528,   173,   832,  1229,  1001,   239,  1528,  1528,    33,  1235,
     591,   183,  1528,     1,  1528,  1010,  1500,  1434,  1244,  1275,
    1460,  1562,  1716,  1314,    60,    61,  1500,   608,   200,  1563,
    1463,  1563,    56,  1259,  1658,    60,    61,   392,  1511,  1227,
    1733,   730,    -1,    -1,    -1,   217,    -1,   684,   470,    -1,
      -1,    -1,    -1,    -1,   195,   470,    -1,    -1,   199,    -1,
     201,   202,  1288,    -1,  1339,    -1,  1536,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1300,   712,   217,    -1,    -1,  1305,
      -1,  1551,   223,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,   128,  1567,    -1,    -1,    -1,    -1,   679,   124,
      -1,    -1,    -1,   128,    -1,    -1,    33,   688,    -1,    -1,
      -1,    -1,  1732,    -1,    -1,    -1,    -1,    -1,    -1,  1590,
    1346,    -1,    -1,    -1,    -1,    -1,  1597,    -1,  1599,   710,
      -1,    -1,    -1,    60,    61,  1606,   773,    -1,    -1,  1609,
      -1,    -1,  1762,    -1,    -1,    -1,    -1,   728,  1622,   730,
    1621,  1377,    -1,    -1,  1624,  1261,    -1,    -1,    -1,   195,
      -1,    -1,  1437,   199,  1439,   201,   202,    -1,    -1,    -1,
     195,    -1,    -1,    -1,   199,    -1,   201,   202,    -1,  1405,
    1406,   217,    -1,  1409,    21,    22,    -1,   223,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,   124,   223,    -1,
      -1,   128,  1673,    -1,    -1,  1675,   843,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1686,    -1,    -1,    -1,    -1,
      -1,  1692,    -1,    -1,    -1,    -1,    -1,    33,    -1,   810,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,    -1,  1719,  1720,
      -1,    -1,    -1,    -1,    60,    61,    -1,  1728,  1729,  1733,
     841,  1536,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,
      -1,    -1,   199,    -1,   201,   202,  1551,    -1,    -1,   116,
     117,   118,   119,   120,    -1,  1756,   123,  1758,    -1,   126,
     217,   128,   129,   130,   131,  1521,   223,    -1,    -1,   136,
      -1,   138,   139,    -1,   885,    -1,    -1,   944,    -1,    -1,
      -1,    -1,   893,    -1,   895,    -1,    -1,   954,   124,    -1,
      -1,    -1,   128,    -1,   905,   906,   907,   908,   909,   910,
     911,   912,   913,   914,   915,   916,   917,   918,   919,   920,
     921,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1624,
      -1,    -1,    -1,    -1,    -1,  1581,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,   203,   204,   205,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,   402,    -1,    -1,    -1,  1613,    -1,   195,
     971,    -1,   411,   199,    -1,   201,   202,    -1,    -1,   980,
      -1,    -1,   421,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,   431,    -1,    -1,   996,    -1,   223,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1066,
    1067,  1068,    -1,   452,  1071,    -1,  1073,    -1,  1075,    -1,
    1077,    -1,  1079,    -1,  1081,    -1,  1083,    -1,  1085,    -1,
    1087,  1677,  1089,    -1,  1091,   474,    -1,  1094,    -1,  1096,
      -1,  1098,    -1,  1100,    -1,  1102,    -1,  1104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1115,    13,
     499,   500,    -1,    21,    22,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,   527,   528,
     529,   530,   531,   532,   533,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,    -1,  1188,   627,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   636,    -1,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1211,  1212,    -1,    -1,    -1,  1216,    -1,    -1,    -1,   173,
      -1,    -1,    -1,  1224,    -1,    -1,  1227,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   678,
     679,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,   690,    -1,    -1,    -1,  1256,   695,    -1,   216,   217,
    1261,    -1,   701,    -1,    -1,   704,    -1,   221,   707,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,   721,   722,   723,   724,   725,   726,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1375,    -1,
      -1,    -1,    -1,    -1,    -1,   764,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   777,   778,
      -1,  1342,   781,   782,   783,   784,    -1,   786,    -1,   788,
     789,   790,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   807,    -1,
     809,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     829,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,   878,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   892,   185,    -1,    -1,    60,    61,    21,
      22,    -1,   193,    -1,   195,   904,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    -1,    -1,  1479,    -1,
    1481,    -1,    -1,    -1,    33,   216,   217,    -1,    -1,    -1,
    1491,  1492,  1493,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1508,  1509,    -1,
      33,    60,    61,  1514,    -1,    -1,    -1,    -1,   957,   958,
     959,   124,    -1,    -1,    -1,   128,    -1,  1528,   967,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
      -1,  1542,    -1,    -1,    -1,    -1,    -1,    -1,   987,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,  1004,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,   124,   138,   139,    -1,   128,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
    1591,  1592,   195,    -1,    -1,    -1,   199,    -1,   201,   202,
      -1,   124,    -1,    -1,    33,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1614,   217,    -1,    -1,    -1,    -1,  1058,
     223,    -1,    -1,  1062,    -1,    -1,  1627,    -1,  1629,    -1,
      -1,    60,    61,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    -1,   195,  1648,    -1,  1650,
     199,    -1,   201,   202,   216,   217,    -1,    -1,   220,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    13,
      -1,    -1,   195,    -1,   223,    19,   199,  1678,   201,   202,
      -1,    25,    -1,  1684,  1685,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   217,   124,    -1,    41,    -1,   128,
     223,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,  1148,
    1711,    -1,  1713,  1714,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,  1727,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,  1208,
     199,  1210,   201,   202,  1213,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
    1229,    -1,    -1,    -1,   223,    -1,  1235,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,  1244,    -1,    -1,    -1,  1248,
    1249,    -1,   156,    -1,    -1,    -1,    -1,    21,    22,    -1,
    1259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1267,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1285,    -1,  1287,  1288,
    1289,    -1,    -1,    -1,    -1,    -1,  1295,    -1,    -1,    -1,
    1299,  1300,    -1,    -1,    -1,    -1,  1305,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,   221,   123,   124,
     125,   126,    33,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,  1346,    -1,    60,
      61,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,  1377,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     195,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    -1,    -1,    -1,    -1,  1405,  1406,    -1,    -1,
    1409,   216,   217,   124,    -1,   220,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1424,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,  1445,    -1,  1447,    -1,
      -1,  1450,   216,   217,  1453,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
    1469,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,   195,    -1,    -1,    -1,   199,    -1,
     201,   202,    -1,    41,    -1,    -1,    -1,    -1,    -1,  1498,
      -1,    49,    -1,    -1,  1503,    -1,   217,    -1,  1507,    -1,
      -1,  1510,   223,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,  1520,  1521,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,  1575,   136,    -1,   138,
     139,    -1,  1581,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,  1601,    -1,    -1,    -1,    -1,    -1,   156,  1608,
      -1,    -1,     1,    -1,  1613,    -1,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,   173,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,   202,   203,   204,   205,   206,    -1,    38,
      39,    -1,    -1,    42,    -1,    44,    45,   216,   217,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,  1670,    -1,   221,    -1,   223,    65,    -1,  1677,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,  1722,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,
     209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,
     219,    -1,   221,   222,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,
      61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,   199,    -1,
      -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,   210,
      -1,    -1,    -1,     5,     6,    -1,   217,    -1,   219,    -1,
     221,   222,   223,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,   195,    -1,    -1,    -1,   199,    -1,    -1,
     202,   203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,
      -1,    -1,     5,     6,    -1,   217,    -1,   219,    -1,   221,
     222,   223,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
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
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
      -1,     5,     6,    -1,   217,    -1,   219,    -1,   221,   222,
     223,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,
       6,    -1,    -1,   217,    -1,   219,    -1,   221,   222,    15,
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
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,
     206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,
      -1,   217,    -1,   219,    -1,   221,   222,    15,    16,    17,
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
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,
      -1,   219,    -1,   221,   222,    15,    16,    17,    18,    19,
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
      -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,
     210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,
      -1,   221,   222,    15,    16,    17,    18,    19,    -1,    -1,
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
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,
      -1,     5,     6,    -1,    -1,   217,    -1,   219,   220,   221,
     222,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,     5,     6,   217,    -1,   219,    -1,   221,   222,    13,
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
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,     5,     6,   217,   218,   219,    -1,   221,   222,    13,
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
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,     5,     6,   217,    -1,   219,    -1,   221,   222,    13,
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
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,     5,     6,   217,   218,   219,    -1,   221,   222,    13,
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
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,
       6,    -1,    -1,   217,    -1,   219,    -1,   221,   222,    15,
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
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,
     206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,
      -1,   217,    -1,   219,    -1,   221,   222,    15,    16,    17,
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
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,
     218,   219,    -1,   221,   222,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
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
      -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,
     210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,
      -1,   221,   222,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,
      -1,    -1,    -1,     5,     6,   217,    -1,   219,    -1,   221,
     222,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,
      -1,     5,     6,    -1,    -1,   217,    -1,   219,    -1,   221,
     222,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   200,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,     5,     6,   217,    -1,   219,    -1,   221,   222,    13,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,
       6,    -1,    -1,   217,    -1,   219,    -1,   221,   222,    15,
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
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,
     206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,
      -1,   217,    -1,   219,    -1,   221,   222,    15,    16,    17,
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
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,
      -1,   219,   220,   221,   222,    15,    16,    17,    18,    19,
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
     200,    -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,
     210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,
      -1,   221,   222,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,
      -1,     5,     6,    -1,    -1,   217,    -1,   219,   220,   221,
     222,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,
       6,    -1,    -1,   217,    -1,   219,   220,   221,   222,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,
     206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,
      -1,   217,    -1,   219,   220,   221,   222,    15,    16,    17,
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
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,
      -1,   219,   220,   221,   222,    15,    16,    17,    18,    19,
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
      -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,
     210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,
     220,   221,   222,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,
      -1,     5,     6,    -1,    -1,   217,    -1,   219,   220,   221,
     222,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,
     204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,
       6,    -1,    -1,   217,    -1,   219,   220,   221,   222,    15,
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
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,
     206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,
      -1,   217,    -1,   219,    -1,   221,   222,    15,    16,    17,
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
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,
      -1,   219,    -1,   221,   222,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,
     210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,
      -1,   221,   222,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     202,   203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,
      -1,     5,     6,    -1,    -1,   217,    -1,   219,    -1,   221,
     222,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     174,   175,   176,    -1,    19,    -1,    -1,    21,    22,   183,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,   202,   203,
     204,    -1,   206,    -1,    49,   209,   210,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,   219,    -1,   221,   222,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    19,    -1,    -1,   173,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    41,   193,    -1,
      -1,    -1,    -1,    -1,    -1,    49,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,   216,   217,    -1,    -1,   221,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   173,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,   221,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      21,    22,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     221,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,   220,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
     195,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,   220,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,   220,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,   220,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   195,    -1,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,   220,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    38,   150,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,   216,   217,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,   144,   145,   146,   147,   148,   149,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   165,    -1,   209,   210,    21,
      22,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,   173,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,   158,    35,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   129,
     130,    -1,    -1,    71,    -1,    73,    -1,    75,    76,    77,
      78,    79,   173,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,   158,    -1,
      98,    99,   100,   101,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,   173,    -1,    -1,    -1,   121,   122,    -1,
     118,   119,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,   140,   141,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,   165,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,   173,   121,   122,    -1,    -1,
      -1,   185,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
     194,    -1,   137,    -1,    -1,   140,   141,    -1,    -1,    -1,
      -1,    -1,   147,   148,   149,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,   221,   222,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,   141,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,    -1,   194,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   221,   222,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   221,
     222
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   225,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   206,   217,   226,   227,   230,
     239,   241,   242,   247,   278,   282,   318,   401,   408,   412,
     419,   465,   470,   475,    19,    20,   173,   270,   271,   272,
     166,   248,   249,   173,   174,   175,   206,   243,   244,   245,
     163,   183,   287,   409,   173,   221,   229,    57,    63,   404,
     404,   404,   143,   173,   304,    34,    63,   107,   136,   210,
     219,   274,   275,   276,   277,   304,   226,     5,     6,     8,
      36,   107,   416,    62,   399,   194,   193,   196,   193,   244,
      22,    57,   205,   216,   246,   404,   405,   407,   405,   399,
     476,   466,   471,   173,   143,   240,   276,   276,   276,   219,
     144,   145,   146,   193,   218,    57,    63,   283,   285,    57,
      63,   410,     5,     6,    57,    63,   417,    57,    63,   400,
      15,    16,   166,   171,   173,   176,   219,   232,   271,   166,
     249,   173,   243,   243,   173,   165,   185,   375,   164,   184,
     288,   405,   226,    57,    63,   228,   173,   173,   173,   173,
     177,   238,   220,   272,   276,   276,   276,   276,   286,   173,
     411,   420,   287,   402,   177,   178,   179,   231,    15,    16,
     166,   171,   173,   232,   268,   269,   246,   406,   165,   375,
     477,   467,   472,   177,   220,    35,    71,    73,    75,    76,
      77,    78,    79,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    93,    94,    95,    98,    99,   100,
     101,   118,   119,   173,   281,   284,   196,   287,   106,   414,
     415,   397,   160,   222,   273,   369,   177,   178,   179,   193,
     220,   194,   287,   287,   287,    21,    22,    38,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   128,   129,   130,   137,
     138,   139,   140,   141,   144,   145,   146,   147,   148,   149,
     150,   195,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   209,   210,   216,   217,    35,    35,   219,   279,
     287,   289,    75,    79,    93,    94,    98,    99,   100,   101,
     424,   403,   173,   421,   288,   398,   272,   271,   222,   226,
     152,   173,   395,   396,   268,    19,    25,    31,    41,    49,
      64,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   156,   221,   304,   423,   425,   426,
     430,   436,   464,   429,   468,   473,   173,   173,   173,   218,
      22,   173,   218,   155,   220,   369,   380,   381,   382,   196,
     280,   292,   287,   173,   288,   196,   413,   287,   418,   369,
     218,   271,   219,    43,   193,   196,   199,   394,   200,   200,
     200,   219,   200,   200,   219,   200,   200,   200,   200,   200,
     200,   219,   304,    33,    60,    61,   124,   128,   195,   199,
     202,   217,   223,   435,   197,   173,   288,   385,   388,   173,
     108,   109,   110,   111,   112,   113,   114,   115,   137,   147,
     148,   149,   219,     7,    50,   317,   185,   185,   193,   220,
     464,     1,     5,     6,     9,    10,    11,    13,    15,    16,
      17,    18,    19,    25,    26,    27,    28,    29,    31,    38,
      39,    42,    44,    45,    48,    51,    52,    54,    55,    58,
      59,    65,    68,    69,    80,   102,   103,   104,   105,   118,
     119,   133,   134,   135,   151,   152,   153,   154,   155,   157,
     159,   160,   161,   162,   165,   166,   167,   168,   169,   170,
     171,   172,   174,   175,   176,   183,   185,   202,   203,   204,
     209,   210,   217,   219,   221,   222,   237,   239,   250,   251,
     254,   257,   258,   260,   263,   264,   265,   266,   287,   288,
     290,   291,   293,   298,   303,   304,   305,   309,   310,   311,
     312,   313,   314,   315,   316,   318,   322,   323,   330,   333,
     336,   341,   344,   345,   347,   348,   349,   351,   356,   359,
     360,   367,   368,   423,   478,   493,   504,   508,   521,   524,
     403,   194,   375,   304,   376,   396,   218,    65,   104,   174,
     298,   360,   173,   173,   436,   127,   137,   194,   393,   437,
     442,   444,   360,   446,   440,   173,   431,   448,   450,   452,
     454,   456,   458,   460,   462,   360,   200,   219,    33,   199,
      33,   199,   217,   223,   218,   360,   217,   223,   436,   165,
     193,   428,   173,   193,   226,   383,   433,   464,   469,   173,
     386,   433,   474,   360,   152,   173,   390,   391,   422,   382,
     382,   382,   200,   200,   367,   261,   262,   200,   306,   425,
     478,   219,   304,   200,     5,   102,   103,   200,   219,   127,
     303,   334,   345,   360,   367,   289,   200,   219,    61,   367,
     219,   367,   173,   200,   200,   219,   226,   200,   166,    58,
     367,   219,   289,   200,   219,   200,   200,   219,   200,   200,
     127,   303,   367,   360,   360,   222,   289,   336,   340,   340,
     340,   219,   219,   219,   219,   219,   219,    13,   436,    13,
     436,    13,   367,   503,   519,   200,   367,   200,   236,    13,
     360,   360,   360,   360,   360,    13,    49,   294,   334,   360,
     334,   222,   226,   226,   367,    10,    13,   296,   503,   520,
      37,   336,   342,   173,   219,   226,   226,   226,   226,   226,
      66,   319,   278,   132,   226,    21,    22,   116,   117,   118,
     119,   120,   123,   124,   125,   126,   128,   129,   130,   131,
     136,   138,   139,   144,   145,   146,   150,   195,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   216,   217,
     108,   109,   110,   111,   112,   113,   114,   115,   121,   122,
     127,   137,   140,   141,   147,   148,   149,   194,   342,   219,
     288,   367,   217,   278,   288,   377,   375,   200,   220,    43,
     226,   393,   303,   367,   464,   464,   434,   464,   220,   464,
     464,   220,   173,   427,   464,   279,   464,   279,   464,   279,
     383,   384,   386,   387,   220,   439,   294,   218,   218,   360,
     193,   196,   433,   288,   196,   433,   288,   220,   219,    43,
     127,   193,   194,   196,   199,   389,   494,   496,   289,   422,
     219,   307,   219,   304,   200,   219,   331,   200,   200,   200,
     515,   334,   303,   334,   193,   108,   109,   110,   111,   112,
     113,   114,   115,   121,   122,   127,   140,   141,   147,   148,
     149,   194,    14,   436,   296,   367,   360,   289,   194,   324,
     326,   360,   328,   196,   166,   360,   517,   334,   500,   505,
     334,   498,   436,   303,   367,   222,   278,   360,   360,   360,
     360,   360,   360,   422,    53,   158,   173,   202,   217,   219,
     367,   479,   482,   486,   502,   507,   422,   219,   482,   507,
     422,   142,   184,   186,   226,   487,   299,   289,   301,   179,
     180,   231,   219,   219,   422,    13,   218,   193,   523,   523,
     152,   157,   200,   304,   350,   289,   259,   422,   288,   193,
     523,   287,   343,    70,   217,   220,   334,   479,   481,   160,
     219,   321,   396,     4,   160,   339,   340,    19,   158,   173,
     423,    19,   158,   173,   423,   360,   360,   360,   360,   360,
     360,   173,   360,   158,   173,   360,   360,   360,   423,   360,
     360,   360,   360,   360,   360,    22,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   129,   130,   158,
     173,   216,   217,   357,   423,   360,   133,   134,   135,   290,
     346,   367,   346,   367,   346,   367,   346,   367,   346,   367,
     346,   367,   346,   367,   346,   367,   346,   367,   346,   367,
     346,   367,   367,   346,   367,   346,   367,   346,   367,   346,
     367,   346,   367,   346,   367,   220,   334,   375,   277,     8,
     369,   374,   436,   173,   303,   367,   226,   201,   201,   201,
     433,   201,   201,   201,   226,   201,   280,   201,   280,   201,
     280,   201,   433,   201,   433,   297,   464,   220,   218,   464,
     464,   360,   173,   173,   464,   367,   436,   436,    20,   422,
     464,    70,   334,   481,   492,   200,   367,   173,   367,   464,
     509,   511,   513,   436,   523,   360,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   289,   201,   433,   220,   220,   267,   436,
     436,   220,   436,   220,   436,   523,   436,   384,   523,   387,
     201,   339,   220,   220,   220,   220,   220,   220,    20,   340,
     219,   137,   389,   217,   360,   220,   142,   193,   226,   486,
     188,   189,   218,   490,   193,   188,   218,   226,   489,    20,
     220,   486,   184,   187,   488,    20,   367,   184,   503,   297,
     297,   367,   422,   422,    20,   219,   422,   220,   219,   219,
     352,   354,    12,    23,    24,   165,   252,   253,   367,    20,
     503,   292,   278,   173,   220,   481,   479,   193,   220,   193,
     523,   220,   173,   320,   320,   219,   127,   137,   173,   194,
     199,   337,   338,   279,   200,   219,   200,   219,   219,   219,
     218,    19,   158,   173,   423,   196,   158,   173,   360,   219,
     219,   158,   173,   360,     1,   219,   218,   340,   340,   340,
     220,   218,    57,    63,   372,    67,   373,   226,   201,   226,
     438,   443,   445,   464,   447,   441,   432,   173,   449,   201,
     453,   201,   457,   201,   461,   383,   463,   386,   201,   433,
     220,    43,   389,   201,   201,   334,    20,   201,   481,   220,
     220,   220,   173,   220,   201,   226,   220,   201,   436,   384,
     387,   201,   220,   219,   436,   367,   201,   201,   201,   201,
     220,   201,   201,   220,   201,   339,   279,   219,   334,   360,
     367,   367,   482,   486,   367,   158,   173,   479,   490,   218,
     367,   218,   502,   334,   482,   184,   187,   190,   491,   218,
     334,   201,   201,   196,   234,    20,    20,   334,   422,    20,
     360,   360,   436,   279,   289,   253,   367,    12,   255,   334,
     288,   339,   220,   218,   217,   193,   218,   220,   338,   173,
     173,   219,   173,   173,   193,   218,   280,   361,   360,   363,
     360,   220,   334,   360,   200,   219,   360,   219,   218,   360,
     217,   220,   334,   219,   218,   358,   220,   334,   226,    47,
     373,    46,   106,   370,   375,   339,   451,   455,   459,   219,
     464,   173,   367,   495,   497,   289,   334,   308,   220,   201,
     433,   219,   173,   332,   201,   201,   201,   516,   296,   201,
     226,   325,   327,   329,   518,   501,   506,   499,   219,   342,
     280,   220,   334,   185,   220,   486,   490,   219,   137,   389,
     185,   486,   218,   185,   300,   302,   235,   181,   334,   334,
     185,    20,   334,   220,   220,   201,   280,   289,   256,   226,
     185,   279,   220,   479,   173,   218,   196,   394,   220,   173,
     337,   218,   142,   289,   335,   436,   220,   464,   220,   220,
     220,   365,   360,   360,   220,   479,   220,   360,   220,   375,
      33,   371,   370,   372,   294,   219,   219,   220,   367,   173,
     367,   201,   510,   512,   514,   219,   220,   219,   367,   367,
     367,   219,    70,   492,   219,   219,   220,   360,   335,   220,
     360,   137,   389,   490,   360,   367,   367,   360,   491,   503,
     367,   219,   295,   233,   220,   220,   360,   334,   185,   353,
     201,   165,   253,    26,   105,   257,   310,   311,   312,   314,
     367,   503,   280,   218,   196,   394,   436,   393,   220,   127,
     367,   201,   201,   464,   220,   220,   218,   220,   378,   371,
     390,   391,   392,   220,   492,   492,   289,   220,   201,   220,
     219,   219,   219,   219,   294,   296,   334,   492,   492,   220,
     226,   522,   367,   367,   220,   522,   522,   294,   177,   185,
     185,   522,   220,   360,   350,   355,   253,   127,   127,   367,
     522,   289,   220,   436,   393,   393,   367,   367,   362,   364,
     201,   220,   284,   379,   219,   479,   483,   484,   485,   485,
     367,   367,   492,   492,   479,   480,   220,   220,   523,   485,
     480,    53,   218,   137,   389,   184,   288,   193,   523,   503,
     360,   218,   185,   522,   350,   367,   288,   393,   367,   367,
     226,   366,   226,   284,   479,   193,   523,   220,   220,   220,
     220,   485,   485,   220,   220,   220,   220,   367,   218,   367,
     367,   218,   288,   220,   522,   522,   360,   218,   367,   226,
     226,   375,   289,   220,   219,   220,   220,   184,   218,   522,
     226,   375,   479,   218,   220
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   224,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   226,   226,   227,
     228,   228,   228,   229,   229,   230,   231,   231,   231,   231,
     232,   232,   233,   233,   234,   235,   234,   236,   236,   236,
     237,   238,   238,   240,   239,   241,   242,   243,   243,   243,
     244,   244,   244,   244,   245,   245,   246,   246,   247,   248,
     248,   249,   249,   250,   251,   251,   252,   252,   253,   253,
     253,   253,   254,   254,   255,   256,   255,   257,   257,   257,
     257,   257,   258,   258,   259,   258,   261,   260,   262,   260,
     263,   264,   265,   267,   266,   268,   268,   268,   268,   268,
     268,   269,   269,   270,   270,   270,   271,   271,   271,   271,
     271,   271,   271,   271,   272,   272,   273,   273,   273,   274,
     274,   274,   274,   275,   275,   276,   276,   276,   276,   276,
     276,   276,   277,   277,   278,   278,   279,   279,   279,   280,
     280,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   282,   283,   283,
     283,   284,   286,   285,   287,   287,   288,   288,   289,   289,
     290,   290,   290,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   292,   292,   292,   293,   294,
     294,   295,   295,   296,   296,   297,   297,   299,   300,   298,
     301,   302,   298,   303,   303,   303,   303,   303,   304,   304,
     304,   305,   305,   307,   308,   306,   306,   309,   309,   309,
     309,   309,   309,   310,   311,   312,   312,   312,   313,   313,
     313,   314,   314,   315,   315,   315,   316,   317,   317,   317,
     318,   318,   319,   319,   320,   320,   321,   321,   321,   321,
     322,   322,   324,   325,   323,   326,   327,   323,   328,   329,
     323,   331,   332,   330,   333,   333,   333,   333,   333,   333,
     334,   334,   335,   335,   335,   336,   336,   336,   337,   337,
     337,   337,   337,   338,   338,   339,   339,   339,   340,   340,
     341,   343,   342,   344,   344,   344,   344,   344,   344,   344,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   346,
     346,   346,   346,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     348,   348,   349,   349,   350,   350,   351,   352,   353,   351,
     354,   355,   351,   356,   356,   356,   356,   356,   356,   356,
     357,   358,   356,   359,   359,   359,   359,   359,   359,   359,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   361,   362,   360,   360,
     360,   360,   363,   364,   360,   360,   360,   365,   366,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   367,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   369,   369,   369,   370,   370,   370,   371,   371,   372,
     372,   372,   373,   373,   374,   375,   375,   375,   375,   376,
     377,   376,   378,   376,   379,   376,   376,   380,   381,   381,
     382,   382,   382,   382,   382,   383,   383,   384,   384,   385,
     385,   385,   386,   387,   387,   388,   388,   388,   389,   389,
     390,   390,   390,   391,   391,   392,   392,   393,   393,   393,
     394,   394,   395,   395,   395,   395,   395,   396,   396,   396,
     396,   396,   396,   397,   398,   397,   399,   399,   400,   400,
     400,   401,   402,   401,   403,   403,   403,   404,   404,   404,
     406,   405,   407,   407,   408,   409,   408,   410,   410,   410,
     411,   412,   412,   413,   413,   414,   414,   415,   416,   416,
     416,   416,   417,   417,   417,   418,   418,   420,   421,   419,
     422,   422,   422,   422,   422,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   424,   424,   424,   424,   424,   424,   424,   424,
     425,   426,   426,   426,   427,   427,   428,   428,   429,   429,
     429,   429,   431,   432,   430,   433,   433,   434,   434,   435,
     435,   436,   436,   436,   436,   436,   436,   437,   438,   436,
     436,   436,   439,   436,   436,   436,   436,   436,   436,   436,
     436,   436,   436,   436,   436,   436,   440,   441,   436,   436,
     442,   443,   436,   444,   445,   436,   446,   447,   436,   436,
     448,   449,   436,   450,   451,   436,   436,   452,   453,   436,
     454,   455,   436,   436,   456,   457,   436,   458,   459,   436,
     460,   461,   436,   462,   463,   436,   464,   464,   464,   466,
     467,   468,   469,   465,   471,   472,   473,   474,   470,   476,
     477,   475,   478,   478,   478,   478,   478,   479,   479,   479,
     479,   479,   479,   479,   479,   480,   480,   481,   482,   482,
     483,   483,   484,   484,   485,   485,   486,   486,   487,   487,
     488,   488,   489,   489,   490,   490,   490,   491,   491,   491,
     492,   492,   493,   493,   493,   493,   493,   493,   494,   495,
     493,   496,   497,   493,   498,   499,   493,   500,   501,   493,
     502,   502,   502,   503,   503,   504,   505,   506,   504,   507,
     507,   508,   508,   508,   509,   510,   508,   511,   512,   508,
     513,   514,   508,   508,   515,   516,   508,   508,   517,   518,
     508,   519,   519,   520,   520,   521,   521,   521,   521,   521,
     522,   522,   523,   523,   524,   524,   524,   524,   524,   524,
     524,   524,   524
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     2,
       0,     1,     1,     1,     1,     4,     1,     1,     2,     2,
       3,     2,     0,     2,     0,     0,     3,     0,     2,     5,
       3,     1,     2,     0,     4,     2,     2,     1,     1,     1,
       1,     2,     3,     3,     2,     4,     0,     1,     2,     1,
       3,     1,     3,     3,     3,     2,     1,     1,     0,     2,
       4,     5,     1,     1,     0,     0,     3,     1,     1,     1,
       1,     1,     5,     4,     0,     6,     0,     6,     0,     8,
       2,     3,     3,     0,     6,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     1,     5,     1,     3,     2,     3,     2,     1,
       1,     1,     1,     1,     4,     1,     2,     3,     3,     3,
       3,     2,     1,     3,     0,     3,     0,     2,     3,     0,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     2,     2,     3,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     3,     2,     2,     2,     2,     2,     3,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       1,     3,     0,     4,     1,     1,     1,     1,     3,     7,
       2,     2,     6,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     2,     2,     0,     2,     2,     3,     0,
       2,     0,     4,     0,     2,     1,     3,     0,     0,     7,
       0,     0,     7,     3,     2,     2,     2,     1,     1,     3,
       2,     2,     3,     0,     0,     5,     1,     2,     5,     5,
       5,     6,     2,     1,     1,     1,     2,     3,     2,     2,
       3,     2,     3,     2,     2,     3,     4,     1,     1,     0,
       1,     1,     1,     0,     1,     3,     9,     8,     8,     7,
       3,     3,     0,     0,     7,     0,     0,     7,     0,     0,
       7,     0,     0,     6,     5,     8,    10,     5,     8,    10,
       1,     3,     1,     2,     3,     1,     1,     2,     2,     2,
       2,     2,     4,     1,     3,     0,     4,     4,     1,     6,
       6,     0,     7,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     8,     5,     6,     1,     4,     3,     0,     0,     8,
       0,     0,     9,     3,     4,     5,     6,     8,     5,     6,
       0,     0,     5,     3,     4,     4,     5,     4,     3,     4,
       1,     1,     1,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     4,     5,     4,     5,     3,     4,     2,
       5,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       1,     4,     4,     7,     8,     3,     0,     0,     8,     3,
       3,     3,     0,     0,     8,     3,     4,     0,     0,     9,
       4,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     2,     4,     1,     1,     4,     4,     4,     4,     4,
       1,     6,     7,     6,     6,     7,     7,     6,     7,     6,
       6,     0,     4,     1,     0,     1,     1,     0,     1,     0,
       1,     1,     0,     1,     5,     1,     1,     2,     0,     0,
       0,     5,     0,    10,     0,    11,     6,     3,     3,     4,
       1,     1,     3,     3,     3,     1,     3,     1,     3,     0,
       2,     3,     3,     1,     3,     0,     2,     3,     1,     1,
       1,     2,     3,     3,     5,     1,     1,     1,     1,     1,
       0,     1,     1,     4,     3,     3,     5,     4,     6,     5,
       5,     4,     4,     0,     0,     5,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     3,     5,     0,     1,     1,
       0,     5,     2,     3,     4,     0,     4,     0,     1,     1,
       1,     7,     9,     0,     2,     0,     1,     3,     1,     1,
       2,     2,     0,     1,     1,     0,     3,     0,     0,     7,
       1,     4,     3,     3,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     1,     3,     1,     2,     0,     3,
       3,     2,     0,     0,     6,     1,     1,     1,     3,     3,
       4,     1,     1,     1,     1,     2,     3,     0,     0,     6,
       4,     5,     0,     9,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     6,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     1,     3,     3,     0,
       0,     0,     0,    10,     0,     0,     0,     0,    10,     0,
       0,     8,     1,     1,     1,     1,     1,     3,     3,     5,
       5,     6,     6,     8,     8,     0,     1,     2,     1,     3,
       3,     5,     1,     2,     1,     0,     0,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     2,     1,     1,
       0,     1,     5,     4,     6,     7,     5,     7,     0,     0,
      10,     0,     0,    10,     0,     0,    10,     0,     0,     7,
       1,     3,     3,     3,     1,     5,     0,     0,    10,     1,
       3,     3,     4,     4,     0,     0,    11,     0,     0,    11,
       0,     0,    10,     5,     0,     0,     9,     5,     0,     0,
      10,     1,     3,     1,     3,     3,     3,     4,     7,     9,
       0,     3,     0,     1,     9,    11,    12,    11,    10,    10,
      10,     9,    10
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

    case YYSYMBOL_format_string: /* format_string  */
            { delete ((*yyvaluep).s); }
        break;

    case YYSYMBOL_optional_format_string: /* optional_format_string  */
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

    case YYSYMBOL_expr2: /* expr2  */
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

    case YYSYMBOL_function_argument_declaration_no_type: /* function_argument_declaration_no_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_function_argument_declaration_type: /* function_argument_declaration_type  */
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

    case YYSYMBOL_variable_declaration_no_type: /* variable_declaration_no_type  */
            { delete ((*yyvaluep).pVarDecl); }
        break;

    case YYSYMBOL_variable_declaration_type: /* variable_declaration_type  */
            { delete ((*yyvaluep).pVarDecl); }
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
            yyextra->g_Program->library.renameModule(yyextra->g_Program->thisModule.get(),*(yyvsp[-2].s));
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

  case 32: /* format_string: %empty  */
        { (yyval.s) = new string(); }
    break;

  case 33: /* format_string: format_string STRING_CHARACTER  */
                                                 { (yyval.s) = (yyvsp[-1].s); (yyvsp[-1].s)->push_back((yyvsp[0].ch)); }
    break;

  case 34: /* optional_format_string: %empty  */
        { (yyval.s) = new string(""); }
    break;

  case 35: /* $@1: %empty  */
            { das_strfmt(scanner); }
    break;

  case 36: /* optional_format_string: ':' $@1 format_string  */
                                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 37: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 38: /* string_builder_body: string_builder_body character_sequence  */
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

  case 39: /* string_builder_body: string_builder_body "{" expr optional_format_string "}"  */
                                                                                                                                     {
        auto se = (yyvsp[-2].pExpression);
        if ( !(yyvsp[-1].s)->empty() ) {
            auto call_fmt = new ExprCall(tokAt(scanner,(yylsp[-1])), "_::fmt");
            call_fmt->arguments.push_back(make_smart<ExprConstString>(tokAt(scanner,(yylsp[-1])),":" + *(yyvsp[-1].s)));
            call_fmt->arguments.push_back(se);
            se = call_fmt;
        }
        static_cast<ExprStringBuilder *>((yyvsp[-4].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-4].pExpression);
        delete (yyvsp[-1].s);
    }
    break;

  case 40: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 41: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 42: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 43: /* $@2: %empty  */
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

  case 44: /* expr_reader: '%' name_in_namespace $@2 reader_character_sequence  */
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

  case 45: /* options_declaration: "options" annotation_argument_list  */
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

  case 47: /* keyword_or_name: "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 48: /* keyword_or_name: "keyword"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 49: /* keyword_or_name: "type function"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 50: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 51: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 52: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 53: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 54: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 55: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 56: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 57: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 61: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 62: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 63: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 64: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 65: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 66: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 67: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 68: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 69: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-2])), (yyvsp[-2].pExpression)->at);
        if (!format::is_else_newline() && (yyvsp[0].pExpression) != nullptr) {
            format::skip_spaces_or_print(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), yyextra->das_tab_size);
        }

        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 71: /* expression_else: elif_or_static_elif expr expression_block SEMICOLON expression_else  */
                                                                                                    {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-3])), (yyvsp[-3].pExpression)->at);
        if (!format::is_else_newline() && (yyvsp[0].pExpression) != nullptr) {
            format::skip_spaces_or_print(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-2])), tokAt(scanner,(yylsp[0])), yyextra->das_tab_size);
        }

        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-4].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 72: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 73: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 74: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 75: /* $@3: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 76: /* expression_else_one_liner: "else" $@3 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 77: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 78: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 79: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 80: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 81: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* expression_if_then_else: if_or_static_if expr expression_block SEMICOLON expression_else  */
                                                                                                {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-3])), (yyvsp[-3].pExpression)->at);
        if (!format::is_else_newline() && (yyvsp[0].pExpression) != nullptr) {
            format::skip_spaces_or_print(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-2])), tokAt(scanner,(yylsp[0])), yyextra->das_tab_size);
        }

        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-4].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 83: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-2])), (yyvsp[-2].pExpression)->at);
        if (!format::is_else_newline() && (yyvsp[0].pExpression) != nullptr) {
            format::skip_spaces_or_print(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), yyextra->das_tab_size);
        }

        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 84: /* $@4: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 85: /* expression_if_then_else: expression_if_one_liner "if" $@4 expr expression_else_one_liner semicolon  */
                                                                                                                                                         {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-2])), (yyvsp[-2].pExpression)->at);

        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),(yyvsp[-2].pExpression),ast_wrapInBlock((yyvsp[-5].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 86: /* $@5: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 87: /* expression_for_loop: "for" $@5 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        format::wrap_par_expr(tokRangeAt(scanner, (yylsp[-3]), (yylsp[-1])), tokRangeAt(scanner, (yylsp[-3]), (yylsp[-1])));
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 88: /* $@6: %empty  */
                     { // Had to add to successfully convert to v2 syntax, just copied from ds2_parser
                             yyextra->das_keyword = true;
     }
    break;

  case 89: /* expression_for_loop: "for" $@6 '(' variable_name_with_pos_list "in" expr_list ')' expression_block  */
                                                                                                  {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 90: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 91: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at);

        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = (yyvsp[-1].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 92: /* expression_with: "with" expr expression_block  */
                                                         {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at);
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = (yyvsp[-1].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 93: /* $@7: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 94: /* expression_with_alias: "assume" "name" '=' $@7 expr semicolon  */
                                                                                                         {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-5])), *(yyvsp[-4].s), (yyvsp[-1].pExpression) );
        delete (yyvsp[-4].s);
    }
    break;

  case 95: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 96: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 97: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 98: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 99: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 100: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 101: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 102: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 103: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 104: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 105: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 106: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 107: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 108: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 112: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 113: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                               {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]))); delete (yyvsp[-4].s); }
    }
    break;

  case 114: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 115: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 116: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 117: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 118: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 119: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 120: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 121: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 122: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 123: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 124: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 125: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 126: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 127: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 128: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 129: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 130: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 131: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 132: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 133: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 134: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 135: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 136: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 137: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 138: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 139: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 140: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 141: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 142: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 143: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 144: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 145: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 146: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 147: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 148: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 149: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 150: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 151: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 152: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 153: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 154: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 155: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 156: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 157: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 158: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 159: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 160: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 161: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 162: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 163: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 164: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 165: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 166: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 167: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 168: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 169: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 170: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 171: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 172: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 173: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 174: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 175: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 176: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 177: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 178: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 179: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 180: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 181: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 182: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 183: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 184: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 185: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 186: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 187: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 188: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 189: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 190: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 191: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 192: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 193: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 194: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 195: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 196: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 197: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 198: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 199: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 200: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 201: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 202: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 203: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 204: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 205: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 206: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 207: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 208: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 209: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 210: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 211: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 212: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 213: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 214: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 215: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 216: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 217: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 218: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 219: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 220: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 221: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 222: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 223: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 224: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 225: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 226: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 227: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 228: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 229: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 230: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 231: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 232: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 233: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 234: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 235: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 236: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 237: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 238: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 239: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 240: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 241: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 242: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 243: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
                                                                {
        (yyvsp[-1].pFuncDecl)->body = (yyvsp[0].pExpression);
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-3].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
        }
    }
    break;

  case 244: /* open_block: "begin of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 245: /* open_block: "new scope"  */
                        { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 246: /* close_block: "end of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 247: /* close_block: "close scope"  */
                         { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 248: /* expression_block: open_block expressions close_block  */
                                                                  {
        auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui), format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size, format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 249: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                                  {
        auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-6])));
        if (format::is_replace_braces() && (yyvsp[-6].ui) != 0xdeadbeef && format::prepare_rule(prev_loc)) {
            handle_brace(prev_loc, (yyvsp[-6].ui), format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-5])))),
                         yyextra->das_tab_size, format::Pos::from_last(tokAt(scanner,(yylsp[-5]))));
            auto prev_loc_f = format::Pos::from(tokAt(scanner,(yylsp[-2])));
            assert((yyvsp[-2].ui) != 0xdeadbeef);
            {
                const auto internal_f = format::get_substring(prev_loc_f, format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
                format::get_writer() << " finally {" << internal_f << "\n" << string((yyvsp[-2].ui) * yyextra->das_tab_size, ' ') + "}";
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

  case 250: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
                                                      {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            auto start = format::Pos::from_last(tokAt(scanner, (yylsp[-1])));
            start.column -= 1; // drop )
            if (format::is_replace_braces() && format::prepare_rule(start)) {
                if (!((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.empty()) {
                    format::get_writer() << ", ";
                }
                format::get_writer() << format::get_substring(tokAt(scanner, (yylsp[0]))) << ");";
                format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[0]))));
            }
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 251: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
                                                                    {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 252: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 253: /* expression_any: SEMICOLON  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 254: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 255: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 257: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 258: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 259: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 260: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 261: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 262: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 263: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 264: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 265: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 266: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 267: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 268: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 269: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 270: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 271: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 272: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 273: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 274: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 275: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 276: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 277: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 278: /* expr_keyword: "keyword" expr expression_block  */
                                                           {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at); // wrap match (expr)
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s));
        pCall->arguments.push_back((yyvsp[-1].pExpression));
        auto resT = new TypeDecl(Type::autoinfer);
        auto blk = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,resT,(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])));
        pCall->arguments.push_back(blk);
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 279: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 280: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 281: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 282: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 283: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 284: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 285: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 286: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 287: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 288: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 289: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 290: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 291: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 292: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 293: /* expr_pipe: expr_assign " <|" expr_block  */
                                                        {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back((yyvsp[0].pExpression));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else if ( pipeCall->rtti_isVar() ) {
            // a += b <| c
            auto pVar = (ExprVar *) pipeCall;
            auto pCall = yyextra->g_Program->makeCall(pVar->at,pVar->name);
            pCall->arguments.push_back((yyvsp[0].pExpression));
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

  case 294: /* expr_pipe: "@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }

        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 295: /* expr_pipe: "@@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 296: /* expr_pipe: "$ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "$";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 297: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 298: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 299: /* name_in_namespace: "name" "::" "name"  */
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

  case 300: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 301: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 302: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 303: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 304: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 305: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 306: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 307: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 308: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 309: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 310: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 311: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 312: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 313: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 314: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 315: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 316: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 317: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 318: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 319: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 320: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 321: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 322: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 323: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 324: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 325: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 326: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                              {
        const auto end_block = format::Pos::from_last(tokAt(scanner, (yylsp[-2])));
        const auto start = format::Pos::from(tokAt(scanner, (yylsp[-3])));
        if (format::is_replace_braces()) {
            format::skip_spaces_or_print(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])), yyextra->das_tab_size);
        }

        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 327: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 328: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 329: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 330: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 331: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 332: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 333: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 334: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 335: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 336: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
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

  case 337: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 338: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 339: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 340: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 341: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 342: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 343: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 344: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 345: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 346: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 347: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 348: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 349: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 350: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 351: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 352: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 353: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 354: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-2].s);
    }
    break;

  case 355: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),(yyvsp[-1].pExpression),*(yyvsp[-3].s));
            }
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 356: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
                                                                                                                                    {
            format::replace_with(false,
                                 format::Pos::from(tokAt(scanner,(yylsp[-8]))),
                                 format::get_substring(tokRangeAt(scanner,(yylsp[-7]),(yylsp[-2]))),
                                 format::Pos::from(tokAt(scanner,(yylsp[-1]))), " ", "(");

            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ptd->typeexpr,*(yyvsp[-5].s),*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),(yyvsp[-1].pExpression),*(yyvsp[-5].s),*(yyvsp[-3].s));
            }
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 357: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),(yyvsp[-1].pExpression));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 358: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),(yyvsp[-1].pExpression),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 359: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" semicolon "name" '>' '(' expr ')'  */
                                                                                                                           {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),(yyvsp[-1].pExpression),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 360: /* expr_list: expr2  */
                       {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 361: /* expr_list: expr_list ',' expr2  */
                                             {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 362: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 363: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 364: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 365: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 366: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 367: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 368: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 369: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 370: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 371: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 372: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 373: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 374: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 375: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 376: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                                              {
        if (format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-3]))))) {
            format::get_writer()
                      << "capture("
                      << format::substring_between(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])))
                      << format::get_substring(tokAt(scanner, (yylsp[-2])))
                      << format::substring_between(tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])))
                      << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[0]))));
        }

         ; (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 377: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 378: /* expr_block: expression_block  */
                                            {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << "$() ";
            format::finish_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))));
        }

        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 379: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 380: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 381: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 382: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 383: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 384: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 385: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 386: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 387: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 388: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 389: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 390: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 391: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 392: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 393: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 394: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 399: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 400: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 401: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                         {
            if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
                auto tok = tokAt(scanner, (yylsp[0]));
                tok.column += 1;
                format::get_writer() << "@" << format::get_substring(tok) << ";";
                format::finish_rule(format::Pos::from_last(tok));
            }

            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    break;

  case 410: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                         {
            if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
                auto tok = tokAt(scanner, (yylsp[0]));
                tok.column += 1;
                format::get_writer() << "@@" << format::get_substring(tok) << ";";
                format::finish_rule(format::Pos::from_last(tok));
            }
            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    break;

  case 411: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                         {
            if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
                auto tok = tokAt(scanner, (yylsp[0]));
                tok.column += 1;
                format::get_writer() << "$" << format::get_substring(tok) << ";";
                format::finish_rule(format::Pos::from_last(tok));
            }
            (yyval.pExpression) = (yyvsp[0].pExpression);
        }
    break;

  case 412: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 413: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 414: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 415: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 416: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 420: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 423: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 424: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 425: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 430: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 431: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 432: /* expr_method_call: expr2 "->" "name" '(' ')'  */
                                                          {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 433: /* expr_method_call: expr2 "->" "name" '(' expr_list ')'  */
                                                                               {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 434: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 435: /* func_addr_name: "$i" '(' expr2 ')'  */
                                           {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 436: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 437: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 438: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 439: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 440: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 441: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 442: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
                                                                                                                                                                                     {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = (yyvsp[-3].pTypeDecl);
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 443: /* expr_field: expr2 '.' "name"  */
                                               {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 444: /* expr_field: expr2 '.' '.' "name"  */
                                                   {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 445: /* expr_field: expr2 '.' "name" '(' ')'  */
                                                       {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 446: /* expr_field: expr2 '.' "name" '(' expr_list ')'  */
                                                                            {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 447: /* expr_field: expr2 '.' "name" '(' '[' make_struct_fields ']' ')'  */
                                                                                        {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->methodCall = true;
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        nc->nonNamedArguments.push_back((yyvsp[-7].pExpression));
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 448: /* expr_field: expr2 '.' basic_type_declaration '(' ')'  */
                                                                         {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 449: /* expr_field: expr2 '.' basic_type_declaration '(' expr_list ')'  */
                                                                                              {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 450: /* $@28: %empty  */
                                { yyextra->das_suppress_errors=true; }
    break;

  case 451: /* $@29: %empty  */
                                                                             { yyextra->das_suppress_errors=false; }
    break;

  case 452: /* expr_field: expr2 '.' $@28 error $@29  */
                                                                                                                     {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 453: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 454: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 455: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 456: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 457: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 458: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 459: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 460: /* expr2: name_in_namespace  */
                                              { need_wrap_current_expr = true; (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 461: /* expr2: expr_field  */
                                              { need_wrap_current_expr = true; (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 462: /* expr2: expr_mtag  */
                                              { need_wrap_current_expr = true; (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 463: /* expr2: '!' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"!",(yyvsp[0].pExpression)); }
    break;

  case 464: /* expr2: '~' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"~",(yyvsp[0].pExpression)); }
    break;

  case 465: /* expr2: '+' expr2  */
                                                   { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+",(yyvsp[0].pExpression)); }
    break;

  case 466: /* expr2: '-' expr2  */
                                                   { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"-",(yyvsp[0].pExpression)); }
    break;

  case 467: /* expr2: expr2 "<<" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr2: expr2 ">>" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr2: expr2 "<<<" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr2: expr2 ">>>" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr2: expr2 '+' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr2: expr2 '-' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr2: expr2 '*' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr2: expr2 '/' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr2: expr2 '%' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr2: expr2 '<' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr2: expr2 '>' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr2: expr2 "==" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr2: expr2 "!=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr2: expr2 "<=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr2: expr2 ">=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr2: expr2 '&' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr2: expr2 '|' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr2: expr2 '^' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr2: expr2 "&&" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr2: expr2 "||" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr2: expr2 "^^" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr2: expr2 ".." expr2  */
                                               {
        need_wrap_current_expr = true;
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 489: /* expr2: "++" expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"++", (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr2: "--" expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"--", (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr2: expr2 "++"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 492: /* expr2: expr2 "--"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 493: /* expr2: expr2 '[' expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 494: /* expr2: expr2 '.' '[' expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 495: /* expr2: expr2 "?[" expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 496: /* expr2: expr2 '.' "?[" expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 497: /* expr2: expr2 "?." "name"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 498: /* expr2: expr2 '.' "?." "name"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 499: /* expr2: '*' expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 500: /* expr2: expr2 '?' expr2 ':' expr2  */
                                                             {
        need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp3(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 501: /* expr2: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 502: /* expr2: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 503: /* expr2: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 504: /* expr2: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 505: /* expr2: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 506: /* expr2: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 507: /* expr2: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 508: /* expr2: '(' expr_list optional_comma ')'  */
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

  case 509: /* expr2: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 510: /* expr2: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 511: /* expr2: "deref" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 512: /* expr2: "addr" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 513: /* expr2: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 514: /* expr2: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr2 ')'  */
                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 515: /* expr2: expr2 "??" expr2  */
                                                     { (yyval.pExpression) = new ExprNullCoalescing(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 516: /* $@30: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 517: /* $@31: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 518: /* expr2: expr2 "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                             {
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 519: /* expr2: expr2 "is" basic_type_declaration  */
                                                                {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 520: /* expr2: expr2 "is" "name"  */
                                               {
        (yyval.pExpression) = new ExprIsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 521: /* expr2: expr2 "as" "name"  */
                                               {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 522: /* $@32: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 523: /* $@33: %empty  */
                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 524: /* expr2: expr2 "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                  {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 525: /* expr2: expr2 "as" basic_type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 526: /* expr2: expr2 '?' "as" "name"  */
                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 527: /* $@34: %empty  */
                                                    { yyextra->das_arrow_depth ++; }
    break;

  case 528: /* $@35: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 529: /* expr2: expr2 '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                      {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-8]), (yylsp[-1])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 530: /* expr2: expr2 '?' "as" basic_type_declaration  */
                                                                    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 531: /* expr2: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 532: /* expr2: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 533: /* expr2: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 534: /* expr2: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 535: /* expr2: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); (yyval.pExpression)->at = tokAt(scanner, (yylsp[0])); }
    break;

  case 536: /* expr2: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 537: /* expr2: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 538: /* expr2: expr2 "<|" expr2  */
                                                  { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); }
    break;

  case 539: /* expr2: expr2 "|>" expr2  */
                                                  { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]), (yylsp[0]))); (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])); }
    break;

  case 540: /* expr2: expr2 "|>" basic_type_declaration  */
                                                           {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
    }
    break;

  case 541: /* expr2: name_in_namespace "name"  */
                                                  {
        if (format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << "." << format::get_substring(tokAt(scanner,(yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 542: /* expr2: "unsafe" '(' expr2 ')'  */
                                          {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 543: /* expr2: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 544: /* expr: expr2  */
                                       {
        if (need_wrap_current_expr) {
            format::wrap_par_expr_newline(tokAt(scanner,(yylsp[0])), (yyvsp[0].pExpression)->at);
            need_wrap_current_expr = false;
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 545: /* expr_mtag: "$$" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 546: /* expr_mtag: "$i" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 547: /* expr_mtag: "$v" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 548: /* expr_mtag: "$b" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 549: /* expr_mtag: "$a" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 550: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 551: /* expr_mtag: "$c" '(' expr2 ')' '(' ')'  */
                                                             {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 552: /* expr_mtag: "$c" '(' expr2 ')' '(' expr_list ')'  */
                                                                                 {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 553: /* expr_mtag: expr2 '.' "$f" '(' expr2 ')'  */
                                                                  {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 554: /* expr_mtag: expr2 "?." "$f" '(' expr2 ')'  */
                                                                   {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 555: /* expr_mtag: expr2 '.' '.' "$f" '(' expr2 ')'  */
                                                                      {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 556: /* expr_mtag: expr2 '.' "?." "$f" '(' expr2 ')'  */
                                                                       {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 557: /* expr_mtag: expr2 "as" "$f" '(' expr2 ')'  */
                                                                     {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 558: /* expr_mtag: expr2 '?' "as" "$f" '(' expr2 ')'  */
                                                                         {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 559: /* expr_mtag: expr2 "is" "$f" '(' expr2 ')'  */
                                                                     {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 560: /* expr_mtag: '@' '@' "$c" '(' expr2 ')'  */
                                                          {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 561: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 562: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                                     {
        if (format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-3]))))) {
            for (const auto &arg: *(yyvsp[-2].aaList)) {
                format::get_writer() << "@" << format::get_substring(arg.at);
            }
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[0]))));
        }
        (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/
    }
    break;

  case 563: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 564: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 565: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 566: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 567: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 568: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 569: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 570: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 571: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 572: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 573: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 574: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 575: /* opt_sem: SEMICOLON  */
                { (yyval.b) = false; }
    break;

  case 576: /* opt_sem: "end of expression"  */
          { (yyval.b) = true; }
    break;

  case 577: /* opt_sem: "end of expression" SEMICOLON  */
                    { (yyval.b) = true; }
    break;

  case 578: /* opt_sem: %empty  */
                  {(yyval.b) = false; }
    break;

  case 579: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 580: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 581: /* struct_variable_declaration_list: struct_variable_declaration_list $@36 structure_variable_declaration semicolon opt_sem  */
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

  case 582: /* $@37: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 583: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@37 function_declaration_header semicolon opt_sem  */
                                                                  {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-6].b),(yyvsp[-4].b), (yyvsp[-2].pFuncDecl));
            }
    break;

  case 584: /* $@38: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 585: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@38 function_declaration_header expression_block opt_sem  */
                                                                                {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[-1])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 586: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon opt_sem  */
                                                                                               {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-3])), CompilationError::syntax_error);
        delete (yyvsp[-3].faList);
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
    }
    break;

  case 587: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 588: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 589: /* function_argument_declaration_type: "$a" '(' expr2 ')'  */
                                      {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 590: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 591: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 592: /* function_argument_list: function_argument_declaration_no_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 593: /* function_argument_list: function_argument_declaration_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 594: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 595: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 596: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 597: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 598: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 599: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 600: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 601: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 602: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 603: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 604: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 605: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 606: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 607: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 608: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 609: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 610: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 611: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 612: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 613: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 614: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 615: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 616: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 617: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 618: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 619: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 620: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 621: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 622: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 623: /* let_variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 624: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 625: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 626: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 627: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 628: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 629: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 630: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 631: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr  */
                                                                                                                  {
        // Until absence of semicolon with lambda is not fixed
        format::try_semicolon_at_eol(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 632: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 633: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 634: /* $@39: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 635: /* global_variable_declaration_list: global_variable_declaration_list $@39 optional_field_annotation let_variable_declaration opt_sem  */
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

  case 636: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 637: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 638: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 639: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 640: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 641: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                                   {
        handle_brace(format::Pos::from(tokAt(scanner, (yylsp[-2]))), (yyvsp[-2].ui),
                     format::get_substring(tokRangeAt(scanner, (yylsp[-2]), (yylsp[0]))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 642: /* $@40: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 643: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@40 optional_field_annotation let_variable_declaration  */
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

  case 644: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 645: /* enum_list: enum_list "name" opt_sem  */
                                              {
        format::skip_token(true, false, tokAt(scanner,(yylsp[0])));
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

  case 646: /* enum_list: enum_list "name" '=' expr opt_sem  */
                                                              {
        format::skip_token(true, false, tokAt(scanner,(yylsp[0])));
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-3]))) ) {
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

  case 647: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 648: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 649: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 650: /* $@41: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 651: /* single_alias: optional_public_or_private_alias "name" $@41 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias((yyvsp[0].pTypeDecl)) ) {
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

  case 652: /* alias_list: single_alias opt_sem  */
                                    {
        (yyval.positions) = new vector<LineInfo>(1, tokAt(scanner, (yylsp[-1])));
    }
    break;

  case 653: /* alias_list: alias_list single_alias opt_sem  */
                                                       {
        (yyvsp[-2].positions)->emplace_back(tokAt(scanner, (yylsp[-1])));
        (yyval.positions) = (yyvsp[-2].positions);
    }
    break;

  case 654: /* alias_declaration: "typedef" open_block alias_list close_block  */
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

  case 655: /* $@42: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 657: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 658: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 659: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 660: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 661: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block enum_list close_block  */
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
        handle_brace(first_loc, (yyvsp[-2].ui),
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

  case 662: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block enum_list close_block  */
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
        handle_brace(first_loc, (yyvsp[-2].ui),
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

  case 663: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 664: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 665: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 666: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 667: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 668: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 669: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 670: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 671: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 672: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 673: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 674: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 675: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 676: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                                      {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 677: /* $@43: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 678: /* $@44: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 679: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@43 structure_name $@44 optional_struct_variable_declaration_list  */
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

  case 680: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 681: /* variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 682: /* variable_name_with_pos_list: "name" "aka" "name"  */
                                         {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 683: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 684: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 685: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 686: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 687: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 688: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 689: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 690: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 691: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 692: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 693: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 694: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 695: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 696: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 697: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 698: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 699: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 700: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 701: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 702: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 703: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 704: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 705: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 706: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 707: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 708: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 709: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 710: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 711: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 712: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 713: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 714: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 715: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 716: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 717: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 718: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 719: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 720: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 721: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 722: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 723: /* auto_type_declaration: "$t" '(' expr2 ')'  */
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

  case 724: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 725: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 728: /* bitfield_alias_bits: %empty  */
              {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 729: /* bitfield_alias_bits: bitfield_alias_bits "name" SEMICOLON  */
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

  case 730: /* bitfield_alias_bits: bitfield_alias_bits "name" commas  */
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

  case 731: /* bitfield_alias_bits: bitfield_alias_bits "name"  */
                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-1].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-1].pNameList);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 732: /* $@45: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 733: /* $@46: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 734: /* bitfield_type_declaration: "bitfield" '<' $@45 bitfield_bits '>' $@46  */
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

  case 737: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 738: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 739: /* dim_list: '[' expr2 ']'  */
                              {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 740: /* dim_list: dim_list '[' expr2 ']'  */
                                             {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 741: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 742: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 743: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 744: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 745: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 746: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 747: /* $@47: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 748: /* $@48: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 749: /* type_declaration_no_options: "type" '<' $@47 type_declaration '>' $@48  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 750: /* type_declaration_no_options: "typedecl" '(' expr2 ')'  */
                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 751: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 752: /* $@49: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 753: /* type_declaration_no_options: '$' name_in_namespace '<' $@49 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 754: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 755: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 756: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 757: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 758: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 759: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 760: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 761: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 762: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 763: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 764: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 765: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 766: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 767: /* $@51: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 768: /* type_declaration_no_options: "smart_ptr" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 769: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 770: /* $@52: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 771: /* $@53: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 772: /* type_declaration_no_options: "array" '<' $@52 type_declaration '>' $@53  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 773: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 774: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 775: /* type_declaration_no_options: "table" '<' $@54 table_type_pair '>' $@55  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 776: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 777: /* $@57: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 778: /* type_declaration_no_options: "iterator" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 779: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 780: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@59: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 782: /* type_declaration_no_options: "block" '<' $@58 type_declaration '>' $@59  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 783: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@61: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 785: /* type_declaration_no_options: "block" '<' $@60 optional_function_argument_list optional_function_type '>' $@61  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 786: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 787: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 788: /* $@63: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 789: /* type_declaration_no_options: "function" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 790: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 791: /* $@65: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 792: /* type_declaration_no_options: "function" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 793: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 794: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 795: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 796: /* type_declaration_no_options: "lambda" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 797: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 798: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 799: /* type_declaration_no_options: "lambda" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 800: /* $@70: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 801: /* $@71: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 802: /* type_declaration_no_options: "tuple" '<' $@70 tuple_type_list '>' $@71  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 803: /* $@72: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 804: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 805: /* type_declaration_no_options: "variant" '<' $@72 variant_type_list '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 806: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 807: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 808: /* type_declaration: type_declaration '|' '#'  */
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

  case 809: /* $@74: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 810: /* $@75: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 811: /* $@76: %empty  */
                      {
        if (format::is_replace_braces() && (yyvsp[0].ui) != 0xdeadbeef && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << " {";
            format::finish_rule(format::Pos::from(tokAt(scanner,(yylsp[0]))));
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 812: /* $@77: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 813: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@74 "name" $@75 open_block $@76 tuple_alias_type_list $@77 close_block  */
                         {
        if (format::is_replace_braces() && (yyvsp[-4].ui) != 0xdeadbeef && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << "\n" << string((yyvsp[0].ui) * yyextra->das_tab_size, ' ') + "}";
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

  case 814: /* $@78: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 815: /* $@79: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 816: /* $@80: %empty  */
                      {
        if (format::is_replace_braces() && (yyvsp[0].ui) != 0xdeadbeef && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << " {";
            format::finish_rule(format::Pos::from(tokAt(scanner,(yylsp[0]))));
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 817: /* $@81: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 818: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@78 "name" $@79 open_block $@80 variant_alias_type_list $@81 close_block  */
                         {
        if (format::is_replace_braces() && (yyvsp[0].ui) != 0xdeadbeef && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[0]))))) {
            format::get_writer() << "\n" << string((yyvsp[0].ui) * yyextra->das_tab_size, ' ') + "}";
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

  case 819: /* $@82: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 820: /* $@83: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 821: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@82 "name" $@83 open_block bitfield_alias_bits close_block  */
                                                                   {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        auto btype = make_smart<TypeDecl>(Type::tBitfield);
        btype->alias = *(yyvsp[-4].s);
        btype->at = tokAt(scanner,(yylsp[-4]));
        btype->argNames = *(yyvsp[-1].pNameList);
        btype->isPrivateAlias = !(yyvsp[-6].b);
        if ( btype->argNames.size()>32 ) {
            das_yyerror(scanner,"only 32 different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-4])),
                CompilationError::invalid_type);
        }
        if ( !yyextra->g_Program->addAlias(btype) ) {
            das_yyerror(scanner,"type alias is already defined "+*(yyvsp[-4].s),tokAt(scanner,(yylsp[-4])),
                CompilationError::type_alias_already_declared);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfield((yyvsp[-4].s)->c_str(),atvname);
        }
        delete (yyvsp[-4].s);
        delete (yyvsp[-1].pNameList);
    }
    break;

  case 822: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 823: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 824: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 825: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 826: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 827: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 828: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 829: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 830: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 831: /* make_struct_fields: "$f" '(' expr2 ')' copy_or_move expr  */
                                                                            {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 832: /* make_struct_fields: "$f" '(' expr2 ')' ":=" expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner, (yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 833: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' copy_or_move expr  */
                                                                                                        {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 834: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' ":=" expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 835: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 836: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 837: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 838: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 839: /* make_struct_dim: make_struct_dim semicolon make_struct_fields  */
                                                               {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 840: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 841: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 842: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 843: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 844: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 845: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 846: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 847: /* optional_block: "where" expr_block  */
                                  { (yyvsp[0].pExpression)->at = tokAt(scanner, (yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 860: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 861: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 862: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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
                if (type.find('[') != size_t(-1)) {
                    format::get_writer() << "fixed_array(" << before << internal << ")";
                } else {
                    format::get_writer() << before << internal;
                }
                if ((yyvsp[-1].pExpression) != nullptr) {
                    format::get_writer() << " <| " << format::get_substring((yyvsp[-1].pExpression)->at);
                }
            } else {
                // array of structs
    //            const auto internal = format::get_substring(format::Pos::from(tokAt(scanner,@msd)),
    //                                                          format::Pos::from(tokAt(scanner,@end)));
                format::get_writer() << "fixed_array(" << internal << ")";
            }
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 863: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
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
            format::get_writer() << before << after << suffix;
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 864: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                            {
        // std::cout << "case3" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-5]))))) {
            const auto type_name = format::type_to_string((yyvsp[-4].pTypeDecl), tokAt(scanner,(yylsp[-4])));
            auto maybe_init = (format::can_default_init(type_name.value_or(""))) ? "" : ""; // Possible uninitialized
            format::get_writer() << type_name.value_or("") << "("
                                 << format::substring_between(tokAt(scanner,(yylsp[-5])), tokAt(scanner, (yylsp[-4])))
                                 << format::substring_between(tokAt(scanner,(yylsp[-3])), tokAt(scanner, (yylsp[-2])))
                                 << format::substring_between(tokAt(scanner,(yylsp[-2])), tokAt(scanner, (yylsp[0])))
                                 << maybe_init << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 865: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 866: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                          {
        // std::cout << "case6" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-4]))))) {
            auto type_name = format::type_to_string((yyvsp[-3].pTypeDecl), tokAt(scanner,(yylsp[-3]))).value_or("");
            format::get_writer() << "[" << format::substring_between(tokAt(scanner, (yylsp[-4])), tokAt(scanner, (yylsp[-3])))
                                     << format::handle_msd(tokAt(scanner, (yylsp[-3])),
                                                              static_cast<ExprMakeStruct*>((yyvsp[-2].pExpression)),
                                                              tokAt(scanner, (yylsp[0])),
                                                              type_name)
                                 << "]";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 867: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                                          {
        // std::cout << "case7" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            auto type_name = format::type_to_string((yyvsp[-5].pTypeDecl), tokAt(scanner,(yylsp[-5]))).value_or("");
            format::get_writer() << "[" << format::substring_between(tokAt(scanner, (yylsp[-6])), tokAt(scanner, (yylsp[-5])))
                                   << format::handle_msd(tokAt(scanner, (yylsp[-3])),
                                                         static_cast<ExprMakeStruct*>((yyvsp[-2].pExpression)),
                                                         tokAt(scanner, (yylsp[0])),
                                                         type_name)
                                   << "]";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 868: /* $@84: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 869: /* $@85: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 870: /* make_struct_decl: "struct" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 871: /* $@86: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 872: /* $@87: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 873: /* make_struct_decl: "class" '<' $@86 type_declaration_no_options '>' $@87 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 874: /* $@88: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 875: /* $@89: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 876: /* make_struct_decl: "variant" '<' $@88 variant_type_list '>' $@89 '(' use_initializer make_variant_dim ')'  */
                                                                                                                                                                                                                        {
        auto mkt = new TypeDecl(Type::tVariant);
        mkt->at = tokAt(scanner,(yylsp[-9]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 877: /* $@90: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 878: /* $@91: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 879: /* make_struct_decl: "default" '<' $@90 type_declaration_no_options '>' $@91 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 880: /* make_tuple: expr  */
                  {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 881: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 882: /* make_tuple: make_tuple ',' expr  */
                                      {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back((yyvsp[-2].pExpression));
        }
        mt->values.push_back((yyvsp[0].pExpression));
        mt->at = format::concat(mt->at, tokAt(scanner, (yylsp[0])));
        (yyval.pExpression) = mt;
    }
    break;

  case 883: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 884: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 885: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 886: /* $@92: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@93: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 888: /* make_tuple_call: "tuple" '<' $@92 tuple_type_list '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                 {
        auto mkt = new TypeDecl(Type::tTuple);
        mkt->at = tokAt(scanner,(yylsp[-9]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 889: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 890: /* make_dim: make_dim semicolon make_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 891: /* make_dim_decl: '[' optional_expr_list ']'  */
                                                    {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = make_smart<TypeDecl>(Type::tArray);
            mks->makeType->firstType = make_smart<TypeDecl>(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }

    }
    break;

  case 892: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                                   {
        // std::cout << "case13" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-3]))))) {
            auto type_name = format::type_to_string((yyvsp[-2].pTypeDecl), tokAt(scanner,(yylsp[-2])));
            auto internal = format::handle_mka(tokAt(scanner, (yylsp[-2])),
                                               static_cast<ExprMakeArray*>((yyvsp[-1].pExpression)),
                                               tokAt(scanner, (yylsp[0])));
            const auto before = format::substring_between(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])));
            if (static_cast<ExprMakeArray*>((yyvsp[-1].pExpression))->values.size() == 1) {
                // single element
                if (type_name.value_or("").find('[') != size_t(-1)) {
                    format::get_writer() << "fixed_array(" << before << internal << ")";
                } else {
                    format::get_writer() << before << internal;
                }
            } else {
                format::get_writer() << "fixed_array";
                if (!(yyvsp[-2].pTypeDecl)->isAuto()) {
                    format::get_writer() << "<" << type_name.value().substr(0, type_name.value().find('[')) << ">";
                }
                format::get_writer() << "(" << before << internal << ")";
            }
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 893: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                                   {
        // std::cout << "case8" << std::endl;
        if (format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-3]))))) {
            string prefix = "[";
            string suffix = "]";
            if (!(yyvsp[-2].pTypeDecl)->isAuto()) {
                prefix = "array<" + format::get_substring(tokAt(scanner,(yylsp[-2]))) + ">(";
                suffix = ")";
            }
            format::get_writer() << prefix
                                 << format::substring_between(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])))
                                 << format::handle_mka(tokAt(scanner, (yylsp[-2])),
                                                                 static_cast<ExprMakeArray*>((yyvsp[-1].pExpression)),
                                                                 tokAt(scanner, (yylsp[0])))
                                 << suffix;
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 894: /* $@94: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 895: /* $@95: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 896: /* make_dim_decl: "array" "struct" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 897: /* $@96: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 898: /* $@97: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 899: /* make_dim_decl: "array" "tuple" '<' $@96 tuple_type_list '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                     {
        auto mkt = new TypeDecl(Type::tTuple);
        mkt->at = tokAt(scanner,(yylsp[-10]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-6].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-6].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 900: /* $@98: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 901: /* $@99: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 902: /* make_dim_decl: "array" "variant" '<' $@98 variant_type_list '>' $@99 '(' make_variant_dim ')'  */
                                                                                                                                                                      {
        auto mkt = new TypeDecl(Type::tVariant);
        mkt->at = tokAt(scanner,(yylsp[-9]));
        varDeclToTypeDecl(scanner, mkt, (yyvsp[-5].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-5].pVarDeclList));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = mkt;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 903: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 904: /* $@100: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 905: /* $@101: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 906: /* make_dim_decl: "array" '<' $@100 type_declaration_no_options '>' $@101 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-5].pTypeDecl);
            mka->gen2 = true;
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = make_smart<TypeDecl>(Type::tArray);
            msd->makeType->firstType = (yyvsp[-5].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 907: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 908: /* $@102: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 909: /* $@103: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 910: /* make_dim_decl: "fixed_array" '<' $@102 type_declaration_no_options '>' $@103 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 911: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 912: /* make_table: make_table semicolon make_map_tuple  */
                                                      {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 913: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 914: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 915: /* make_table_decl: open_block optional_expr_map_tuple_list close_block  */
                                                                              {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-2])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto mks = new ExprMakeStruct();
            mks->at = tokAt(scanner,(yylsp[-2]));
            mks->makeType = make_smart<TypeDecl>(Type::tTable);
            mks->makeType->firstType = make_smart<TypeDecl>(Type::autoinfer);
            mks->makeType->secondType = make_smart<TypeDecl>(Type::autoinfer);
            mks->useInitializer = true;
            mks->alwaysUseInitializer = true;
            (yyval.pExpression) = mks;
        }

    }
    break;

  case 916: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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
        ttm->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = ttm;
    }
    break;

  case 917: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 918: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = (yyvsp[-4].pTypeDecl);
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = make_smart<TypeDecl>(Type::tTable);
            msd->makeType->firstType = (yyvsp[-4].pTypeDecl);
            msd->makeType->secondType = make_smart<TypeDecl>(Type::tVoid);
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 919: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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
            msd->makeType->firstType = (yyvsp[-6].pTypeDecl);
            msd->makeType->secondType = (yyvsp[-4].pTypeDecl);
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 920: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 921: /* array_comprehension_where: semicolon "where" expr  */
                                          { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 922: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 923: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 924: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 925: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 926: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                         {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 927: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 928: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                  {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 929: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']' ']'  */
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

  case 930: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where "end of code block" ']'  */
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

  case 931: /* array_comprehension: open_block "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block  */
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

  case 932: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block close_block  */
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


