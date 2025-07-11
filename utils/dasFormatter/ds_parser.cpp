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
#define YYLAST   14364

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  224
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  301
/* YYNRULES -- Number of rules.  */
#define YYNRULES  921
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1764

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
    1267,  1268,  1269,  1270,  1271,  1272,  1277,  1295,  1296,  1297,
    1301,  1307,  1307,  1324,  1325,  1328,  1329,  1332,  1339,  1363,
    1381,  1390,  1396,  1397,  1398,  1399,  1400,  1401,  1402,  1403,
    1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1421,  1426,  1432,  1438,  1450,  1451,
    1455,  1456,  1460,  1461,  1465,  1469,  1476,  1476,  1476,  1482,
    1482,  1482,  1491,  1525,  1533,  1540,  1547,  1553,  1554,  1565,
    1569,  1572,  1580,  1580,  1580,  1583,  1589,  1592,  1596,  1600,
    1607,  1614,  1620,  1624,  1628,  1631,  1634,  1642,  1645,  1648,
    1656,  1659,  1667,  1670,  1673,  1681,  1693,  1694,  1695,  1699,
    1700,  1704,  1705,  1709,  1714,  1722,  1733,  1739,  1754,  1766,
    1769,  1775,  1775,  1775,  1778,  1778,  1778,  1783,  1783,  1783,
    1791,  1791,  1791,  1797,  1811,  1827,  1845,  1855,  1866,  1881,
    1884,  1890,  1891,  1898,  1909,  1910,  1911,  1915,  1916,  1917,
    1918,  1919,  1923,  1928,  1936,  1937,  1950,  1954,  1964,  1971,
    1978,  1978,  1987,  1988,  1989,  1990,  1991,  1992,  1993,  1997,
    1998,  1999,  2000,  2001,  2002,  2003,  2004,  2005,  2006,  2007,
    2008,  2009,  2010,  2011,  2012,  2013,  2014,  2015,  2019,  2029,
    2038,  2047,  2052,  2053,  2054,  2055,  2056,  2057,  2058,  2059,
    2060,  2061,  2062,  2063,  2064,  2065,  2066,  2067,  2068,  2073,
    2080,  2092,  2097,  2107,  2111,  2118,  2121,  2121,  2121,  2126,
    2126,  2126,  2139,  2143,  2147,  2152,  2159,  2168,  2173,  2180,
    2180,  2180,  2187,  2191,  2201,  2210,  2219,  2223,  2226,  2232,
    2233,  2234,  2235,  2236,  2237,  2238,  2239,  2240,  2241,  2242,
    2243,  2244,  2245,  2246,  2247,  2248,  2249,  2250,  2251,  2252,
    2253,  2254,  2255,  2256,  2257,  2258,  2259,  2260,  2267,  2268,
    2269,  2270,  2271,  2272,  2273,  2274,  2275,  2276,  2277,  2278,
    2285,  2286,  2287,  2288,  2289,  2290,  2291,  2292,  2306,  2307,
    2308,  2309,  2310,  2313,  2316,  2317,  2317,  2317,  2320,  2325,
    2329,  2333,  2333,  2333,  2338,  2341,  2345,  2345,  2345,  2350,
    2353,  2354,  2355,  2356,  2357,  2358,  2359,  2360,  2361,  2363,
    2367,  2375,  2380,  2384,  2393,  2394,  2395,  2396,  2397,  2398,
    2399,  2403,  2407,  2411,  2415,  2419,  2423,  2427,  2431,  2435,
    2442,  2443,  2452,  2456,  2457,  2458,  2462,  2463,  2467,  2468,
    2469,  2473,  2474,  2478,  2489,  2490,  2491,  2492,  2497,  2500,
    2500,  2519,  2518,  2534,  2533,  2547,  2556,  2568,  2577,  2587,
    2588,  2589,  2590,  2591,  2595,  2598,  2607,  2608,  2612,  2615,
    2618,  2634,  2643,  2644,  2648,  2651,  2654,  2668,  2669,  2673,
    2679,  2685,  2694,  2697,  2704,  2707,  2713,  2714,  2715,  2719,
    2720,  2724,  2731,  2736,  2745,  2751,  2762,  2765,  2770,  2775,
    2783,  2793,  2804,  2807,  2807,  2827,  2828,  2832,  2833,  2834,
    2838,  2845,  2845,  2864,  2867,  2883,  2903,  2904,  2905,  2910,
    2910,  2940,  2943,  2950,  2960,  2960,  2964,  2965,  2966,  2970,
    2980,  3000,  3023,  3024,  3028,  3029,  3033,  3039,  3040,  3041,
    3042,  3046,  3047,  3048,  3052,  3055,  3066,  3071,  3066,  3091,
    3098,  3103,  3112,  3118,  3129,  3130,  3131,  3132,  3133,  3134,
    3135,  3136,  3137,  3138,  3139,  3140,  3141,  3142,  3143,  3144,
    3145,  3146,  3147,  3148,  3149,  3150,  3151,  3152,  3153,  3154,
    3155,  3159,  3160,  3161,  3162,  3163,  3164,  3165,  3166,  3170,
    3181,  3185,  3192,  3204,  3211,  3221,  3222,  3227,  3232,  3247,
    3257,  3270,  3270,  3270,  3283,  3284,  3288,  3292,  3299,  3303,
    3310,  3311,  3312,  3313,  3314,  3329,  3335,  3335,  3335,  3339,
    3344,  3351,  3351,  3358,  3362,  3366,  3371,  3376,  3381,  3386,
    3390,  3394,  3399,  3403,  3407,  3412,  3412,  3412,  3418,  3425,
    3425,  3425,  3430,  3430,  3430,  3436,  3436,  3436,  3441,  3445,
    3445,  3445,  3450,  3450,  3450,  3459,  3463,  3463,  3463,  3468,
    3468,  3468,  3477,  3481,  3481,  3481,  3486,  3486,  3486,  3495,
    3495,  3495,  3501,  3501,  3501,  3510,  3513,  3524,  3540,  3540,
    3545,  3554,  3540,  3583,  3583,  3588,  3598,  3583,  3627,  3627,
    3627,  3670,  3671,  3672,  3673,  3674,  3678,  3685,  3692,  3698,
    3704,  3711,  3718,  3724,  3733,  3736,  3742,  3750,  3755,  3762,
    3767,  3774,  3779,  3785,  3786,  3790,  3791,  3796,  3797,  3801,
    3802,  3806,  3807,  3811,  3812,  3813,  3817,  3818,  3819,  3823,
    3824,  3828,  3861,  3900,  3919,  3939,  3959,  3980,  3980,  3980,
    3988,  3988,  3988,  3995,  3995,  3995,  4006,  4006,  4006,  4017,
    4021,  4027,  4043,  4049,  4055,  4061,  4061,  4061,  4075,  4080,
    4087,  4107,  4135,  4159,  4159,  4159,  4169,  4169,  4169,  4183,
    4183,  4183,  4197,  4206,  4206,  4206,  4226,  4233,  4233,  4233,
    4243,  4248,  4255,  4258,  4264,  4284,  4303,  4311,  4331,  4356,
    4357,  4361,  4362,  4367,  4377,  4380,  4383,  4386,  4394,  4403,
    4415,  4425
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

#define YYPACT_NINF (-1568)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-787)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1568,   730, -1568, -1568,   103,  -110,   499,   624, -1568,     1,
     816,   816,   816, -1568, -1568,   213,    83, -1568, -1568, -1568,
     632, -1568, -1568, -1568,   349, -1568,    18, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568,   -75, -1568,   -11,
     -32,    20, -1568, -1568, -1568, -1568,   499, -1568,    42, -1568,
   -1568, -1568,   816,   816, -1568, -1568,    18, -1568, -1568, -1568,
   -1568, -1568,    33,   106, -1568, -1568, -1568, -1568,    83,    83,
      83,   112, -1568,   838,    -2, -1568, -1568, -1568, -1568,   847,
     849,   529,   854, -1568,   859,   114,   103,   115,  -110,   102,
     126, -1568,   813,   813, -1568,   248,   649,    14,   632,   866,
     255,   260,   290, -1568,   412,   182, -1568, -1568,   -18,   103,
      83,    83,    83,    83, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568,   453, -1568, -1568, -1568, -1568, -1568, -1568, -1568,   624,
   -1568, -1568, -1568, -1568, -1568,   830,   125, -1568, -1568, -1568,
   -1568,   625, -1568, -1568, -1568, -1568,   471, -1568, -1568, -1568,
   -1568,   649, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568,   470, -1568,   104, -1568,   566,   654,   838, 14046, -1568,
     147,   718, -1568,   -29, -1568, -1568, -1568,   846, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568,   176, -1568,   609, -1568, -1568,
     624,   624,   624, -1568, -1568, 13031, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568,   812,   827, -1568,   669,   624,   744, -1568, -1568,   735,
   -1568,   577,   103,   103,   138,   298, -1568, -1568, -1568,   125,
   -1568, 10550, -1568, -1568, -1568,   762,   764, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568,   782,   708, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568,   923, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568,   788,   752, -1568, -1568,   153,   798,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
     624,   636,   799,   624, -1568,   -29,   159, -1568,   103, -1568,
     779,   920,   704, -1568, -1568,   803,   804,   815,   780,   818,
     819, -1568, -1568, -1568,   797, -1568, -1568, -1568, -1568, -1568,
     821, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568,   833, -1568, -1568, -1568,   837,   843, -1568, -1568,
   -1568, -1568,   850,   855,   808,   213, -1568, -1568, -1568, -1568,
   -1568,    43,   857,   638, -1568, -1568, -1568, -1568, -1568, -1568,
     828,   897, -1568,   853, -1568,   187,   877,    90,   856, 10550,
   -1568,  2732, -1568,   436, -1568,   213, -1568, -1568, -1568,   298,
     839, -1568,  9819,   892,   902, 10550, -1568,   -13, -1568, -1568,
   -1568,  9819, -1568, -1568,   904, -1568,   283,   312,   593, -1568,
   -1568,  9819,   -20, -1568, -1568, -1568,    13, -1568, -1568, -1568,
      69,  5863, -1568,   861, 10296,   -60, -1568, 10399,   394, -1568,
   -1568,  9819, -1568, -1568,   372,   177,   177,   177, -1568,   857,
   -1568,   879,   883,  9819, -1568, -1568,   869, -1568, -1568,    67,
     -23,   885,    35,  3567, -1568, -1568,   624,   627,  6071,   873,
    9819,   921,   893,   900,   878, -1568,   632,   901,   932,  6279,
     201,   630,   905, -1568,   639,   906,   907,  3775,  9819,  9819,
     315,   315,   315,   889,   891,   894,   895,   896,   899, -1568,
    2144, 10193,  6489, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568,  6697,   911, -1568,  1089, -1568,  9819,  9819,  9819,  9819,
    9819,  5447,  9819, -1568,   890, -1568, -1568,   632,   632,  9819,
    1094, -1568, -1568, -1568, -1568, -1568, -1568,  6907,  1079, -1568,
   -1568, -1568,   680, -1568,   -34,   632, -1568,   632,   632,   632,
   -1568,   632, -1568, -1568,  1051, -1568, -1568, -1568, -1568,   903,
   -1568, -1568,   329, -1568, -1568, -1568, -1568, -1568, -1568, 13285,
    1071, -1568,   908, -1568, -1568, -1568, -1568, -1568, -1568,   636,
    9819, -1568, -1568,    36,   649, -1568,   909,   922,   924, -1568,
   10644, -1568,  1082,  1182, -1568, -1568, -1568,  4191, 10550, 10550,
   10550, 10763, 10550, 10550,   910,   953, 10550,   669, 10550,   669,
   10550,   669, 10653,   956, 10798, -1568,  9819, -1568, -1568, -1568,
   -1568,   913, -1568, -1568, 12580,  9819, -1568,    43, -1568, -1568,
     944,   -64, -1568, -1568,   459, -1568,   857,   577,   942,   459,
   -1568,   577, 10909,   925,  1096, -1568, -1568,   194, -1568, -1568,
   -1568, -1568, -1568,   624,   372,   929, -1568,   934, -1568, -1568,
     213,   640, -1568,   940,   941,   943, -1568,  9819,  4191, -1568,
     949,  1023,  1840, 14142,  1142, 10550,  9819,  9819, -1568,  9819,
     624,   965, -1568, -1568,  9819, -1568, -1568,   964,   996, -1568,
    9819, -1568, -1568,  9819, -1568, -1568,  9819, -1568, 10550,  4191,
   -1568, 14099,   186,   186,   945, -1568,   903, -1568, -1568, -1568,
    9819,  9819,  9819,  9819,  9819,  9819,   372,  2940,   372,  3149,
     372,  1021, -1568,   620, -1568,   624, -1568,   834,   946,   186,
     186,   -68,   186,   186,   -10,  1151,   948,   975, 13285,   975,
     366, -1568, -1568,   624, -1568,   372,   577, -1568,   977,   624,
   -1568, -1568, -1568,  4399, -1568, -1568, -1568, -1568, -1568, -1568,
      51,    54,   315, -1568, 13823, 13922,  9819,  9819, -1568, -1568,
    9819,  9819,  9819,  9819,   998,  9819,   465,  9819,  9819,  9819,
    9819,  9819,  9819,  9819,  9819,  9819,  7115,  9819,  9819,  9819,
    9819,  9819,  9819,  9819,  9819,  9819,  9819, 13984,  9819,  4607,
    4607,  4607,  4607,  4607,  4607,  4607,  4607,  4607,  4607,  4607,
    9819,  4607,  4607,  4607,  4607,  4607,  4607, -1568,  7323, -1568,
     649,    83,  1164, -1568,   -29, -1568, 10550, -1568,  1000, -1568,
    4191, -1568, 14099,   288,   555,   974,   393, -1568,   677,   712,
   -1568, -1568,   419,   737,   798,   745,   798,   747,   798, -1568,
     395, -1568,   426, -1568, 10550,   957, -1568, -1568, 12691, -1568,
   10550, -1568, -1568, 10550, -1568, -1568, -1568,  9819,  1005, -1568,
    1014, -1568, 10550, -1568,  9819, 10550, 10550, -1568,    30,   372,
   10550,  5655,  7531,  1015,  9819, 10550, -1568, -1568, -1568, 10550,
     975, -1568,   949,  9819,  9819,  9819,  9819,  9819,  9819,  9819,
    9819,  9819,  9819,  9819,  9819,  9819,  9819,  9819,  9819,  9819,
    9819,   624,   444,   969, -1568, 10949, -1568, -1568, 10550, 10550,
   11060, 10550, -1568, -1568, 11095, 10550,   975, 10550, 10653,   975,
     956,   872, -1568, 14099, -1568,    54, 11206, 11246, 11357, 11392,
   11503, 11543,    32,   315,   972,   303,  3358,  4817,  7739,  1052,
    1002,    -6,   191,  1004,   307,    55,  7947,    -6,   390,    62,
    9819,  1016, -1568,  9819, -1568, 10550, -1568, 10550, -1568,  9819,
     801,   372,   372,    66,    24, -1568,  9819, -1568,   979,   982,
     983,   594, -1568, -1568,    71,  9819,    70, -1568,  9819, -1568,
   -1568,   903,   190,  5027, -1568,   205,  1010,   984,  1032,  1032,
   -1568, -1568,   987,   224,   669, -1568,  1007,   990, -1568, -1568,
    1013,   995, -1568, -1568,   210,   210,   930,   930,  2143,  2143,
     997,   313,  1003, -1568, 12731,   -63,   -63,   908,   210,   210,
   13582, 13471,  2483, 13322, 13953, 13174, 13619, 13730, 13754,   930,
     930,   561,   561,   313,   313,   313,   586,  9819,  1008,  1009,
     693,  9819,  1216,  1011, 12842,   315,   315,   315, -1568, -1568,
     680, -1568,   680, -1568,   680, -1568,   680, -1568,   680, -1568,
     680, -1568,   680, -1568,   680, -1568,   680, -1568,   680, -1568,
     680, -1568, -1568,   680, -1568,   680, -1568,   680, -1568,   680,
   -1568,   680, -1568,   680, -1568,   262, -1568,   199,   867,  1154,
     632,  1030, -1568, -1568, 14040, -1568, -1568, -1568, -1568, 10550,
   -1568, -1568, -1568,  1060, -1568,  1033, -1568,  1034, -1568,  1035,
   -1568, 10653, -1568,   956,   449,   857, -1568, -1568,   857,   857,
   11654, -1568,  1181,   -39, -1568,  1190,  1215,  9819,    76,   757,
     694,   276,  1020,  1024,  1068,  1025,   450,  1026,   765, 10550,
   10653,   956,  1313,  1036, 13285, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568,  1039, 10550, -1568, -1568,  9819,  1409,  1607,
   -1568,  1874, -1568,  1950,  1040,  1988,   492,  1041,   493,    54,
     669, -1568, -1568, -1568, -1568, -1568,  1043,  9819, -1568,  9819,
    9819,  9819,  5237, 12580,    99,  9819,   697,   694,   191, -1568,
   -1568,  1031, -1568,  9819, -1568,  1037,  9819, -1568,  9819,   694,
     703,  1045, -1568, -1568,  9819, -1568, -1568, -1568,   549,   553,
    1056,    80,    86,  9819,   372,    89, -1568,  9819,  9819, 10550,
     669,   624, -1568, -1568,   845,  9819, -1568,  1242,  9819, -1568,
    2732,    54,    68, -1568,  1044,   229, 10027, -1568,   697, -1568,
   -1568, -1568,   477,   287,   224,  1086,  1097,  1053,  1098,  1104,
   -1568,   495,   798, -1568,  9819, -1568,  9819,  8155,  9819, -1568,
    1078,  1061, -1568, -1568,  9819,  1062, -1568, 12877,  9819,  8363,
    1063, -1568, 12988, -1568,  8571, -1568, -1568, -1568, -1568, -1568,
     632, -1568, -1568,   769, -1568,    28,   649,    54, -1568, -1568,
   -1568, -1568,   857, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568,  1064, 10550, -1568,
    1106,  9819, -1568, -1568,   310,  9819, -1568,  1065, -1568, -1568,
   -1568,   578, -1568,  1069,  1111, -1568, -1568,  2250,   583,   590,
   -1568, -1568,  9819,  2280,   632, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568,   753,   798,  8779,   396, 11689, -1568,
   -1568,    -6,   191, -1568,  1070,   235,  1002, -1568, -1568, -1568,
   -1568,  1004,   440,    -6,  1072, -1568, -1568, -1568, -1568,   663,
   -1568, -1568, -1568,  1114,  9819,  9819,   699,   101,  9819, 11800,
   11840,  2342,   798, -1568, -1568,   624, -1568,   632,   701, -1568,
     669, -1568,  1067,  5027,  1123,  1083,   504,   377, -1568, -1568,
    1127, -1568, -1568,   224,  1084,   241, 10550, 11951, 10550, 11986,
   -1568,   439, 12097, -1568,  9819, 13433,  9819, -1568, 12137,  5027,
   -1568,   448,  9819, -1568, -1568, -1568,   461,   649,  1270,    28,
   -1568, -1568,   867, -1568,  1085, -1568, -1568, -1568,  9819,   857,
   -1568, -1568,  1088,  1092, -1568,   463, -1568, -1568,  9819,  1132,
    9819,  1112, -1568, -1568, -1568, -1568,  1093,  1095,  1101, -1568,
    9819,  9819,  9819,  1102,  1246,  1103,  1105,  8987, -1568,   241,
   -1568,   508,  9819,   321,   191, -1568,  9819,  9819,  9819,  9819,
     703, -1568,  9819,  9819,  1107, -1568, -1568,   533,   576,  9819,
    9819,   706, -1568, -1568, -1568,  1116,   160,  3983, -1568,  9819,
     798, -1568,   518, -1568,   665, 10550,   -13, -1568,  1109, -1568,
   -1568,  9195, -1568, -1568,  2450, -1568,   767, -1568, -1568, -1568,
   10550, 12248, 12283, -1568,   564, -1568, 12394, -1568, -1568, -1568,
   -1568,  1270,   372,  1113,  1246,  1246,   624,  1117,  1122,  1118,
    1108,  1115,  1121,  1125,  9819, -1568,  9819, -1568, -1568, -1568,
    9819, -1568, -1568,  1246,  1246, -1568, 12434, -1568, -1568, 13136,
    9819,  9819, -1568, 12545, -1568, -1568, 13136, -1568,   632, -1568,
    9819, -1568,  1148,  1146,  1147, 13136,   579,  9819,   400, -1568,
     845, -1568,  9403,  9611, -1568, -1568, -1568, -1568, -1568, -1568,
     632,   624,  1128, 10550,   -13,  1389,  9819, -1568,  9819, -1568,
   -1568, -1568,   772, -1568, -1568,  1129, -1568, 14046, -1568, -1568,
   -1568, -1568, -1568,   -57,   -57, -1568, -1568,  9819, -1568,  9819,
    1246,  1246,   694,  1131,  1135,   975,   -57,   694, -1568,  1283,
    1124, -1568, -1568,   327,  1174,   577,  1166, -1568,  9819,  9819,
    1144,  1175, 13136, -1568,   400, -1568,  9819,  9819, -1568,   577,
   -1568, -1568,  1389,  9819,  9819,   632, -1568, -1568, -1568, -1568,
   -1568,   632, 14046,   694,  1002,  1170, -1568,  1145,  1152,  1155,
    1158,   -57,   -57,  1002,  1159, -1568, -1568,  1160,  1163,  1167,
    9819,  1150,  9819,  9819,  1153,   577, -1568,  1177,   632, 13136,
   -1568,  9819,  1168, -1568, -1568, -1568,  9819,   632,   632, -1568,
   -1568,   649,   624,   581,  1171, -1568, -1568, -1568, -1568, -1568,
    1178,  1180, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568,  1210,  1184, 13136, -1568,   632, -1568, -1568,
   -1568,   649, -1568,   694, -1568, -1568, -1568, -1568,  1185, -1568,
   -1568,   588, -1568, -1568
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   134,     1,   319,     0,     0,     0,   644,   320,     0,
     636,   636,   636,    17,    18,     0,     0,    16,    15,     3,
       0,    10,     9,     8,     0,     7,   625,     6,    11,     5,
       4,    13,    12,    14,   104,   105,   103,   112,   114,    45,
      61,    58,    59,    47,    48,    49,     0,    50,    56,    46,
     234,   233,   636,   636,    24,    23,   625,   638,   637,   808,
     798,   803,     0,   287,    43,   120,   121,   122,     0,     0,
       0,   123,   125,   132,     0,   119,    19,   658,   657,   227,
     646,     0,   661,   626,   627,     0,     0,     0,     0,    51,
       0,    57,     0,     0,    54,     0,   567,   636,     0,    20,
       0,     0,     0,   289,     0,     0,   131,   126,     0,     0,
       0,     0,     0,     0,   135,   229,   228,   231,   226,   648,
     647,     0,   660,   659,   663,   662,   666,   629,   628,   631,
     110,   111,   108,   109,   107,     0,     0,   106,   115,    62,
      60,    56,    53,    52,   639,   564,   565,   641,   236,   235,
     643,   567,   645,    21,    22,    25,   809,   799,   804,   288,
      41,    44,   130,     0,   127,   128,   129,   133,     0,   649,
       0,   654,   622,   550,    26,    27,    31,     0,    99,   100,
      97,    98,    96,    95,   101,     0,    55,     0,   566,   642,
       0,     0,     0,    42,   124,     0,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,     0,     0,   141,   136,     0,     0,   633,   655,     0,
     667,   623,     0,     0,   552,     0,    28,    29,    30,     0,
     113,     0,   717,   800,   805,   195,   196,   193,   144,   145,
     147,   146,   148,   149,   150,   151,   177,   178,   175,   176,
     168,   179,   180,   169,   166,   167,   194,   188,     0,   192,
     181,   182,   183,   184,   155,   156,   157,   152,   153,   154,
     165,     0,   171,   172,   170,   163,   164,   159,   158,   160,
     161,   162,   143,   142,   187,     0,   173,   174,   550,   139,
     264,   232,   701,   704,   707,   708,   702,   705,   703,   706,
       0,     0,   652,   664,   630,   550,     0,   116,     0,   118,
       0,   611,   609,   632,   102,     0,     0,     0,     0,     0,
       0,   674,   694,   675,   710,   676,   680,   681,   682,   683,
     700,   687,   688,   689,   690,   691,   692,   693,   695,   696,
     697,   698,   768,   679,   686,   699,   775,   782,   677,   684,
     678,   685,     0,     0,     0,     0,   709,   730,   733,   731,
     732,   795,   640,     0,   588,   594,   197,   198,   191,   186,
     199,   189,   185,     0,   137,   318,   579,   580,     0,     0,
     230,     0,   633,   567,   650,     0,   656,   568,   668,     0,
       0,   117,     0,     0,     0,     0,   610,     0,   736,   759,
     762,     0,   765,   755,     0,   721,   769,   776,   783,   789,
     792,     0,     0,   745,   750,   744,     0,   758,   754,   747,
       0,     0,   749,   734,     0,   720,   810,   801,   806,   200,
     190,     0,   316,   317,     0,   550,   550,   550,   138,   140,
     266,     0,     0,     0,    72,    73,    86,   495,   496,     0,
       0,     0,     0,   304,   490,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,   700,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   539,
       0,     0,     0,   242,   372,   374,   373,   375,   376,   377,
     378,     0,     0,    37,   233,   243,     0,     0,     0,     0,
       0,   268,     0,   354,   355,   493,   492,     0,     0,     0,
       0,   259,   254,   251,   250,   252,   253,   272,   237,   286,
     265,   245,   532,   244,   449,     0,   523,    80,    81,    78,
     257,    79,   258,   260,   322,   249,   522,   521,   520,   134,
     526,   491,     0,   246,   525,   524,   498,   450,   499,   533,
     379,   451,     0,   494,   811,   815,   812,   813,   814,     0,
       0,   634,   653,   569,   567,   551,     0,     0,     0,   532,
       0,   613,   614,     0,   607,   608,   606,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   136,     0,   136,
       0,   136,     0,     0,     0,   741,   268,   752,   753,   746,
     748,     0,   751,   735,     0,     0,   797,   796,   718,   715,
     719,   287,   724,   725,     0,   589,   584,     0,     0,     0,
     595,     0,     0,     0,   669,   576,   577,   599,   581,   582,
     583,   857,   860,     0,     0,     0,   292,   296,   295,   301,
       0,     0,   340,     0,     0,     0,   893,     0,     0,   308,
     305,     0,   533,   379,     0,     0,   272,     0,   290,     0,
       0,     0,   331,   334,     0,   263,   337,     0,     0,    65,
       0,    90,   897,     0,   866,   875,     0,   863,     0,     0,
     313,   310,   478,   479,   355,   367,   134,   285,   283,   284,
       0,     0,     0,     0,     0,     0,     0,   835,     0,     0,
       0,   873,   900,     0,   276,     0,   279,     0,     0,   455,
     454,   488,   453,   452,     0,     0,     0,   911,   349,   911,
     356,   261,   262,     0,    84,     0,     0,   902,   911,     0,
     370,   240,   530,     0,   248,   255,   256,   307,   312,   321,
       0,   364,     0,   247,     0,     0,     0,     0,   480,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,     0,   651,
     567,     0,     0,   665,   550,   624,     0,   612,     0,   616,
       0,   621,   620,     0,     0,     0,   726,   739,     0,     0,
     711,   713,     0,     0,   139,     0,   139,     0,   139,   586,
       0,   592,     0,   712,     0,     0,   743,   728,     0,   716,
       0,   590,   802,     0,   596,   807,   578,     0,     0,   598,
       0,   597,     0,   600,     0,     0,     0,    91,     0,     0,
       0,   849,     0,     0,     0,     0,   883,   886,   889,     0,
     911,   309,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   291,     0,    92,    93,     0,     0,
       0,     0,    63,    64,     0,     0,   911,     0,     0,   911,
       0,     0,   314,   311,   356,   364,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   287,     0,     0,     0,   869,
     827,   835,     0,   878,     0,     0,     0,   835,     0,     0,
       0,     0,   838,     0,   905,     0,   267,     0,    40,     0,
      38,     0,     0,     0,     0,   880,   912,   269,     0,     0,
       0,   426,   423,   425,    68,     0,     0,   904,   912,   273,
     264,   134,     0,   268,   442,     0,   911,     0,     0,     0,
     330,   329,     0,     0,   136,   282,     0,     0,   509,   508,
       0,     0,   510,   514,   456,   457,   469,   470,   467,   468,
       0,   504,     0,   486,     0,   527,   528,   529,   458,   459,
     474,   475,   476,   477,     0,     0,   472,   473,   471,   465,
     466,   461,   460,   462,   463,   464,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,   401,   410,
     389,   411,   390,   413,   392,   412,   391,   414,   393,   404,
     383,   405,   384,   406,   385,   415,   394,   416,   395,   403,
     381,   382,   417,   396,   418,   397,   407,   386,   408,   387,
     409,   388,   402,   380,   447,     0,   635,     0,   558,   561,
       0,     0,   615,   618,   379,   619,   737,   760,   763,     0,
     766,   756,   722,     0,   770,     0,   777,     0,   784,     0,
     790,     0,   793,     0,     0,   274,   740,   729,   585,   591,
       0,   671,   672,   602,   601,     0,     0,     0,     0,     0,
     850,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,   389,   390,   392,   391,   393,
     383,   384,   385,   394,   395,   381,   396,   397,   386,   387,
     388,   380,   315,     0,     0,   906,   500,     0,     0,     0,
     501,     0,   531,     0,     0,     0,     0,     0,     0,   364,
     136,   534,   535,   536,   537,   538,     0,     0,   836,     0,
       0,     0,     0,   349,   835,     0,     0,     0,     0,   844,
     845,     0,   852,     0,   842,     0,     0,   881,     0,     0,
       0,     0,   840,   882,     0,   872,   837,   901,     0,     0,
      34,     0,     0,     0,     0,     0,   497,     0,     0,     0,
     136,     0,    66,    67,    68,     0,    83,    74,     0,   903,
       0,   364,     0,   443,     0,     0,     0,   446,   912,   826,
     444,   323,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,   139,   505,     0,   511,     0,     0,     0,   484,
       0,     0,   515,   519,     0,     0,   487,     0,     0,     0,
       0,   433,     0,   440,     0,   482,   400,   398,   399,   448,
     135,   559,   560,   561,   562,   553,   567,   364,   617,   738,
     761,   764,   727,   767,   757,   723,   714,   771,   773,   778,
     780,   785,   787,   791,   587,   794,   593,     0,     0,   670,
       0,     0,   858,   861,     0,     0,   293,     0,   298,   299,
     297,     0,   343,     0,     0,   346,   341,     0,     0,     0,
     894,   892,   272,     0,     0,   332,   335,   338,   898,   896,
     867,   876,   874,   864,     0,   139,     0,     0,     0,   817,
     816,   835,     0,   870,     0,     0,   828,   851,   843,   871,
     841,   879,     0,   835,     0,   847,   848,   855,   839,     0,
     277,   280,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   139,    69,    82,     0,    75,     0,     0,   238,
     136,   445,     0,   268,     0,     0,   609,     0,   359,   360,
       0,   358,   357,     0,     0,     0,     0,     0,     0,     0,
     421,     0,     0,   516,     0,   489,     0,   485,     0,   268,
     434,     0,     0,   483,   441,   437,     0,   567,   556,   553,
     554,   555,   558,   570,     0,   774,   781,   788,   268,   275,
     673,   603,     0,     0,    87,     0,   294,   300,     0,     0,
       0,     0,   342,   884,   887,   890,     0,     0,     0,    94,
       0,     0,     0,     0,   849,     0,     0,     0,   241,     0,
     540,     0,     0,     0,     0,   853,     0,     0,     0,     0,
       0,   846,     0,     0,   270,    32,    39,     0,     0,     0,
       0,     0,   424,   549,   427,     0,    68,     0,    85,     0,
     139,   419,     0,   324,   609,     0,     0,   366,     0,   363,
     365,     0,   351,   369,     0,   548,     0,   546,   422,   543,
       0,     0,     0,   542,     0,   435,     0,   438,   575,   557,
     571,   556,     0,     0,   849,   849,     0,     0,     0,     0,
       0,     0,     0,     0,   268,   907,   272,   333,   336,   339,
       0,   850,   868,   849,   849,   502,     0,   368,   541,   909,
       0,     0,   854,     0,   819,   818,   909,   856,   909,   278,
     268,   281,    36,     0,     0,   909,     0,     0,     0,   430,
      68,    70,   304,     0,    76,    80,    81,    78,    79,    77,
     909,     0,     0,     0,     0,     0,     0,   361,     0,   352,
     506,   512,     0,   547,   545,     0,   544,     0,   573,   605,
     604,   563,   742,   834,   834,    89,   344,     0,   347,     0,
     849,   849,   824,     0,     0,   911,   834,   824,   503,     0,
       0,   821,   820,     0,     0,     0,   911,    33,     0,     0,
       0,     0,   909,   428,     0,    71,     0,     0,   310,     0,
     371,   420,     0,     0,     0,     0,   353,   507,   513,   517,
     436,     0,     0,     0,   831,   911,   833,     0,     0,     0,
       0,   834,   834,   825,     0,   895,   908,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   912,     0,   909,   909,
     913,     0,     0,   431,   311,   920,     0,     0,     0,   328,
     518,   567,     0,     0,   912,   832,   859,   862,   345,   348,
       0,     0,   891,   899,   877,   865,   910,   918,   823,   822,
     919,   921,   271,     0,     0,   909,   917,     0,   327,   326,
     572,   567,   829,     0,   885,   888,   916,   914,     0,   325,
     574,     0,   915,   830
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1568, -1568,    -1, -1568, -1568, -1568, -1568,   637,  1281, -1568,
   -1568, -1568, -1568, -1568, -1568,  1394, -1568, -1568, -1568,   898,
    1358, -1568,  1267, -1568, -1568,  1321, -1568, -1568, -1568, -1209,
   -1568, -1568, -1568,  -105, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568,  1176, -1568, -1568,   -44,   -55, -1568,
   -1568, -1568,   596,   597,  -538,  -585,  -789, -1568, -1568, -1568,
   -1567, -1568, -1568,    -4,  -230,  -215,  -379, -1568,   430, -1568,
    -605, -1568,  -668,  -216,   689, -1568, -1568, -1568, -1568,  -422,
       0, -1568, -1568, -1568, -1568, -1568,   -94,   -93,   -90, -1568,
     -88, -1568, -1568, -1568,  1429, -1568,   432, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
     601,   -56,  1229,    11,   171,  -901,  -462, -1568,  -529, -1568,
   -1568,  -385,   652, -1568, -1568, -1568, -1513, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568,  1133, -1568, -1568, -1568,
   -1568, -1568, -1568,   882, -1568,  -155,    -9,  -115,    -8,   140,
   -1568,  -149, -1568, -1568, -1568, -1568, -1568, -1568,   585,  -417,
    -889, -1568,  -421,  -887, -1568,  -643,  -106,  -104, -1568,  -586,
   -1368, -1568,  -366, -1568, -1568,  1391, -1568, -1568, -1568,  1059,
    1029,   120, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,  -627,   117,
   -1568,  1012, -1568, -1568, -1568, -1568, -1568, -1568, -1568,  -400,
   -1568, -1568,  -356, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
    -232, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568,  1017,  -666,  -175,  -845,  -687, -1568,
   -1568, -1135,  -926, -1568, -1568, -1568, -1180,   -27, -1173, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568,   258,  -497,
   -1568, -1568, -1568,   758, -1568, -1568, -1568, -1568, -1568, -1568,
   -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1568, -1046,  -725,
   -1568
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   633,    18,   155,    56,    19,   177,   183,  1592,
    1393,  1505,   727,   525,   161,   526,   105,    21,    22,    47,
      48,    49,    94,    23,    41,    42,   527,   528,  1245,  1246,
     529,  1407,  1517,   530,   531,   985,   532,   654,   655,   533,
     534,   535,   536,  1177,   184,   185,    37,    38,    39,   234,
      71,    72,    73,    74,    24,   299,   390,   224,    25,   117,
     225,   118,   168,   537,   150,   705,  1058,   540,   391,   541,
     736,  1591,   746,  1124,   589,   965,  1503,   967,  1504,   543,
     544,   545,   657,   880,  1466,   546,   547,   548,   549,   550,
     551,   552,   553,   444,   554,   760,  1262,  1000,   555,   556,
     918,  1480,   919,  1481,   921,  1482,   557,   885,  1472,   558,
     737,  1533,   559,  1270,  1271,  1004,   707,   560,   817,   991,
     561,   671,  1059,   563,   564,   565,   983,   566,  1239,  1598,
    1240,  1664,   567,  1052,  1444,   568,   569,  1426,  1677,  1428,
    1678,  1540,  1720,   721,   571,   385,  1452,  1550,  1303,  1305,
    1100,   147,   583,   824,  1627,  1682,   386,   387,   388,   849,
     850,   437,   851,   852,   438,  1201,   645,   646,  1631,   597,
     407,   322,   323,   231,   315,    84,   129,    27,   173,   311,
      95,    96,   187,    97,    28,    53,   121,   170,    29,   396,
     229,   230,    82,   126,   398,    30,   171,   313,   647,   572,
     310,   368,   369,   842,   630,   373,   370,   605,  1315,  1121,
     835,   433,   371,   598,  1309,   854,   603,  1314,   599,  1310,
     600,  1311,   602,  1313,   606,  1317,   607,  1455,   608,  1319,
     609,  1456,   610,  1321,   611,  1457,   612,  1323,   613,  1325,
     636,    31,   101,   191,   374,   637,    32,   102,   192,   375,
     641,    33,   100,   190,   573,  1684,  1694,   997,   951,  1685,
    1686,  1687,   952,   964,  1223,  1217,  1212,  1387,  1143,   574,
     875,  1462,   876,  1463,   930,  1486,   927,  1484,   953,   747,
     575,   928,  1485,   954,   576,  1149,  1561,  1150,  1562,  1151,
    1563,   889,  1476,   925,  1483,   723,   748,   577,  1650,   977,
     578
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   314,   189,    52,   874,   722,   562,   830,   913,   372,
     301,   855,   539,   751,   978,    64,    75,   639,   235,    76,
     634,   761,   844,   989,   846,  1208,   848,   878,  1377,   708,
     709,  1220,   957,   584,  1190,  1404,  1142,   635,   640,  1186,
     663,   669,   138,  1188,  -134,   822,   617,   943,  1526,   593,
    1137,   950,  1197,   950,   163,  1115,    40,  1117,  1002,  1119,
    1681,   776,   777,   774,    90,   700,   776,   777,    75,    75,
      75,    57,   451,   452,  1450,  1218,   423,    58,   627,   104,
      83,   394,  1224,  1241,   539,  1663,  1233,   996,   869,   942,
    1248,   955,   462,   959,  1242,  1243,  1335,   152,   467,    91,
    1394,   944,   619,   424,   425,   628,  1395,   973,   539,  1398,
      75,    75,    75,    75,   594,  1722,  1252,    65,   986,    85,
      62,  1510,    34,    35,   595,   172,   110,   111,   112,   130,
     131,   232,   860,   629,  1451,   481,   482,   664,   665,   752,
     178,   179,   643,   436,   717,   719,    66,  1254,   797,   798,
      63,  1713,   943,   797,   798,   871,  1614,   449,   434,    13,
     399,   538,  1683,   644,    87,  1153,   227,   426,   935,   484,
     485,   427,  1241,    98,    54,   831,   643,   316,   148,    14,
     615,   596,    86,  1242,  1243,   753,   242,   243,   244,   317,
      67,   113,  1495,   233,   442,   869,   660,   644,   149,   616,
     148,  1184,   162,   320,  1187,  1200,   103,   764,   765,   972,
      62,   998,   618,    88,  1003,   996,   114,   151,   539,    68,
     149,   300,    55,   870,   321,   870,    62,   500,   501,   502,
      50,   764,   765,   319,   861,   666,  1244,   443,   428,   864,
      63,   366,   429,  1234,   581,   430,   891,    92,   870,   104,
     514,   674,  1138,   821,   667,   870,    63,   944,    93,   870,
     431,  1348,   871,   870,  1349,   691,   432,   656,   620,   870,
     999,  1259,  1252,   870,   401,   446,    36,   932,  1372,   870,
     132,   139,   870,   447,   521,   133,   621,   134,  1364,   539,
     135,   180,   622,    69,   870,  1337,   181,    86,   182,   141,
    1005,   135,    70,    13,   768,   769,   392,  1601,   383,   397,
      50,  1572,   774,   232,  1582,   776,   777,   778,    93,   912,
     539,   869,   779,    14,   194,  1600,   996,  1255,   768,   769,
      51,   109,   383,   136,   764,   765,   774,   232,   775,   776,
     777,   778,   931,   226,  1231,  1232,   779,  1235,   944,   819,
    1410,  1265,    86,   823,    77,    78,    62,    79,   367,   160,
     318,  1266,   869,  1252,    50,   422,   833,   834,   836,   239,
     838,   839,  1497,   384,   843,   233,   845,   400,   847,  1209,
    1210,  1633,  1634,  1531,    51,    80,    63,   870,   871,   366,
     872,  1665,   113,   873,  1001,   582,   240,  1267,  1256,   233,
    1646,  1647,   797,   798,    50,   366,  1454,   862,  1103,  1211,
    1253,   865,   792,   793,   794,   795,   796,  1300,  1268,  1272,
     690,   144,  1206,  1269,    51,  1257,   797,   798,   156,   871,
     869,   768,   769,   157,   366,   825,  1109,   366,   877,   774,
    1200,   775,   776,   777,   778,  1494,   104,  1412,   869,   779,
     320,   539,  1123,  1251,   869,   893,    81,  1500,  1580,   366,
     661,   762,   300,   158,  1702,   916,  1227,  1691,  1692,   893,
    1101,   321,    13,    50,   996,   685,   300,   423,    50,  -772,
    1414,  1198,  1299,  1425,  -772,   434,   300,   300,   300,  1106,
    1331,  1249,    14,    51,    13,  1214,  1338,   871,    51,  1688,
     366,   366,  -772,   893,   424,   425,   367,  1416,  -779,    62,
     966,  1698,  1174,  -779,    14,   871,   987,  1371,   979,  1135,
    1136,   871,   367,   980,   643,  1215,   741,   742,   984,   797,
     798,  -779,  1383,  1152,   122,   123,   523,   704,   950,    63,
    1654,  1376,  1655,    62,   754,   644,   755,   756,   757,  1660,
     758,   367,   979,   950,   367,    13,  1730,  1731,    13,    13,
      13,   763,  1178,  1179,  1669,  1181,   981,   638,   426,  1183,
    1423,  1185,   427,    63,  1221,    14,  1489,  1222,    14,    14,
      14,  1492,   764,   765,    13,   159,   632,   632,   632,   893,
     434,    13,   829,  1296,  1297,  1298,  1120,  1527,   366,   366,
     366,   145,   366,   366,    14,  1365,   366,  1397,   366,    13,
     366,    14,   366,  1515,    13,    13,  1712,   367,   367,   632,
    1112,   146,  1125,  1022,    13,  1499,   169,  1122,  1128,    14,
     580,  1129,   893,   893,    14,    14,   188,   632,  1023,   428,
    1133,   893,   632,   429,    14,  1173,   430,   193,  1139,   300,
    1327,  1343,   632,  1148,   893,  1402,   893,    13,    13,  1538,
     882,   431,  1743,  1744,   106,   107,   108,   432,  1545,  1099,
    1414,  1096,    43,    44,    45,   366,   300,    14,    14,   768,
     769,  1547,    91,  1556,  1477,   632,   632,   774,  1423,   775,
     776,   777,   778,  1361,  1363,  1415,  1172,   779,   366,  1758,
    1525,   893,  1326,   406,  1324,    46,   164,   165,   166,   167,
     110,  1206,   112,  1424,    13,   367,   367,   367,    13,   367,
     367,   300,   963,   367,  1328,   367,   893,   367,  1578,   367,
       2,  1611,  1498,  1125,    14,  1125,  1612,     3,    14,   300,
     982,   148,   632,    13,  1285,   990,   632,  1522,    13,  1228,
    1390,  1229,   434,  1593,  1391,    13,  1107,  1206,   300,  1286,
       4,   149,     5,    14,     6,   794,   795,   796,    14,   893,
       7,   632,   893,  1544,  1206,    14,   632,   797,   798,  1468,
       8,  1206,  1625,   632,  1474,    13,     9,    50,  1123,  -786,
    -429,  1475,   367,  1347,  -786,  -429,  1594,    13,   110,  1661,
     148,  1752,   148,   241,   961,    14,   962,    51,  1763,   393,
      10,   435,  -786,  -429,   145,   367,  1448,    14,  1353,   302,
     149,    75,   149,   303,   228,  1520,   366,   675,  1328,  1328,
     692,  1105,    11,    12,   146,  1488,  1304,   304,   305,   695,
     883,  1113,   306,   307,   308,   309,   676,   296,  1502,   693,
    1581,  1290,   944,  1553,   366,  1374,   893,  1241,   696,   884,
     366,  1613,   297,   366,   406,   562,  1291,  1252,  1242,  1243,
    1375,   539,   366,    57,   434,   366,   366,  1312,  1110,    58,
     366,  1009,  1013,  1401,  1509,   366,  1519,  1384,   298,   366,
    1385,  1597,   893,  1386,   893,    13,  1027,   404,  1644,   893,
     405,   523,   704,   406,   115,   423,   119,   300,   312,   434,
     116,   124,   120,  1111,  1053,    14,   127,   125,   366,   366,
    1697,   366,   128,   153,  1301,   366,   379,   366,   366,   154,
    1302,  1707,   424,   425,   434,   376,    15,   377,  1114,   300,
    1616,  1469,   434,   367,   434,   380,  1116,    16,  1118,  1123,
    1207,   764,   765,  1216,   434,   378,  1207,  1216,  1336,  1643,
    1725,   381,   434,   403,   434,   366,  1346,   366,  1621,   434,
     382,   367,  1487,  1679,   523,   704,  1693,   367,   236,   237,
     367,  1693,   110,   111,   112,  1656,    43,    44,    45,   367,
     142,   143,   367,   367,   389,   395,   426,   367,   402,   411,
     427,   439,   367,   408,   409,  1588,   367,   174,   175,   176,
    1703,   174,   175,   968,   969,   410,   414,  1723,   412,   413,
    1409,   415,  1610,   236,   237,   238,  1403,   421,  1673,  1674,
     648,   649,   650,   416,   440,   367,   367,   417,   367,    59,
      60,    61,   367,   418,   367,   367,   766,   767,   768,   769,
     419,   300,   300,   300,   434,   420,   774,   585,   775,   776,
     777,   778,   445,   423,   670,   591,   779,   428,   780,   781,
    1534,   429,   441,  1189,   430,   592,   448,   604,   625,   651,
     542,   -77,   367,   652,   367,   662,  1716,  1761,   -88,   431,
     424,   425,   679,   682,   681,   432,  1459,   684,   687,  1306,
     683,   686,   728,  1308,   744,   694,   697,   698,   710,   366,
     711,   726,   740,   712,   713,   714,   749,   759,   715,  1464,
      16,   366,   826,   739,   724,   828,   841,   818,   690,   638,
     840,   856,   792,   793,   794,   795,   796,   859,   863,   868,
     886,   887,   893,   888,   867,  1344,   797,   798,   879,   366,
     366,  1283,   542,   881,   426,   762,   911,  1453,   427,   917,
     922,  1708,   923,   960,   974,   971,   975,   934,   976,  1615,
     988,  1020,  1098,  1102,   366,  1108,   542,  1126,  1131,   799,
     800,   801,   802,   803,   804,   805,   806,  1132,  1146,  1175,
    1516,  1199,   807,   808,  1205,  1206,  1536,  1213,   809,  1236,
    1226,  1237,  1238,  1258,  1260,  1261,  1264,  1273,   810,  1274,
    1532,   811,   812,  1275,  1276,   423,  1277,  1293,   813,   814,
     815,  1304,  1278,   423,  1330,   428,   367,  1288,  1289,   429,
    1294,  1307,   430,  1316,  1318,  1320,  1322,   300,   367,   366,
    1339,  1341,   424,   425,  1340,  1342,  1345,   431,   423,  1378,
     424,   425,  1392,   432,  1406,  1380,  1351,  1672,  1352,  1418,
    1359,  1362,  1366,  1388,  1411,   816,   367,   367,   890,   892,
    1419,  1421,  1420,   570,  1532,   424,   425,  1422,  1433,  1460,
    1434,  1436,  1442,  1458,  1471,  1467,   542,  1521,  1470,  1496,
    1501,   367,   523,   704,   926,  1506,  1523,   929,  1548,  1447,
    1528,  1524,  1530,  1549,  1487,  1558,   426,  1554,  1622,   594,
     427,  1555,  1564,  1560,   426,  1565,  1571,  1599,   427,   595,
    1566,  1570,  1573,  1637,  1574,  1657,  1590,  1639,   366,  1617,
     300,  1658,  1659,  1632,  1640,   653,  1700,  1636,  1638,   426,
    1641,  1635,  1701,   427,  1642,   673,   423,    13,  1671,  1680,
     678,  1695,   680,  1479,   995,  1696,   367,   542,  1704,  1706,
    1711,   689,  1710,  1724,   970,  1726,   137,    14,  1737,   701,
    1207,  1740,  1727,   424,   425,  1728,   596,   428,  1729,  1732,
    1733,   429,  1207,  1734,   430,   428,  1746,  1735,   542,   429,
    1753,  1332,   430,   725,  1756,    20,  1670,  1742,  1754,   431,
    1755,   300,  1757,  1762,    89,   432,  1518,   431,   186,   140,
     428,   743,  1604,   432,   429,   324,  1333,   430,  1097,  1095,
    1250,   300,   423,  1605,  1606,  1705,   366,  1607,   366,  1608,
      26,  1263,   431,  1577,  1529,  1417,  1628,   426,   432,  1715,
    1551,   427,   423,  1449,  1552,   367,  1629,    99,  1630,   424,
     425,   579,  1061,  1063,  1065,  1067,  1069,  1071,  1073,  1075,
    1077,  1079,   820,  1082,  1084,  1086,  1088,  1090,  1092,   424,
     425,   658,  1699,  1587,  1381,  1741,   659,   958,     0,   832,
       0,     0,  1141,     0,     0,   300,     0,     0,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,     0,
     542,   542,   542,   542,   542,   542,     0,  1751,   428,     0,
       0,     0,   429,   426,  1350,   430,   594,   427,     0,   542,
       0,     0,     0,     0,     0,   366,   595,     0,     0,     0,
     431,     0,     0,   426,     0,   590,   432,   427,     0,     0,
     366,     0,     0,   367,   601,   367,     0,     0,     0,   739,
     673,     0,   300,     0,   614,     0,     0,   739,     0,   914,
       0,     0,     0,     0,   624,     0,     0,     0,     0,     0,
       0,     0,  1750,     0,   642,     0,     0,     0,  1649,     0,
       0,   933,     0,   596,   428,  1649,     0,  1649,   429,     0,
       0,   430,     0,     0,  1649,     0,   672,     0,   982,   949,
       0,   949,  1760,     0,   428,     0,   431,   300,   429,  1649,
    1355,   430,   432,   366,     0,     0,     0,     0,     0,     0,
       0,   702,   703,     0,     0,     0,   431,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
     423,     0,   367,     0,     0,     0,     0,     0,     0,   729,
     730,   731,   732,   733,   738,   738,     0,   367,     0,     0,
       0,  1649,     0,     0,   982,     0,     0,   424,   425,     0,
       0,     0,     0,     0,  1719,     0,     0,     0,     0,     0,
    1721,  1060,  1062,  1064,  1066,  1068,  1070,  1072,  1074,  1076,
    1078,  1080,  1081,  1083,  1085,  1087,  1089,  1091,  1093,     0,
       0,     0,     0,     0,     0,     0,     0,  1649,  1649,     0,
       0,     0,  1104,     0,     0,     0,  1748,  1749,   300,   706,
     706,   706,     0,     0,     0,     0,     0,     0,     0,     0,
     367,   426,     0,     0,     0,   427,     0,     0,  1334,     0,
       0,     0,     0,     0,  1649,     0,  1759,     0,     0,   738,
       0,     0,     0,     0,     0,     0,  1134,     0,   858,     0,
       0,     0,     0,     0,  1145,     0,  1147,     0,     0,     0,
       0,   750,     0,     0,     0,     0,  1155,  1156,  1157,  1158,
    1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,  1170,  1171,     0,     0,     0,     0,     0,  1367,   750,
     738,   672,   428,     0,     0,     0,   429,     0,  1356,   430,
       0,     0,   915,     0,     0,     0,     0,   920,     0,  1382,
       0,     0,     0,   924,   431,  1389,   738,     0,     0,   738,
     432,     0,     0,     0,  1396,     0,     0,     0,     0,     0,
       0,     0,  1225,   936,   937,   938,   939,   940,   941,  1408,
    -349,  1230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   764,   765,     0,     0,     0,     0,  1247,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1431,     0,
       0,     0,     0,     0,     0,     0,   738,     0,     0,     0,
    1441,     0,     0,     0,     0,  1446,     0,     0,     0,  1014,
    1015,     0,   750,  1016,  1017,  1018,  1019,   423,  1021,     0,
    1024,  1025,  1026,  1028,  1029,  1030,  1031,  1032,  1033,  1035,
    1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,
     750,  1054,     0,     0,   424,   425,  1465,     0,     0,   542,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   738,     0,     0,     0,     0,   766,   767,   768,   769,
     770,     0,     0,   771,   772,   773,   774,  1491,   775,   776,
     777,   778,     0,     0,     0,     0,   779,     0,   780,   781,
       0,     0,     0,   423,   782,   783,   784,     0,     0,     0,
     785,   706,     0,     0,     0,  1507,  1508,     0,   426,  1511,
    1130,     0,   427,     0,     0,  -349,     0,     0,     0,     0,
     424,   425,     0,     0,   738,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,  -349,  1154,     0,     0,     0,
       0,     0,     0,  -349,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,   424,   425,
       0,     0,     0,     0,     0,     0,   797,   798,     0,  1354,
       0,   750,     0,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,   429,   426,  1357,   430,     0,   427,   729,
    1203,   738,  1369,  1370,     0,     0,     0,  1373,     0,   738,
       0,   431,     0,     0,     0,  1379,     0,   432,   949,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1154,
       0,  1596,   426,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   738,  1405,     0,     0,
       0,     0,   570,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,   429,
       0,  1358,   430,     0,     0,     0,     0,   716,     0,     0,
       0,     0,   750,   325,   764,   765,     0,   431,     0,   326,
       0,  1645,   706,   432,     0,   327,     0,     0,     0,     0,
    1287,     0,     0,   428,  1292,   328,     0,   429,     0,  1360,
     430,     0,     0,   329,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   670,     0,   431,     0,     0,   330,     0,
       0,   432,     0,  1461,     0,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   771,   892,     0,   774,
     738,   775,   776,   777,   778,     0,     0,     0,     0,   779,
       0,   780,   781,   423,   706,   706,   706,    62,     0,   750,
       0,   750,     0,   750,     0,   750,     0,   750,     0,   750,
     364,   750,     0,   750,     0,   750,     0,   750,     0,   750,
     424,   425,   750,   423,   750,     0,   750,    63,   750,     0,
     750,     0,   750,     0,     0,     0,     0,     0,     0,     0,
     738,     0,  1368,   750,     0,   738,     0,     0,     0,     0,
     424,   425,     0,   790,   791,   792,   793,   794,   795,   796,
    1557,   738,  1559,     0,     0,     0,     0,   738,     0,   797,
     798,     0,  1567,  1568,  1569,   365,   738,     0,     0,     0,
    1399,  1400,     0,     0,   426,   423,     0,     0,   427,  1584,
    1585,   738,     0,     0,     0,  1589,     0,     0,     0,  1154,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1609,
       0,     0,   424,   425,   426,     0,     0,  1427,   427,  1429,
     738,  1432,     0,  1619,     0,     0,     0,  1435,     0,     0,
       0,  1438,   738,     0,     0,     0,     0,   738,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,   429,
       0,  1473,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1651,  1652,     0,     0,   426,   431,   738,     0,
     427,     0,     0,   432,     0,   428,     0,     0,     0,   429,
       0,  1478,   430,   423,     0,  1668,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,  1675,   738,
    1676,     0,     0,   432,   764,   765,     0,     0,     0,     0,
     424,   425,     0,     0,     0,     0,     0,     0,     0,  1689,
       0,  1690,     0,     0,     0,     0,     0,   738,   738,     0,
       0,   738,     0,     0,     0,     0,     0,   428,     0,     0,
       0,   429,     0,  1514,   430,     0,   738,     0,     0,  1714,
       0,     0,     0,     0,     0,  1717,  1718,     0,     0,   431,
       0,     0,     0,     0,     0,   432,     0,  1541,     0,  1542,
       0,     0,   738,     0,   426,  1546,     0,     0,   427,     0,
       0,     0,  1736,     0,  1738,  1739,     0,     0,     0,     0,
       0,   738,     0,   750,     0,     0,     0,     0,  1747,   766,
     767,   768,   769,   770,     0,     0,   771,   772,   773,   774,
       0,   775,   776,   777,   778,     0,     0,     0,     0,   779,
    1576,   780,   781,     0,     0,  1579,     0,   782,     0,  1583,
       0,     0,  1586,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1595,   738,     0,   428,     0,     0,     0,   429,
       0,  1620,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
       0,     0,     0,     0,     0,     0,     0,   738,     0,   797,
     798,     0,     0,   738,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   738,     0,     0,     0,     0,     0,     0,
    1662,     0,     0,   450,     0,   738,     0,   451,   452,     3,
       0,   453,   454,   455,     0,   456,     0,   457,   458,   459,
     460,   461,     0,     0,     0,     0,     0,   462,   463,   464,
     465,   466,     0,   467,     0,     0,     0,     0,     0,     0,
     468,   469,     0,     0,   470,     0,   471,   472,     0,     0,
     473,     0,     8,   474,   475,     0,   476,   477,     0,     0,
     478,   479,  1709,     0,     0,     0,     0,   480,     0,   738,
     481,   482,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   483,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   484,   485,   486,   487,     0,     0,
       0,     0,     0,     0,  1745,     0,     0,     0,     0,     0,
     488,   489,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,   492,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   493,   494,   495,   496,   497,     0,   498,
       0,   499,   500,   501,   502,    50,   148,   503,   504,   505,
     506,   507,   508,   509,   510,    63,   511,   512,   513,     0,
       0,     0,     0,     0,     0,   514,   149,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,   517,   518,     0,    15,     0,
       0,   519,   520,     0,     0,   451,   452,     0,     0,   521,
       0,   522,     0,   523,   524,   457,   458,   459,   460,   461,
       0,     0,     0,     0,     0,   462,     0,   464,     0,     0,
       0,   467,     0,   423,     0,     0,     0,     0,     0,   469,
       0,     0,     0,     0,     0,   472,     0,     0,   473,     0,
       0,   474,     0,   943,     0,   477,     0,     0,     0,     0,
     424,   425,     0,     0,     0,   586,     0,     0,   481,   482,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     483,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   484,   485,   587,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   488,   489,
       0,     0,     0,     0,   426,     0,     0,     0,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   493,   494,   495,   496,   497,     0,   498,   944,   499,
     500,   501,   502,    50,     0,     0,   504,   505,   506,   507,
     508,   509,   510,   945,   588,   512,   513,     0,     0,     0,
       0,     0,     0,   514,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,   429,
       0,     0,   946,   517,   518,     0,    15,     0,     0,   519,
     520,     0,     0,     0,   451,   452,     0,   947,     0,   948,
       0,   523,   524,   432,   457,   458,   459,   460,   461,     0,
       0,     0,     0,     0,   462,     0,   464,     0,     0,     0,
     467,     0,   423,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     474,     0,     0,     0,   477,     0,     0,     0,     0,   424,
     425,     0,     0,     0,   586,     0,     0,   481,   482,     0,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   483,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   484,   485,   587,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   489,     0,
       0,     0,     0,   426,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     493,   494,   495,   496,   497,     0,   498,   944,   499,   500,
     501,   502,    50,     0,     0,   504,   505,   506,   507,   508,
     509,   510,   945,   588,   512,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,   429,     0,
       0,   946,   517,   518,     0,    15,     0,     0,   519,   520,
       0,     0,     0,   451,   452,     0,   947,     0,   956,     0,
     523,   524,   432,   457,   458,   459,   460,   461,     0,     0,
       0,     0,     0,   462,     0,   464,     0,     0,     0,   467,
       0,   619,     0,     0,     0,     0,     0,   469,     0,     0,
       0,     0,     0,   472,     0,     0,   473,     0,     0,   474,
       0,     0,     0,   477,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   586,     0,     0,   481,   482,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   483,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     484,   485,   587,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   488,   489,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   493,
     494,   495,   496,   497,     0,   498,     0,   499,   500,   501,
     502,    50,     0,     0,   504,   505,   506,   507,   508,   509,
     510,    63,   588,   512,   513,     0,     0,     0,     0,     0,
       0,   514,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   620,     0,     0,
     516,   517,   518,     0,    15,     0,     0,   519,   520,     0,
       0,     0,   451,   452,     0,  1202,     0,   522,     0,   523,
     524,   622,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,     0,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   486,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,   668,     0,     0,     0,     0,     0,
     490,   491,   492,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
     451,   452,     0,     0,   521,     0,   522,     0,   523,   524,
     457,   458,   459,   460,   461,     0,     0,     0,     0,     0,
     462,     0,   464,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     472,     0,     0,   473,     0,     0,   474,     0,     0,     0,
     477,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     586,     0,     0,   481,   482,     0,   331,   332,   333,     0,
     335,   336,   337,   338,   339,   483,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,   353,   354,
     355,     0,     0,   358,   359,   360,   361,   484,   485,   486,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,     0,     0,     0,     0,     0,
       0,     0,   699,     0,     0,     0,     0,     0,   490,   491,
     492,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   493,   494,   495,   496,
     497,     0,   498,     0,   499,   500,   501,   502,    50,     0,
       0,   504,   505,   506,   507,   508,   509,   510,    63,   588,
     512,   513,     0,     0,     0,     0,     0,     0,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   516,   517,   518,
       0,    15,     0,     0,   519,   520,     0,     0,   451,   452,
       0,     0,   521,     0,   522,     0,   523,   524,   457,   458,
     459,   460,   461,     0,     0,     0,     0,     0,   462,  1602,
     464,   465,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,   474,   475,     0,     0,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   586,     0,
       0,   481,   482,     0,   331,   332,   333,     0,   335,   336,
     337,   338,   339,   483,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,   353,   354,   355,     0,
       0,   358,   359,   360,   361,   484,   485,   587,  1603,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   489,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   493,   494,   495,   496,   497,     0,
     498,     0,   499,   500,   501,   502,    50,     0,     0,   504,
     505,   506,   507,   508,   509,   510,    63,   588,   512,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,   517,   518,     0,    15,
       0,     0,   519,   520,     0,     0,   451,   452,     0,     0,
     521,     0,   522,     0,   523,   524,   457,   458,   459,   460,
     461,     0,     0,     0,     0,     0,   462,     0,   464,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     469,     0,     0,     0,     0,     0,   472,     0,     0,   473,
       0,     0,   474,     0,     0,     0,   477,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   586,     0,     0,   481,
     482,     0,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   483,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,     0,   358,
     359,   360,   361,   484,   485,   486,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   488,
     489,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,   492,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   493,   494,   495,   496,   497,     0,   498,     0,
     499,   500,   501,   502,    50,     0,     0,   504,   505,   506,
     507,   508,   509,   510,    63,   588,   512,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   517,   518,     0,    15,     0,     0,
     519,   520,     0,     0,   451,   452,     0,     0,   521,     0,
     522,     0,   523,   524,   457,   458,   459,   460,   461,     0,
       0,     0,     0,     0,   462,     0,   464,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     474,     0,     0,     0,   477,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   586,     0,     0,   481,   482,   992,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   483,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   484,   485,   587,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     493,   494,   495,   496,   497,     0,   498,   944,   499,   500,
     501,   502,    50,     0,     0,   504,   505,   506,   507,   508,
     509,   510,   945,   588,   512,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,     0,    15,     0,     0,   519,   520,
       0,     0,   451,   452,     0,     0,   993,     0,   522,   994,
     523,   524,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,     0,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   486,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1055,  1056,  1057,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
       0,     0,   451,   452,   521,     0,   522,     0,   523,   524,
     734,     0,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,   735,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
       0,     0,   451,   452,   521,   623,   522,     0,   523,   524,
     734,     0,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,   735,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,   944,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
     945,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
       0,     0,   451,   452,   521,     0,   522,     0,   523,   524,
     734,     0,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,   735,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
       0,     0,   451,   452,   521,   856,   522,     0,   523,   524,
     734,     0,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,   735,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
     451,   452,     0,     0,   521,     0,   522,     0,   523,   524,
     457,   458,   459,   460,   461,     0,     0,     0,     0,     0,
     462,     0,   464,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     472,     0,     0,   473,     0,     0,   474,     0,     0,     0,
     477,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     586,     0,     0,   481,   482,  1140,   331,   332,   333,     0,
     335,   336,   337,   338,   339,   483,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,   353,   354,
     355,     0,     0,   358,   359,   360,   361,   484,   485,   587,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   493,   494,   495,   496,
     497,     0,   498,   944,   499,   500,   501,   502,    50,     0,
       0,   504,   505,   506,   507,   508,   509,   510,   945,   588,
     512,   513,     0,     0,     0,     0,     0,     0,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   516,   517,   518,
       0,    15,     0,     0,   519,   520,     0,     0,   451,   452,
       0,     0,   521,     0,   522,     0,   523,   524,   457,   458,
     459,   460,   461,     0,     0,     0,     0,     0,   462,     0,
     464,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,   474,     0,     0,     0,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   586,     0,
       0,   481,   482,     0,   331,   332,   333,     0,   335,   336,
     337,   338,   339,   483,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,   353,   354,   355,     0,
       0,   358,   359,   360,   361,   484,   485,   587,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   489,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   493,   494,   495,   496,   497,     0,
     498,     0,   499,   500,   501,   502,    50,     0,     0,   504,
     505,   506,   507,   508,   509,   510,    63,   588,   512,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,   517,   518,     0,    15,
       0,     0,   519,   520,     0,     0,   451,   452,     0,     0,
     521,   623,   522,     0,   523,   524,   457,   458,   459,   460,
     461,     0,     0,     0,     0,     0,   462,     0,   464,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     469,     0,     0,     0,     0,     0,   472,     0,     0,   473,
       0,     0,   474,     0,     0,     0,   477,     0,     0,     0,
       0,     0,   677,     0,     0,     0,   586,     0,     0,   481,
     482,     0,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   483,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,     0,   358,
     359,   360,   361,   484,   485,   587,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   488,
     489,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   493,   494,   495,   496,   497,     0,   498,     0,
     499,   500,   501,   502,    50,     0,     0,   504,   505,   506,
     507,   508,   509,   510,    63,   588,   512,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   517,   518,     0,    15,     0,     0,
     519,   520,     0,     0,   451,   452,     0,     0,   521,     0,
     522,     0,   523,   524,   457,   458,   459,   460,   461,     0,
       0,     0,     0,     0,   462,     0,   464,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     474,     0,     0,     0,   477,     0,     0,   688,     0,     0,
       0,     0,     0,     0,   586,     0,     0,   481,   482,     0,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   483,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   484,   485,   587,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     493,   494,   495,   496,   497,     0,   498,     0,   499,   500,
     501,   502,    50,     0,     0,   504,   505,   506,   507,   508,
     509,   510,    63,   588,   512,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,     0,    15,     0,     0,   519,   520,
       0,     0,     0,     0,   451,   452,   521,     0,   522,     0,
     523,   524,   720,     0,   457,   458,   459,   460,   461,     0,
       0,     0,     0,     0,   462,     0,   464,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     474,     0,     0,     0,   477,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   586,     0,     0,   481,   482,     0,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   483,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   484,   485,   587,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     493,   494,   495,   496,   497,     0,   498,     0,   499,   500,
     501,   502,    50,     0,     0,   504,   505,   506,   507,   508,
     509,   510,    63,   588,   512,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,     0,    15,     0,     0,   519,   520,
       0,     0,   451,   452,     0,     0,   521,     0,   522,     0,
     523,   524,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,     0,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   724,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
       0,     0,   451,   452,   521,     0,   522,     0,   523,   524,
     745,     0,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,     0,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
     451,   452,     0,     0,   521,     0,   522,     0,   523,   524,
     457,   458,   459,   460,   461,     0,     0,  1034,     0,     0,
     462,     0,   464,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     472,     0,     0,   473,     0,     0,   474,     0,     0,     0,
     477,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     586,     0,     0,   481,   482,     0,   331,   332,   333,     0,
     335,   336,   337,   338,   339,   483,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,   353,   354,
     355,     0,     0,   358,   359,   360,   361,   484,   485,   587,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   493,   494,   495,   496,
     497,     0,   498,     0,   499,   500,   501,   502,    50,     0,
       0,   504,   505,   506,   507,   508,   509,   510,    63,   588,
     512,   513,     0,     0,     0,     0,     0,     0,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   516,   517,   518,
       0,    15,     0,     0,   519,   520,     0,     0,   451,   452,
       0,     0,   521,     0,   522,     0,   523,   524,   457,   458,
     459,   460,   461,     0,     0,     0,     0,     0,   462,     0,
     464,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,   474,     0,     0,     0,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   586,     0,
       0,   481,   482,     0,   331,   332,   333,     0,   335,   336,
     337,   338,   339,   483,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,   353,   354,   355,     0,
       0,   358,   359,   360,   361,   484,   485,   587,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   489,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   493,   494,   495,   496,   497,     0,
     498,     0,   499,   500,   501,   502,    50,     0,     0,   504,
     505,   506,   507,   508,   509,   510,    63,   588,   512,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,   517,   518,     0,    15,
       0,     0,   519,   520,     0,     0,   451,   452,     0,     0,
     521,     0,   522,  1094,   523,   524,   457,   458,   459,   460,
     461,     0,     0,     0,     0,     0,   462,     0,   464,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     469,     0,     0,     0,     0,     0,   472,     0,     0,   473,
       0,     0,   474,     0,     0,     0,   477,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   586,     0,     0,   481,
     482,     0,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   483,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,     0,   358,
     359,   360,   361,   484,   485,   587,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   488,
     489,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   493,   494,   495,   496,   497,     0,   498,     0,
     499,   500,   501,   502,    50,     0,     0,   504,   505,   506,
     507,   508,   509,   510,    63,   588,   512,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1144,     0,   516,   517,   518,     0,    15,     0,     0,
     519,   520,     0,     0,   451,   452,     0,     0,   521,     0,
     522,     0,   523,   524,   457,   458,   459,   460,   461,     0,
       0,     0,     0,     0,   462,     0,   464,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     474,     0,     0,     0,   477,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   586,     0,     0,   481,   482,     0,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   483,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   484,   485,   587,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     493,   494,   495,   496,   497,     0,   498,     0,   499,   500,
     501,   502,    50,     0,     0,   504,   505,   506,   507,   508,
     509,   510,    63,   588,   512,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,     0,    15,     0,     0,   519,   520,
       0,     0,   451,   452,     0,     0,   521,     0,   522,  1204,
     523,   524,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,     0,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
     451,   452,     0,     0,   521,     0,   522,  1219,   523,   524,
     457,   458,   459,   460,   461,     0,     0,     0,     0,     0,
     462,     0,   464,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     472,     0,     0,   473,     0,     0,   474,     0,     0,     0,
     477,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     586,     0,     0,   481,   482,     0,   331,   332,   333,     0,
     335,   336,   337,   338,   339,   483,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,   353,   354,
     355,     0,     0,   358,   359,   360,   361,   484,   485,   587,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   493,   494,   495,   496,
     497,     0,   498,     0,   499,   500,   501,   502,    50,     0,
       0,   504,   505,   506,   507,   508,   509,   510,    63,   588,
     512,   513,     0,     0,     0,     0,     0,     0,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   516,   517,   518,
       0,    15,     0,     0,   519,   520,     0,     0,   451,   452,
       0,     0,   521,     0,   522,  1430,   523,   524,   457,   458,
     459,   460,   461,     0,     0,     0,     0,     0,   462,     0,
     464,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,   474,     0,     0,     0,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   586,     0,
       0,   481,   482,     0,   331,   332,   333,     0,   335,   336,
     337,   338,   339,   483,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,   353,   354,   355,     0,
       0,   358,   359,   360,   361,   484,   485,   587,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   489,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   493,   494,   495,   496,   497,     0,
     498,     0,   499,   500,   501,   502,    50,     0,     0,   504,
     505,   506,   507,   508,   509,   510,    63,   588,   512,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,   517,   518,     0,    15,
       0,     0,   519,   520,     0,     0,   451,   452,     0,     0,
    1439,     0,   522,  1440,   523,   524,   457,   458,   459,   460,
     461,     0,     0,     0,     0,     0,   462,     0,   464,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     469,     0,     0,     0,     0,     0,   472,     0,     0,   473,
       0,     0,   474,     0,     0,     0,   477,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   586,     0,     0,   481,
     482,     0,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   483,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,     0,   358,
     359,   360,   361,   484,   485,   587,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   488,
     489,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   493,   494,   495,   496,   497,     0,   498,     0,
     499,   500,   501,   502,    50,     0,     0,   504,   505,   506,
     507,   508,   509,   510,    63,   588,   512,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   517,   518,     0,    15,     0,     0,
     519,   520,     0,     0,   451,   452,     0,     0,   521,     0,
     522,  1445,   523,   524,   457,   458,   459,   460,   461,     0,
       0,     0,     0,     0,   462,     0,   464,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     474,     0,     0,     0,   477,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   586,     0,     0,   481,   482,     0,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   483,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   484,   485,   587,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     493,   494,   495,   496,   497,     0,   498,     0,   499,   500,
     501,   502,    50,     0,     0,   504,   505,   506,   507,   508,
     509,   510,    63,   588,   512,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,     0,    15,     0,     0,   519,   520,
       0,     0,   451,   452,     0,     0,   521,     0,   522,  1490,
     523,   524,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,     0,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
     517,   518,     0,    15,     0,     0,   519,   520,     0,     0,
     451,   452,     0,     0,   521,     0,   522,  1575,   523,   524,
     457,   458,   459,   460,   461,     0,     0,     0,     0,     0,
     462,     0,   464,     0,     0,     0,   467,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     472,     0,     0,   473,     0,     0,   474,     0,     0,     0,
     477,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     586,     0,     0,   481,   482,     0,   331,   332,   333,     0,
     335,   336,   337,   338,   339,   483,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,     0,   353,   354,
     355,     0,     0,   358,   359,   360,   361,   484,   485,   587,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,     0,     0,     0,     0,     0,
       0,     0,  1618,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   493,   494,   495,   496,
     497,     0,   498,     0,   499,   500,   501,   502,    50,     0,
       0,   504,   505,   506,   507,   508,   509,   510,    63,   588,
     512,   513,     0,     0,     0,     0,     0,     0,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   516,   517,   518,
       0,    15,     0,     0,   519,   520,     0,     0,   451,   452,
       0,     0,   521,     0,   522,     0,   523,   524,   457,   458,
     459,   460,   461,     0,     0,     0,     0,     0,   462,     0,
     464,     0,     0,     0,   467,     0,     0,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,   472,     0,
       0,   473,     0,     0,   474,     0,     0,     0,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   586,     0,
       0,   481,   482,     0,   331,   332,   333,     0,   335,   336,
     337,   338,   339,   483,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,   353,   354,   355,     0,
       0,   358,   359,   360,   361,   484,   485,   587,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   489,     0,     0,     0,     0,     0,     0,     0,
    1666,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   493,   494,   495,   496,   497,     0,
     498,     0,   499,   500,   501,   502,    50,     0,     0,   504,
     505,   506,   507,   508,   509,   510,    63,   588,   512,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   516,   517,   518,     0,    15,
       0,     0,   519,   520,     0,     0,   451,   452,     0,     0,
     521,     0,   522,     0,   523,   524,   457,   458,   459,   460,
     461,     0,     0,     0,     0,     0,   462,     0,   464,     0,
       0,     0,   467,     0,     0,     0,     0,     0,     0,     0,
     469,     0,     0,     0,     0,     0,   472,     0,     0,   473,
       0,     0,   474,     0,     0,     0,   477,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   586,     0,     0,   481,
     482,     0,   331,   332,   333,     0,   335,   336,   337,   338,
     339,   483,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,   353,   354,   355,     0,     0,   358,
     359,   360,   361,   484,   485,   587,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   488,
     489,     0,     0,     0,     0,     0,     0,     0,  1667,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   493,   494,   495,   496,   497,     0,   498,     0,
     499,   500,   501,   502,    50,     0,     0,   504,   505,   506,
     507,   508,   509,   510,    63,   588,   512,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   517,   518,     0,    15,     0,     0,
     519,   520,     0,     0,   451,   452,     0,     0,   521,     0,
     522,     0,   523,   524,   457,   458,   459,   460,   461,     0,
       0,     0,     0,     0,   462,     0,   464,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   472,     0,     0,   473,     0,     0,
     474,     0,     0,     0,   477,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   586,     0,     0,   481,   482,     0,
     331,   332,   333,     0,   335,   336,   337,   338,   339,   483,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   484,   485,   587,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     493,   494,   495,   496,   497,     0,   498,     0,   499,   500,
     501,   502,    50,     0,     0,   504,   505,   506,   507,   508,
     509,   510,    63,   588,   512,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,     0,    15,     0,     0,   519,   520,
       0,     0,   451,   452,     0,     0,   521,     0,   522,     0,
     523,   524,   457,   458,   459,   460,   461,     0,     0,     0,
       0,     0,   462,     0,   464,     0,     0,     0,   467,     0,
       0,     0,     0,     0,     0,     0,   469,     0,     0,     0,
       0,     0,   472,     0,     0,   473,     0,     0,   474,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,     0,   481,   482,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   483,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   484,
     485,   587,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   498,     0,   499,   500,   501,   502,
      50,     0,     0,   504,   505,   506,   507,   508,   509,   510,
      63,   588,   512,   513,     0,     0,   718,     0,     0,     0,
     514,     0,   325,     0,     0,     0,     0,     0,   326,     0,
       0,     0,     0,     0,   327,     0,     0,     0,     0,   516,
     517,   518,     0,    15,   328,     0,   519,   520,     0,     0,
       0,     0,   329,     0,  1413,     0,   522,     0,   523,   524,
       0,     0,     0,     0,     0,     0,     0,   330,     0,     0,
       0,     0,     0,     0,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,     0,     0,     0,     0,
       0,   326,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,     0,     0,     0,     0,    62,   328,     0,     0,
       0,     0,     0,     0,     0,   329,     0,     0,     0,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     330,     0,     0,     0,     0,     0,    63,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,     0,     0,     0,   325,     0,
       0,     0,     0,     0,   326,     0,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,     0,     0,    62,
     328,     0,     0,     0,     0,     0,     0,     0,   329,     0,
       0,     0,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   330,     0,     0,     0,     0,     0,    63,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,   626,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   364,     0,     0,     0,     0,
       0,     0,     0,     0,    13,     0,     0,     0,     0,   325,
       0,     0,   631,     0,     0,   326,     0,     0,     0,     0,
       0,   327,     0,     0,    14,     0,     0,     0,     0,     0,
       0,   328,   632,     0,     0,     0,     0,     0,     0,   329,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   330,     0,     0,     0,     0,     0,
     365,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   764,   765,     0,     0,     0,
       0,     0,   325,     0,     0,     0,     0,     0,   326,     0,
       0,     0,     0,     0,   327,     0,     0,     0,     0,     0,
       0,     0,     0,    62,   328,     0,     0,     0,     0,     0,
       0,     0,   329,     0,     0,     0,   364,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,     0,     0,
       0,     0,     0,    63,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   771,   772,   773,
     774,   365,   775,   776,   777,   778,     0,     0,     0,     0,
     779,     0,   780,   781,   764,   765,     0,     0,   782,   783,
     784,     0,     0,     0,   785,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   764,
     765,     0,     0,     0,     0,     0,   631,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,   827,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   771,   772,   773,   774,
       0,   775,   776,   777,   778,     0,     0,     0,     0,   779,
       0,   780,   781,     0,     0,     0,     0,   782,   783,   784,
       0,     0,     0,   785,   766,   767,   768,   769,   770,     0,
       0,   771,   772,   773,   774,     0,   775,   776,   777,   778,
     764,   765,     0,     0,   779,     0,   780,   781,     0,     0,
       0,     0,   782,   783,   784,     0,     0,     0,   785,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,     0,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     764,   765,     0,     0,     0,     0,     0,     0,     0,   797,
     798,     0,     0,   837,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   786,     0,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   797,   798,     0,     0,   853,     0,
       0,     0,     0,     0,     0,   766,   767,   768,   769,   770,
       0,     0,   771,   772,   773,   774,     0,   775,   776,   777,
     778,     0,     0,     0,     0,   779,     0,   780,   781,     0,
       0,     0,     0,   782,   783,   784,     0,     0,     0,   785,
       0,     0,     0,     0,     0,   766,   767,   768,   769,   770,
       0,     0,   771,   772,   773,   774,     0,   775,   776,   777,
     778,   764,   765,     0,     0,   779,     0,   780,   781,     0,
       0,     0,     0,   782,   783,   784,     0,     0,     0,   785,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,   764,   765,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,   866,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1176,
       0,     0,     0,     0,     0,     0,   766,   767,   768,   769,
     770,     0,     0,   771,   772,   773,   774,     0,   775,   776,
     777,   778,     0,     0,     0,     0,   779,     0,   780,   781,
       0,     0,     0,     0,   782,   783,   784,     0,     0,     0,
     785,   766,   767,   768,   769,   770,     0,     0,   771,   772,
     773,   774,     0,   775,   776,   777,   778,   764,   765,     0,
       0,   779,     0,   780,   781,     0,     0,     0,     0,   782,
     783,   784,     0,     0,     0,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   764,   765,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
    1180,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,     0,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,  1182,     0,     0,     0,     0,
       0,     0,   766,   767,   768,   769,   770,     0,     0,   771,
     772,   773,   774,     0,   775,   776,   777,   778,     0,     0,
       0,     0,   779,     0,   780,   781,     0,     0,     0,     0,
     782,   783,   784,     0,     0,     0,   785,     0,     0,     0,
       0,     0,   766,   767,   768,   769,   770,     0,     0,   771,
     772,   773,   774,     0,   775,   776,   777,   778,   764,   765,
       0,     0,   779,     0,   780,   781,     0,     0,     0,     0,
     782,   783,   784,     0,     0,     0,   785,     0,     0,     0,
       0,   786,     0,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,   764,   765,     0,     0,     0,     0,     0,
       0,     0,   797,   798,     0,     0,  1191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   786,     0,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   797,   798,     0,     0,  1192,     0,     0,     0,
       0,     0,     0,   766,   767,   768,   769,   770,     0,     0,
     771,   772,   773,   774,     0,   775,   776,   777,   778,     0,
       0,     0,     0,   779,     0,   780,   781,     0,     0,     0,
       0,   782,   783,   784,     0,     0,     0,   785,   766,   767,
     768,   769,   770,     0,     0,   771,   772,   773,   774,     0,
     775,   776,   777,   778,   764,   765,     0,     0,   779,     0,
     780,   781,     0,     0,     0,     0,   782,   783,   784,     0,
       0,     0,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,     0,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,   764,   765,     0,     0,     0,     0,
       0,     0,     0,   797,   798,     0,     0,  1193,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,     0,   787,
     788,   789,   790,   791,   792,   793,   794,   795,   796,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   797,   798,
       0,     0,  1194,     0,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   771,   772,   773,   774,
       0,   775,   776,   777,   778,     0,     0,     0,     0,   779,
       0,   780,   781,     0,     0,     0,     0,   782,   783,   784,
       0,     0,     0,   785,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   771,   772,   773,   774,
       0,   775,   776,   777,   778,   764,   765,     0,     0,   779,
       0,   780,   781,     0,     0,     0,     0,   782,   783,   784,
       0,     0,     0,   785,     0,     0,     0,     0,   786,     0,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     764,   765,     0,     0,     0,     0,     0,     0,     0,   797,
     798,     0,     0,  1195,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,     0,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   797,
     798,     0,     0,  1196,     0,     0,     0,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   771,   772,   773,
     774,     0,   775,   776,   777,   778,     0,     0,     0,     0,
     779,     0,   780,   781,     0,     0,     0,     0,   782,   783,
     784,     0,     0,     0,   785,   766,   767,   768,   769,   770,
       0,     0,   771,   772,   773,   774,     0,   775,   776,   777,
     778,   764,   765,     0,     0,   779,     0,   780,   781,     0,
       0,     0,     0,   782,   783,   784,     0,     0,     0,   785,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,   764,   765,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1329,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,     0,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,  1493,
       0,     0,     0,     0,     0,     0,   766,   767,   768,   769,
     770,     0,     0,   771,   772,   773,   774,     0,   775,   776,
     777,   778,     0,     0,     0,     0,   779,     0,   780,   781,
       0,     0,     0,     0,   782,   783,   784,     0,     0,     0,
     785,     0,     0,     0,     0,     0,   766,   767,   768,   769,
     770,     0,     0,   771,   772,   773,   774,     0,   775,   776,
     777,   778,   764,   765,     0,     0,   779,     0,   780,   781,
       0,     0,     0,     0,   782,   783,   784,     0,     0,     0,
     785,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   764,   765,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
    1512,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   798,     0,     0,
    1513,     0,     0,     0,     0,     0,     0,   766,   767,   768,
     769,   770,     0,     0,   771,   772,   773,   774,     0,   775,
     776,   777,   778,     0,     0,     0,     0,   779,     0,   780,
     781,     0,     0,     0,     0,   782,   783,   784,     0,     0,
       0,   785,   766,   767,   768,   769,   770,     0,     0,   771,
     772,   773,   774,     0,   775,   776,   777,   778,   764,   765,
       0,     0,   779,     0,   780,   781,     0,     0,     0,     0,
     782,   783,   784,     0,     0,     0,   785,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   786,     0,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,   764,   765,
       0,     0,     0,     0,     0,     0,     0,   797,   798,     0,
       0,  1535,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   786,     0,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   797,   798,     0,     0,  1537,     0,     0,     0,
       0,     0,     0,   766,   767,   768,   769,   770,     0,     0,
     771,   772,   773,   774,     0,   775,   776,   777,   778,     0,
       0,     0,     0,   779,     0,   780,   781,     0,     0,     0,
       0,   782,   783,   784,     0,     0,     0,   785,     0,     0,
       0,     0,     0,   766,   767,   768,   769,   770,     0,     0,
     771,   772,   773,   774,     0,   775,   776,   777,   778,   764,
     765,     0,     0,   779,     0,   780,   781,     0,     0,     0,
       0,   782,   783,   784,     0,     0,     0,   785,     0,     0,
       0,     0,   786,     0,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,   764,   765,     0,     0,     0,     0,
       0,     0,     0,   797,   798,     0,     0,  1539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,     0,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   797,   798,     0,     0,  1543,     0,     0,
       0,     0,     0,     0,   766,   767,   768,   769,   770,     0,
       0,   771,   772,   773,   774,     0,   775,   776,   777,   778,
       0,     0,     0,     0,   779,     0,   780,   781,     0,     0,
       0,     0,   782,   783,   784,     0,     0,     0,   785,   766,
     767,   768,   769,   770,     0,     0,   771,   772,   773,   774,
       0,   775,   776,   777,   778,   764,   765,     0,     0,   779,
       0,   780,   781,     0,     0,     0,     0,   782,   783,   784,
       0,     0,     0,   785,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   786,     0,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,   764,   765,     0,     0,     0,
       0,     0,     0,     0,   797,   798,     0,     0,  1623,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,     0,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   797,
     798,     0,     0,  1624,     0,     0,     0,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   771,   772,   773,
     774,     0,   775,   776,   777,   778,     0,     0,     0,     0,
     779,     0,   780,   781,     0,     0,     0,     0,   782,   783,
     784,     0,     0,     0,   785,     0,     0,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   771,   772,   773,
     774,     0,   775,   776,   777,   778,   764,   765,     0,     0,
     779,     0,   780,   781,     0,     0,     0,     0,   782,   783,
     784,     0,     0,     0,   785,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,   764,   765,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1626,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
       0,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,  1648,     0,     0,     0,     0,     0,
       0,   766,   767,   768,   769,   770,     0,     0,   771,   772,
     773,   774,     0,   775,   776,   777,   778,     0,     0,     0,
       0,   779,     0,   780,   781,     0,     0,     0,     0,   782,
     783,   784,     0,     0,     0,   785,   766,   767,   768,   769,
     770,     0,     0,   771,   772,   773,   774,     0,   775,   776,
     777,   778,   764,   765,     0,     0,   779,     0,   780,   781,
       0,     0,     0,     0,   782,   783,   784,     0,     0,     0,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,     0,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,   764,   765,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,  1653,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,     0,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   797,   798,   857,     0,
       0,     0,     0,     0,     0,     0,     0,   766,   767,   768,
     769,   770,     0,     0,   771,   772,   773,   774,     0,   775,
     776,   777,   778,     0,     0,     0,     0,   779,     0,   780,
     781,     0,     0,     0,     0,   782,   783,   784,     0,     0,
       0,   785,     0,     0,     0,     0,     0,   766,   767,   768,
     769,   770,     0,     0,   771,   772,   773,   774,     0,   775,
     776,   777,   778,   764,   765,     0,     0,   779,     0,   780,
     781,     0,     0,     0,     0,   782,   783,   784,     0,     0,
       0,   785,     0,     0,     0,     0,   786,     0,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,   764,   765,
       0,     0,     0,     0,     0,     0,     0,   797,   798,  1127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   786,     0,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   797,   798,  1279,
       0,     0,     0,     0,     0,     0,     0,     0,   766,   767,
     768,   769,   770,     0,     0,   771,   772,   773,   774,     0,
     775,   776,   777,   778,     0,     0,     0,     0,   779,     0,
     780,   781,     0,     0,     0,     0,   782,   783,   784,     0,
       0,     0,   785,   766,   767,   768,   769,   770,     0,     0,
     771,   772,   773,   774,     0,   775,   776,   777,   778,   764,
     765,     0,     0,   779,     0,   780,   781,     0,     0,     0,
       0,   782,   783,   784,     0,     0,     0,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,     0,   787,
     788,   789,   790,   791,   792,   793,   794,   795,   796,     0,
       0,     0,   245,   246,     0,     0,     0,     0,   797,   798,
    1295,     0,     0,     0,     0,     0,     0,     0,     0,   247,
       0,     0,   786,     0,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   796,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   797,   798,  1437,     0,     0,     0,     0,
       0,     0,     0,     0,   766,   767,   768,   769,   770,     0,
       0,   771,   772,   773,   774,     0,   775,   776,   777,   778,
       0,     0,     0,     0,   779,     0,   780,   781,     0,     0,
       0,     0,   782,   783,   784,     0,     0,     0,   785,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   764,   765,   266,
     267,   268,     0,     0,     0,     0,     0,     0,   269,   270,
     271,   272,   273,     0,     0,   274,   275,   276,   277,   278,
     279,   280,     0,   786,     0,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,   764,   765,     0,     0,     0,
       0,     0,     0,     0,   797,   798,  1443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,     0,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,     0,     0,
     292,   293,     0,     0,     0,     0,     0,   294,   295,     0,
       0,     0,   766,   767,   768,   769,   770,     0,     0,   771,
     772,   773,   774,     0,   775,   776,   777,   778,     0,     0,
       0,     0,   779,     0,   780,   781,     0,     0,     0,     0,
     782,   783,   784,     0,     0,     0,   785,     0,     0,     0,
     766,   767,   768,   769,   770,     0,     0,   771,   772,   773,
     774,    13,   775,   776,   777,   778,   764,   765,     0,     0,
     779,     0,   780,   781,     0,     0,     0,     0,   782,   783,
     784,    14,     0,     0,   785,     0,     0,     0,     0,     0,
       0,   786,     0,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,   764,   765,     0,     0,     0,     0,     0,
       0,     0,   797,   798,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
    1284,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   766,   767,   768,   769,   770,     0,     0,   771,   772,
     773,   774,     0,   775,   776,   777,   778,     0,     0,     0,
       0,   779,     0,   780,   781,     0,     0,     0,     0,   782,
     783,   784,     0,     0,     0,   785,     0,     0,   766,   767,
     768,   769,   770,     0,     0,   771,   772,   773,   774,     0,
     775,   776,   777,   778,   764,   765,     0,     0,   779,     0,
     780,   781,     0,     0,     0,     0,   782,   783,   784,     0,
       0,     0,  -787,     0,     0,     0,     0,     0,     0,     0,
     786,     0,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,   764,   765,     0,     0,     0,     0,     0,     0,
       0,   797,   798,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,     0,   787,
     788,   789,   790,   791,   792,   793,   794,   795,   796,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   797,   798,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   766,
     767,   768,   769,   770,     0,     0,   771,   772,   773,   774,
       0,   775,   776,   777,   778,     0,     0,     0,     0,   779,
       0,   780,   781,     0,     0,     0,     0,   782,   783,   784,
       0,     0,     0,     0,     0,     0,     0,   766,   767,   768,
     769,   770,     0,     0,   771,   772,   773,   774,     0,   775,
     776,   777,   778,   764,   765,     0,     0,   779,     0,   780,
     781,     0,     0,     0,     0,   782,     0,   784,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,     0,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     764,   765,     0,     0,     0,     0,     0,     0,     0,   797,
     798,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   787,   788,
     789,   790,   791,   792,   793,   794,   795,   796,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   797,   798,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   766,   767,
     768,   769,   770,     0,     0,   771,   772,   773,   774,     0,
     775,   776,   777,   778,     0,     0,     0,     0,   779,     0,
     780,   781,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   766,   767,   768,   769,   770,
       0,     0,   771,   772,   773,   774,     0,   775,   776,   777,
     778,   764,   765,     0,     0,   779,     0,   780,   781,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   764,   765,     0,     0,   787,
     788,   789,   790,   791,   792,   793,   794,   795,   796,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   797,   798,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   788,   789,   790,
     791,   792,   793,   794,   795,   796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   797,   798,     0,     0,     0,
       0,     0,  1006,     0,     0,     0,   766,   767,   768,   769,
     770,     0,     0,   771,   772,   773,   774,     0,   775,   776,
     777,   778,     0,     0,     0,     0,   779,     0,   780,   781,
     766,   767,   768,   769,   770,     0,     0,   771,   772,   773,
     774,     0,   775,   776,   777,   778,     0,     0,     0,     0,
     779,     0,   780,   781,   331,   332,   333,     0,   335,   336,
     337,   338,   339,   483,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,   353,   354,   355,     0,
       0,   358,   359,   360,   361,     0,     0,     0,     0,   789,
     790,   791,   792,   793,   794,   795,   796,     0,     0,     0,
       0,  1010,     0,     0,     0,     0,   797,   798,     0,     0,
       0,     0,     0,     0,   790,   791,   792,   793,   794,   795,
     796,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     797,   798,  1280,     0,     0,     0,     0,     0,     0,     0,
       0,  1007,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   331,   332,   333,  1008,   335,   336,   337,
     338,   339,   483,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   331,   332,   333,     0,   335,   336,
     337,   338,   339,   483,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,   353,   354,   355,     0,
       0,   358,   359,   360,   361,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   483,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
    1011,   195,   358,   359,   360,   361,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1012,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1281,     0,  1046,  1047,     0,     0,   196,     0,   197,
       0,   198,   199,   200,   201,   202,  1282,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,     0,   214,
     215,   216,  1048,     0,   217,   218,   219,   220,   894,   895,
     896,   897,   898,   899,   900,   901,     0,  1049,     0,     0,
       0,   902,   903,     0,   221,   222,     0,   904,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,     0,     0,
     905,   906,     0,     0,     0,     0,     0,   907,   908,   909,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1050,  1051,     0,     0,     0,    13,     0,   894,   895,   896,
     897,   898,   899,   900,   901,     0,     0,     0,     0,   223,
     902,   903,     0,     0,     0,    14,   904,     0,     0,     0,
       0,  -379,     0,     0,   910,     0,   810,     0,     0,   905,
     906,     0,     0,     0,     0,     0,   907,   908,   909,     0,
     894,   895,   896,   897,   898,   899,   900,   901,     0,     0,
       0,   523,   704,   902,   903,     0,     0,     0,     0,   904,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   810,
       0,     0,   905,   906,     0,     0,     0,     0,     0,   907,
     908,   909,     0,   910,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   704,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   910,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   704
};

static const yytype_int16 yycheck[] =
{
       1,   231,   151,     7,   647,   502,   391,   593,   676,   241,
     225,   616,   391,   542,   739,    15,    16,   438,   173,    20,
     437,   559,   607,   748,   609,   951,   611,   654,  1208,   491,
     492,   957,   719,   399,   935,  1244,   881,   437,   438,   928,
       5,   463,    86,   930,     8,   583,    33,    53,  1416,   405,
      20,   717,    20,   719,   109,   844,   166,   846,     4,   848,
    1627,   129,   130,   126,    22,   487,   129,   130,    68,    69,
      70,    57,     5,     6,    46,    20,    33,    63,   434,   143,
      62,   311,    20,    12,   463,  1598,    20,   753,   127,   716,
      20,   718,    25,   720,    23,    24,    20,    98,    31,    57,
      20,   158,    33,    60,    61,   165,    20,   734,   487,    20,
     110,   111,   112,   113,   127,  1682,   173,    34,   745,   194,
     143,    20,    19,    20,   137,   129,   144,   145,   146,    15,
      16,   160,   196,   193,   106,    68,    69,   102,   103,   173,
      15,    16,   152,   373,   500,   501,    63,   992,   216,   217,
     173,  1664,    53,   216,   217,   194,  1524,   389,   197,   165,
     315,   391,   219,   173,   196,   890,   170,   124,   706,   102,
     103,   128,    12,    53,   173,   597,   152,   232,   164,   185,
     200,   194,   193,    23,    24,   219,   190,   191,   192,   233,
     107,   193,  1372,   222,     7,   127,   219,   173,   184,   219,
     164,   926,   220,   152,   929,   137,   173,    21,    22,   219,
     143,   160,   199,   193,   160,   881,   218,    97,   597,   136,
     184,   225,   221,   193,   173,   193,   143,   160,   161,   162,
     163,    21,    22,   234,   634,   200,   165,    50,   195,   639,
     173,   241,   199,   219,   393,   202,   668,   205,   193,   143,
     183,   466,   879,   217,   219,   193,   173,   158,   216,   193,
     217,  1150,   194,   193,  1151,   480,   223,   200,   199,   193,
     219,   996,   173,   193,   318,   185,   173,   699,  1204,   193,
     166,   166,   193,   193,   217,   171,   217,   173,  1189,   668,
     176,   166,   223,   210,   193,  1140,   171,   193,   173,   173,
     762,   176,   219,   165,   118,   119,   310,  1516,   155,   313,
     163,  1484,   126,   160,  1494,   129,   130,   131,   216,   675,
     699,   127,   136,   185,   220,   165,   992,   993,   118,   119,
     183,   219,   155,   219,    21,    22,   126,   160,   128,   129,
     130,   131,   698,   196,   971,   972,   136,   974,   158,   579,
    1251,   127,   193,   583,     5,     6,   143,     8,   241,   177,
     222,   137,   127,   173,   163,   365,   598,   599,   600,   193,
     602,   603,   137,   220,   606,   222,   608,   218,   610,   188,
     189,  1554,  1555,   142,   183,    36,   173,   193,   194,   389,
     196,  1600,   193,   199,   760,   395,   220,   173,   193,   222,
    1573,  1574,   216,   217,   163,   405,  1307,   637,   830,   218,
     220,   641,   202,   203,   204,   205,   206,   218,   194,  1004,
     219,   173,   193,   199,   183,   220,   216,   217,   173,   194,
     127,   118,   119,   173,   434,   584,   836,   437,   653,   126,
     137,   128,   129,   130,   131,  1371,   143,   218,   127,   136,
     152,   830,   852,   991,   127,   193,   107,  1383,   137,   459,
     460,   132,   466,   173,   137,   680,   963,  1640,  1641,   193,
     826,   173,   165,   163,  1140,   476,   480,    33,   163,   196,
     193,   943,   220,  1272,   201,   197,   490,   491,   492,   201,
    1133,   988,   185,   183,   165,   188,   220,   194,   183,  1634,
     500,   501,   219,   193,    60,    61,   389,   220,   196,   143,
     725,  1646,   912,   201,   185,   194,   746,  1204,   152,   875,
     876,   194,   405,   157,   152,   218,   527,   528,   743,   216,
     217,   219,  1219,   889,     5,     6,   221,   222,  1204,   173,
    1586,  1207,  1588,   143,   545,   173,   547,   548,   549,  1595,
     551,   434,   152,  1219,   437,   165,  1691,  1692,   165,   165,
     165,   562,   918,   919,  1610,   921,   200,   173,   124,   925,
     193,   927,   128,   173,   184,   185,  1365,   187,   185,   185,
     185,   185,    21,    22,   165,   173,   193,   193,   193,   193,
     197,   165,   593,  1055,  1056,  1057,   201,   220,   598,   599,
     600,   165,   602,   603,   185,  1190,   606,  1234,   608,   165,
     610,   185,   612,  1402,   165,   165,  1662,   500,   501,   193,
     201,   185,   854,   158,   165,   185,   173,   201,   860,   185,
     194,   863,   193,   193,   185,   185,   165,   193,   173,   195,
     872,   193,   193,   199,   185,   201,   202,   177,   880,   653,
     201,   201,   193,   885,   193,  1240,   193,   165,   165,   220,
     660,   217,  1708,  1709,    68,    69,    70,   223,   220,   824,
     193,   820,   173,   174,   175,   675,   680,   185,   185,   118,
     119,   220,    57,   220,  1352,   193,   193,   126,   193,   128,
     129,   130,   131,   201,   201,   218,   911,   136,   698,  1745,
     196,   193,  1123,   199,  1121,   206,   110,   111,   112,   113,
     144,   193,   146,   218,   165,   598,   599,   600,   165,   602,
     603,   725,   723,   606,  1124,   608,   193,   610,   220,   612,
       0,  1520,  1375,   965,   185,   967,   218,     7,   185,   743,
     740,   164,   193,   165,   158,   749,   193,  1413,   165,   965,
     201,   967,   197,   220,   201,   165,   201,   193,   762,   173,
      30,   184,    32,   185,    34,   204,   205,   206,   185,   193,
      40,   193,   193,  1439,   193,   185,   193,   216,   217,   201,
      50,   193,   218,   193,   201,   165,    56,   163,  1188,   196,
     196,   201,   675,  1149,   201,   201,   220,   165,   144,   220,
     164,   220,   164,   194,   184,   185,   186,   183,   220,   173,
      80,   173,   219,   219,   165,   698,    47,   185,  1174,    75,
     184,   821,   184,    79,   106,  1410,   826,   200,  1228,  1229,
     200,   832,   102,   103,   185,  1364,    67,    93,    94,   200,
     200,   842,    98,    99,   100,   101,   219,    35,   185,   219,
    1493,   158,   158,  1458,   854,   158,   193,    12,   219,   219,
     860,   196,    35,   863,   199,  1250,   173,   173,    23,    24,
     173,  1250,   872,    57,   197,   875,   876,  1109,   201,    63,
     880,   764,   765,  1239,   185,   885,   185,   184,   219,   889,
     187,   185,   193,   190,   193,   165,   779,   193,  1566,   193,
     196,   221,   222,   199,    57,    33,    57,   911,   173,   197,
      63,    57,    63,   201,   797,   185,    57,    63,   918,   919,
    1645,   921,    63,    57,    57,   925,   218,   927,   928,    63,
      63,  1656,    60,    61,   197,   173,   206,   173,   201,   943,
    1526,  1341,   197,   826,   197,    22,   201,   217,   201,  1349,
     951,    21,    22,   954,   197,   173,   957,   958,   201,  1564,
    1685,   173,   197,    43,   197,   965,   201,   967,   201,   197,
     218,   854,   219,   201,   221,   222,  1642,   860,   177,   178,
     863,  1647,   144,   145,   146,  1590,   173,   174,   175,   872,
      92,    93,   875,   876,   196,   196,   124,   880,   219,   219,
     128,   173,   885,   200,   200,  1502,   889,   177,   178,   179,
    1653,   177,   178,   179,   180,   200,   219,  1683,   200,   200,
    1250,   200,  1519,   177,   178,   179,  1241,   219,  1614,  1615,
     445,   446,   447,   200,   137,   918,   919,   200,   921,    10,
      11,    12,   925,   200,   927,   928,   116,   117,   118,   119,
     200,  1055,  1056,  1057,   197,   200,   126,   218,   128,   129,
     130,   131,   185,    33,   463,   173,   136,   195,   138,   139,
    1426,   199,   219,   201,   202,   173,   220,   173,   217,   200,
     391,    10,   965,   200,   967,   200,  1672,  1753,   219,   217,
      60,    61,   219,   200,   173,   223,  1328,   219,   166,  1100,
     200,   200,    13,  1104,    10,   200,   200,   200,   219,  1109,
     219,   200,   222,   219,   219,   219,    37,    66,   219,  1334,
     217,  1121,   200,   522,   200,    43,   173,   219,   219,   173,
     220,   218,   202,   203,   204,   205,   206,   193,   196,    43,
     200,   200,   193,   200,   219,  1146,   216,   217,   219,  1149,
    1150,  1034,   463,   219,   124,   132,    14,  1306,   128,   194,
     196,  1658,   166,   142,    13,   219,   218,   222,   193,  1525,
     193,   173,     8,   173,  1174,   201,   487,   220,   173,   108,
     109,   110,   111,   112,   113,   114,   115,   173,   173,   220,
    1405,   219,   121,   122,   142,   193,  1428,   193,   127,   220,
     184,   219,   219,   193,   220,   173,   219,   200,   137,   219,
    1425,   140,   141,   200,   219,    33,   219,     1,   147,   148,
     149,    67,   219,    33,    43,   195,  1109,   219,   219,   199,
     219,   201,   202,   173,   201,   201,   201,  1241,  1121,  1239,
     220,   173,    60,    61,   220,   220,   220,   217,    33,   218,
      60,    61,   196,   223,    12,   218,   220,  1613,   219,   173,
     220,   220,   219,   218,   220,   194,  1149,  1150,   667,   668,
     173,   173,   219,   391,  1489,    60,    61,   173,   200,   173,
     219,   219,   219,   219,   173,   220,   597,   220,   219,   219,
     218,  1174,   221,   222,   693,   181,   173,   696,  1447,  1300,
     173,   218,   218,    33,   219,   173,   124,   219,  1540,   127,
     128,   219,   219,   201,   124,   220,    70,   201,   128,   137,
     219,   219,   219,   201,   219,   177,   219,   219,  1328,   220,
    1334,   185,   185,   220,   219,   453,    53,   220,   220,   124,
     219,  1556,   218,   128,   219,   463,    33,   165,   220,   220,
     468,   220,   470,  1354,   753,   220,  1239,   668,   184,   193,
     185,   479,   218,   193,   727,   220,    85,   185,   218,   487,
    1371,   218,   220,    60,    61,   220,   194,   195,   220,   220,
     220,   199,  1383,   220,   202,   195,   218,   220,   699,   199,
     219,   201,   202,   511,   184,     1,  1611,   220,   220,   217,
     220,  1405,   218,   218,    46,   223,  1407,   217,   141,    88,
     195,   529,  1517,   223,   199,   239,   201,   202,   821,   818,
     990,  1425,    33,  1517,  1517,  1655,  1426,  1517,  1428,  1517,
       1,   999,   217,  1489,  1423,  1264,  1551,   124,   223,  1669,
    1449,   128,    33,  1303,  1452,  1328,  1552,    56,  1552,    60,
      61,   392,   800,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   580,   811,   812,   813,   814,   815,   816,    60,
      61,   459,  1647,  1500,  1216,  1705,   459,   719,    -1,   597,
      -1,    -1,   881,    -1,    -1,  1489,    -1,    -1,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,    -1,
     811,   812,   813,   814,   815,   816,    -1,  1722,   195,    -1,
      -1,    -1,   199,   124,   201,   202,   127,   128,    -1,   830,
      -1,    -1,    -1,    -1,    -1,  1525,   137,    -1,    -1,    -1,
     217,    -1,    -1,   124,    -1,   402,   223,   128,    -1,    -1,
    1540,    -1,    -1,  1426,   411,  1428,    -1,    -1,    -1,   948,
     668,    -1,  1556,    -1,   421,    -1,    -1,   956,    -1,   677,
      -1,    -1,    -1,    -1,   431,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1721,    -1,   441,    -1,    -1,    -1,  1579,    -1,
      -1,   699,    -1,   194,   195,  1586,    -1,  1588,   199,    -1,
      -1,   202,    -1,    -1,  1595,    -1,   463,    -1,  1598,   717,
      -1,   719,  1751,    -1,   195,    -1,   217,  1611,   199,  1610,
     201,   202,   223,  1613,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   488,   489,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,  1525,    -1,    -1,    -1,    -1,    -1,    -1,   516,
     517,   518,   519,   520,   521,   522,    -1,  1540,    -1,    -1,
      -1,  1662,    -1,    -1,  1664,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,  1675,    -1,    -1,    -1,    -1,    -1,
    1681,   799,   800,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1708,  1709,    -1,
      -1,    -1,   830,    -1,    -1,    -1,  1717,  1718,  1722,   490,
     491,   492,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1613,   124,    -1,    -1,    -1,   128,    -1,    -1,  1137,    -1,
      -1,    -1,    -1,    -1,  1745,    -1,  1747,    -1,    -1,   616,
      -1,    -1,    -1,    -1,    -1,    -1,   874,    -1,   625,    -1,
      -1,    -1,    -1,    -1,   882,    -1,   884,    -1,    -1,    -1,
      -1,   542,    -1,    -1,    -1,    -1,   894,   895,   896,   897,
     898,   899,   900,   901,   902,   903,   904,   905,   906,   907,
     908,   909,   910,    -1,    -1,    -1,    -1,    -1,  1197,   570,
     667,   668,   195,    -1,    -1,    -1,   199,    -1,   201,   202,
      -1,    -1,   679,    -1,    -1,    -1,    -1,   684,    -1,  1218,
      -1,    -1,    -1,   690,   217,  1224,   693,    -1,    -1,   696,
     223,    -1,    -1,    -1,  1233,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   960,   710,   711,   712,   713,   714,   715,  1248,
      10,   969,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,   985,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1277,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   753,    -1,    -1,    -1,
    1289,    -1,    -1,    -1,    -1,  1294,    -1,    -1,    -1,   766,
     767,    -1,   673,   770,   771,   772,   773,    33,   775,    -1,
     777,   778,   779,   780,   781,   782,   783,   784,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     701,   798,    -1,    -1,    60,    61,  1335,    -1,    -1,  1250,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   818,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,  1366,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    33,   144,   145,   146,    -1,    -1,    -1,
     150,   762,    -1,    -1,    -1,  1394,  1395,    -1,   124,  1398,
     867,    -1,   128,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      60,    61,    -1,    -1,   881,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,   185,   893,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,  1177,
      -1,   832,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,    -1,    -1,   199,   124,   201,   202,    -1,   128,   946,
     947,   948,  1200,  1201,    -1,    -1,    -1,  1205,    -1,   956,
      -1,   217,    -1,    -1,    -1,  1213,    -1,   223,  1216,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   976,
      -1,  1510,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   993,  1245,    -1,    -1,
      -1,    -1,  1250,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,   199,
      -1,   201,   202,    -1,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,   933,    19,    21,    22,    -1,   217,    -1,    25,
      -1,  1570,   943,   223,    -1,    31,    -1,    -1,    -1,    -1,
    1047,    -1,    -1,   195,  1051,    41,    -1,   199,    -1,   201,
     202,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1602,    -1,   217,    -1,    -1,    64,    -1,
      -1,   223,    -1,  1331,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,  1666,    -1,   126,
    1137,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    33,  1055,  1056,  1057,   143,    -1,  1060,
      -1,  1062,    -1,  1064,    -1,  1066,    -1,  1068,    -1,  1070,
     156,  1072,    -1,  1074,    -1,  1076,    -1,  1078,    -1,  1080,
      60,    61,  1083,    33,  1085,    -1,  1087,   173,  1089,    -1,
    1091,    -1,  1093,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1197,    -1,  1199,  1104,    -1,  1202,    -1,    -1,    -1,    -1,
      60,    61,    -1,   200,   201,   202,   203,   204,   205,   206,
    1468,  1218,  1470,    -1,    -1,    -1,    -1,  1224,    -1,   216,
     217,    -1,  1480,  1481,  1482,   221,  1233,    -1,    -1,    -1,
    1237,  1238,    -1,    -1,   124,    33,    -1,    -1,   128,  1497,
    1498,  1248,    -1,    -1,    -1,  1503,    -1,    -1,    -1,  1256,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1517,
      -1,    -1,    60,    61,   124,    -1,    -1,  1274,   128,  1276,
    1277,  1278,    -1,  1531,    -1,    -1,    -1,  1284,    -1,    -1,
      -1,  1288,  1289,    -1,    -1,    -1,    -1,  1294,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,   199,
      -1,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1580,  1581,    -1,    -1,   124,   217,  1335,    -1,
     128,    -1,    -1,   223,    -1,   195,    -1,    -1,    -1,   199,
      -1,   201,   202,    33,    -1,  1603,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,  1616,  1366,
    1618,    -1,    -1,   223,    21,    22,    -1,    -1,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1637,
      -1,  1639,    -1,    -1,    -1,    -1,    -1,  1394,  1395,    -1,
      -1,  1398,    -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,
      -1,   199,    -1,   201,   202,    -1,  1413,    -1,    -1,  1667,
      -1,    -1,    -1,    -1,    -1,  1673,  1674,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,   223,    -1,  1434,    -1,  1436,
      -1,    -1,  1439,    -1,   124,  1442,    -1,    -1,   128,    -1,
      -1,    -1,  1700,    -1,  1702,  1703,    -1,    -1,    -1,    -1,
      -1,  1458,    -1,  1364,    -1,    -1,    -1,    -1,  1716,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
    1487,   138,   139,    -1,    -1,  1492,    -1,   144,    -1,  1496,
      -1,    -1,  1499,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1509,  1510,    -1,   195,    -1,    -1,    -1,   199,
      -1,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,   223,    -1,    -1,    -1,    -1,    -1,    -1,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1564,    -1,   216,
     217,    -1,    -1,  1570,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1590,    -1,    -1,    -1,    -1,    -1,    -1,
    1597,    -1,    -1,     1,    -1,  1602,    -1,     5,     6,     7,
      -1,     9,    10,    11,    -1,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    42,    -1,    44,    45,    -1,    -1,
      48,    -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,
      58,    59,  1659,    -1,    -1,    -1,    -1,    65,    -1,  1666,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,  1711,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,
      -1,   219,    -1,   221,   222,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      60,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,   199,
      -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,
     210,    -1,    -1,    -1,     5,     6,    -1,   217,    -1,   219,
      -1,   221,   222,   223,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    60,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,    -1,    -1,
     202,   203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,
      -1,    -1,     5,     6,    -1,   217,    -1,   219,    -1,   221,
     222,   223,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
       5,     6,    -1,    -1,   217,    -1,   219,    -1,   221,   222,
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
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,
      -1,    -1,   217,    -1,   219,    -1,   221,   222,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    52,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,
      -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,
     217,    -1,   219,    -1,   221,   222,    15,    16,    17,    18,
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
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,
     209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,
     219,    -1,   221,   222,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,   210,
      -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,   220,
     221,   222,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
      -1,    -1,     5,     6,   217,    -1,   219,    -1,   221,   222,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
      -1,    -1,     5,     6,   217,   218,   219,    -1,   221,   222,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
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
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
      -1,    -1,     5,     6,   217,    -1,   219,    -1,   221,   222,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
      -1,    -1,     5,     6,   217,   218,   219,    -1,   221,   222,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
       5,     6,    -1,    -1,   217,    -1,   219,    -1,   221,   222,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,
      -1,    -1,   217,    -1,   219,    -1,   221,   222,    15,    16,
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
     157,    -1,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,
      -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,
     217,   218,   219,    -1,   221,   222,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,
     209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,
     219,    -1,   221,   222,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,   210,
      -1,    -1,    -1,    -1,     5,     6,   217,    -1,   219,    -1,
     221,   222,    13,    -1,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,   210,
      -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,    -1,
     221,   222,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
      -1,    -1,     5,     6,   217,    -1,   219,    -1,   221,   222,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
       5,     6,    -1,    -1,   217,    -1,   219,    -1,   221,   222,
      15,    16,    17,    18,    19,    -1,    -1,    22,    -1,    -1,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,
      -1,    -1,   217,    -1,   219,    -1,   221,   222,    15,    16,
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
     157,    -1,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,
      -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,
     217,    -1,   219,   220,   221,   222,    15,    16,    17,    18,
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
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,    -1,   202,   203,   204,    -1,   206,    -1,    -1,
     209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,
     219,    -1,   221,   222,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,   210,
      -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,   220,
     221,   222,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
       5,     6,    -1,    -1,   217,    -1,   219,   220,   221,   222,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,
      -1,    -1,   217,    -1,   219,   220,   221,   222,    15,    16,
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
     157,    -1,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,
      -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,
     217,    -1,   219,   220,   221,   222,    15,    16,    17,    18,
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
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,
     209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,
     219,   220,   221,   222,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,   210,
      -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,   220,
     221,   222,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
       5,     6,    -1,    -1,   217,    -1,   219,   220,   221,   222,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,   206,    -1,    -1,   209,   210,    -1,    -1,     5,     6,
      -1,    -1,   217,    -1,   219,    -1,   221,   222,    15,    16,
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
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,
      -1,    -1,   209,   210,    -1,    -1,     5,     6,    -1,    -1,
     217,    -1,   219,    -1,   221,   222,    15,    16,    17,    18,
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
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,    -1,
     209,   210,    -1,    -1,     5,     6,    -1,    -1,   217,    -1,
     219,    -1,   221,   222,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   204,    -1,   206,    -1,    -1,   209,   210,
      -1,    -1,     5,     6,    -1,    -1,   217,    -1,   219,    -1,
     221,   222,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    13,    -1,    -1,    -1,
     183,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    41,    -1,   209,   210,    -1,    -1,
      -1,    -1,    49,    -1,   217,    -1,   219,    -1,   221,   222,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,   173,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   173,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   221,    -1,   223,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    19,
      -1,    -1,   173,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    41,   193,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
     221,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   173,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,   221,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    21,    22,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,   116,
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
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
     220,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,   220,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   195,
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    21,    22,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,
     139,   140,   141,    -1,    -1,   144,   145,   146,   147,   148,
     149,   150,    -1,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
     209,   210,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,   165,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,   185,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,   203,   204,   205,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,   173,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
     158,    35,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,   129,   130,    -1,    -1,    71,    -1,    73,
      -1,    75,    76,    77,    78,    79,   173,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,   158,    -1,    98,    99,   100,   101,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,   173,    -1,    -1,
      -1,   121,   122,    -1,   118,   119,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
     140,   141,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    -1,   165,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   173,
     121,   122,    -1,    -1,    -1,   185,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,   194,    -1,   137,    -1,    -1,   140,
     141,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,   221,   222,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
      -1,    -1,   140,   141,    -1,    -1,    -1,    -1,    -1,   147,
     148,   149,    -1,   194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     221,   222,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   221,   222
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
     137,   219,     7,    50,   317,   185,   185,   193,   220,   464,
       1,     5,     6,     9,    10,    11,    13,    15,    16,    17,
      18,    19,    25,    26,    27,    28,    29,    31,    38,    39,
      42,    44,    45,    48,    51,    52,    54,    55,    58,    59,
      65,    68,    69,    80,   102,   103,   104,   105,   118,   119,
     133,   134,   135,   151,   152,   153,   154,   155,   157,   159,
     160,   161,   162,   165,   166,   167,   168,   169,   170,   171,
     172,   174,   175,   176,   183,   185,   202,   203,   204,   209,
     210,   217,   219,   221,   222,   237,   239,   250,   251,   254,
     257,   258,   260,   263,   264,   265,   266,   287,   288,   290,
     291,   293,   298,   303,   304,   305,   309,   310,   311,   312,
     313,   314,   315,   316,   318,   322,   323,   330,   333,   336,
     341,   344,   345,   347,   348,   349,   351,   356,   359,   360,
     367,   368,   423,   478,   493,   504,   508,   521,   524,   403,
     194,   375,   304,   376,   396,   218,    65,   104,   174,   298,
     360,   173,   173,   436,   127,   137,   194,   393,   437,   442,
     444,   360,   446,   440,   173,   431,   448,   450,   452,   454,
     456,   458,   460,   462,   360,   200,   219,    33,   199,    33,
     199,   217,   223,   218,   360,   217,   223,   436,   165,   193,
     428,   173,   193,   226,   383,   433,   464,   469,   173,   386,
     433,   474,   360,   152,   173,   390,   391,   422,   382,   382,
     382,   200,   200,   367,   261,   262,   200,   306,   425,   478,
     219,   304,   200,     5,   102,   103,   200,   219,   127,   303,
     334,   345,   360,   367,   289,   200,   219,    61,   367,   219,
     367,   173,   200,   200,   219,   226,   200,   166,    58,   367,
     219,   289,   200,   219,   200,   200,   219,   200,   200,   127,
     303,   367,   360,   360,   222,   289,   336,   340,   340,   340,
     219,   219,   219,   219,   219,   219,    13,   436,    13,   436,
      13,   367,   503,   519,   200,   367,   200,   236,    13,   360,
     360,   360,   360,   360,    13,    49,   294,   334,   360,   334,
     222,   226,   226,   367,    10,    13,   296,   503,   520,    37,
     336,   342,   173,   219,   226,   226,   226,   226,   226,    66,
     319,   278,   132,   226,    21,    22,   116,   117,   118,   119,
     120,   123,   124,   125,   126,   128,   129,   130,   131,   136,
     138,   139,   144,   145,   146,   150,   195,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   216,   217,   108,
     109,   110,   111,   112,   113,   114,   115,   121,   122,   127,
     137,   140,   141,   147,   148,   149,   194,   342,   219,   288,
     367,   217,   278,   288,   377,   375,   200,   220,    43,   226,
     393,   303,   367,   464,   464,   434,   464,   220,   464,   464,
     220,   173,   427,   464,   279,   464,   279,   464,   279,   383,
     384,   386,   387,   220,   439,   294,   218,   218,   360,   193,
     196,   433,   288,   196,   433,   288,   220,   219,    43,   127,
     193,   194,   196,   199,   389,   494,   496,   289,   422,   219,
     307,   219,   304,   200,   219,   331,   200,   200,   200,   515,
     334,   303,   334,   193,   108,   109,   110,   111,   112,   113,
     114,   115,   121,   122,   127,   140,   141,   147,   148,   149,
     194,    14,   436,   296,   367,   360,   289,   194,   324,   326,
     360,   328,   196,   166,   360,   517,   334,   500,   505,   334,
     498,   436,   303,   367,   222,   278,   360,   360,   360,   360,
     360,   360,   422,    53,   158,   173,   202,   217,   219,   367,
     479,   482,   486,   502,   507,   422,   219,   482,   507,   422,
     142,   184,   186,   226,   487,   299,   289,   301,   179,   180,
     231,   219,   219,   422,    13,   218,   193,   523,   523,   152,
     157,   200,   304,   350,   289,   259,   422,   288,   193,   523,
     287,   343,    70,   217,   220,   334,   479,   481,   160,   219,
     321,   396,     4,   160,   339,   340,    19,   158,   173,   423,
      19,   158,   173,   423,   360,   360,   360,   360,   360,   360,
     173,   360,   158,   173,   360,   360,   360,   423,   360,   360,
     360,   360,   360,   360,    22,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   129,   130,   158,   173,
     216,   217,   357,   423,   360,   133,   134,   135,   290,   346,
     367,   346,   367,   346,   367,   346,   367,   346,   367,   346,
     367,   346,   367,   346,   367,   346,   367,   346,   367,   346,
     367,   367,   346,   367,   346,   367,   346,   367,   346,   367,
     346,   367,   346,   367,   220,   334,   375,   277,     8,   369,
     374,   436,   173,   303,   367,   226,   201,   201,   201,   433,
     201,   201,   201,   226,   201,   280,   201,   280,   201,   280,
     201,   433,   201,   433,   297,   464,   220,   218,   464,   464,
     360,   173,   173,   464,   367,   436,   436,    20,   422,   464,
      70,   334,   481,   492,   200,   367,   173,   367,   464,   509,
     511,   513,   436,   523,   360,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   289,   201,   433,   220,   220,   267,   436,   436,
     220,   436,   220,   436,   523,   436,   384,   523,   387,   201,
     339,   220,   220,   220,   220,   220,   220,    20,   340,   219,
     137,   389,   217,   360,   220,   142,   193,   226,   486,   188,
     189,   218,   490,   193,   188,   218,   226,   489,    20,   220,
     486,   184,   187,   488,    20,   367,   184,   503,   297,   297,
     367,   422,   422,    20,   219,   422,   220,   219,   219,   352,
     354,    12,    23,    24,   165,   252,   253,   367,    20,   503,
     292,   278,   173,   220,   481,   479,   193,   220,   193,   523,
     220,   173,   320,   320,   219,   127,   137,   173,   194,   199,
     337,   338,   279,   200,   219,   200,   219,   219,   219,   218,
      19,   158,   173,   423,   196,   158,   173,   360,   219,   219,
     158,   173,   360,     1,   219,   218,   340,   340,   340,   220,
     218,    57,    63,   372,    67,   373,   226,   201,   226,   438,
     443,   445,   464,   447,   441,   432,   173,   449,   201,   453,
     201,   457,   201,   461,   383,   463,   386,   201,   433,   220,
      43,   389,   201,   201,   334,    20,   201,   481,   220,   220,
     220,   173,   220,   201,   226,   220,   201,   436,   384,   387,
     201,   220,   219,   436,   367,   201,   201,   201,   201,   220,
     201,   201,   220,   201,   339,   279,   219,   334,   360,   367,
     367,   482,   486,   367,   158,   173,   479,   490,   218,   367,
     218,   502,   334,   482,   184,   187,   190,   491,   218,   334,
     201,   201,   196,   234,    20,    20,   334,   422,    20,   360,
     360,   436,   279,   289,   253,   367,    12,   255,   334,   288,
     339,   220,   218,   217,   193,   218,   220,   338,   173,   173,
     219,   173,   173,   193,   218,   280,   361,   360,   363,   360,
     220,   334,   360,   200,   219,   360,   219,   218,   360,   217,
     220,   334,   219,   218,   358,   220,   334,   226,    47,   373,
      46,   106,   370,   375,   339,   451,   455,   459,   219,   464,
     173,   367,   495,   497,   289,   334,   308,   220,   201,   433,
     219,   173,   332,   201,   201,   201,   516,   296,   201,   226,
     325,   327,   329,   518,   501,   506,   499,   219,   342,   280,
     220,   334,   185,   220,   486,   490,   219,   137,   389,   185,
     486,   218,   185,   300,   302,   235,   181,   334,   334,   185,
      20,   334,   220,   220,   201,   280,   289,   256,   226,   185,
     279,   220,   479,   173,   218,   196,   394,   220,   173,   337,
     218,   142,   289,   335,   436,   220,   464,   220,   220,   220,
     365,   360,   360,   220,   479,   220,   360,   220,   375,    33,
     371,   370,   372,   294,   219,   219,   220,   367,   173,   367,
     201,   510,   512,   514,   219,   220,   219,   367,   367,   367,
     219,    70,   492,   219,   219,   220,   360,   335,   220,   360,
     137,   389,   490,   360,   367,   367,   360,   491,   503,   367,
     219,   295,   233,   220,   220,   360,   334,   185,   353,   201,
     165,   253,    26,   105,   257,   310,   311,   312,   314,   367,
     503,   280,   218,   196,   394,   436,   393,   220,   127,   367,
     201,   201,   464,   220,   220,   218,   220,   378,   371,   390,
     391,   392,   220,   492,   492,   289,   220,   201,   220,   219,
     219,   219,   219,   294,   296,   334,   492,   492,   220,   226,
     522,   367,   367,   220,   522,   522,   294,   177,   185,   185,
     522,   220,   360,   350,   355,   253,   127,   127,   367,   522,
     289,   220,   436,   393,   393,   367,   367,   362,   364,   201,
     220,   284,   379,   219,   479,   483,   484,   485,   485,   367,
     367,   492,   492,   479,   480,   220,   220,   523,   485,   480,
      53,   218,   137,   389,   184,   288,   193,   523,   503,   360,
     218,   185,   522,   350,   367,   288,   393,   367,   367,   226,
     366,   226,   284,   479,   193,   523,   220,   220,   220,   220,
     485,   485,   220,   220,   220,   220,   367,   218,   367,   367,
     218,   288,   220,   522,   522,   360,   218,   367,   226,   226,
     375,   289,   220,   219,   220,   220,   184,   218,   522,   226,
     375,   479,   218,   220
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
     281,   281,   281,   281,   281,   281,   282,   283,   283,   283,
     284,   286,   285,   287,   287,   288,   288,   289,   289,   290,
     290,   290,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   292,   292,   292,   293,   294,   294,
     295,   295,   296,   296,   297,   297,   299,   300,   298,   301,
     302,   298,   303,   303,   303,   303,   303,   304,   304,   304,
     305,   305,   307,   308,   306,   306,   309,   309,   309,   309,
     309,   309,   310,   311,   312,   312,   312,   313,   313,   313,
     314,   314,   315,   315,   315,   316,   317,   317,   317,   318,
     318,   319,   319,   320,   320,   321,   321,   321,   321,   322,
     322,   324,   325,   323,   326,   327,   323,   328,   329,   323,
     331,   332,   330,   333,   333,   333,   333,   333,   333,   334,
     334,   335,   335,   335,   336,   336,   336,   337,   337,   337,
     337,   337,   338,   338,   339,   339,   339,   340,   340,   341,
     343,   342,   344,   344,   344,   344,   344,   344,   344,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   346,   346,
     346,   346,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   348,
     348,   349,   349,   350,   350,   351,   352,   353,   351,   354,
     355,   351,   356,   356,   356,   356,   356,   356,   356,   357,
     358,   356,   359,   359,   359,   359,   359,   359,   359,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   361,   362,   360,   360,   360,
     360,   363,   364,   360,   360,   360,   365,   366,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   367,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     369,   369,   369,   370,   370,   370,   371,   371,   372,   372,
     372,   373,   373,   374,   375,   375,   375,   375,   376,   377,
     376,   378,   376,   379,   376,   376,   380,   381,   381,   382,
     382,   382,   382,   382,   383,   383,   384,   384,   385,   385,
     385,   386,   387,   387,   388,   388,   388,   389,   389,   390,
     390,   390,   391,   391,   392,   392,   393,   393,   393,   394,
     394,   395,   395,   395,   395,   395,   396,   396,   396,   396,
     396,   396,   397,   398,   397,   399,   399,   400,   400,   400,
     401,   402,   401,   403,   403,   403,   404,   404,   404,   406,
     405,   407,   407,   408,   409,   408,   410,   410,   410,   411,
     412,   412,   413,   413,   414,   414,   415,   416,   416,   416,
     416,   417,   417,   417,   418,   418,   420,   421,   419,   422,
     422,   422,   422,   422,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   424,   424,   424,   424,   424,   424,   424,   424,   425,
     426,   426,   426,   427,   427,   428,   428,   429,   429,   429,
     429,   431,   432,   430,   433,   433,   434,   434,   435,   435,
     436,   436,   436,   436,   436,   436,   437,   438,   436,   436,
     436,   439,   436,   436,   436,   436,   436,   436,   436,   436,
     436,   436,   436,   436,   436,   440,   441,   436,   436,   442,
     443,   436,   444,   445,   436,   446,   447,   436,   436,   448,
     449,   436,   450,   451,   436,   436,   452,   453,   436,   454,
     455,   436,   436,   456,   457,   436,   458,   459,   436,   460,
     461,   436,   462,   463,   436,   464,   464,   464,   466,   467,
     468,   469,   465,   471,   472,   473,   474,   470,   476,   477,
     475,   478,   478,   478,   478,   478,   479,   479,   479,   479,
     479,   479,   479,   479,   480,   480,   481,   482,   482,   483,
     483,   484,   484,   485,   485,   486,   486,   487,   487,   488,
     488,   489,   489,   490,   490,   490,   491,   491,   491,   492,
     492,   493,   493,   493,   493,   493,   493,   494,   495,   493,
     496,   497,   493,   498,   499,   493,   500,   501,   493,   502,
     502,   502,   503,   503,   504,   505,   506,   504,   507,   507,
     508,   508,   508,   509,   510,   508,   511,   512,   508,   513,
     514,   508,   508,   515,   516,   508,   508,   517,   518,   508,
     519,   519,   520,   520,   521,   521,   521,   521,   521,   522,
     522,   523,   523,   524,   524,   524,   524,   524,   524,   524,
     524,   524
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
       4,     3,     2,     2,     2,     2,     2,     3,     3,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     1,
       3,     0,     4,     1,     1,     1,     1,     3,     7,     2,
       2,     6,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     2,     0,     2,     2,     3,     0,     2,
       0,     4,     0,     2,     1,     3,     0,     0,     7,     0,
       0,     7,     3,     2,     2,     2,     1,     1,     3,     2,
       2,     3,     0,     0,     5,     1,     2,     5,     5,     5,
       6,     2,     1,     1,     1,     2,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     4,     1,     1,     0,     1,
       1,     1,     0,     1,     3,     9,     8,     8,     7,     3,
       3,     0,     0,     7,     0,     0,     7,     0,     0,     7,
       0,     0,     6,     5,     8,    10,     5,     8,    10,     1,
       3,     1,     2,     3,     1,     1,     2,     2,     2,     2,
       2,     4,     1,     3,     0,     4,     4,     1,     6,     6,
       0,     7,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       8,     5,     6,     1,     4,     3,     0,     0,     8,     0,
       0,     9,     3,     4,     5,     6,     8,     5,     6,     0,
       0,     5,     3,     4,     4,     5,     4,     3,     4,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     4,     5,     4,     5,     3,     4,     2,     5,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     1,
       4,     4,     7,     8,     3,     0,     0,     8,     3,     3,
       3,     0,     0,     8,     3,     4,     0,     0,     9,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       2,     4,     1,     1,     4,     4,     4,     4,     4,     1,
       6,     7,     6,     6,     7,     7,     6,     7,     6,     6,
       0,     4,     1,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     1,     1,     2,     0,     0,     0,
       5,     0,    10,     0,    11,     6,     3,     3,     4,     1,
       1,     3,     3,     3,     1,     3,     1,     3,     0,     2,
       3,     3,     1,     3,     0,     2,     3,     1,     1,     1,
       2,     3,     3,     5,     1,     1,     1,     1,     1,     0,
       1,     1,     4,     3,     3,     5,     4,     6,     5,     5,
       4,     4,     0,     0,     5,     0,     1,     0,     1,     1,
       6,     0,     6,     0,     3,     5,     0,     1,     1,     0,
       5,     2,     3,     4,     0,     4,     0,     1,     1,     1,
       7,     9,     0,     2,     0,     1,     3,     1,     1,     2,
       2,     0,     1,     1,     0,     3,     0,     0,     7,     1,
       4,     3,     3,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     1,     3,     1,     2,     0,     3,     3,
       2,     0,     0,     6,     1,     1,     1,     3,     3,     4,
       1,     1,     1,     1,     2,     3,     0,     0,     6,     4,
       5,     0,     9,     4,     2,     2,     3,     2,     3,     2,
       2,     3,     3,     3,     2,     0,     0,     6,     2,     0,
       0,     6,     0,     0,     6,     0,     0,     6,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     0,
       0,     6,     0,     0,     6,     1,     3,     3,     0,     0,
       0,     0,    10,     0,     0,     0,     0,    10,     0,     0,
       8,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     0,     1,     2,     1,     3,     3,
       5,     1,     2,     1,     0,     0,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     2,     1,     1,     0,
       1,     5,     4,     6,     7,     5,     7,     0,     0,    10,
       0,     0,    10,     0,     0,    10,     0,     0,     7,     1,
       3,     3,     3,     1,     5,     0,     0,    10,     1,     3,
       3,     4,     4,     0,     0,    11,     0,     0,    11,     0,
       0,    10,     5,     0,     0,     9,     5,     0,     0,    10,
       1,     3,     1,     3,     3,     3,     4,     7,     9,     0,
       3,     0,     1,     9,    11,    12,    11,    10,    10,    10,
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

  case 191: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 192: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 193: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 194: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 195: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 196: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 197: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 198: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 199: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 200: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 201: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 202: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 203: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 204: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 205: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 206: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 207: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 208: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 209: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 210: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 211: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 212: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 213: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 214: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 215: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 216: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 217: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 218: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 219: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 220: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 221: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 222: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 223: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 224: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 225: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 226: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 227: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 228: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 229: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 230: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 231: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 232: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
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

  case 233: /* open_block: "begin of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 234: /* open_block: "new scope"  */
                        { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 235: /* close_block: "end of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 236: /* close_block: "close scope"  */
                         { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 237: /* expression_block: open_block expressions close_block  */
                                                                  {
        auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui), format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size, format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 238: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
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

  case 239: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 240: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 241: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 242: /* expression_any: SEMICOLON  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 243: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 244: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 245: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 246: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 247: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 248: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 249: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 250: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 251: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 252: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 254: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 255: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 256: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 257: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 258: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 259: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 260: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 261: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 262: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 263: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 264: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 265: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 266: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 267: /* expr_keyword: "keyword" expr expression_block  */
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

  case 268: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 269: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 270: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 271: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 272: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 273: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 274: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 275: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 276: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 277: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 278: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 279: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 280: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 281: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 282: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 283: /* expr_pipe: "@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }

        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 284: /* expr_pipe: "@@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 285: /* expr_pipe: "$ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "$";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 286: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 287: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 288: /* name_in_namespace: "name" "::" "name"  */
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

  case 289: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 290: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 291: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 292: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 293: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 294: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 295: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 296: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 297: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 298: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 299: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 300: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 301: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 302: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 303: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 304: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 305: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 306: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 307: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 308: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 309: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 310: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 311: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 312: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 313: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 314: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 315: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                              {
        const auto end_block = format::Pos::from_last(tokAt(scanner, (yylsp[-2])));
        const auto start = format::Pos::from(tokAt(scanner, (yylsp[-3])));
        if (format::is_replace_braces()) {
            format::skip_spaces_or_print(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])), yyextra->das_tab_size);
        }

        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 316: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 317: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 318: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 319: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 320: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 321: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 322: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 323: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 324: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 325: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
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

  case 326: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 327: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 328: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 329: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 330: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 331: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 332: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 333: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 334: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 335: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 336: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 337: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 338: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 339: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 340: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 341: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 342: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 343: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 344: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 345: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 346: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 347: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 348: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" semicolon "name" '>' '(' expr ')'  */
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

  case 349: /* expr_list: expr2  */
                       {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 350: /* expr_list: expr_list ',' expr2  */
                                             {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 351: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 352: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 353: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 354: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 355: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 356: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 357: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 358: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 359: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 360: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 361: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 362: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 363: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 364: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 365: /* optional_capture_list: "[[" capture_list ']' ']'  */
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

  case 366: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 367: /* expr_block: expression_block  */
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

  case 368: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 369: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 370: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 371: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 372: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 373: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 374: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 375: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 376: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 377: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 378: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 379: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 380: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 381: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 382: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 383: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 384: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 385: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 386: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 387: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 388: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 389: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 390: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 391: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 392: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 393: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 394: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign_pipe_right: "@ <|" expr_block  */
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

  case 399: /* expr_assign_pipe_right: "@@ <|" expr_block  */
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

  case 400: /* expr_assign_pipe_right: "$ <|" expr_block  */
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

  case 401: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 402: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 410: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 411: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 412: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 413: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 414: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 415: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 416: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 420: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 421: /* expr_method_call: expr2 "->" "name" '(' ')'  */
                                                          {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 422: /* expr_method_call: expr2 "->" "name" '(' expr_list ')'  */
                                                                               {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 423: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 424: /* func_addr_name: "$i" '(' expr2 ')'  */
                                           {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 425: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 426: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 427: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 428: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 429: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 430: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 431: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
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

  case 432: /* expr_field: expr2 '.' "name"  */
                                               {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 433: /* expr_field: expr2 '.' '.' "name"  */
                                                   {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 434: /* expr_field: expr2 '.' "name" '(' ')'  */
                                                       {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 435: /* expr_field: expr2 '.' "name" '(' expr_list ')'  */
                                                                            {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 436: /* expr_field: expr2 '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 437: /* expr_field: expr2 '.' basic_type_declaration '(' ')'  */
                                                                         {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 438: /* expr_field: expr2 '.' basic_type_declaration '(' expr_list ')'  */
                                                                                              {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 439: /* $@28: %empty  */
                                { yyextra->das_suppress_errors=true; }
    break;

  case 440: /* $@29: %empty  */
                                                                             { yyextra->das_suppress_errors=false; }
    break;

  case 441: /* expr_field: expr2 '.' $@28 error $@29  */
                                                                                                                     {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 442: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 443: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 444: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 445: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 446: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 447: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 448: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 449: /* expr2: name_in_namespace  */
                                              { need_wrap_current_expr = true; (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 450: /* expr2: expr_field  */
                                              { need_wrap_current_expr = true; (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 451: /* expr2: expr_mtag  */
                                              { need_wrap_current_expr = true; (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 452: /* expr2: '!' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"!",(yyvsp[0].pExpression)); }
    break;

  case 453: /* expr2: '~' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"~",(yyvsp[0].pExpression)); }
    break;

  case 454: /* expr2: '+' expr2  */
                                                   { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+",(yyvsp[0].pExpression)); }
    break;

  case 455: /* expr2: '-' expr2  */
                                                   { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"-",(yyvsp[0].pExpression)); }
    break;

  case 456: /* expr2: expr2 "<<" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr2: expr2 ">>" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr2: expr2 "<<<" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr2: expr2 ">>>" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr2: expr2 '+' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr2: expr2 '-' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr2: expr2 '*' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr2: expr2 '/' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr2: expr2 '%' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr2: expr2 '<' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr2: expr2 '>' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr2: expr2 "==" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr2: expr2 "!=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr2: expr2 "<=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr2: expr2 ">=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr2: expr2 '&' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr2: expr2 '|' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr2: expr2 '^' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr2: expr2 "&&" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr2: expr2 "||" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr2: expr2 "^^" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr2: expr2 ".." expr2  */
                                               {
        need_wrap_current_expr = true;
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 478: /* expr2: "++" expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"++", (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr2: "--" expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"--", (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr2: expr2 "++"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 481: /* expr2: expr2 "--"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 482: /* expr2: expr2 '[' expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 483: /* expr2: expr2 '.' '[' expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 484: /* expr2: expr2 "?[" expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 485: /* expr2: expr2 '.' "?[" expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 486: /* expr2: expr2 "?." "name"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 487: /* expr2: expr2 '.' "?." "name"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 488: /* expr2: '*' expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 489: /* expr2: expr2 '?' expr2 ':' expr2  */
                                                             {
        need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp3(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 490: /* expr2: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 491: /* expr2: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 492: /* expr2: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 493: /* expr2: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 494: /* expr2: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 495: /* expr2: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 496: /* expr2: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 497: /* expr2: '(' expr_list optional_comma ')'  */
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

  case 498: /* expr2: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 499: /* expr2: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 500: /* expr2: "deref" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 501: /* expr2: "addr" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 502: /* expr2: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 503: /* expr2: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr2 ')'  */
                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 504: /* expr2: expr2 "??" expr2  */
                                                     { (yyval.pExpression) = new ExprNullCoalescing(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 505: /* $@30: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 506: /* $@31: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 507: /* expr2: expr2 "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                             {
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 508: /* expr2: expr2 "is" basic_type_declaration  */
                                                                {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 509: /* expr2: expr2 "is" "name"  */
                                               {
        (yyval.pExpression) = new ExprIsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* expr2: expr2 "as" "name"  */
                                               {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* $@32: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 512: /* $@33: %empty  */
                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 513: /* expr2: expr2 "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                  {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 514: /* expr2: expr2 "as" basic_type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 515: /* expr2: expr2 '?' "as" "name"  */
                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 516: /* $@34: %empty  */
                                                    { yyextra->das_arrow_depth ++; }
    break;

  case 517: /* $@35: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 518: /* expr2: expr2 '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                      {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-8]), (yylsp[-1])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 519: /* expr2: expr2 '?' "as" basic_type_declaration  */
                                                                    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 520: /* expr2: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 521: /* expr2: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 522: /* expr2: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 523: /* expr2: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr2: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); (yyval.pExpression)->at = tokAt(scanner, (yylsp[0])); }
    break;

  case 525: /* expr2: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 526: /* expr2: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 527: /* expr2: expr2 "<|" expr2  */
                                                  { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); }
    break;

  case 528: /* expr2: expr2 "|>" expr2  */
                                                  { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]), (yylsp[0]))); (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])); }
    break;

  case 529: /* expr2: expr2 "|>" basic_type_declaration  */
                                                           {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
    }
    break;

  case 530: /* expr2: name_in_namespace "name"  */
                                                  {
        if (format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << "." << format::get_substring(tokAt(scanner,(yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 531: /* expr2: "unsafe" '(' expr2 ')'  */
                                          {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 532: /* expr2: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 533: /* expr: expr2  */
                                       {
        if (need_wrap_current_expr) {
            format::wrap_par_expr_newline(tokAt(scanner,(yylsp[0])), (yyvsp[0].pExpression)->at);
            need_wrap_current_expr = false;
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 534: /* expr_mtag: "$$" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 535: /* expr_mtag: "$i" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 536: /* expr_mtag: "$v" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 537: /* expr_mtag: "$b" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 538: /* expr_mtag: "$a" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 539: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 540: /* expr_mtag: "$c" '(' expr2 ')' '(' ')'  */
                                                             {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 541: /* expr_mtag: "$c" '(' expr2 ')' '(' expr_list ')'  */
                                                                                 {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 542: /* expr_mtag: expr2 '.' "$f" '(' expr2 ')'  */
                                                                  {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 543: /* expr_mtag: expr2 "?." "$f" '(' expr2 ')'  */
                                                                   {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 544: /* expr_mtag: expr2 '.' '.' "$f" '(' expr2 ')'  */
                                                                      {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 545: /* expr_mtag: expr2 '.' "?." "$f" '(' expr2 ')'  */
                                                                       {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 546: /* expr_mtag: expr2 "as" "$f" '(' expr2 ')'  */
                                                                     {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 547: /* expr_mtag: expr2 '?' "as" "$f" '(' expr2 ')'  */
                                                                         {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 548: /* expr_mtag: expr2 "is" "$f" '(' expr2 ')'  */
                                                                     {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 549: /* expr_mtag: '@' '@' "$c" '(' expr2 ')'  */
                                                          {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 550: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 551: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
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

  case 552: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 553: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 554: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 555: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 556: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 557: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 558: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 559: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 560: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 561: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 562: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 563: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 564: /* opt_sem: SEMICOLON  */
                { (yyval.b) = false; }
    break;

  case 565: /* opt_sem: "end of expression"  */
          { (yyval.b) = true; }
    break;

  case 566: /* opt_sem: "end of expression" SEMICOLON  */
                    { (yyval.b) = true; }
    break;

  case 567: /* opt_sem: %empty  */
                  {(yyval.b) = false; }
    break;

  case 568: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 569: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 570: /* struct_variable_declaration_list: struct_variable_declaration_list $@36 structure_variable_declaration semicolon opt_sem  */
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

  case 571: /* $@37: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 572: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@37 function_declaration_header semicolon opt_sem  */
                                                                  {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-6].b),(yyvsp[-4].b), (yyvsp[-2].pFuncDecl));
            }
    break;

  case 573: /* $@38: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 574: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@38 function_declaration_header expression_block opt_sem  */
                                                                                {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[-1])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 575: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon opt_sem  */
                                                                                               {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-3])), CompilationError::syntax_error);
        delete (yyvsp[-3].faList);
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
    }
    break;

  case 576: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 577: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 578: /* function_argument_declaration_type: "$a" '(' expr2 ')'  */
                                      {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 579: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 580: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 581: /* function_argument_list: function_argument_declaration_no_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 582: /* function_argument_list: function_argument_declaration_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 583: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 584: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 585: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 586: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 587: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 588: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 589: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 590: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 591: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 592: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 593: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 594: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 595: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 596: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 597: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 598: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 599: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 600: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 601: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 602: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 603: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 604: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 605: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 606: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 607: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 608: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 609: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 610: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 611: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 612: /* let_variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 613: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 614: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 615: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 616: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 617: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 618: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 619: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 620: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr  */
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

  case 621: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 622: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 623: /* $@39: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 624: /* global_variable_declaration_list: global_variable_declaration_list $@39 optional_field_annotation let_variable_declaration opt_sem  */
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

  case 625: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 626: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 627: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 628: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 629: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 630: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                                   {
        handle_brace(format::Pos::from(tokAt(scanner, (yylsp[-2]))), (yyvsp[-2].ui),
                     format::get_substring(tokRangeAt(scanner, (yylsp[-2]), (yylsp[0]))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 631: /* $@40: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 632: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@40 optional_field_annotation let_variable_declaration  */
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

  case 633: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 634: /* enum_list: enum_list "name" opt_sem  */
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

  case 635: /* enum_list: enum_list "name" '=' expr opt_sem  */
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

  case 636: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 637: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 638: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 639: /* $@41: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 640: /* single_alias: optional_public_or_private_alias "name" $@41 '=' type_declaration  */
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

  case 641: /* alias_list: single_alias opt_sem  */
                                    {
        (yyval.positions) = new vector<LineInfo>(1, tokAt(scanner, (yylsp[-1])));
    }
    break;

  case 642: /* alias_list: alias_list single_alias opt_sem  */
                                                       {
        (yyvsp[-2].positions)->emplace_back(tokAt(scanner, (yylsp[-1])));
        (yyval.positions) = (yyvsp[-2].positions);
    }
    break;

  case 643: /* alias_declaration: "typedef" open_block alias_list close_block  */
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

  case 644: /* $@42: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 646: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 647: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 648: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 649: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 650: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block enum_list close_block  */
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

  case 651: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block enum_list close_block  */
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

  case 652: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 653: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 654: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 655: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 656: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 657: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 658: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 659: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 660: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 661: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 662: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 663: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 664: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 665: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                                      {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 666: /* $@43: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 667: /* $@44: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 668: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@43 structure_name $@44 optional_struct_variable_declaration_list  */
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

  case 669: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 670: /* variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 671: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 672: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 673: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 674: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 675: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 676: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 677: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 678: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 679: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 680: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 681: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 682: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 683: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 684: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 685: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 686: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 687: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 688: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 689: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 690: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 691: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 692: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 693: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 694: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 695: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 696: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 697: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 698: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 699: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 700: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 701: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 702: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 703: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 704: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 705: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 706: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 707: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 708: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 709: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 710: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 711: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 712: /* auto_type_declaration: "$t" '(' expr2 ')'  */
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

  case 713: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 714: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 717: /* bitfield_alias_bits: %empty  */
              {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 718: /* bitfield_alias_bits: bitfield_alias_bits "name" SEMICOLON  */
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

  case 719: /* bitfield_alias_bits: bitfield_alias_bits "name" commas  */
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

  case 720: /* bitfield_alias_bits: bitfield_alias_bits "name"  */
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

  case 721: /* $@45: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 722: /* $@46: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 723: /* bitfield_type_declaration: "bitfield" '<' $@45 bitfield_bits '>' $@46  */
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

  case 726: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 727: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 728: /* dim_list: '[' expr2 ']'  */
                              {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 729: /* dim_list: dim_list '[' expr2 ']'  */
                                             {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 730: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 731: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 732: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 733: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 734: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 735: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 736: /* $@47: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 737: /* $@48: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 738: /* type_declaration_no_options: "type" '<' $@47 type_declaration '>' $@48  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 739: /* type_declaration_no_options: "typedecl" '(' expr2 ')'  */
                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 740: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 741: /* $@49: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 742: /* type_declaration_no_options: '$' name_in_namespace '<' $@49 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 743: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 744: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 745: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 746: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 747: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 748: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 749: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 750: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 751: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 752: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 753: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 754: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 755: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 756: /* $@51: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 757: /* type_declaration_no_options: "smart_ptr" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 758: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 759: /* $@52: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 760: /* $@53: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 761: /* type_declaration_no_options: "array" '<' $@52 type_declaration '>' $@53  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 762: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 763: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 764: /* type_declaration_no_options: "table" '<' $@54 table_type_pair '>' $@55  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 765: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 766: /* $@57: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 767: /* type_declaration_no_options: "iterator" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 768: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 769: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 770: /* $@59: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 771: /* type_declaration_no_options: "block" '<' $@58 type_declaration '>' $@59  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 772: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 773: /* $@61: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 774: /* type_declaration_no_options: "block" '<' $@60 optional_function_argument_list optional_function_type '>' $@61  */
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

  case 775: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 776: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 777: /* $@63: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 778: /* type_declaration_no_options: "function" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 779: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 780: /* $@65: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 781: /* type_declaration_no_options: "function" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
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

  case 782: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 783: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 785: /* type_declaration_no_options: "lambda" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 786: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 787: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 788: /* type_declaration_no_options: "lambda" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 789: /* $@70: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 790: /* $@71: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 791: /* type_declaration_no_options: "tuple" '<' $@70 tuple_type_list '>' $@71  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 792: /* $@72: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 793: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 794: /* type_declaration_no_options: "variant" '<' $@72 variant_type_list '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 795: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 796: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 797: /* type_declaration: type_declaration '|' '#'  */
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

  case 798: /* $@74: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 799: /* $@75: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 800: /* $@76: %empty  */
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

  case 801: /* $@77: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 802: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@74 "name" $@75 open_block $@76 tuple_alias_type_list $@77 close_block  */
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

  case 803: /* $@78: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 804: /* $@79: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 805: /* $@80: %empty  */
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

  case 806: /* $@81: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 807: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@78 "name" $@79 open_block $@80 variant_alias_type_list $@81 close_block  */
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

  case 808: /* $@82: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 809: /* $@83: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 810: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@82 "name" $@83 open_block bitfield_alias_bits close_block  */
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

  case 811: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 812: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 813: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 814: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 815: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 816: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 817: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 818: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 819: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 820: /* make_struct_fields: "$f" '(' expr2 ')' copy_or_move expr  */
                                                                            {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 821: /* make_struct_fields: "$f" '(' expr2 ')' ":=" expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner, (yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 822: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' copy_or_move expr  */
                                                                                                        {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 823: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' ":=" expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 824: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 825: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 826: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 827: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 828: /* make_struct_dim: make_struct_dim semicolon make_struct_fields  */
                                                               {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 829: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 830: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 831: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 832: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 833: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 834: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 835: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 836: /* optional_block: "where" expr_block  */
                                  { (yyvsp[0].pExpression)->at = tokAt(scanner, (yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 849: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 850: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 851: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 852: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
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

  case 853: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
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

  case 854: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 855: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 856: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 857: /* $@84: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 858: /* $@85: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 859: /* make_struct_decl: "struct" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 860: /* $@86: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 861: /* $@87: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 862: /* make_struct_decl: "class" '<' $@86 type_declaration_no_options '>' $@87 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 863: /* $@88: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 864: /* $@89: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 865: /* make_struct_decl: "variant" '<' $@88 variant_type_list '>' $@89 '(' use_initializer make_variant_dim ')'  */
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

  case 866: /* $@90: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 867: /* $@91: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 868: /* make_struct_decl: "default" '<' $@90 type_declaration_no_options '>' $@91 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 869: /* make_tuple: expr  */
                  {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 870: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 871: /* make_tuple: make_tuple ',' expr  */
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

  case 872: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 873: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 874: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 875: /* $@92: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 876: /* $@93: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 877: /* make_tuple_call: "tuple" '<' $@92 tuple_type_list '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 878: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 879: /* make_dim: make_dim semicolon make_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 880: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 881: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
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

  case 882: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
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

  case 883: /* $@94: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@95: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 885: /* make_dim_decl: "array" "struct" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 886: /* $@96: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@97: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 888: /* make_dim_decl: "array" "tuple" '<' $@96 tuple_type_list '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 889: /* $@98: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 890: /* $@99: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 891: /* make_dim_decl: "array" "variant" '<' $@98 variant_type_list '>' $@99 '(' make_variant_dim ')'  */
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

  case 892: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 893: /* $@100: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 894: /* $@101: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 895: /* make_dim_decl: "array" '<' $@100 type_declaration_no_options '>' $@101 '(' optional_expr_list ')'  */
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

  case 896: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 897: /* $@102: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 898: /* $@103: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 899: /* make_dim_decl: "fixed_array" '<' $@102 type_declaration_no_options '>' $@103 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 900: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 901: /* make_table: make_table semicolon make_map_tuple  */
                                                      {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 902: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 903: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 904: /* make_table_decl: open_block optional_expr_map_tuple_list close_block  */
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

  case 905: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 906: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 907: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 908: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 909: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 910: /* array_comprehension_where: semicolon "where" expr  */
                                          { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 911: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 912: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 913: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 914: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 915: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                         {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 916: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 917: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                  {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 918: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']' ']'  */
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

  case 919: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where "end of code block" ']'  */
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

  case 920: /* array_comprehension: open_block "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block  */
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

  case 921: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block close_block  */
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


