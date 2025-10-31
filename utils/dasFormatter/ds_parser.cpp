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
  YYSYMBOL_optional_not_required = 230,    /* optional_not_required  */
  YYSYMBOL_module_declaration = 231,       /* module_declaration  */
  YYSYMBOL_character_sequence = 232,       /* character_sequence  */
  YYSYMBOL_string_constant = 233,          /* string_constant  */
  YYSYMBOL_format_string = 234,            /* format_string  */
  YYSYMBOL_optional_format_string = 235,   /* optional_format_string  */
  YYSYMBOL_236_1 = 236,                    /* $@1  */
  YYSYMBOL_string_builder_body = 237,      /* string_builder_body  */
  YYSYMBOL_string_builder = 238,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 239, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 240,              /* expr_reader  */
  YYSYMBOL_241_2 = 241,                    /* $@2  */
  YYSYMBOL_options_declaration = 242,      /* options_declaration  */
  YYSYMBOL_require_declaration = 243,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 244,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 245,      /* require_module_name  */
  YYSYMBOL_require_module = 246,           /* require_module  */
  YYSYMBOL_is_public_module = 247,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 248,       /* expect_declaration  */
  YYSYMBOL_expect_list = 249,              /* expect_list  */
  YYSYMBOL_expect_error = 250,             /* expect_error  */
  YYSYMBOL_expression_label = 251,         /* expression_label  */
  YYSYMBOL_expression_goto = 252,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 253,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 254,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 255,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 256, /* expression_else_one_liner  */
  YYSYMBOL_257_3 = 257,                    /* $@3  */
  YYSYMBOL_expression_if_one_liner = 258,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 259,  /* expression_if_then_else  */
  YYSYMBOL_260_4 = 260,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 261,      /* expression_for_loop  */
  YYSYMBOL_262_5 = 262,                    /* $@5  */
  YYSYMBOL_263_6 = 263,                    /* $@6  */
  YYSYMBOL_expression_unsafe = 264,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 265,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 266,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 267,    /* expression_with_alias  */
  YYSYMBOL_268_7 = 268,                    /* $@7  */
  YYSYMBOL_annotation_argument_value = 269, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 270, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 271, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 272,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 273, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 274,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 275, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 276, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 277,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 278,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 279, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 280, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 281,   /* optional_function_type  */
  YYSYMBOL_function_name = 282,            /* function_name  */
  YYSYMBOL_global_function_declaration = 283, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 284, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 285, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 286,     /* function_declaration  */
  YYSYMBOL_287_8 = 287,                    /* $@8  */
  YYSYMBOL_open_block = 288,               /* open_block  */
  YYSYMBOL_close_block = 289,              /* close_block  */
  YYSYMBOL_expression_block = 290,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 291,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 292,           /* expression_any  */
  YYSYMBOL_expressions = 293,              /* expressions  */
  YYSYMBOL_expr_keyword = 294,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 295,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 296, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 297, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 298, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 299,       /* expression_keyword  */
  YYSYMBOL_300_9 = 300,                    /* $@9  */
  YYSYMBOL_301_10 = 301,                   /* $@10  */
  YYSYMBOL_302_11 = 302,                   /* $@11  */
  YYSYMBOL_303_12 = 303,                   /* $@12  */
  YYSYMBOL_expr_pipe = 304,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 305,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 306,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 307,     /* new_type_declaration  */
  YYSYMBOL_308_13 = 308,                   /* $@13  */
  YYSYMBOL_309_14 = 309,                   /* $@14  */
  YYSYMBOL_expr_new = 310,                 /* expr_new  */
  YYSYMBOL_expression_break = 311,         /* expression_break  */
  YYSYMBOL_expression_continue = 312,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 313, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 314,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 315, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 316,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 317,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 318,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 319,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 320,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 321,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 322, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 323,           /* expression_let  */
  YYSYMBOL_expr_cast = 324,                /* expr_cast  */
  YYSYMBOL_325_15 = 325,                   /* $@15  */
  YYSYMBOL_326_16 = 326,                   /* $@16  */
  YYSYMBOL_327_17 = 327,                   /* $@17  */
  YYSYMBOL_328_18 = 328,                   /* $@18  */
  YYSYMBOL_329_19 = 329,                   /* $@19  */
  YYSYMBOL_330_20 = 330,                   /* $@20  */
  YYSYMBOL_expr_type_decl = 331,           /* expr_type_decl  */
  YYSYMBOL_332_21 = 332,                   /* $@21  */
  YYSYMBOL_333_22 = 333,                   /* $@22  */
  YYSYMBOL_expr_type_info = 334,           /* expr_type_info  */
  YYSYMBOL_expr_list = 335,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 336,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 337,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 338,            /* capture_entry  */
  YYSYMBOL_capture_list = 339,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 340,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 341,               /* expr_block  */
  YYSYMBOL_expr_full_block = 342,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 343, /* expr_full_block_assumed_piped  */
  YYSYMBOL_344_23 = 344,                   /* $@23  */
  YYSYMBOL_expr_numeric_const = 345,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 346,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 347,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 348,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 349,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 350,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 351,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 352,           /* func_addr_expr  */
  YYSYMBOL_353_24 = 353,                   /* $@24  */
  YYSYMBOL_354_25 = 354,                   /* $@25  */
  YYSYMBOL_355_26 = 355,                   /* $@26  */
  YYSYMBOL_356_27 = 356,                   /* $@27  */
  YYSYMBOL_expr_field = 357,               /* expr_field  */
  YYSYMBOL_358_28 = 358,                   /* $@28  */
  YYSYMBOL_359_29 = 359,                   /* $@29  */
  YYSYMBOL_expr_call = 360,                /* expr_call  */
  YYSYMBOL_expr2 = 361,                    /* expr2  */
  YYSYMBOL_362_30 = 362,                   /* $@30  */
  YYSYMBOL_363_31 = 363,                   /* $@31  */
  YYSYMBOL_364_32 = 364,                   /* $@32  */
  YYSYMBOL_365_33 = 365,                   /* $@33  */
  YYSYMBOL_366_34 = 366,                   /* $@34  */
  YYSYMBOL_367_35 = 367,                   /* $@35  */
  YYSYMBOL_expr = 368,                     /* expr  */
  YYSYMBOL_expr_mtag = 369,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 370, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 371,        /* optional_override  */
  YYSYMBOL_optional_constant = 372,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 373, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 374, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 375, /* structure_variable_declaration  */
  YYSYMBOL_opt_sem = 376,                  /* opt_sem  */
  YYSYMBOL_struct_variable_declaration_list = 377, /* struct_variable_declaration_list  */
  YYSYMBOL_378_36 = 378,                   /* $@36  */
  YYSYMBOL_379_37 = 379,                   /* $@37  */
  YYSYMBOL_380_38 = 380,                   /* $@38  */
  YYSYMBOL_function_argument_declaration_no_type = 381, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 382, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 383,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 384,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 385,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 386,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 387,             /* variant_type  */
  YYSYMBOL_variant_type_list = 388,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 389,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 390,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 391, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 392, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 393,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 394,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 395,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 396, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 397, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 398, /* global_variable_declaration_list  */
  YYSYMBOL_399_39 = 399,                   /* $@39  */
  YYSYMBOL_optional_shared = 400,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 401, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 402,               /* global_let  */
  YYSYMBOL_403_40 = 403,                   /* $@40  */
  YYSYMBOL_enum_list = 404,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 405, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 406,             /* single_alias  */
  YYSYMBOL_407_41 = 407,                   /* $@41  */
  YYSYMBOL_alias_list = 408,               /* alias_list  */
  YYSYMBOL_alias_declaration = 409,        /* alias_declaration  */
  YYSYMBOL_410_42 = 410,                   /* $@42  */
  YYSYMBOL_optional_public_or_private_enum = 411, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 412,                /* enum_name  */
  YYSYMBOL_enum_declaration = 413,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 414, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 415,          /* optional_sealed  */
  YYSYMBOL_structure_name = 416,           /* structure_name  */
  YYSYMBOL_class_or_struct = 417,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 418, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 419, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 420,    /* structure_declaration  */
  YYSYMBOL_421_43 = 421,                   /* $@43  */
  YYSYMBOL_422_44 = 422,                   /* $@44  */
  YYSYMBOL_variable_name_with_pos_list = 423, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 424,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 425, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 426, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 427,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 428,            /* bitfield_bits  */
  YYSYMBOL_commas = 429,                   /* commas  */
  YYSYMBOL_bitfield_alias_bits = 430,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 431, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 432, /* bitfield_type_declaration  */
  YYSYMBOL_433_45 = 433,                   /* $@45  */
  YYSYMBOL_434_46 = 434,                   /* $@46  */
  YYSYMBOL_c_or_s = 435,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 436,          /* table_type_pair  */
  YYSYMBOL_dim_list = 437,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 438, /* type_declaration_no_options  */
  YYSYMBOL_439_47 = 439,                   /* $@47  */
  YYSYMBOL_440_48 = 440,                   /* $@48  */
  YYSYMBOL_441_49 = 441,                   /* $@49  */
  YYSYMBOL_442_50 = 442,                   /* $@50  */
  YYSYMBOL_443_51 = 443,                   /* $@51  */
  YYSYMBOL_444_52 = 444,                   /* $@52  */
  YYSYMBOL_445_53 = 445,                   /* $@53  */
  YYSYMBOL_446_54 = 446,                   /* $@54  */
  YYSYMBOL_447_55 = 447,                   /* $@55  */
  YYSYMBOL_448_56 = 448,                   /* $@56  */
  YYSYMBOL_449_57 = 449,                   /* $@57  */
  YYSYMBOL_450_58 = 450,                   /* $@58  */
  YYSYMBOL_451_59 = 451,                   /* $@59  */
  YYSYMBOL_452_60 = 452,                   /* $@60  */
  YYSYMBOL_453_61 = 453,                   /* $@61  */
  YYSYMBOL_454_62 = 454,                   /* $@62  */
  YYSYMBOL_455_63 = 455,                   /* $@63  */
  YYSYMBOL_456_64 = 456,                   /* $@64  */
  YYSYMBOL_457_65 = 457,                   /* $@65  */
  YYSYMBOL_458_66 = 458,                   /* $@66  */
  YYSYMBOL_459_67 = 459,                   /* $@67  */
  YYSYMBOL_460_68 = 460,                   /* $@68  */
  YYSYMBOL_461_69 = 461,                   /* $@69  */
  YYSYMBOL_462_70 = 462,                   /* $@70  */
  YYSYMBOL_463_71 = 463,                   /* $@71  */
  YYSYMBOL_464_72 = 464,                   /* $@72  */
  YYSYMBOL_465_73 = 465,                   /* $@73  */
  YYSYMBOL_type_declaration = 466,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 467,  /* tuple_alias_declaration  */
  YYSYMBOL_468_74 = 468,                   /* $@74  */
  YYSYMBOL_469_75 = 469,                   /* $@75  */
  YYSYMBOL_470_76 = 470,                   /* $@76  */
  YYSYMBOL_471_77 = 471,                   /* $@77  */
  YYSYMBOL_variant_alias_declaration = 472, /* variant_alias_declaration  */
  YYSYMBOL_473_78 = 473,                   /* $@78  */
  YYSYMBOL_474_79 = 474,                   /* $@79  */
  YYSYMBOL_475_80 = 475,                   /* $@80  */
  YYSYMBOL_476_81 = 476,                   /* $@81  */
  YYSYMBOL_bitfield_alias_declaration = 477, /* bitfield_alias_declaration  */
  YYSYMBOL_478_82 = 478,                   /* $@82  */
  YYSYMBOL_479_83 = 479,                   /* $@83  */
  YYSYMBOL_make_decl = 480,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 481,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 482,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 483,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 484,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 485,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 486,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 487, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 488,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 489, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 490, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 491, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 492, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 493, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 494,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 495,         /* make_struct_decl  */
  YYSYMBOL_496_84 = 496,                   /* $@84  */
  YYSYMBOL_497_85 = 497,                   /* $@85  */
  YYSYMBOL_498_86 = 498,                   /* $@86  */
  YYSYMBOL_499_87 = 499,                   /* $@87  */
  YYSYMBOL_500_88 = 500,                   /* $@88  */
  YYSYMBOL_501_89 = 501,                   /* $@89  */
  YYSYMBOL_502_90 = 502,                   /* $@90  */
  YYSYMBOL_503_91 = 503,                   /* $@91  */
  YYSYMBOL_make_tuple = 504,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 505,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 506,          /* make_tuple_call  */
  YYSYMBOL_507_92 = 507,                   /* $@92  */
  YYSYMBOL_508_93 = 508,                   /* $@93  */
  YYSYMBOL_make_dim = 509,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 510,            /* make_dim_decl  */
  YYSYMBOL_511_94 = 511,                   /* $@94  */
  YYSYMBOL_512_95 = 512,                   /* $@95  */
  YYSYMBOL_513_96 = 513,                   /* $@96  */
  YYSYMBOL_514_97 = 514,                   /* $@97  */
  YYSYMBOL_515_98 = 515,                   /* $@98  */
  YYSYMBOL_516_99 = 516,                   /* $@99  */
  YYSYMBOL_517_100 = 517,                  /* $@100  */
  YYSYMBOL_518_101 = 518,                  /* $@101  */
  YYSYMBOL_519_102 = 519,                  /* $@102  */
  YYSYMBOL_520_103 = 520,                  /* $@103  */
  YYSYMBOL_make_table = 521,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 522,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 523,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 524, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 525,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 526       /* array_comprehension  */
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
#define YYLAST   14242

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  224
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  303
/* YYNRULES -- Number of rules.  */
#define YYNRULES  943
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1793

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
       0,   582,   582,   583,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   604,   607,   610,
     616,   617,   618,   622,   623,   627,   628,   632,   651,   652,
     653,   654,   658,   659,   663,   664,   668,   669,   669,   673,
     678,   687,   702,   718,   723,   731,   731,   772,   798,   802,
     803,   804,   808,   811,   815,   821,   830,   833,   839,   840,
     844,   848,   849,   853,   856,   862,   868,   871,   877,   878,
     882,   883,   884,   894,   907,   908,   912,   913,   913,   919,
     920,   921,   922,   923,   927,   937,   947,   947,   955,   955,
     959,   959,   968,   976,   988,   998,   998,  1002,  1008,  1009,
    1010,  1011,  1012,  1013,  1017,  1022,  1030,  1031,  1032,  1036,
    1037,  1038,  1039,  1040,  1041,  1042,  1043,  1049,  1052,  1058,
    1061,  1064,  1070,  1071,  1072,  1073,  1077,  1090,  1108,  1111,
    1119,  1130,  1141,  1152,  1155,  1162,  1166,  1173,  1174,  1178,
    1179,  1180,  1184,  1187,  1194,  1198,  1199,  1200,  1201,  1202,
    1203,  1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,
    1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,  1222,
    1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,  1231,  1232,
    1233,  1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,
    1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,
    1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,
    1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,
    1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,
    1297,  1315,  1316,  1317,  1321,  1327,  1327,  1344,  1345,  1348,
    1349,  1352,  1359,  1383,  1401,  1410,  1416,  1417,  1418,  1419,
    1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,
    1430,  1431,  1432,  1433,  1434,  1435,  1436,  1437,  1441,  1446,
    1452,  1458,  1470,  1471,  1475,  1476,  1480,  1481,  1485,  1489,
    1496,  1496,  1496,  1502,  1502,  1502,  1511,  1545,  1553,  1560,
    1567,  1573,  1574,  1585,  1589,  1592,  1600,  1600,  1600,  1603,
    1609,  1612,  1616,  1620,  1627,  1634,  1640,  1644,  1648,  1651,
    1654,  1662,  1665,  1668,  1676,  1679,  1687,  1690,  1693,  1701,
    1713,  1714,  1715,  1719,  1720,  1724,  1725,  1729,  1734,  1742,
    1753,  1759,  1774,  1786,  1789,  1795,  1795,  1795,  1798,  1798,
    1798,  1803,  1803,  1803,  1811,  1811,  1811,  1817,  1831,  1847,
    1865,  1875,  1886,  1901,  1904,  1910,  1911,  1918,  1929,  1930,
    1931,  1935,  1936,  1937,  1938,  1939,  1943,  1948,  1956,  1957,
    1970,  1974,  1984,  1991,  1998,  1998,  2007,  2008,  2009,  2010,
    2011,  2012,  2013,  2017,  2018,  2019,  2020,  2021,  2022,  2023,
    2024,  2025,  2026,  2027,  2028,  2029,  2030,  2031,  2032,  2033,
    2034,  2035,  2039,  2049,  2058,  2067,  2072,  2073,  2074,  2075,
    2076,  2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,  2085,
    2086,  2087,  2088,  2093,  2100,  2112,  2117,  2127,  2131,  2138,
    2141,  2141,  2141,  2146,  2146,  2146,  2159,  2163,  2167,  2172,
    2179,  2188,  2193,  2200,  2200,  2200,  2207,  2211,  2221,  2230,
    2239,  2243,  2246,  2252,  2253,  2254,  2255,  2256,  2257,  2258,
    2259,  2260,  2261,  2262,  2263,  2264,  2265,  2266,  2267,  2268,
    2269,  2270,  2271,  2272,  2273,  2274,  2275,  2276,  2277,  2278,
    2279,  2280,  2287,  2288,  2289,  2290,  2291,  2292,  2293,  2294,
    2295,  2296,  2297,  2298,  2305,  2306,  2307,  2308,  2309,  2310,
    2311,  2312,  2326,  2327,  2328,  2329,  2330,  2333,  2336,  2337,
    2337,  2337,  2340,  2345,  2349,  2353,  2353,  2353,  2358,  2361,
    2365,  2365,  2365,  2370,  2373,  2374,  2375,  2376,  2377,  2378,
    2379,  2380,  2381,  2383,  2387,  2395,  2400,  2404,  2413,  2414,
    2415,  2416,  2417,  2418,  2419,  2423,  2427,  2431,  2435,  2439,
    2443,  2447,  2451,  2455,  2462,  2463,  2472,  2476,  2477,  2478,
    2482,  2483,  2487,  2488,  2489,  2493,  2494,  2498,  2509,  2510,
    2511,  2512,  2517,  2520,  2520,  2539,  2538,  2554,  2553,  2567,
    2576,  2588,  2597,  2607,  2608,  2609,  2610,  2611,  2615,  2618,
    2627,  2628,  2632,  2635,  2638,  2654,  2663,  2664,  2668,  2671,
    2674,  2688,  2689,  2693,  2699,  2705,  2714,  2717,  2724,  2727,
    2733,  2734,  2735,  2739,  2740,  2744,  2751,  2756,  2765,  2771,
    2782,  2785,  2790,  2795,  2803,  2813,  2824,  2827,  2827,  2847,
    2848,  2852,  2853,  2854,  2858,  2865,  2865,  2884,  2887,  2903,
    2923,  2924,  2925,  2930,  2930,  2960,  2963,  2970,  2980,  2980,
    2984,  2985,  2986,  2990,  3000,  3020,  3043,  3044,  3048,  3049,
    3053,  3059,  3060,  3061,  3062,  3066,  3067,  3068,  3072,  3075,
    3086,  3091,  3086,  3111,  3118,  3123,  3132,  3138,  3149,  3150,
    3151,  3152,  3153,  3154,  3155,  3156,  3157,  3158,  3159,  3160,
    3161,  3162,  3163,  3164,  3165,  3166,  3167,  3168,  3169,  3170,
    3171,  3172,  3173,  3174,  3175,  3179,  3180,  3181,  3182,  3183,
    3184,  3185,  3186,  3190,  3201,  3205,  3212,  3224,  3231,  3241,
    3242,  3247,  3252,  3266,  3276,  3286,  3296,  3306,  3319,  3320,
    3321,  3322,  3323,  3327,  3327,  3327,  3341,  3342,  3346,  3350,
    3357,  3361,  3368,  3369,  3370,  3371,  3372,  3387,  3393,  3393,
    3393,  3397,  3402,  3409,  3409,  3416,  3420,  3424,  3429,  3434,
    3439,  3444,  3448,  3452,  3457,  3461,  3465,  3470,  3470,  3470,
    3476,  3483,  3483,  3483,  3488,  3488,  3488,  3494,  3494,  3494,
    3499,  3504,  3504,  3504,  3509,  3509,  3509,  3518,  3523,  3523,
    3523,  3528,  3528,  3528,  3537,  3542,  3542,  3542,  3547,  3547,
    3547,  3556,  3556,  3556,  3562,  3562,  3562,  3571,  3574,  3585,
    3601,  3601,  3606,  3615,  3601,  3644,  3644,  3649,  3659,  3644,
    3688,  3688,  3688,  3741,  3742,  3743,  3744,  3745,  3749,  3756,
    3763,  3769,  3775,  3782,  3789,  3795,  3804,  3807,  3813,  3821,
    3826,  3833,  3838,  3845,  3850,  3856,  3857,  3861,  3862,  3867,
    3868,  3872,  3873,  3877,  3878,  3882,  3883,  3884,  3888,  3889,
    3890,  3894,  3895,  3899,  3932,  3971,  3990,  4010,  4030,  4051,
    4051,  4051,  4059,  4059,  4059,  4066,  4066,  4066,  4077,  4077,
    4077,  4088,  4092,  4098,  4114,  4120,  4126,  4132,  4132,  4132,
    4146,  4151,  4158,  4178,  4206,  4230,  4230,  4230,  4240,  4240,
    4240,  4254,  4254,  4254,  4268,  4277,  4277,  4277,  4297,  4304,
    4304,  4304,  4314,  4319,  4326,  4329,  4335,  4355,  4374,  4382,
    4402,  4427,  4428,  4432,  4433,  4438,  4448,  4451,  4454,  4457,
    4465,  4474,  4486,  4496
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
  "module_name", "optional_not_required", "module_declaration",
  "character_sequence", "string_constant", "format_string",
  "optional_format_string", "$@1", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@2", "options_declaration",
  "require_declaration", "keyword_or_name", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_else_one_liner", "$@3", "expression_if_one_liner",
  "expression_if_then_else", "$@4", "expression_for_loop", "$@5", "$@6",
  "expression_unsafe", "expression_while_loop", "expression_with",
  "expression_with_alias", "$@7", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
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
  "bitfield_alias_bits", "bitfield_basic_type_declaration",
  "bitfield_type_declaration", "$@45", "$@46", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@47", "$@48", "$@49",
  "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58",
  "$@59", "$@60", "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67",
  "$@68", "$@69", "$@70", "$@71", "$@72", "$@73", "type_declaration",
  "tuple_alias_declaration", "$@74", "$@75", "$@76", "$@77",
  "variant_alias_declaration", "$@78", "$@79", "$@80", "$@81",
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

#define YYPACT_NINF (-1553)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-809)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1553,    49, -1553, -1553,    47,   -60,   378,   139, -1553,   118,
     354,   354,   354, -1553, -1553,   -15,    28, -1553, -1553, -1553,
     202, -1553, -1553, -1553,   336, -1553,    61, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553,   -53, -1553,   -20,
      11,    15, -1553, -1553, -1553, -1553,   378, -1553,    25, -1553,
   -1553, -1553,   354,   354, -1553, -1553,    61, -1553, -1553, -1553,
   -1553, -1553,    98,    96, -1553, -1553, -1553, -1553,    28,    28,
      28,   152, -1553,   889,    93, -1553, -1553, -1553, -1553,   841,
     856,   897,   861, -1553,   871,    60,    47,    83,   -60,   199,
     268, -1553,   863,   863, -1553,   278,   398,    21,   202,   873,
     285,   326,   340, -1553,   362,   404, -1553, -1553,   -14,    47,
      28,    28,    28,    28, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553,   527, -1553, -1553, -1553, -1553, -1553, -1553, -1553,   139,
   -1553, -1553, -1553, -1553, -1553,   862,   128, -1553, -1553, -1553,
   -1553,   556, -1553, -1553, -1553, -1553,   524, -1553, -1553, -1553,
   -1553,   398, -1553, -1553, -1553,   526, -1553, -1553, -1553, -1553,
   -1553,   568, -1553,    80, -1553,   427,   633,   889, 13935, -1553,
     312,   716, -1553,    -7, -1553, -1553, -1553,   900, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553,   219, -1553,   635, -1553, -1553,
     768, -1553,   676,   139,   139, -1553, -1553, 12946, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553,   844,   875, -1553,   658,   139,   822, -1553,
   -1553,   744, -1553,   626,    47,    47,   307,   622, -1553, -1553,
   -1553,   128, -1553, 10762, -1553,   766,   139, -1553, -1553,   752,
     771, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553,   790,   763, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553,   916, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,   797,   769,
   -1553, -1553,   -34,   788, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553,   139,   690,   794,   139, -1553,    -7,
     171, -1553,    47, -1553,   782,   967,   715, -1553, -1553,   815,
     817,   827,   833,   846,   853, -1553, -1553, -1553,   845, -1553,
   -1553, -1553, -1553, -1553,   737, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553,   866, -1553, -1553, -1553,
     868,   869, -1553, -1553, -1553, -1553,   890,   893,   870,   -15,
   -1553, -1553, -1553, -1553, -1553,  1058,   901, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,   902,   947,
   -1553,   880, -1553,   115,   912,   297,   882, 10762, -1553,  3157,
   -1553,   130, -1553,   -15, -1553, -1553, -1553,   622,   887, -1553,
   10244,   927,   933, 10762, -1553,   210, -1553, -1553, -1553, 10244,
   -1553, -1553,   934,   908,    -9,   596,   607, -1553, -1553, 10244,
     -45, -1553, -1553, -1553,    30, -1553, -1553, -1553,    41,  6288,
   -1553,   892,  2998,   709, 10611,   540, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, 10244,
   -1553, -1553,   634,   -22,   -22,   -22, -1553,   901, -1553,   910,
     911, 10244, -1553, -1553,   896, -1553, -1553,  1045,   -26,   917,
      46,  3992, -1553, -1553,   139,   114,  6496,   903, 10244,    36,
     921,   923,   906, -1553,   202,   926,   946,  6704,   246,   380,
     928, -1553,   631,   936,   943,  4200, 10244, 10244,   233,   233,
     233,   920,   925,   931,   932,   935,   941, -1553,  2008,  2638,
    6914, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,  7122,
     945, -1553,  1114, -1553, 10244, 10244, 10244, 10244, 10244,  5872,
   10244, -1553,   924, -1553, -1553,   202,   202, 10244,  1142, -1553,
   -1553, -1553, -1553, -1553, -1553,  7332,  1116, -1553, -1553, -1553,
     446, -1553,   -83,   202, -1553,   202,   202,   202, -1553,   202,
   -1553, -1553,  1092, -1553, -1553, -1553, -1553,   944, -1553, -1553,
     337, -1553, -1553, -1553, -1553, -1553, -1553, 13200,  1484, -1553,
     948, -1553, -1553, -1553, -1553, -1553, -1553,   690, 10244, -1553,
   -1553,    40,   398, -1553,   955,   975,   976, -1553,  1700, -1553,
    1121,   594, -1553, -1553, -1553,  4616, 10762, 10762, 10762,  2003,
   10762, 10762,   957, -1553, 10762,   658, 10762,   658, 10762,   658,
   10865,  1005,  2152, -1553, 10244, -1553, -1553, -1553, -1553,   961,
   -1553, -1553, 12495, 10244, -1553,  1058,   367, -1553,   -19, -1553,
   -1553,   563, -1553,   901,   626,   987,   563, -1553,   626,  2416,
     965,  1146, -1553, -1553,   135, -1553, -1553, -1553, -1553, -1553,
     139,   634,   971, -1553,   972, -1553, -1553,   -15,   659, -1553,
     992,   993,   994, -1553, 10244,  4616, -1553,  1003,  1066,  2732,
   14020,  1185, 10762, 10244, 10244, -1553, 10244,   139,  1028,  1008,
   -1553, -1553, 10244, -1553, -1553,  1014,  1038, -1553, 10244, -1553,
   -1553, 10244, -1553, -1553, 10244, -1553, 10762,  4616, -1553, 13978,
     554,   554,   989, -1553,   944, -1553, -1553, -1553, 10244, 10244,
   10244, 10244, 10244, 10244,   634,  3365,   634,  3574,   634,  1070,
   -1553,   672, -1553,   139, -1553,   842,   995,   554,   554,   -37,
     554,   554,   200,  1200,   997,  1023, 13200,  1023,   355, -1553,
   -1553,   139, -1553,   634,   626, -1553,  1024,   139, -1553, -1553,
   -1553,  4824, -1553, -1553, -1553, -1553, -1553, -1553,   180,    53,
     233, -1553, 13685, 13722, 10244, 10244, -1553, -1553, 10244, 10244,
   10244, 10244,  1047, 10244,   665, 10244, 10244, 10244, 10244, 10244,
   10244, 10244, 10244, 10244,  7540, 10244, 10244, 10244, 10244, 10244,
   10244, 10244, 10244, 10244, 10244, 13788, 10244,  5032,  5032,  5032,
    5032,  5032,  5032,  5032,  5032,  5032,  5032,  5032, 10244,  5032,
    5032,  5032,  5032,  5032,  5032, -1553,  7748, -1553,   398,    28,
    1213, -1553,    -7, -1553, 10762, -1553,  1051, -1553,  4616, -1553,
   13978,   746,   756,  1025,   449, -1553,   764,   765, -1553,  1052,
     772,   788,   778,   788,   779,   788, -1553,   275, -1553,   324,
   -1553, 10762,  1007, -1553, -1553, 12606, -1553, -1553, 10244,  1037,
   10762, -1553, -1553, 10762, -1553, -1553, -1553, 10244,  1060, -1553,
    1061, -1553, 10762, -1553, 10244, 10762, 10762, -1553,    26,   634,
   10762,  6080,  7956,  1062, 10244, 10762, -1553, -1553, -1553, 10762,
    1023, -1553,  1003, 10244, 10244, 10244, 10244, 10244, 10244, 10244,
   10244, 10244, 10244, 10244, 10244, 10244, 10244, 10244, 10244, 10244,
   10244,   139,   964,  1011, -1553, 10856, -1553,  1042, -1553, 10762,
   10762, 10975, 10762, -1553, -1553, 11010, 10762,  1023, 10762, 10865,
    1023,  1005,   939, -1553, 13978, -1553,    53, 11121, 11161, 11272,
   11307, 11418, 11458,    32,   233,  1018,   211,  3783,  5242,  8164,
    1096,  1046,    -3,   -49,  1048,   298,    39,  8372,    -3,   629,
      44, 10244,  1056, -1553, 10244, -1553, 10762, -1553, 10762, -1553,
   10244,   773,   634,   634,    51,   203, -1553, 10244, -1553,  1022,
    1027,  1029,   624, -1553, -1553,    91, 10244,    66, -1553, 10244,
   -1553, -1553,   944,   -78,  5452, -1553,   264,  1054,  1030,  1071,
    1071, -1553, -1553,  1032,    23,   658, -1553,  1055,  1033, -1553,
   -1553,  1064,  1035, -1553, -1553,   232,   232,  2245,  2245, 10612,
   10612,  1039,   190,  1050, -1553, 12646,    68,    68,   948,   232,
     232, 13534, 13386, 13497, 13237, 13821, 13089,  2273,  1296,  2902,
    2245,  2245,   636,   636,   190,   190,   190,   688, 10244,  1053,
    1057,   717, 10244,  1255,  1059, 12757,   233,   233,   233, -1553,
   -1553,   446, -1553,   446, -1553,   446, -1553,   446, -1553,   446,
   -1553,   446, -1553,   446, -1553,   446, -1553,   446, -1553,   446,
   -1553,   446, -1553, -1553,   446, -1553,   446, -1553,   446, -1553,
     446, -1553,   446, -1553,   446, -1553,   365, -1553,   235,   878,
    1198,   202,  1162, -1553, -1553, 13929, -1553, -1553, -1553, -1553,
   10762, -1553, -1553, -1553,   213, -1553,  1065, -1553,  1069, -1553,
    1072, -1553, 10865, -1553,  1005,   338,   901, -1553, -1553,   184,
   -1553,   901,   901, 11569, -1553,  1224,   -40, -1553,  1322,  1339,
   10244,    76,   792,   718,   439,  1067,  1074,  1101,  1076,   371,
    1078,   795, 10762, 10865,  1005,  1444,  1099, 13200, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553,  1063, 10762, -1553, -1553,
   10762, 10244,  1604,  1736, -1553,  2375, -1553,  2385,  1100,  2447,
     385,  1102,   460,    53,   658, -1553, -1553, -1553, -1553, -1553,
    1073, 10244, -1553, 10244, 10244, 10244,  5662, 12495,     5, 10244,
     726,   718,   -49, -1553, -1553,  1098, -1553, 10244, -1553,  1103,
   10244, -1553, 10244,   718,   742,  1107, -1553, -1553, 10244, -1553,
   -1553, -1553,   509,   511,  1081,    81,    87, 10244,   634,    88,
   -1553, 10244, 10244, 10762,   658,   139, -1553, -1553,   720, 10244,
   -1553,  1267, 10244, -1553,  3157,    53,   238, -1553,  1106,   341,
   10452, -1553,   726, -1553, -1553, -1553,   349,   440,    23,  1112,
    1118,  1110,  1158,  1159, -1553,   489,   788, -1553, 10244, -1553,
   10244,  8580, 10244, -1553,  1089,  1119, -1553, -1553, 10244,  1120,
   -1553, 12792, 10244,  8788,  1123, -1553, 12903, -1553,  8996, -1553,
   -1553, -1553, -1553, -1553,   202, -1553, -1553,   454, -1553,   121,
     398,    53, -1553, -1553, -1553, -1553,   901, -1553, -1553, -1553,
    1167, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553,  1124, 10762, -1553,  1037, -1553,  1171, 10244, -1553, -1553,
     394, 10244, -1553,  1115, -1553, -1553, -1553,   513, -1553,  1126,
    1173, -1553, -1553,  2621,   516,   534, -1553, -1553, 10244,  2745,
     901,   202, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553,   810,   788,  9204,   389, 11604, -1553, -1553,    -3,   -49,
   -1553,  1128,   256,  1046, -1553, -1553, -1553, -1553,  1048,   430,
      -3,  1130, -1553, -1553, -1553, -1553,   444, -1553, -1553, -1553,
    1168, 10244, 10244,   598,    89, 10244, 11715, 11755,  2764,   788,
   -1553, -1553,   139, -1553,   202,   662, -1553,   658, -1553,  1131,
    5452,  1177,  1135,   625,   443, -1553, -1553,  1181, -1553, -1553,
      23,  1141,   183, 10762, 11866, 10762, 11901, -1553,   445, 12012,
   -1553, 10244, 13348, 10244, -1553, 12052,  5452, -1553,   455, 10244,
   -1553, -1553, -1553,   504,   398,  1327,   121, -1553, -1553,   878,
   -1553,  1147, -1553, -1553, -1553, -1553, -1553, 10244,   901, -1553,
   -1553,  1148,  1149, -1553,   510, -1553, -1553, 10244,  1192, 10244,
    1169, -1553, -1553, -1553, -1553,  1150,  1151,  1154, -1553, 10244,
   10244, 10244,  1156,  1307,  1165,  1170,  9412, -1553,   183, -1553,
     548, 10244,   273,   -49, -1553, 10244, 10244, 10244, 10244,   742,
   -1553, 10244, 10244,  1172, -1553, -1553,   549,   558, 10244, 10244,
     696, -1553, -1553, -1553,  1180,   122,  4408, -1553, 10244,   788,
   -1553,   493, -1553,   652, 10762,   210, -1553,  1166, -1553, -1553,
    9620, -1553, -1553,  2928, -1553,   801, -1553, -1553, -1553, 10762,
   12163, 12198, -1553,   588, -1553, 12309, -1553, -1553, -1553, -1553,
    1327,   634,  1174,  1307,  1307,   139,  1175,  1186,  1176,  1179,
    1182,  1187,  1188, 10244, -1553, 10244, -1553, -1553, -1553, 10244,
   -1553, -1553,  1307,  1307, -1553, 12349, -1553, -1553, 13051, 10244,
   10244, -1553, 12460, -1553, -1553, 13051, -1553,   202, -1553, 10244,
   -1553,  1201,  1203,  1205, 13051,   560, 10244,   381, -1553,   720,
   -1553,  9828, 10036, -1553, -1553, -1553, -1553, -1553, -1553,   202,
     139,  1190, 10762,   210,   610, 10244, -1553, 10244, -1553, -1553,
   -1553,   806, -1553, -1553,  1191, -1553, 13935, -1553, -1553, -1553,
   -1553, -1553,   260,   260, -1553, -1553, 10244, -1553, 10244,  1307,
    1307,   718,  1209,  1211,  1023,   260,   718, -1553,  1340,  1218,
   -1553, -1553,   293,  1208,   626,  1215, -1553, 10244, 10244,  1219,
    1233, 13051, -1553,   381, -1553, 10244, 10244, -1553,   626, -1553,
   -1553,   610, 10244, 10244,   202, -1553, -1553, -1553, -1553, -1553,
     202, 13935,   718,  1046,  1230, -1553,  1221,  1222,  1229,  1231,
     260,   260,  1046,  1232, -1553, -1553,  1238,  1239,  1240, 10244,
    1220, 10244, 10244,  1226,   626, -1553,  1241,   202, 13051, -1553,
   10244,  1244, -1553, -1553, -1553, 10244,   202,   202, -1553, -1553,
     398,   139,   567,  1246, -1553, -1553, -1553, -1553, -1553,  1248,
    1249, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553,  1282,  1252, 13051, -1553,   202, -1553, -1553, -1553,
     398, -1553,   718, -1553, -1553, -1553, -1553,  1254, -1553, -1553,
     582, -1553, -1553
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   137,     1,   333,     0,     0,     0,   658,   334,     0,
     650,   650,   650,    17,    18,     0,     0,    16,    15,     3,
       0,    10,     9,     8,     0,     7,   639,     6,    11,     5,
       4,    13,    12,    14,   107,   108,   106,   115,   117,    47,
      63,    60,    61,    49,    50,    51,     0,    52,    58,    48,
     248,   247,   650,   650,    24,    23,   639,   652,   651,   830,
     820,   825,     0,   301,    45,   123,   124,   125,     0,     0,
       0,   126,   128,   135,     0,   122,    19,   672,   671,   241,
     660,     0,   675,   640,   641,     0,     0,     0,     0,    53,
       0,    59,     0,     0,    56,     0,   581,   650,     0,    20,
       0,     0,     0,   303,     0,     0,   134,   129,     0,     0,
       0,     0,     0,     0,   138,   243,   242,   245,   240,   662,
     661,     0,   674,   673,   677,   676,   680,   643,   642,   645,
     113,   114,   111,   112,   110,     0,     0,   109,   118,    64,
      62,    58,    55,    54,   653,   578,   579,   655,   250,   249,
     657,   581,   659,    21,    22,    25,   831,   821,   826,   302,
      43,    46,   133,     0,   130,   131,   132,   136,     0,   663,
       0,   668,   636,   564,    28,    29,    33,     0,   102,   103,
     100,   101,    99,    98,   104,     0,    57,     0,   580,   656,
       0,    27,   738,     0,     0,    44,   127,     0,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,     0,   144,   139,     0,     0,   647,
     669,     0,   681,   637,     0,     0,   566,     0,    30,    31,
      32,     0,   116,     0,    26,     0,     0,   822,   827,   209,
     210,   207,   147,   148,   150,   149,   151,   152,   153,   154,
     180,   181,   178,   179,   171,   182,   183,   172,   169,   170,
     208,   191,     0,   206,   184,   185,   186,   187,   158,   159,
     160,   155,   156,   157,   168,     0,   174,   175,   173,   166,
     167,   162,   161,   163,   164,   165,   146,   145,   190,     0,
     176,   177,   564,   142,   278,   246,   715,   718,   721,   722,
     716,   719,   717,   720,     0,     0,   666,   678,   644,   564,
       0,   119,     0,   121,     0,   625,   623,   646,   105,     0,
       0,     0,     0,     0,     0,   688,   708,   689,   724,   690,
     694,   695,   696,   697,   714,   701,   702,   703,   704,   705,
     706,   707,   709,   710,   711,   712,   790,   693,   700,   713,
     797,   804,   691,   698,   692,   699,     0,     0,     0,     0,
     723,   752,   755,   753,   754,   817,   654,   741,   742,   739,
     740,   731,   602,   608,   211,   212,   205,   189,   213,   192,
     188,     0,   140,   332,   593,   594,     0,     0,   244,     0,
     647,   581,   664,     0,   670,   582,   682,     0,     0,   120,
       0,     0,     0,     0,   624,     0,   758,   781,   784,     0,
     787,   777,     0,     0,   791,   798,   805,   811,   814,     0,
       0,   767,   772,   766,     0,   780,   776,   769,     0,     0,
     771,   756,     0,     0,   823,   828,   214,   194,   195,   197,
     196,   198,   199,   200,   201,   193,   202,   203,   204,     0,
     330,   331,     0,   564,   564,   564,   141,   143,   280,     0,
       0,     0,    74,    75,    88,   509,   510,     0,     0,     0,
       0,   318,   504,   316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,     0,     0,     0,     0,     0,
       0,   714,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   553,     0,     0,
       0,   256,   386,   388,   387,   389,   390,   391,   392,     0,
       0,    39,   247,   257,     0,     0,     0,     0,     0,   282,
       0,   368,   369,   507,   506,     0,     0,     0,     0,   273,
     268,   265,   264,   266,   267,   286,   251,   300,   279,   259,
     546,   258,   463,     0,   537,    82,    83,    80,   271,    81,
     272,   274,   336,   263,   536,   535,   534,   137,   540,   505,
       0,   260,   539,   538,   512,   464,   513,   547,   393,   465,
       0,   508,   833,   837,   834,   835,   836,     0,     0,   648,
     667,   583,   581,   565,     0,     0,     0,   546,     0,   627,
     628,     0,   621,   622,   620,     0,     0,     0,     0,     0,
       0,     0,     0,   743,     0,   139,     0,   139,     0,   139,
       0,     0,     0,   763,   282,   774,   775,   768,   770,     0,
     773,   757,     0,     0,   819,   818,   734,   832,   301,   746,
     747,     0,   603,   598,     0,     0,     0,   609,     0,     0,
       0,   683,   590,   591,   613,   595,   596,   597,   879,   882,
       0,     0,     0,   306,   310,   309,   315,     0,     0,   354,
       0,     0,     0,   915,     0,     0,   322,   319,     0,   547,
     393,     0,     0,   286,     0,   304,     0,     0,     0,     0,
     345,   348,     0,   277,   351,     0,     0,    67,     0,    92,
     919,     0,   888,   897,     0,   885,     0,     0,   327,   324,
     492,   493,   369,   381,   137,   299,   297,   298,     0,     0,
       0,     0,     0,     0,     0,   857,     0,     0,     0,   895,
     922,     0,   290,     0,   293,     0,     0,   469,   468,   502,
     467,   466,     0,     0,     0,   933,   363,   933,   370,   275,
     276,     0,    86,     0,     0,   924,   933,     0,   384,   254,
     544,     0,   262,   269,   270,   321,   326,   335,     0,   378,
       0,   261,     0,     0,     0,     0,   494,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   253,     0,   665,   581,     0,
       0,   679,   564,   638,     0,   626,     0,   630,     0,   635,
     634,     0,     0,     0,   748,   761,     0,     0,   725,     0,
       0,   142,     0,   142,     0,   142,   600,     0,   606,     0,
     726,     0,     0,   765,   750,     0,   732,   729,     0,   733,
       0,   604,   824,     0,   610,   829,   592,     0,     0,   612,
       0,   611,     0,   614,     0,     0,     0,    93,     0,     0,
       0,   871,     0,     0,     0,     0,   905,   908,   911,     0,
     933,   323,   320,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   305,     0,    94,     0,    95,     0,
       0,     0,     0,    65,    66,     0,     0,   933,     0,     0,
     933,     0,     0,   328,   325,   370,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
     891,   849,   857,     0,   900,     0,     0,     0,   857,     0,
       0,     0,     0,   860,     0,   927,     0,   281,     0,    42,
       0,    40,     0,     0,     0,     0,   902,   934,   283,     0,
       0,     0,   440,   437,   439,    70,     0,     0,   926,   934,
     287,   278,   137,     0,   282,   456,     0,   933,     0,     0,
       0,   344,   343,     0,     0,   139,   296,     0,     0,   523,
     522,     0,     0,   524,   528,   470,   471,   483,   484,   481,
     482,     0,   518,     0,   500,     0,   541,   542,   543,   472,
     473,   488,   489,   490,   491,     0,     0,   486,   487,   485,
     479,   480,   475,   474,   476,   477,   478,     0,     0,     0,
     446,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     424,   403,   425,   404,   427,   406,   426,   405,   428,   407,
     418,   397,   419,   398,   420,   399,   429,   408,   430,   409,
     417,   395,   396,   431,   410,   432,   411,   421,   400,   422,
     401,   423,   402,   416,   394,   461,     0,   649,     0,   572,
     575,     0,     0,   629,   632,   393,   633,   759,   782,   785,
       0,   788,   778,   727,     0,   792,     0,   799,     0,   806,
       0,   812,     0,   815,     0,     0,   288,   762,   751,   737,
     730,   599,   605,     0,   685,   686,   616,   615,     0,     0,
       0,     0,     0,   872,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   364,   403,   404,
     406,   405,   407,   397,   398,   399,   408,   409,   395,   410,
     411,   400,   401,   402,   394,   329,     0,     0,   928,   514,
       0,     0,     0,     0,   515,     0,   545,     0,     0,     0,
       0,     0,     0,   378,   139,   548,   549,   550,   551,   552,
       0,     0,   858,     0,     0,     0,     0,   363,   857,     0,
       0,     0,     0,   866,   867,     0,   874,     0,   864,     0,
       0,   903,     0,     0,     0,     0,   862,   904,     0,   894,
     859,   923,     0,     0,    36,     0,     0,     0,     0,     0,
     511,     0,     0,     0,   139,     0,    68,    69,    70,     0,
      85,    76,     0,   925,     0,   378,     0,   457,     0,     0,
       0,   460,   934,   848,   458,   337,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,   142,   519,     0,   525,
       0,     0,     0,   498,     0,     0,   529,   533,     0,     0,
     501,     0,     0,     0,     0,   447,     0,   454,     0,   496,
     414,   412,   413,   462,   138,   573,   574,   575,   576,   567,
     581,   378,   631,   760,   783,   786,   749,   789,   779,   744,
       0,   793,   795,   800,   802,   807,   809,   813,   601,   816,
     607,     0,     0,   735,   736,   684,     0,     0,   880,   883,
       0,     0,   307,     0,   312,   313,   311,     0,   357,     0,
       0,   360,   355,     0,     0,     0,   916,   914,   286,     0,
      97,     0,   346,   349,   352,   920,   918,   889,   898,   896,
     886,     0,   142,     0,     0,     0,   839,   838,   857,     0,
     892,     0,     0,   850,   873,   865,   893,   863,   901,     0,
     857,     0,   869,   870,   877,   861,     0,   291,   294,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
      71,    84,     0,    77,     0,     0,   252,   139,   459,     0,
     282,     0,     0,   623,     0,   373,   374,     0,   372,   371,
       0,     0,     0,     0,     0,     0,     0,   435,     0,     0,
     530,     0,   503,     0,   499,     0,   282,   448,     0,     0,
     497,   455,   451,     0,   581,   570,   567,   568,   569,   572,
     584,     0,   745,   728,   796,   803,   810,   282,   289,   687,
     617,     0,     0,    89,     0,   308,   314,     0,     0,     0,
       0,   356,   906,   909,   912,     0,     0,     0,    96,     0,
       0,     0,     0,   871,     0,     0,     0,   255,     0,   554,
       0,     0,     0,     0,   875,     0,     0,     0,     0,     0,
     868,     0,     0,   284,    34,    41,     0,     0,     0,     0,
       0,   438,   563,   441,     0,    70,     0,    87,     0,   142,
     433,     0,   338,   623,     0,     0,   380,     0,   377,   379,
       0,   365,   383,     0,   562,     0,   560,   436,   557,     0,
       0,     0,   556,     0,   449,     0,   452,   589,   571,   585,
     570,     0,     0,   871,   871,     0,     0,     0,     0,     0,
       0,     0,     0,   282,   929,   286,   347,   350,   353,     0,
     872,   890,   871,   871,   516,     0,   382,   555,   931,     0,
       0,   876,     0,   841,   840,   931,   878,   931,   292,   282,
     295,    38,     0,     0,   931,     0,     0,     0,   444,    70,
      72,   318,     0,    78,    82,    83,    80,    81,    79,   931,
       0,     0,     0,     0,     0,     0,   375,     0,   366,   520,
     526,     0,   561,   559,     0,   558,     0,   587,   619,   618,
     577,   764,   856,   856,    91,   358,     0,   361,     0,   871,
     871,   846,     0,     0,   933,   856,   846,   517,     0,     0,
     843,   842,     0,     0,     0,   933,    35,     0,     0,     0,
       0,   931,   442,     0,    73,     0,     0,   324,     0,   385,
     434,     0,     0,     0,     0,   367,   521,   527,   531,   450,
       0,     0,     0,   853,   933,   855,     0,     0,     0,     0,
     856,   856,   847,     0,   917,   930,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   934,     0,   931,   931,   935,
       0,     0,   445,   325,   942,     0,     0,     0,   342,   532,
     581,     0,     0,   934,   854,   881,   884,   359,   362,     0,
       0,   913,   921,   899,   887,   932,   940,   845,   844,   941,
     943,   285,     0,     0,   931,   939,     0,   341,   340,   586,
     581,   851,     0,   907,   910,   938,   936,     0,   339,   588,
       0,   937,   852
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1553, -1553,    -1, -1553, -1553, -1553, -1553, -1553,   694,  1388,
   -1553, -1553, -1553, -1553, -1553, -1553,  1473, -1553, -1553, -1553,
     956,  1429, -1553,  1335, -1553, -1553,  1390, -1553, -1553, -1553,
   -1238, -1553, -1553, -1553,   -67, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553,  1242, -1553, -1553,   -52,   -32,
   -1553, -1553, -1553,   539,   641,  -548,  -602,  -823, -1553, -1553,
   -1553, -1552, -1553, -1553,    -4,  -227,  -223,  -387, -1553,   470,
   -1553,  -625, -1553,  -683,  -204,  -229, -1553, -1553, -1553, -1553,
    -440,     0, -1553, -1553, -1553, -1553, -1553,   -62,   -61,   -59,
   -1553,   -57, -1553, -1553, -1553,  1485, -1553,   471, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553,  -290,   -28,   894,    42,   205,  -930,  -437, -1553,  -555,
   -1553, -1553,  -386,   481, -1553, -1553, -1553, -1490, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553,  1529, -1553, -1553,
   -1553, -1553, -1553, -1553,   959, -1553,  -151,    27,   -72,    31,
     182, -1553,  -150, -1553, -1553, -1553, -1553, -1553, -1553,   618,
    -427,  -913, -1553,  -424,  -906, -1553,  -656,   -70,   -66, -1553,
    -600, -1358, -1553,  -376, -1553, -1553,  1460, -1553, -1553, -1553,
    1122,  1075,   195, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,  -636,
     388, -1553,  1043, -1553, -1553,   370, -1553,  1183, -1553, -1553,
   -1553,  -384, -1553, -1553,  -399, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553,  -236, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553,  1049,  -717,  -147,  -857,
    -713, -1553, -1553, -1385,  -939, -1553, -1553, -1553, -1200,     2,
    -784, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
     292,  -518, -1553, -1553, -1553,   796, -1553, -1553, -1553, -1553,
   -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553, -1553,
   -1312,  -729, -1553
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   650,    18,   155,    56,   191,    19,   177,   183,
    1621,  1420,  1534,   745,   543,   161,   544,   105,    21,    22,
      47,    48,    49,    94,    23,    41,    42,   545,   546,  1269,
    1270,   547,  1434,  1546,   548,   549,  1006,   550,   671,   672,
     551,   552,   553,   554,  1201,   184,   185,    37,    38,    39,
     236,    71,    72,    73,    74,    24,   303,   398,   226,    25,
     117,   227,   118,   168,   555,   150,   723,  1079,   558,   399,
     559,   754,  1620,   764,  1145,   607,   986,  1532,   988,  1533,
     561,   562,   563,   674,   900,  1495,   564,   565,   566,   567,
     568,   569,   570,   571,   462,   572,   778,  1286,  1021,   573,
     574,   939,  1509,   940,  1510,   942,  1511,   575,   905,  1501,
     576,   755,  1562,   577,  1294,  1295,  1025,   725,   578,   835,
    1012,   579,   688,  1080,   581,   582,   583,  1004,   584,  1263,
    1627,  1264,  1693,   585,  1073,  1471,   586,   587,  1453,  1706,
    1455,  1707,  1569,  1749,   739,   589,   393,  1479,  1579,  1327,
    1329,  1121,   147,   601,   842,  1656,  1711,   394,   395,   396,
     866,   867,   444,   868,   869,   445,  1225,   662,   663,  1660,
     615,   415,   326,   327,   233,   319,    84,   129,    27,   173,
     315,    95,    96,   187,    97,    28,    53,   121,   170,    29,
     404,   231,   232,    82,   126,   406,    30,   171,   317,   664,
     590,   314,   372,   373,  1134,   879,   443,   246,   374,   859,
    1482,  1142,   853,   441,   375,   616,  1333,   871,   621,  1338,
     617,  1334,   618,  1335,   620,  1337,   624,  1341,   625,  1484,
     626,  1343,   627,  1485,   628,  1345,   629,  1486,   630,  1347,
     631,  1349,   653,    31,   101,   193,   382,   654,    32,   102,
     194,   383,   658,    33,   100,   192,   591,  1713,  1723,  1018,
     972,  1714,  1715,  1716,   973,   985,  1247,  1241,  1236,  1414,
    1166,   592,   895,  1491,   896,  1492,   951,  1515,   948,  1513,
     974,   765,   593,   949,  1514,   975,   594,  1172,  1590,  1173,
    1591,  1174,  1592,   909,  1505,   946,  1512,   741,   766,   595,
    1679,   998,   596
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   189,   740,    52,   305,   769,   318,   376,   894,   872,
     933,   848,   557,   580,   611,    64,    75,   651,   971,    76,
     971,   656,   237,   861,   978,   863,  1214,   865,   999,   779,
    1431,   602,  1404,  1232,   138,   898,  1210,  1010,  1136,  1244,
    1138,   686,  1140,   645,  1165,  1212,  1160,    90,  -137,     2,
     964,   680,  1221,   840,  1017,   698,     3,  1023,   964,  1242,
     652,   657,    65,   635,  1248,   718,    34,    35,    75,    75,
      75,  1257,   726,   727,   637,   130,   131,   163,    57,     4,
     965,     5,    91,     6,    58,  1555,  1272,   889,   402,     7,
     770,    66,   794,   795,   557,  1276,  1361,   152,   963,     8,
     976,  1421,   980,  1265,  1710,     9,    40,  1422,  1425,  1539,
      75,    75,    75,    75,  1266,  1267,   994,    62,   557,   735,
     737,   391,   460,    83,   104,   172,   234,  1007,    62,    10,
     110,   111,   112,   391,  1265,    67,   771,  1692,   234,  1233,
    1234,    85,  1277,   178,   179,  1266,  1267,    63,   681,   682,
    1289,    11,    12,   234,   891,   633,  1278,   442,    63,  1751,
    1290,   467,    13,   965,    68,   461,   229,  1477,   407,  1235,
     560,    62,   556,    86,   634,   849,   956,   880,  1276,   815,
     816,  1176,    14,   321,  1017,   148,   392,  -794,   235,   247,
     248,   687,  -794,   677,   792,  1643,  1291,   794,   795,  1524,
     235,    63,   320,  1742,   148,   149,   162,    87,    88,   699,
    -794,   782,   783,  1024,    13,   235,   647,  1292,  1208,   890,
      36,  1211,  1293,   304,   149,   890,   132,  1478,   557,   636,
      92,   133,   890,   134,    14,   323,   135,   890,    69,   104,
     638,    93,   381,   370,   890,   911,   683,    70,    98,   139,
     757,   599,   560,   782,   783,    15,  1268,   839,   639,   890,
    1374,   691,   889,  1161,   640,   684,    16,   881,  1375,   890,
     409,   103,   884,    86,   890,   709,   560,   953,  1717,   136,
     890,   890,   890,  1391,   815,   816,   113,  1629,  1283,  1399,
    1727,    54,   151,   932,   180,   145,  1017,  1279,   557,   181,
     196,   182,    50,  1683,   135,  1684,  1363,  1630,   786,   787,
     400,   114,  1689,   405,   692,   146,   792,   952,   793,   794,
     795,   796,    51,  1611,   598,  1560,   797,  1698,   890,   891,
     557,   892,   324,   693,   893,  1759,  1760,   612,   889,    55,
    1019,    77,    78,  1026,    79,  1437,    50,   613,  1224,  1353,
     786,   787,   660,   325,   104,   660,  1255,  1256,   792,  1259,
     793,   794,   795,   796,    86,   889,    51,    13,   797,   430,
     837,   109,    80,   661,   841,  1224,   661,   877,    13,  1741,
     851,   852,   854,   889,   856,   857,   560,    14,   860,   408,
     862,  1694,   864,  1526,   910,   912,    50,   370,    14,  1020,
     889,  1481,  1022,   600,   614,   891,   815,   816,  1124,    50,
    1609,    57,   241,   370,  1339,    93,    51,    58,   965,   993,
     889,   947,  1258,  1296,   950,  1772,  1773,   882,   113,    51,
    1731,   885,   891,  1276,   810,   811,   812,   813,   814,   242,
      13,   141,   370,    81,   370,  1122,  1017,   897,   815,   816,
     891,   144,   843,  1324,   541,   722,   560,  1280,   156,  1523,
      14,   557,  1787,    13,  1275,   708,  1251,   891,   649,   780,
    1130,  1529,    13,  1452,   936,    50,  1141,   370,   678,  1712,
     304,  1016,   464,    14,  1281,  1144,  1238,   891,   560,    13,
     465,  1273,    14,   703,   304,    51,  1158,  1159,    62,   157,
    1357,  1475,    13,    13,   304,   304,   304,  1000,   228,    14,
    1175,   971,  1001,   158,  1403,  1398,  1239,   649,   370,   370,
     987,  1328,    14,    14,    62,  1143,   971,  1222,    63,   322,
    1410,   649,   876,  1000,  1230,   159,    13,  1008,  1005,  1351,
    1202,  1203,  1441,  1205,   759,   760,  1116,  1207,  1197,  1209,
      13,    43,    44,    45,    63,  1002,    14,    50,   913,  1439,
     877,   878,   772,   145,   773,   774,   775,  1442,   776,  1518,
      14,   110,  1369,   112,  1521,   782,   783,    51,   649,   781,
     710,   160,   913,   146,    46,  1323,  1388,   913,   560,   560,
     560,   560,   560,   560,   560,   560,   560,   560,   560,   711,
     560,   560,   560,   560,   560,   560,  1544,   106,   107,   108,
     847,  1164,  1392,    91,    13,  1528,   370,   370,   370,   560,
     370,   370,  1424,   913,   370,    13,   370,   431,   370,  1531,
     370,   371,   913,  1441,    14,  1146,  1450,   913,   913,  1320,
    1321,  1322,   649,   431,  1151,    14,   442,  1152,   913,   164,
     165,   166,   167,   649,   432,   433,  1156,   782,   783,  1364,
    1443,  1390,  1429,  1556,  1162,  1567,   304,   541,   722,  1171,
     432,   433,   786,   787,    13,  1574,    13,   902,    13,   757,
     792,    13,  1450,   794,   795,   796,  1230,   757,  1117,   188,
     797,  1120,   370,   304,    14,  1506,    14,   913,    14,    13,
     169,    14,   649,   913,   649,    13,   649,  1451,  1195,   649,
    1417,  1641,  1418,   655,  1497,  1348,   370,  1503,   434,    14,
    1350,   612,   435,  1551,  1576,    14,  1640,   649,    13,  1601,
    1585,   613,  1265,   649,   434,  1504,   190,   612,   435,   304,
     984,   913,   913,  1266,  1267,   195,  1527,   613,    14,  1573,
    1146,   913,  1146,   913,   786,   787,   649,   304,  1003,    13,
    1230,  1352,   792,  1011,   793,   794,   795,   796,  1607,  1622,
     815,   816,   797,  1373,   324,  1230,   304,   110,  1623,    14,
    1690,  1230,  1252,  1538,  1253,   371,   660,  1781,   614,   436,
     148,   913,  -801,   437,    13,   325,   438,  -801,  1379,  1662,
    1663,   371,  1792,  -808,   614,   436,  1654,   661,  -808,   437,
     149,   439,   438,  1245,    14,  -801,  1246,   440,  1675,  1676,
    -443,  1554,   230,  1043,   414,  -443,  -808,   439,  1144,   243,
     371,   713,   371,   440,   244,  1549,  1517,    13,  1044,    75,
     812,   813,   814,  -443,   370,   377,  1309,  1548,  1642,  1126,
     714,   414,   815,   816,   148,   913,   982,    14,   983,   903,
     378,  1310,  1582,   401,  1428,   379,  1610,   380,  1352,  1352,
    1360,   370,   245,   148,   149,  1314,   965,   302,   904,   300,
     370,  1626,   646,   370,  1401,  1720,  1721,   557,   580,   913,
    1315,  1276,   370,   149,  1336,   370,   370,   306,   115,  1402,
     370,   307,   122,   123,   116,   370,   371,   371,   412,   370,
     301,   413,  1673,   119,   414,   308,   309,   316,   124,   120,
     310,   311,   312,   313,   125,   384,  1411,   304,   127,  1412,
     153,  1394,  1413,   245,   128,  1325,   154,  -738,   388,   370,
     370,  1326,   370,   442,   385,  1726,   370,  1127,   370,   370,
     238,   239,  1409,   442,  1722,  1645,  1736,  1128,  1416,  1722,
     304,   442,   442,   386,  1380,  1131,  1132,  1423,  1672,   442,
     389,  1231,   431,  1135,  1240,   442,   442,  1231,  1240,  1137,
    1139,   387,  1435,  1498,   397,  1754,   370,   390,   370,   442,
     403,  1144,   442,  1362,  1685,  1752,  1372,   431,   442,   432,
     433,   410,  1650,   442,   371,   371,   371,  1708,   371,   371,
     411,  1458,   371,  1617,   371,   416,   371,   417,   371,   174,
     175,   989,   990,  1468,   432,   433,  1732,   418,  1473,  1516,
    1639,   541,   722,   110,   111,   112,    43,    44,    45,   174,
     175,   176,  1430,  1702,  1703,   560,   420,  1436,   142,   143,
     469,   470,   419,   421,  1563,   447,   448,   449,   450,   451,
     452,   453,   454,   434,   422,  1790,   424,   435,   425,   426,
     480,  1494,   304,   304,   304,   446,   485,   238,   239,   240,
     371,   665,   666,   667,   455,    59,    60,    61,   434,   429,
     427,   431,   435,   428,   456,   457,   458,   463,   442,   459,
     609,  1745,   466,  1520,   371,   603,   610,   622,   623,   643,
     668,   669,   705,   499,   500,   -90,  1488,   679,   432,   433,
    1330,   700,   696,   701,  1332,   702,   704,   746,   712,    13,
     370,  1536,  1537,  1340,   436,  1540,   715,  1493,   437,   728,
    1213,   438,   370,   716,   729,   744,   758,   502,   503,    14,
     730,   731,   762,   767,   732,  1644,   439,   649,   777,   436,
     733,    16,   440,   437,   846,  1196,   438,   836,  1370,  1737,
    1030,  1034,   370,   370,   708,   844,   742,   858,   655,   873,
    1480,   439,   434,   883,   887,  1048,   435,   440,    62,   888,
     899,   901,   906,   907,   908,   431,   913,   370,   780,   931,
     370,   937,   938,  1074,   944,   518,   519,   520,    50,  1545,
     943,   955,   981,   995,   992,   996,   997,  1009,    63,  1565,
    1041,  1119,   432,   433,  1123,  1133,  1129,  1147,   532,  1561,
    1150,  1198,   371,  1154,  1155,  1169,  1200,  1223,  1229,  1230,
    1250,  1237,  1260,  1701,  1285,   673,  1261,  1282,  1262,  1625,
    1284,  1288,  1298,   436,  1300,  1297,  1317,   437,  1301,   371,
     438,   304,   539,   370,  1299,  1328,  1342,  1356,   371,  1302,
    1344,   371,  1312,  1346,  1367,   439,  1313,  1419,  1318,  1433,
     371,   440,  1378,   371,   371,  1445,   434,  1365,   371,  1460,
     435,  1446,  1393,   371,  1366,  1561,  1368,   371,  1371,  1082,
    1084,  1086,  1088,  1090,  1092,  1094,  1096,  1098,  1100,  1674,
    1103,  1105,  1107,  1109,  1111,  1113,  1405,   782,   783,  1377,
    1386,  1407,  1389,  1474,  1577,  1415,  1438,   371,   371,  1447,
     371,  1448,  1449,  1651,   371,  1496,   371,   371,  1461,  1463,
    1483,   687,  1469,  1487,  1489,  1499,  1500,  1525,  1530,  1535,
    1552,  1550,   370,  1553,  1557,   431,   304,   436,   588,  1559,
    1578,   437,  1664,  1331,   438,  1587,  1516,  1583,  1584,  1593,
    1589,  1594,   431,  1595,   371,  1599,   371,  1600,  1686,   439,
    1508,  1628,   432,   433,  1602,   440,  1646,  1666,  1687,  1603,
    1688,  1619,  1733,  1729,  1661,  1665,  1667,  1231,  1668,   432,
     433,  1669,   724,   724,   724,   912,  1670,  1671,  1735,  1231,
    1700,  1709,   784,   785,   786,   787,   788,  1699,  1740,   789,
     790,   791,   792,  1753,   793,   794,   795,   796,   304,  1724,
     670,  1725,   797,  1547,   798,   799,  1730,  1739,  1766,   991,
     690,  1755,  1756,  1307,  1769,   695,   434,   697,   304,  1757,
     435,  1758,  1761,   370,   768,   370,   707,  1734,  1762,  1763,
    1764,  1771,  1775,   434,   719,  1782,  1785,   435,  1783,  1784,
    1786,  1744,  1791,   137,    20,    89,   186,   431,   140,  1633,
    1118,  1274,   768,   328,  1634,  1635,    26,  1636,   743,  1637,
    1606,  1287,  1558,  1444,   -79,   807,   808,   809,   810,   811,
     812,   813,   814,  1580,   432,   433,   761,  1770,  1657,  1476,
    1581,  1658,   815,   816,   304,  1659,    99,   436,   371,  1354,
     675,   437,   597,  1358,   438,     0,   676,   423,  1780,  1728,
     371,  1616,  1408,   979,   436,     0,     0,     0,   437,   439,
    1359,   438,     0,     0,     0,   440,     0,     0,     0,     0,
       0,     0,     0,     0,   370,     0,   439,   838,     0,     0,
     371,   371,   440,     0,     0,     0,     0,     0,   434,   370,
       0,     0,   435,     0,   850,     0,     0,     0,     0,     0,
       0,   304,     0,     0,   768,   371,     0,     0,   371,     0,
       0,     0,   817,   818,   819,   820,   821,   822,   823,   824,
    1779,     0,     0,     0,     0,   825,   826,  1678,     0,     0,
       0,   827,     0,   768,  1678,     0,  1678,     0,     0,     0,
       0,   828,     0,  1678,   829,   830,     0,  1003,     0,     0,
    1789,   831,   832,   833,     0,     0,   304,   431,  1678,   436,
       0,     0,   370,   437,   690,  1376,   438,     0,     0,     0,
       0,   371,     0,   934,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,   432,   433,     0,   440,     0,     0,
       0,     0,     0,     0,   724,     0,   954,     0,   834,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1678,     0,     0,  1003,   970,     0,   970,     0,     0,     0,
       0,     0,     0,  1748,     0,   541,   722,     0,     0,  1750,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   782,   783,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,  1678,  1678,     0,     0,
     371,     0,     0,     0,   768,  1777,  1778,   304,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,  1678,     0,  1788,  1081,  1083,  1085,  1087,
    1089,  1091,  1093,  1095,  1097,  1099,  1101,  1102,  1104,  1106,
    1108,  1110,  1112,  1114,     0,     0,   432,   433,     0,   436,
       0,     0,     0,   437,     0,  1382,   438,  1125,     0,     0,
       0,     0,     0,     0,     0,     0,   784,   785,   786,   787,
     788,   439,     0,   789,   790,   791,   792,   440,   793,   794,
     795,   796,     0,     0,     0,     0,   797,  1149,   798,   799,
       0,   371,     0,   371,   800,   801,   802,     0,   768,     0,
     803,     0,     0,  1157,     0,     0,     0,     0,   724,     0,
     434,  1168,     0,  1170,   435,     0,     0,     0,     0,     0,
       0,     0,     0,  1178,  1179,  1180,  1181,  1182,  1183,  1184,
    1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,  1194,
       0,     0,     0,     0,     0,   804,     0,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   814,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   815,   816,     0,     0,
     845,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,   437,     0,  1383,   438,   608,
    1249,     0,   371,     0,     0,     0,     0,     0,   619,  1254,
       0,     0,     0,   439,     0,     0,     0,   371,   632,   440,
       0,     0,     0,     0,     0,  1271,     0,     0,   642,     0,
     724,   724,   724,     0,     0,   768,     0,   768,     0,   768,
       0,   768,     0,   768,     0,   768,     0,   768,   659,   768,
       0,   768,     0,   768,     0,   768,     0,     0,   768,     0,
     768,     0,   768,     0,   768,     0,   768,     0,   768,     0,
     689,     0,     0,     0,     0,     0,     0,     0,     0,   768,
       0,   734,     0,     0,   782,   783,     0,   329,     0,     0,
     371,     0,     0,   330,     0,   720,   721,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
       0,     0,     0,     0,     0,     0,     0,   333,     0,     0,
       0,     0,     0,   747,   748,   749,   750,   751,   756,   756,
       0,     0,   334,     0,     0,     0,     0,     0,     0,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,     0,     0,     0,     0,     0,     0,     0,   784,
     785,   786,   787,   788,     0,     0,   789,   790,   791,   792,
       0,   793,   794,   795,   796,     0,     0,     0,     0,   797,
       0,   798,   799,     0,     0,     0,     0,   800,   801,   802,
       0,    62,     0,   803,     0,     0,     0,     0,     0,     0,
    1381,     0,     0,   756,   368,     0,     0,     0,     0,     0,
       0,     0,   875,   782,   783,     0,     0,     0,     0,     0,
       0,    63,     0,  1396,  1397,     0,     0,     0,  1400,     0,
       0,     0,     0,     0,     0,     0,  1406,     0,   804,   970,
     805,   806,   807,   808,   809,   810,   811,   812,   813,   814,
       0,     0,     0,   756,   689,     0,     0,     0,     0,   815,
     816,     0,     0,   855,     0,   935,     0,     0,  1432,   369,
       0,   941,     0,   588,     0,     0,     0,   945,     0,     0,
     756,     0,     0,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   957,   958,   959,
     960,   961,   962,     0,     0,     0,   782,   783,   784,   785,
     786,   787,   788,     0,     0,   789,   790,   791,   792,     0,
     793,   794,   795,   796,     0,   768,     0,     0,   797,     0,
     798,   799,     0,     0,   782,   783,   800,   801,   802,     0,
     756,     0,   803,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1035,  1036,     0,  1490,  1037,  1038,  1039,
    1040,     0,  1042,     0,  1045,  1046,  1047,  1049,  1050,  1051,
    1052,  1053,  1054,  1056,  1057,  1058,  1059,  1060,  1061,  1062,
    1063,  1064,  1065,  1066,     0,  1075,     0,   804,     0,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,     0,
       0,   784,   785,   786,   787,   756,     0,     0,   815,   816,
       0,   792,   870,   793,   794,   795,   796,     0,     0,     0,
       0,   797,     0,   798,   799,     0,     0,     0,     0,   784,
     785,   786,   787,   788,     0,     0,   789,   790,   791,   792,
       0,   793,   794,   795,   796,     0,     0,     0,   431,   797,
       0,   798,   799,     0,     0,     0,  1153,     0,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,     0,     0,     0,     0,   432,   433,   782,   783,     0,
       0,     0,  1177,     0,     0,   432,   433,   810,   811,   812,
     813,   814,     0,     0,     0,     0,  1586,     0,  1588,     0,
       0,   815,   816,     0,     0,     0,     0,     0,  1596,  1597,
    1598,   806,   807,   808,   809,   810,   811,   812,   813,   814,
     431,     0,     0,     0,     0,  1613,  1614,     0,     0,   815,
     816,  1618,     0,     0,     0,     0,   747,  1227,   756,   434,
       0,     0,     0,   435,     0,  1638,   756,   432,   433,   434,
       0,     0,     0,   435,     0,     0,     0,     0,     0,  1648,
       0,     0,     0,     0,     0,     0,  1177,     0,     0,     0,
       0,     0,   784,   785,   786,   787,   788,     0,     0,   789,
     790,   791,   792,   756,   793,   794,   795,   796,     0,     0,
       0,     0,   797,     0,   798,   799,     0,     0,     0,     0,
     800,   801,   802,     0,     0,     0,   803,     0,  1680,  1681,
     436,   434,     0,     0,   437,   435,  1384,   438,     0,     0,
     436,     0,     0,     0,   437,     0,  1385,   438,     0,     0,
       0,  1697,   439,     0,     0,     0,     0,  1311,   440,     0,
       0,  1316,   439,     0,  1704,     0,  1705,     0,   440,     0,
       0,   804,     0,   805,   806,   807,   808,   809,   810,   811,
     812,   813,   814,     0,     0,  1718,     0,  1719,     0,     0,
       0,     0,   815,   816,     0,     0,   886,     0,     0,     0,
       0,     0,   436,     0,     0,     0,   437,     0,  1387,   438,
       0,   736,     0,     0,   431,  1743,     0,   329,     0,     0,
       0,  1746,  1747,   330,   439,     0,     0,     0,     0,   331,
     440,     0,     0,     0,     0,     0,     0,     0,     0,   332,
       0,   432,   433,     0,     0,     0,     0,   333,  1765,   756,
    1767,  1768,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,     0,  1776,     0,     0,     0,     0,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,  -363,     0,     0,   434,     0,     0,     0,   435,
     756,     0,  1395,   782,   783,   756,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   756,     0,     0,     0,     0,     0,   756,   431,     0,
       0,    62,     0,     0,     0,     0,   756,     0,     0,     0,
    1426,  1427,     0,     0,   368,     0,     0,   431,     0,     0,
       0,   756,     0,     0,     0,   432,   433,     0,     0,  1177,
       0,    63,     0,     0,     0,     0,   436,     0,     0,     0,
     437,     0,  1502,   438,   432,   433,     0,  1454,     0,  1456,
     756,  1459,     0,     0,     0,     0,     0,  1462,   439,     0,
       0,  1465,   756,     0,   440,     0,     0,   756,   784,   785,
     786,   787,   788,     0,     0,   789,   790,   791,   792,   369,
     793,   794,   795,   796,     0,     0,     0,     0,   797,   434,
     798,   799,     0,   435,     0,     0,   800,   801,   802,     0,
       0,     0,   803,     0,     0,     0,     0,     0,   434,     0,
     756,     0,   435,     0,     0,     0,     0,  -363,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -363,     0,     0,
       0,     0,   756,   782,   783,  -363,     0,   804,     0,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,     0,
     436,     0,     0,     0,   437,     0,  1507,   438,   815,   816,
     756,   756,     0,     0,   756,     0,     0,     0,     0,   436,
       0,   431,   439,   437,     0,  1543,   438,     0,   440,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,   440,   432,   433,
    1570,     0,  1571,     0,     0,   756,     0,     0,  1575,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   756,   329,   784,   785,
     786,   787,   788,   330,     0,   789,   790,   791,   792,   331,
     793,   794,   795,   796,     0,     0,     0,     0,   797,   332,
     798,   799,     0,     0,     0,  1605,     0,   333,     0,     0,
    1608,     0,   434,     0,  1612,     0,   435,  1615,     0,     0,
       0,     0,   334,     0,     0,     0,     0,  1624,   756,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   808,   809,   810,   811,   812,   813,   814,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   815,   816,
       0,     0,   756,   436,     0,     0,     0,   437,   756,  1649,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,   439,     0,     0,   756,     0,
       0,   440,     0,     0,   368,  1691,     0,     0,   468,     0,
     756,     0,   469,   470,     3,     0,   471,   472,   473,     0,
     474,    63,   475,   476,   477,   478,   479,     0,     0,     0,
       0,     0,   480,   481,   482,   483,   484,     0,   485,     0,
       0,     0,     0,     0,     0,   486,   487,     0,     0,   488,
       0,   489,   490,     0,     0,   491,     0,     8,   492,   493,
       0,   494,   495,     0,     0,   496,   497,  1738,     0,   369,
       0,   644,   498,     0,   756,   499,   500,     0,   335,   336,
     337,     0,   339,   340,   341,   342,   343,   501,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,     0,
     357,   358,   359,     0,     0,   362,   363,   364,   365,   502,
     503,   504,   505,     0,     0,     0,     0,     0,     0,  1774,
       0,     0,     0,     0,     0,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     508,   509,   510,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   511,   512,
     513,   514,   515,     0,   516,     0,   517,   518,   519,   520,
      50,   148,   521,   522,   523,   524,   525,   526,   527,   528,
      63,   529,   530,   531,     0,     0,     0,     0,     0,     0,
     532,   149,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     535,   536,     0,    15,     0,     0,   537,   538,     0,     0,
     469,   470,     0,     0,   539,     0,   540,     0,   541,   542,
     475,   476,   477,   478,   479,     0,     0,     0,     0,     0,
     480,     0,   482,     0,     0,     0,   485,     0,   431,     0,
       0,     0,     0,     0,   487,     0,     0,     0,     0,     0,
     490,     0,     0,   491,     0,     0,   492,     0,   964,     0,
     495,     0,     0,     0,     0,   432,   433,     0,     0,     0,
     604,     0,     0,   499,   500,     0,   335,   336,   337,     0,
     339,   340,   341,   342,   343,   501,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,     0,   357,   358,
     359,     0,     0,   362,   363,   364,   365,   502,   503,   605,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   507,     0,     0,     0,     0,   434,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   511,   512,   513,   514,
     515,     0,   516,   965,   517,   518,   519,   520,    50,     0,
       0,   522,   523,   524,   525,   526,   527,   528,   966,   606,
     530,   531,     0,     0,     0,     0,     0,     0,   532,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,   437,     0,     0,   967,   535,   536,
       0,    15,     0,     0,   537,   538,     0,     0,     0,   469,
     470,     0,   968,     0,   969,     0,   541,   542,   440,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,   431,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   490,
       0,     0,   491,     0,     0,   492,     0,     0,     0,   495,
       0,     0,     0,     0,   432,   433,     0,     0,     0,   604,
       0,     0,   499,   500,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   501,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   502,   503,   605,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   511,   512,   513,   514,   515,
       0,   516,   965,   517,   518,   519,   520,    50,     0,     0,
     522,   523,   524,   525,   526,   527,   528,   966,   606,   530,
     531,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,   437,     0,     0,   967,   535,   536,     0,
      15,     0,     0,   537,   538,     0,     0,     0,   469,   470,
       0,   968,     0,   977,     0,   541,   542,   440,   475,   476,
     477,   478,   479,     0,     0,     0,     0,     0,   480,     0,
     482,     0,     0,     0,   485,     0,   637,     0,     0,     0,
       0,     0,   487,     0,     0,     0,     0,     0,   490,     0,
       0,   491,     0,     0,   492,     0,     0,     0,   495,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   604,     0,
       0,   499,   500,     0,   335,   336,   337,     0,   339,   340,
     341,   342,   343,   501,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,     0,   357,   358,   359,     0,
       0,   362,   363,   364,   365,   502,   503,   605,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   507,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   511,   512,   513,   514,   515,     0,
     516,     0,   517,   518,   519,   520,    50,     0,     0,   522,
     523,   524,   525,   526,   527,   528,    63,   606,   530,   531,
       0,     0,     0,     0,     0,     0,   532,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   638,     0,     0,   534,   535,   536,     0,    15,
       0,     0,   537,   538,     0,     0,     0,   469,   470,     0,
    1226,     0,   540,     0,   541,   542,   640,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,     0,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,   685,
       0,     0,     0,     0,     0,   508,   509,   510,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,   469,   470,     0,     0,   539,
       0,   540,     0,   541,   542,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   490,     0,     0,   491,     0,
       0,   492,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   604,     0,     0,   499,   500,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     501,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   502,   503,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   507,
       0,     0,     0,     0,     0,     0,     0,   717,     0,     0,
       0,     0,     0,   508,   509,   510,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   511,   512,   513,   514,   515,     0,   516,     0,   517,
     518,   519,   520,    50,     0,     0,   522,   523,   524,   525,
     526,   527,   528,    63,   606,   530,   531,     0,     0,     0,
       0,     0,     0,   532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,   536,     0,    15,     0,     0,   537,
     538,     0,     0,   469,   470,     0,     0,   539,     0,   540,
       0,   541,   542,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,  1631,   482,   483,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   490,     0,     0,   491,     0,     0,   492,
     493,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   604,     0,     0,   499,   500,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   501,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     502,   503,   605,  1632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   511,
     512,   513,   514,   515,     0,   516,     0,   517,   518,   519,
     520,    50,     0,     0,   522,   523,   524,   525,   526,   527,
     528,    63,   606,   530,   531,     0,     0,     0,     0,     0,
       0,   532,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   536,     0,    15,     0,     0,   537,   538,     0,
       0,   469,   470,     0,     0,   539,     0,   540,     0,   541,
     542,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   490,     0,     0,   491,     0,     0,   492,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   604,     0,     0,   499,   500,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   501,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   502,   503,
     504,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   508,
     509,   510,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   511,   512,   513,
     514,   515,     0,   516,     0,   517,   518,   519,   520,    50,
       0,     0,   522,   523,   524,   525,   526,   527,   528,    63,
     606,   530,   531,     0,     0,     0,     0,     0,     0,   532,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
     536,     0,    15,     0,     0,   537,   538,     0,     0,   469,
     470,     0,     0,   539,     0,   540,     0,   541,   542,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   490,
       0,     0,   491,     0,     0,   492,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   604,
       0,     0,   499,   500,  1013,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   501,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   502,   503,   605,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   511,   512,   513,   514,   515,
       0,   516,   965,   517,   518,   519,   520,    50,     0,     0,
     522,   523,   524,   525,   526,   527,   528,   966,   606,   530,
     531,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   536,     0,
      15,     0,     0,   537,   538,     0,     0,   469,   470,     0,
       0,  1014,     0,   540,  1015,   541,   542,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,     0,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1076,  1077,  1078,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,     0,     0,   469,   470,   539,
       0,   540,     0,   541,   542,   752,     0,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,   753,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,     0,     0,   469,   470,   539,
     641,   540,     0,   541,   542,   752,     0,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,   753,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
     965,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,   966,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,     0,     0,   469,   470,   539,
       0,   540,     0,   541,   542,   752,     0,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,   753,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,     0,     0,   469,   470,   539,
     873,   540,     0,   541,   542,   752,     0,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,   753,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,   469,   470,     0,     0,   539,
       0,   540,     0,   541,   542,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   490,     0,     0,   491,     0,
       0,   492,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   604,     0,     0,   499,   500,
    1163,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     501,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   502,   503,   605,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   511,   512,   513,   514,   515,     0,   516,   965,   517,
     518,   519,   520,    50,     0,     0,   522,   523,   524,   525,
     526,   527,   528,   966,   606,   530,   531,     0,     0,     0,
       0,     0,     0,   532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,   536,     0,    15,     0,     0,   537,
     538,     0,     0,   469,   470,     0,     0,   539,     0,   540,
       0,   541,   542,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   490,     0,     0,   491,     0,     0,   492,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   604,     0,     0,   499,   500,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   501,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     502,   503,   605,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   511,
     512,   513,   514,   515,     0,   516,     0,   517,   518,   519,
     520,    50,     0,     0,   522,   523,   524,   525,   526,   527,
     528,    63,   606,   530,   531,     0,     0,     0,     0,     0,
       0,   532,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   536,     0,    15,     0,     0,   537,   538,     0,
       0,   469,   470,     0,     0,   539,   641,   540,     0,   541,
     542,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   490,     0,     0,   491,     0,     0,   492,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   694,     0,     0,
       0,   604,     0,     0,   499,   500,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   501,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   502,   503,
     605,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   511,   512,   513,
     514,   515,     0,   516,     0,   517,   518,   519,   520,    50,
       0,     0,   522,   523,   524,   525,   526,   527,   528,    63,
     606,   530,   531,     0,     0,     0,     0,     0,     0,   532,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
     536,     0,    15,     0,     0,   537,   538,     0,     0,   469,
     470,     0,     0,   539,     0,   540,     0,   541,   542,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   490,
       0,     0,   491,     0,     0,   492,     0,     0,     0,   495,
       0,     0,   706,     0,     0,     0,     0,     0,     0,   604,
       0,     0,   499,   500,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   501,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   502,   503,   605,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   511,   512,   513,   514,   515,
       0,   516,     0,   517,   518,   519,   520,    50,     0,     0,
     522,   523,   524,   525,   526,   527,   528,    63,   606,   530,
     531,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   536,     0,
      15,     0,     0,   537,   538,     0,     0,     0,     0,   469,
     470,   539,     0,   540,     0,   541,   542,   738,     0,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   490,
       0,     0,   491,     0,     0,   492,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   604,
       0,     0,   499,   500,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   501,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   502,   503,   605,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   511,   512,   513,   514,   515,
       0,   516,     0,   517,   518,   519,   520,    50,     0,     0,
     522,   523,   524,   525,   526,   527,   528,    63,   606,   530,
     531,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   536,     0,
      15,     0,     0,   537,   538,     0,     0,   469,   470,     0,
       0,   539,     0,   540,     0,   541,   542,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,     0,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   742,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,     0,     0,   469,   470,   539,
       0,   540,     0,   541,   542,   763,     0,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,     0,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,   469,   470,     0,     0,   539,
       0,   540,     0,   541,   542,   475,   476,   477,   478,   479,
       0,     0,  1055,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   490,     0,     0,   491,     0,
       0,   492,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   604,     0,     0,   499,   500,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     501,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   502,   503,   605,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   511,   512,   513,   514,   515,     0,   516,     0,   517,
     518,   519,   520,    50,     0,     0,   522,   523,   524,   525,
     526,   527,   528,    63,   606,   530,   531,     0,     0,     0,
       0,     0,     0,   532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,   536,     0,    15,     0,     0,   537,
     538,     0,     0,   469,   470,     0,     0,   539,     0,   540,
       0,   541,   542,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   490,     0,     0,   491,     0,     0,   492,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   604,     0,     0,   499,   500,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   501,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     502,   503,   605,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   511,
     512,   513,   514,   515,     0,   516,     0,   517,   518,   519,
     520,    50,     0,     0,   522,   523,   524,   525,   526,   527,
     528,    63,   606,   530,   531,     0,     0,     0,     0,     0,
       0,   532,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   536,     0,    15,     0,     0,   537,   538,     0,
       0,   469,   470,     0,     0,   539,     0,   540,  1115,   541,
     542,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   490,     0,     0,   491,     0,     0,   492,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   604,     0,     0,   499,   500,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   501,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   502,   503,
     605,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   511,   512,   513,
     514,   515,     0,   516,     0,   517,   518,   519,   520,    50,
       0,     0,   522,   523,   524,   525,   526,   527,   528,    63,
     606,   530,   531,     0,     0,     0,     0,     0,     0,   532,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1167,     0,   534,   535,
     536,     0,    15,     0,     0,   537,   538,     0,     0,   469,
     470,     0,     0,   539,     0,   540,     0,   541,   542,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   490,
       0,     0,   491,     0,     0,   492,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   604,
       0,     0,   499,   500,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   501,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   502,   503,   605,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   511,   512,   513,   514,   515,
       0,   516,     0,   517,   518,   519,   520,    50,     0,     0,
     522,   523,   524,   525,   526,   527,   528,    63,   606,   530,
     531,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   536,     0,
      15,     0,     0,   537,   538,     0,     0,   469,   470,     0,
       0,   539,     0,   540,  1228,   541,   542,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,     0,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,   469,   470,     0,     0,   539,
       0,   540,  1243,   541,   542,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   490,     0,     0,   491,     0,
       0,   492,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   604,     0,     0,   499,   500,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     501,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   502,   503,   605,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   511,   512,   513,   514,   515,     0,   516,     0,   517,
     518,   519,   520,    50,     0,     0,   522,   523,   524,   525,
     526,   527,   528,    63,   606,   530,   531,     0,     0,     0,
       0,     0,     0,   532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,   536,     0,    15,     0,     0,   537,
     538,     0,     0,   469,   470,     0,     0,   539,     0,   540,
    1457,   541,   542,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   490,     0,     0,   491,     0,     0,   492,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   604,     0,     0,   499,   500,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   501,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     502,   503,   605,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   511,
     512,   513,   514,   515,     0,   516,     0,   517,   518,   519,
     520,    50,     0,     0,   522,   523,   524,   525,   526,   527,
     528,    63,   606,   530,   531,     0,     0,     0,     0,     0,
       0,   532,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   536,     0,    15,     0,     0,   537,   538,     0,
       0,   469,   470,     0,     0,  1466,     0,   540,  1467,   541,
     542,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   490,     0,     0,   491,     0,     0,   492,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   604,     0,     0,   499,   500,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   501,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   502,   503,
     605,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,   507,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   511,   512,   513,
     514,   515,     0,   516,     0,   517,   518,   519,   520,    50,
       0,     0,   522,   523,   524,   525,   526,   527,   528,    63,
     606,   530,   531,     0,     0,     0,     0,     0,     0,   532,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
     536,     0,    15,     0,     0,   537,   538,     0,     0,   469,
     470,     0,     0,   539,     0,   540,  1472,   541,   542,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   490,
       0,     0,   491,     0,     0,   492,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   604,
       0,     0,   499,   500,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   501,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   502,   503,   605,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   511,   512,   513,   514,   515,
       0,   516,     0,   517,   518,   519,   520,    50,     0,     0,
     522,   523,   524,   525,   526,   527,   528,    63,   606,   530,
     531,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   536,     0,
      15,     0,     0,   537,   538,     0,     0,   469,   470,     0,
       0,   539,     0,   540,  1519,   541,   542,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,     0,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
       0,     0,     0,     0,     0,   532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   534,   535,   536,     0,    15,     0,
       0,   537,   538,     0,     0,   469,   470,     0,     0,   539,
       0,   540,  1604,   541,   542,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   490,     0,     0,   491,     0,
       0,   492,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   604,     0,     0,   499,   500,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     501,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   502,   503,   605,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   507,
       0,     0,     0,     0,     0,     0,     0,  1647,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   511,   512,   513,   514,   515,     0,   516,     0,   517,
     518,   519,   520,    50,     0,     0,   522,   523,   524,   525,
     526,   527,   528,    63,   606,   530,   531,     0,     0,     0,
       0,     0,     0,   532,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,   536,     0,    15,     0,     0,   537,
     538,     0,     0,   469,   470,     0,     0,   539,     0,   540,
       0,   541,   542,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   490,     0,     0,   491,     0,     0,   492,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   604,     0,     0,   499,   500,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   501,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     502,   503,   605,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,  1695,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   511,
     512,   513,   514,   515,     0,   516,     0,   517,   518,   519,
     520,    50,     0,     0,   522,   523,   524,   525,   526,   527,
     528,    63,   606,   530,   531,     0,     0,     0,     0,     0,
       0,   532,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     534,   535,   536,     0,    15,     0,     0,   537,   538,     0,
       0,   469,   470,     0,     0,   539,     0,   540,     0,   541,
     542,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   490,     0,     0,   491,     0,     0,   492,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   604,     0,     0,   499,   500,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   501,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   502,   503,
     605,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   506,   507,     0,     0,     0,     0,
       0,     0,     0,  1696,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   511,   512,   513,
     514,   515,     0,   516,     0,   517,   518,   519,   520,    50,
       0,     0,   522,   523,   524,   525,   526,   527,   528,    63,
     606,   530,   531,     0,     0,     0,     0,     0,     0,   532,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,   535,
     536,     0,    15,     0,     0,   537,   538,     0,     0,   469,
     470,     0,     0,   539,     0,   540,     0,   541,   542,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   490,
       0,     0,   491,     0,     0,   492,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   604,
       0,     0,   499,   500,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   501,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   502,   503,   605,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   511,   512,   513,   514,   515,
       0,   516,     0,   517,   518,   519,   520,    50,     0,     0,
     522,   523,   524,   525,   526,   527,   528,    63,   606,   530,
     531,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   536,     0,
      15,     0,     0,   537,   538,     0,     0,   469,   470,     0,
       0,   539,     0,   540,     0,   541,   542,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   490,     0,     0,
     491,     0,     0,   492,     0,     0,     0,   495,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,     0,     0,
     499,   500,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   502,   503,   605,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   511,   512,   513,   514,   515,     0,   516,
       0,   517,   518,   519,   520,    50,     0,     0,   522,   523,
     524,   525,   526,   527,   528,    63,   606,   530,   531,     0,
     329,     0,     0,   782,   783,   532,   330,     0,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   332,     0,   534,   535,   536,     0,    15,     0,
     333,   537,   538,     0,     0,     0,     0,     0,     0,  1440,
       0,   540,     0,   541,   542,   334,     0,     0,     0,     0,
       0,     0,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   784,   785,
     786,   787,   788,     0,     0,   789,     0,     0,   792,     0,
     793,   794,   795,   796,     0,     0,     0,     0,   797,     0,
     798,   799,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   368,     0,     0,
       0,     0,     0,     0,     0,     0,    13,     0,     0,     0,
       0,   329,     0,     0,   648,     0,     0,   330,     0,     0,
       0,     0,     0,   331,     0,     0,    14,     0,     0,     0,
       0,     0,     0,   332,   649,     0,     0,     0,     0,     0,
       0,   333,   808,   809,   810,   811,   812,   813,   814,     0,
       0,     0,     0,     0,     0,     0,   334,     0,   815,   816,
       0,     0,   369,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   782,   783,     0,
       0,     0,     0,     0,   329,     0,     0,     0,     0,     0,
     330,     0,     0,     0,     0,     0,   331,     0,     0,     0,
       0,     0,     0,     0,     0,    62,   332,     0,     0,     0,
       0,     0,     0,     0,   333,     0,     0,     0,   368,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
       0,     0,     0,     0,     0,    63,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,     0,
       0,     0,   784,   785,   786,   787,   788,     0,     0,   789,
     790,   791,   792,   369,   793,   794,   795,   796,     0,     0,
       0,     0,   797,     0,   798,   799,   782,   783,     0,     0,
     800,   801,   802,     0,     0,     0,   803,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   368,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   782,   783,     0,     0,     0,     0,     0,   648,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   804,     0,   805,   806,   807,   808,   809,   810,   811,
     812,   813,   814,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   815,   816,     0,     0,  1199,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   369,     0,     0,     0,
       0,   784,   785,   786,   787,   788,     0,     0,   789,   790,
     791,   792,     0,   793,   794,   795,   796,     0,     0,     0,
       0,   797,     0,   798,   799,     0,     0,     0,     0,   800,
     801,   802,     0,     0,     0,   803,   784,   785,   786,   787,
     788,     0,     0,   789,   790,   791,   792,     0,   793,   794,
     795,   796,   782,   783,     0,     0,   797,     0,   798,   799,
       0,     0,     0,     0,   800,   801,   802,     0,     0,     0,
     803,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     804,     0,   805,   806,   807,   808,   809,   810,   811,   812,
     813,   814,   782,   783,     0,     0,     0,     0,     0,     0,
       0,   815,   816,     0,     0,  1204,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   804,     0,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   814,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   815,   816,     0,     0,
    1206,     0,     0,     0,     0,     0,     0,   784,   785,   786,
     787,   788,     0,     0,   789,   790,   791,   792,     0,   793,
     794,   795,   796,     0,     0,     0,     0,   797,     0,   798,
     799,     0,     0,     0,     0,   800,   801,   802,     0,     0,
       0,   803,     0,     0,     0,     0,     0,   784,   785,   786,
     787,   788,     0,     0,   789,   790,   791,   792,     0,   793,
     794,   795,   796,   782,   783,     0,     0,   797,     0,   798,
     799,     0,     0,     0,     0,   800,   801,   802,     0,     0,
       0,   803,     0,     0,     0,     0,   804,     0,   805,   806,
     807,   808,   809,   810,   811,   812,   813,   814,   782,   783,
       0,     0,     0,     0,     0,     0,     0,   815,   816,     0,
       0,  1215,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   804,     0,   805,   806,
     807,   808,   809,   810,   811,   812,   813,   814,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   815,   816,     0,
       0,  1216,     0,     0,     0,     0,     0,     0,   784,   785,
     786,   787,   788,     0,     0,   789,   790,   791,   792,     0,
     793,   794,   795,   796,     0,     0,     0,     0,   797,     0,
     798,   799,     0,     0,     0,     0,   800,   801,   802,     0,
       0,     0,   803,   784,   785,   786,   787,   788,     0,     0,
     789,   790,   791,   792,     0,   793,   794,   795,   796,   782,
     783,     0,     0,   797,     0,   798,   799,     0,     0,     0,
       0,   800,   801,   802,     0,     0,     0,   803,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   804,     0,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,   782,
     783,     0,     0,     0,     0,     0,     0,     0,   815,   816,
       0,     0,  1217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   804,     0,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   815,   816,     0,     0,  1218,     0,     0,
       0,     0,     0,     0,   784,   785,   786,   787,   788,     0,
       0,   789,   790,   791,   792,     0,   793,   794,   795,   796,
       0,     0,     0,     0,   797,     0,   798,   799,     0,     0,
       0,     0,   800,   801,   802,     0,     0,     0,   803,     0,
       0,     0,     0,     0,   784,   785,   786,   787,   788,     0,
       0,   789,   790,   791,   792,     0,   793,   794,   795,   796,
     782,   783,     0,     0,   797,     0,   798,   799,     0,     0,
       0,     0,   800,   801,   802,     0,     0,     0,   803,     0,
       0,     0,     0,   804,     0,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,   782,   783,     0,     0,     0,
       0,     0,     0,     0,   815,   816,     0,     0,  1219,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   804,     0,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   815,   816,     0,     0,  1220,     0,
       0,     0,     0,     0,     0,   784,   785,   786,   787,   788,
       0,     0,   789,   790,   791,   792,     0,   793,   794,   795,
     796,     0,     0,     0,     0,   797,     0,   798,   799,     0,
       0,     0,     0,   800,   801,   802,     0,     0,     0,   803,
     784,   785,   786,   787,   788,     0,     0,   789,   790,   791,
     792,     0,   793,   794,   795,   796,   782,   783,     0,     0,
     797,     0,   798,   799,     0,     0,     0,     0,   800,   801,
     802,     0,     0,     0,   803,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   804,     0,   805,   806,   807,   808,
     809,   810,   811,   812,   813,   814,   782,   783,     0,     0,
       0,     0,     0,     0,     0,   815,   816,     0,     0,  1355,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   804,
       0,   805,   806,   807,   808,   809,   810,   811,   812,   813,
     814,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     815,   816,     0,     0,  1522,     0,     0,     0,     0,     0,
       0,   784,   785,   786,   787,   788,     0,     0,   789,   790,
     791,   792,     0,   793,   794,   795,   796,     0,     0,     0,
       0,   797,     0,   798,   799,     0,     0,     0,     0,   800,
     801,   802,     0,     0,     0,   803,     0,     0,     0,     0,
       0,   784,   785,   786,   787,   788,     0,     0,   789,   790,
     791,   792,     0,   793,   794,   795,   796,   782,   783,     0,
       0,   797,     0,   798,   799,     0,     0,     0,     0,   800,
     801,   802,     0,     0,     0,   803,     0,     0,     0,     0,
     804,     0,   805,   806,   807,   808,   809,   810,   811,   812,
     813,   814,   782,   783,     0,     0,     0,     0,     0,     0,
       0,   815,   816,     0,     0,  1541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     804,     0,   805,   806,   807,   808,   809,   810,   811,   812,
     813,   814,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   815,   816,     0,     0,  1542,     0,     0,     0,     0,
       0,     0,   784,   785,   786,   787,   788,     0,     0,   789,
     790,   791,   792,     0,   793,   794,   795,   796,     0,     0,
       0,     0,   797,     0,   798,   799,     0,     0,     0,     0,
     800,   801,   802,     0,     0,     0,   803,   784,   785,   786,
     787,   788,     0,     0,   789,   790,   791,   792,     0,   793,
     794,   795,   796,   782,   783,     0,     0,   797,     0,   798,
     799,     0,     0,     0,     0,   800,   801,   802,     0,     0,
       0,   803,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   804,     0,   805,   806,   807,   808,   809,   810,   811,
     812,   813,   814,   782,   783,     0,     0,     0,     0,     0,
       0,     0,   815,   816,     0,     0,  1564,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   804,     0,   805,   806,
     807,   808,   809,   810,   811,   812,   813,   814,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   815,   816,     0,
       0,  1566,     0,     0,     0,     0,     0,     0,   784,   785,
     786,   787,   788,     0,     0,   789,   790,   791,   792,     0,
     793,   794,   795,   796,     0,     0,     0,     0,   797,     0,
     798,   799,     0,     0,     0,     0,   800,   801,   802,     0,
       0,     0,   803,     0,     0,     0,     0,     0,   784,   785,
     786,   787,   788,     0,     0,   789,   790,   791,   792,     0,
     793,   794,   795,   796,   782,   783,     0,     0,   797,     0,
     798,   799,     0,     0,     0,     0,   800,   801,   802,     0,
       0,     0,   803,     0,     0,     0,     0,   804,     0,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,   782,
     783,     0,     0,     0,     0,     0,     0,     0,   815,   816,
       0,     0,  1568,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   804,     0,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   815,   816,
       0,     0,  1572,     0,     0,     0,     0,     0,     0,   784,
     785,   786,   787,   788,     0,     0,   789,   790,   791,   792,
       0,   793,   794,   795,   796,     0,     0,     0,     0,   797,
       0,   798,   799,     0,     0,     0,     0,   800,   801,   802,
       0,     0,     0,   803,   784,   785,   786,   787,   788,     0,
       0,   789,   790,   791,   792,     0,   793,   794,   795,   796,
     782,   783,     0,     0,   797,     0,   798,   799,     0,     0,
       0,     0,   800,   801,   802,     0,     0,     0,   803,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   804,     0,
     805,   806,   807,   808,   809,   810,   811,   812,   813,   814,
     782,   783,     0,     0,     0,     0,     0,     0,     0,   815,
     816,     0,     0,  1652,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   804,     0,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   815,   816,     0,     0,  1653,     0,
       0,     0,     0,     0,     0,   784,   785,   786,   787,   788,
       0,     0,   789,   790,   791,   792,     0,   793,   794,   795,
     796,     0,     0,     0,     0,   797,     0,   798,   799,     0,
       0,     0,     0,   800,   801,   802,     0,     0,     0,   803,
       0,     0,     0,     0,     0,   784,   785,   786,   787,   788,
       0,     0,   789,   790,   791,   792,     0,   793,   794,   795,
     796,   782,   783,     0,     0,   797,     0,   798,   799,     0,
       0,     0,     0,   800,   801,   802,     0,     0,     0,   803,
       0,     0,     0,     0,   804,     0,   805,   806,   807,   808,
     809,   810,   811,   812,   813,   814,   782,   783,     0,     0,
       0,     0,     0,     0,     0,   815,   816,     0,     0,  1655,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   804,     0,   805,   806,   807,   808,
     809,   810,   811,   812,   813,   814,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   815,   816,     0,     0,  1677,
       0,     0,     0,     0,     0,     0,   784,   785,   786,   787,
     788,     0,     0,   789,   790,   791,   792,     0,   793,   794,
     795,   796,     0,     0,     0,     0,   797,     0,   798,   799,
       0,     0,     0,     0,   800,   801,   802,     0,     0,     0,
     803,   784,   785,   786,   787,   788,     0,     0,   789,   790,
     791,   792,     0,   793,   794,   795,   796,   782,   783,     0,
       0,   797,     0,   798,   799,     0,     0,     0,     0,   800,
     801,   802,     0,     0,     0,   803,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   804,     0,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   814,   782,   783,     0,
       0,     0,     0,     0,     0,     0,   815,   816,     0,     0,
    1682,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     804,     0,   805,   806,   807,   808,   809,   810,   811,   812,
     813,   814,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   815,   816,   874,     0,     0,     0,     0,     0,     0,
       0,     0,   784,   785,   786,   787,   788,     0,     0,   789,
     790,   791,   792,     0,   793,   794,   795,   796,     0,     0,
       0,     0,   797,     0,   798,   799,     0,     0,     0,     0,
     800,   801,   802,     0,     0,     0,   803,     0,     0,     0,
       0,     0,   784,   785,   786,   787,   788,     0,     0,   789,
     790,   791,   792,     0,   793,   794,   795,   796,   782,   783,
       0,     0,   797,     0,   798,   799,     0,     0,     0,     0,
     800,   801,   802,     0,     0,     0,   803,     0,     0,     0,
       0,   804,     0,   805,   806,   807,   808,   809,   810,   811,
     812,   813,   814,   782,   783,     0,     0,     0,     0,     0,
       0,     0,   815,   816,  1148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   804,     0,   805,   806,   807,   808,   809,   810,   811,
     812,   813,   814,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   815,   816,  1303,     0,     0,     0,     0,     0,
       0,     0,     0,   784,   785,   786,   787,   788,     0,     0,
     789,   790,   791,   792,     0,   793,   794,   795,   796,     0,
       0,     0,     0,   797,     0,   798,   799,     0,     0,     0,
       0,   800,   801,   802,     0,     0,     0,   803,   784,   785,
     786,   787,   788,     0,     0,   789,   790,   791,   792,     0,
     793,   794,   795,   796,   782,   783,     0,     0,   797,     0,
     798,   799,     0,     0,     0,     0,   800,   801,   802,     0,
       0,     0,   803,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   804,     0,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,     0,     0,     0,   249,   250,     0,
       0,     0,     0,   815,   816,  1319,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,     0,   804,     0,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   815,   816,
    1464,     0,     0,     0,     0,     0,     0,     0,     0,   784,
     785,   786,   787,   788,     0,     0,   789,   790,   791,   792,
       0,   793,   794,   795,   796,     0,     0,     0,     0,   797,
       0,   798,   799,     0,     0,     0,     0,   800,   801,   802,
       0,     0,     0,   803,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   782,   783,   270,   271,   272,     0,     0,     0,
       0,     0,     0,   273,   274,   275,   276,   277,     0,     0,
     278,   279,   280,   281,   282,   283,   284,     0,   804,     0,
     805,   806,   807,   808,   809,   810,   811,   812,   813,   814,
     782,   783,     0,     0,     0,     0,     0,     0,     0,   815,
     816,  1470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   285,     0,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,     0,     0,   296,   297,     0,     0,     0,
       0,     0,   298,   299,     0,     0,     0,   784,   785,   786,
     787,   788,     0,     0,   789,   790,   791,   792,     0,   793,
     794,   795,   796,     0,     0,     0,     0,   797,     0,   798,
     799,     0,     0,     0,     0,   800,   801,   802,     0,     0,
       0,   803,     0,     0,     0,   784,   785,   786,   787,   788,
       0,     0,   789,   790,   791,   792,    13,   793,   794,   795,
     796,   782,   783,     0,     0,   797,     0,   798,   799,     0,
       0,     0,     0,   800,   801,   802,    14,     0,     0,   803,
       0,     0,     0,     0,     0,     0,   804,     0,   805,   806,
     807,   808,   809,   810,   811,   812,   813,   814,   782,   783,
       0,     0,     0,     0,     0,     0,     0,   815,   816,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   804,  1308,   805,   806,   807,   808,
     809,   810,   811,   812,   813,   814,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   815,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   784,   785,   786,   787,
     788,     0,     0,   789,   790,   791,   792,     0,   793,   794,
     795,   796,     0,     0,     0,     0,   797,     0,   798,   799,
       0,     0,     0,     0,   800,   801,   802,     0,     0,     0,
     803,     0,     0,   784,   785,   786,   787,   788,     0,     0,
     789,   790,   791,   792,     0,   793,   794,   795,   796,   782,
     783,     0,     0,   797,     0,   798,   799,     0,     0,     0,
       0,   800,   801,   802,     0,     0,     0,  -809,     0,     0,
       0,     0,     0,     0,     0,   804,     0,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   814,   782,   783,     0,
       0,     0,     0,     0,     0,     0,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   804,     0,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   815,   816,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   784,   785,   786,   787,   788,     0,
       0,   789,   790,   791,   792,     0,   793,   794,   795,   796,
       0,     0,     0,     0,   797,     0,   798,   799,     0,     0,
       0,     0,   800,   801,   802,     0,     0,     0,     0,     0,
       0,     0,   784,   785,   786,   787,   788,     0,     0,   789,
     790,   791,   792,     0,   793,   794,   795,   796,   782,   783,
       0,     0,   797,     0,   798,   799,     0,     0,     0,     0,
     800,     0,   802,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   804,     0,   805,   806,   807,   808,   809,
     810,   811,   812,   813,   814,   782,   783,     0,     0,     0,
       0,     0,     0,     0,   815,   816,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   805,   806,   807,   808,   809,   810,   811,
     812,   813,   814,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   815,   816,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   784,   785,   786,   787,   788,     0,     0,
     789,   790,   791,   792,     0,   793,   794,   795,   796,     0,
       0,     0,     0,   797,     0,   798,   799,     0,     0,     0,
       0,   800,     0,     0,     0,     0,     0,     0,     0,     0,
     784,   785,   786,   787,   788,     0,     0,   789,   790,   791,
     792,     0,   793,   794,   795,   796,     0,     0,     0,     0,
     797,     0,   798,   799,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,  1027,     0,     0,     0,     0,     0,
       0,     0,     0,   815,   816,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   805,   806,   807,   808,   809,   810,   811,   812,   813,
     814,  1031,     0,     0,     0,     0,     0,     0,     0,     0,
     815,   816,     0,     0,     0,     0,   335,   336,   337,     0,
     339,   340,   341,   342,   343,   501,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,     0,   357,   358,
     359,     0,     0,   362,   363,   364,   365,     0,     0,     0,
       0,     0,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   501,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1304,     0,     0,  1028,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1029,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   501,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
    1032,   357,   358,   359,     0,     0,   362,   363,   364,   365,
       0,     0,   335,   336,   337,  1033,   339,   340,   341,   342,
     343,   501,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,     0,   357,   358,   359,  1067,  1068,   362,
     363,   364,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1069,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1070,     0,     0,     0,     0,     0,     0,     0,     0,
     197,     0,     0,     0,     0,     0,     0,     0,     0,  1305,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1306,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1071,  1072,   198,     0,   199,     0,
     200,   201,   202,   203,   204,     0,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,     0,   216,   217,
     218,     0,     0,   219,   220,   221,   222,   914,   915,   916,
     917,   918,   919,   920,   921,     0,     0,     0,     0,     0,
     922,   923,     0,   223,   224,     0,   924,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   828,     0,     0,   925,
     926,     0,     0,     0,     0,     0,   927,   928,   929,     0,
       0,     0,     0,     0,     0,     0,   914,   915,   916,   917,
     918,   919,   920,   921,    13,     0,     0,     0,     0,   922,
     923,     0,     0,     0,     0,   924,     0,     0,   225,     0,
    -393,     0,     0,     0,    14,   828,     0,     0,   925,   926,
       0,     0,     0,   930,     0,   927,   928,   929,   914,   915,
     916,   917,   918,   919,   920,   921,     0,     0,     0,     0,
       0,   922,   923,     0,     0,     0,     0,   924,     0,     0,
     541,   722,     0,     0,     0,     0,     0,   828,     0,     0,
     925,   926,     0,     0,     0,     0,     0,   927,   928,   929,
       0,     0,   930,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   541,
     722,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   930,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   541,   722
};

static const yytype_int16 yycheck[] =
{
       1,   151,   520,     7,   227,   560,   233,   243,   664,   634,
     693,   611,   399,   399,   413,    15,    16,   444,   735,    20,
     737,   445,   173,   625,   737,   627,   956,   629,   757,   577,
    1268,   407,  1232,   972,    86,   671,   949,   766,   861,   978,
     863,   481,   865,   442,   901,   951,    20,    22,     8,     0,
      53,     5,    20,   601,   771,    19,     7,     4,    53,    20,
     444,   445,    34,    33,    20,   505,    19,    20,    68,    69,
      70,    20,   509,   510,    33,    15,    16,   109,    57,    30,
     158,    32,    57,    34,    63,  1443,    20,   127,   315,    40,
     173,    63,   129,   130,   481,   173,    20,    98,   734,    50,
     736,    20,   738,    12,  1656,    56,   166,    20,    20,    20,
     110,   111,   112,   113,    23,    24,   752,   143,   505,   518,
     519,   155,     7,    62,   143,   129,   160,   763,   143,    80,
     144,   145,   146,   155,    12,   107,   219,  1627,   160,   188,
     189,   194,   220,    15,    16,    23,    24,   173,   102,   103,
     127,   102,   103,   160,   194,   200,  1013,   197,   173,  1711,
     137,   397,   165,   158,   136,    50,   170,    46,   319,   218,
     399,   143,   399,   193,   219,   615,   724,   196,   173,   216,
     217,   910,   185,   235,   901,   164,   220,   196,   222,   193,
     194,   481,   201,   219,   126,  1553,   173,   129,   130,  1399,
     222,   173,   234,  1693,   164,   184,   220,   196,   193,   173,
     219,    21,    22,   160,   165,   222,   443,   194,   947,   193,
     173,   950,   199,   227,   184,   193,   166,   106,   615,   199,
     205,   171,   193,   173,   185,   236,   176,   193,   210,   143,
     199,   216,   246,   243,   193,   685,   200,   219,    53,   166,
     540,   401,   481,    21,    22,   206,   165,   217,   217,   193,
    1173,   484,   127,   899,   223,   219,   217,   651,  1174,   193,
     322,   173,   656,   193,   193,   498,   505,   717,  1663,   219,
     193,   193,   193,  1213,   216,   217,   193,   165,  1017,  1228,
    1675,   173,    97,   692,   166,   165,  1013,  1014,   685,   171,
     220,   173,   163,  1615,   176,  1617,  1163,  1545,   118,   119,
     314,   218,  1624,   317,   200,   185,   126,   716,   128,   129,
     130,   131,   183,  1523,   194,   142,   136,  1639,   193,   194,
     717,   196,   152,   219,   199,  1720,  1721,   127,   127,   221,
     160,     5,     6,   780,     8,  1275,   163,   137,   137,   165,
     118,   119,   152,   173,   143,   152,   992,   993,   126,   995,
     128,   129,   130,   131,   193,   127,   183,   165,   136,   369,
     597,   219,    36,   173,   601,   137,   173,   193,   165,  1691,
     616,   617,   618,   127,   620,   621,   615,   185,   624,   218,
     626,  1629,   628,   137,   684,   685,   163,   397,   185,   219,
     127,  1331,   778,   403,   194,   194,   216,   217,   848,   163,
     137,    57,   193,   413,   201,   216,   183,    63,   158,   219,
     127,   711,   219,  1025,   714,  1737,  1738,   654,   193,   183,
     137,   658,   194,   173,   202,   203,   204,   205,   206,   220,
     165,   173,   442,   107,   444,   844,  1163,   670,   216,   217,
     194,   173,   602,   218,   221,   222,   685,   193,   173,  1398,
     185,   848,  1774,   165,  1012,   219,   984,   194,   193,   132,
     854,  1410,   165,  1296,   697,   163,   201,   477,   478,   219,
     484,   771,   185,   185,   220,   869,   188,   194,   717,   165,
     193,  1009,   185,   494,   498,   183,   895,   896,   143,   173,
    1156,    47,   165,   165,   508,   509,   510,   152,   196,   185,
     909,  1228,   157,   173,  1231,  1228,   218,   193,   518,   519,
     743,    67,   185,   185,   143,   201,  1243,   964,   173,   222,
    1243,   193,   165,   152,   193,   173,   165,   764,   761,   201,
     939,   940,   193,   942,   545,   546,   836,   946,   932,   948,
     165,   173,   174,   175,   173,   200,   185,   163,   193,   218,
     193,   194,   563,   165,   565,   566,   567,   218,   569,  1392,
     185,   144,   201,   146,   185,    21,    22,   183,   193,   580,
     200,   177,   193,   185,   206,   220,   201,   193,   817,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,   219,
     829,   830,   831,   832,   833,   834,  1429,    68,    69,    70,
     611,   901,  1214,    57,   165,   185,   616,   617,   618,   848,
     620,   621,  1258,   193,   624,   165,   626,    33,   628,   185,
     630,   243,   193,   193,   185,   871,   193,   193,   193,  1076,
    1077,  1078,   193,    33,   880,   185,   197,   883,   193,   110,
     111,   112,   113,   193,    60,    61,   892,    21,    22,   220,
     220,   201,  1264,   220,   900,   220,   670,   221,   222,   905,
      60,    61,   118,   119,   165,   220,   165,   677,   165,   969,
     126,   165,   193,   129,   130,   131,   193,   977,   838,   165,
     136,   842,   692,   697,   185,  1378,   185,   193,   185,   165,
     173,   185,   193,   193,   193,   165,   193,   218,   931,   193,
     201,   218,   201,   173,   201,  1142,   716,   201,   124,   185,
    1144,   127,   128,  1440,   220,   185,  1549,   193,   165,  1513,
     220,   137,    12,   193,   124,   201,   210,   127,   128,   743,
     741,   193,   193,    23,    24,   177,  1402,   137,   185,  1466,
     986,   193,   988,   193,   118,   119,   193,   761,   758,   165,
     193,  1145,   126,   767,   128,   129,   130,   131,   220,   220,
     216,   217,   136,  1172,   152,   193,   780,   144,   220,   185,
     220,   193,   986,   185,   988,   397,   152,   220,   194,   195,
     164,   193,   196,   199,   165,   173,   202,   201,  1197,  1583,
    1584,   413,   220,   196,   194,   195,   218,   173,   201,   199,
     184,   217,   202,   184,   185,   219,   187,   223,  1602,  1603,
     196,   196,   106,   158,   199,   201,   219,   217,  1212,   194,
     442,   200,   444,   223,    66,  1437,  1391,   165,   173,   839,
     204,   205,   206,   219,   844,    79,   158,   185,   196,   850,
     219,   199,   216,   217,   164,   193,   184,   185,   186,   200,
      94,   173,  1487,   173,  1263,    99,  1522,   101,  1252,  1253,
    1160,   871,   196,   164,   184,   158,   158,   219,   219,    35,
     880,   185,   173,   883,   158,  1669,  1670,  1274,  1274,   193,
     173,   173,   892,   184,  1130,   895,   896,    75,    57,   173,
     900,    79,     5,     6,    63,   905,   518,   519,   193,   909,
      35,   196,  1595,    57,   199,    93,    94,   173,    57,    63,
      98,    99,   100,   101,    63,   173,   184,   931,    57,   187,
      57,  1221,   190,   196,    63,    57,    63,   200,    22,   939,
     940,    63,   942,   197,   173,  1674,   946,   201,   948,   949,
     177,   178,  1242,   197,  1671,  1555,  1685,   201,  1248,  1676,
     964,   197,   197,   173,  1200,   201,   201,  1257,  1593,   197,
     173,   972,    33,   201,   975,   197,   197,   978,   979,   201,
     201,   218,  1272,  1367,   196,  1714,   986,   218,   988,   197,
     196,  1375,   197,   201,  1619,  1712,   201,    33,   197,    60,
      61,   219,   201,   197,   616,   617,   618,   201,   620,   621,
      43,  1301,   624,  1531,   626,   200,   628,   200,   630,   177,
     178,   179,   180,  1313,    60,    61,  1682,   200,  1318,   219,
    1548,   221,   222,   144,   145,   146,   173,   174,   175,   177,
     178,   179,  1265,  1643,  1644,  1274,   200,  1274,    92,    93,
       5,     6,   219,   200,  1453,   108,   109,   110,   111,   112,
     113,   114,   115,   124,   219,  1782,   200,   128,   200,   200,
      25,  1361,  1076,  1077,  1078,   173,    31,   177,   178,   179,
     692,   463,   464,   465,   137,    10,    11,    12,   124,   219,
     200,    33,   128,   200,   147,   148,   149,   185,   197,   219,
     173,  1701,   220,  1393,   716,   218,   173,   173,   200,   217,
     200,   200,   166,    68,    69,   219,  1352,   200,    60,    61,
    1121,   200,   219,   200,  1125,   219,   200,    13,   200,   165,
    1130,  1421,  1422,  1134,   195,  1425,   200,  1360,   199,   219,
     201,   202,  1142,   200,   219,   200,   222,   102,   103,   185,
     219,   219,    10,    37,   219,  1554,   217,   193,    66,   195,
     219,   217,   223,   199,    43,   201,   202,   219,  1169,  1687,
     782,   783,  1172,  1173,   219,   200,   200,   220,   173,   218,
    1330,   217,   124,   196,   219,   797,   128,   223,   143,    43,
     219,   219,   200,   200,   200,    33,   193,  1197,   132,    14,
    1200,   173,   194,   815,   166,   160,   161,   162,   163,  1432,
     196,   222,   142,    13,   219,   218,   193,   193,   173,  1455,
     173,     8,    60,    61,   173,   173,   201,   220,   183,  1452,
     193,   220,   844,   173,   173,   173,   194,   219,   142,   193,
     184,   193,   220,  1642,   173,   200,   219,   193,   219,  1539,
     220,   219,   219,   195,   219,   200,     1,   199,   219,   871,
     202,  1265,   217,  1263,   200,    67,   201,    43,   880,   219,
     201,   883,   219,   201,   173,   217,   219,   196,   219,    12,
     892,   223,   219,   895,   896,   173,   124,   220,   900,   200,
     128,   173,   219,   905,   220,  1518,   220,   909,   220,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   827,  1599,
     829,   830,   831,   832,   833,   834,   218,    21,    22,   220,
     220,   218,   220,  1324,  1474,   218,   220,   939,   940,   219,
     942,   173,   173,  1569,   946,   220,   948,   949,   219,   219,
     173,  1631,   219,   219,   173,   219,   173,   219,   218,   181,
     173,   220,  1352,   218,   173,    33,  1360,   195,   399,   218,
      33,   199,  1585,   201,   202,   173,   219,   219,   219,   219,
     201,   220,    33,   219,   986,   219,   988,    70,   177,   217,
    1381,   201,    60,    61,   219,   223,   220,   201,   185,   219,
     185,   219,   184,    53,   220,   220,   220,  1398,   219,    60,
      61,   219,   508,   509,   510,  1695,   219,   219,   193,  1410,
     220,   220,   116,   117,   118,   119,   120,  1640,   185,   123,
     124,   125,   126,   193,   128,   129,   130,   131,  1432,   220,
     471,   220,   136,  1434,   138,   139,   218,   218,   218,   745,
     481,   220,   220,  1055,   218,   486,   124,   488,  1452,   220,
     128,   220,   220,  1453,   560,  1455,   497,  1684,   220,   220,
     220,   220,   218,   124,   505,   219,   184,   128,   220,   220,
     218,  1698,   218,    85,     1,    46,   141,    33,    88,  1546,
     839,  1011,   588,   241,  1546,  1546,     1,  1546,   529,  1546,
    1518,  1020,  1450,  1288,    10,   199,   200,   201,   202,   203,
     204,   205,   206,  1476,    60,    61,   547,  1734,  1580,  1327,
    1479,  1581,   216,   217,  1518,  1581,    56,   195,  1130,  1149,
     477,   199,   400,   201,   202,    -1,   477,   344,  1751,  1676,
    1142,  1529,  1240,   737,   195,    -1,    -1,    -1,   199,   217,
     201,   202,    -1,    -1,    -1,   223,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1554,    -1,   217,   598,    -1,    -1,
    1172,  1173,   223,    -1,    -1,    -1,    -1,    -1,   124,  1569,
      -1,    -1,   128,    -1,   615,    -1,    -1,    -1,    -1,    -1,
      -1,  1585,    -1,    -1,   690,  1197,    -1,    -1,  1200,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
    1750,    -1,    -1,    -1,    -1,   121,   122,  1608,    -1,    -1,
      -1,   127,    -1,   719,  1615,    -1,  1617,    -1,    -1,    -1,
      -1,   137,    -1,  1624,   140,   141,    -1,  1627,    -1,    -1,
    1780,   147,   148,   149,    -1,    -1,  1640,    33,  1639,   195,
      -1,    -1,  1642,   199,   685,   201,   202,    -1,    -1,    -1,
      -1,  1263,    -1,   694,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    60,    61,    -1,   223,    -1,    -1,
      -1,    -1,    -1,    -1,   780,    -1,   717,    -1,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1691,    -1,    -1,  1693,   735,    -1,   737,    -1,    -1,    -1,
      -1,    -1,    -1,  1704,    -1,   221,   222,    -1,    -1,  1710,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,  1737,  1738,    -1,    -1,
    1352,    -1,    -1,    -1,   850,  1746,  1747,  1751,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,  1774,    -1,  1776,   817,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,   828,   829,   830,
     831,   832,   833,   834,    -1,    -1,    60,    61,    -1,   195,
      -1,    -1,    -1,   199,    -1,   201,   202,   848,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   217,    -1,   123,   124,   125,   126,   223,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,   878,   138,   139,
      -1,  1453,    -1,  1455,   144,   145,   146,    -1,   954,    -1,
     150,    -1,    -1,   894,    -1,    -1,    -1,    -1,   964,    -1,
     124,   902,    -1,   904,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   914,   915,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,   926,   927,   928,   929,   930,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,    -1,   199,    -1,   201,   202,   410,
     981,    -1,  1554,    -1,    -1,    -1,    -1,    -1,   419,   990,
      -1,    -1,    -1,   217,    -1,    -1,    -1,  1569,   429,   223,
      -1,    -1,    -1,    -1,    -1,  1006,    -1,    -1,   439,    -1,
    1076,  1077,  1078,    -1,    -1,  1081,    -1,  1083,    -1,  1085,
      -1,  1087,    -1,  1089,    -1,  1091,    -1,  1093,   459,  1095,
      -1,  1097,    -1,  1099,    -1,  1101,    -1,    -1,  1104,    -1,
    1106,    -1,  1108,    -1,  1110,    -1,  1112,    -1,  1114,    -1,
     481,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1125,
      -1,    13,    -1,    -1,    21,    22,    -1,    19,    -1,    -1,
    1642,    -1,    -1,    25,    -1,   506,   507,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,   534,   535,   536,   537,   538,   539,   540,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,   143,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
    1201,    -1,    -1,   634,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   643,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,  1224,  1225,    -1,    -1,    -1,  1229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1237,    -1,   195,  1240,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,    -1,    -1,   684,   685,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,   220,    -1,   696,    -1,    -1,  1269,   221,
      -1,   702,    -1,  1274,    -1,    -1,    -1,   708,    -1,    -1,
     711,    -1,    -1,   714,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   728,   729,   730,
     731,   732,   733,    -1,    -1,    -1,    21,    22,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,  1391,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    21,    22,   144,   145,   146,    -1,
     771,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   784,   785,    -1,  1357,   788,   789,   790,
     791,    -1,   793,    -1,   795,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,    -1,   816,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,   116,   117,   118,   119,   836,    -1,    -1,   216,   217,
      -1,   126,   220,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    33,   136,
      -1,   138,   139,    -1,    -1,    -1,   887,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     901,    -1,    -1,    -1,    -1,    60,    61,    21,    22,    -1,
      -1,    -1,   913,    -1,    -1,    60,    61,   202,   203,   204,
     205,   206,    -1,    -1,    -1,    -1,  1497,    -1,  1499,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,  1509,  1510,
    1511,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      33,    -1,    -1,    -1,    -1,  1526,  1527,    -1,    -1,   216,
     217,  1532,    -1,    -1,    -1,    -1,   967,   968,   969,   124,
      -1,    -1,    -1,   128,    -1,  1546,   977,    60,    61,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,  1560,
      -1,    -1,    -1,    -1,    -1,    -1,   997,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,  1014,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,  1609,  1610,
     195,   124,    -1,    -1,   199,   128,   201,   202,    -1,    -1,
     195,    -1,    -1,    -1,   199,    -1,   201,   202,    -1,    -1,
      -1,  1632,   217,    -1,    -1,    -1,    -1,  1068,   223,    -1,
      -1,  1072,   217,    -1,  1645,    -1,  1647,    -1,   223,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,  1666,    -1,  1668,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,   195,    -1,    -1,    -1,   199,    -1,   201,   202,
      -1,    13,    -1,    -1,    33,  1696,    -1,    19,    -1,    -1,
      -1,  1702,  1703,    25,   217,    -1,    -1,    -1,    -1,    31,
     223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    60,    61,    -1,    -1,    -1,    -1,    49,  1729,  1160,
    1731,  1732,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,  1745,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    10,    -1,    -1,   124,    -1,    -1,    -1,   128,
    1221,    -1,  1223,    21,    22,  1226,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1242,    -1,    -1,    -1,    -1,    -1,  1248,    33,    -1,
      -1,   143,    -1,    -1,    -1,    -1,  1257,    -1,    -1,    -1,
    1261,  1262,    -1,    -1,   156,    -1,    -1,    33,    -1,    -1,
      -1,  1272,    -1,    -1,    -1,    60,    61,    -1,    -1,  1280,
      -1,   173,    -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,
     199,    -1,   201,   202,    60,    61,    -1,  1298,    -1,  1300,
    1301,  1302,    -1,    -1,    -1,    -1,    -1,  1308,   217,    -1,
      -1,  1312,  1313,    -1,   223,    -1,    -1,  1318,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,   221,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,   124,
     138,   139,    -1,   128,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,   124,    -1,
    1361,    -1,   128,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      -1,    -1,  1393,    21,    22,   193,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
     195,    -1,    -1,    -1,   199,    -1,   201,   202,   216,   217,
    1421,  1422,    -1,    -1,  1425,    -1,    -1,    -1,    -1,   195,
      -1,    33,   217,   199,    -1,   201,   202,    -1,   223,  1440,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,   223,    60,    61,
    1461,    -1,  1463,    -1,    -1,  1466,    -1,    -1,  1469,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1487,    19,   116,   117,
     118,   119,   120,    25,    -1,   123,   124,   125,   126,    31,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    41,
     138,   139,    -1,    -1,    -1,  1516,    -1,    49,    -1,    -1,
    1521,    -1,   124,    -1,  1525,    -1,   128,  1528,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,  1538,  1539,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,  1593,   195,    -1,    -1,    -1,   199,  1599,   201,
     202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,   217,    -1,    -1,  1619,    -1,
      -1,   223,    -1,    -1,   156,  1626,    -1,    -1,     1,    -1,
    1631,    -1,     5,     6,     7,    -1,     9,    10,    11,    -1,
      13,   173,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,
      -1,    44,    45,    -1,    -1,    48,    -1,    50,    51,    52,
      -1,    54,    55,    -1,    -1,    58,    59,  1688,    -1,   221,
      -1,   223,    65,    -1,  1695,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,  1740,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,    -1,   206,    -1,    -1,   209,   210,    -1,    -1,
       5,     6,    -1,    -1,   217,    -1,   219,    -1,   221,   222,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,    -1,    -1,   199,    -1,    -1,   202,   203,   204,
      -1,   206,    -1,    -1,   209,   210,    -1,    -1,    -1,     5,
       6,    -1,   217,    -1,   219,    -1,   221,   222,   223,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,
      -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,    -1,    -1,   199,    -1,    -1,   202,   203,   204,    -1,
     206,    -1,    -1,   209,   210,    -1,    -1,    -1,     5,     6,
      -1,   217,    -1,   219,    -1,   221,   222,   223,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,
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
      -1,    -1,   199,    -1,    -1,   202,   203,   204,    -1,   206,
      -1,    -1,   209,   210,    -1,    -1,    -1,     5,     6,    -1,
     217,    -1,   219,    -1,   221,   222,   223,    15,    16,    17,
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
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
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
      -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,    -1,    -1,     5,     6,   217,
      -1,   219,    -1,   221,   222,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
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
      -1,   209,   210,    -1,    -1,    -1,    -1,     5,     6,   217,
     218,   219,    -1,   221,   222,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
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
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,    -1,    -1,     5,     6,   217,
      -1,   219,    -1,   221,   222,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
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
      -1,   209,   210,    -1,    -1,    -1,    -1,     5,     6,   217,
     218,   219,    -1,   221,   222,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
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
      -1,   219,    -1,   221,   222,    15,    16,    17,    18,    19,
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
      -1,     5,     6,    -1,    -1,   217,   218,   219,    -1,   221,
     222,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,
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
     206,    -1,    -1,   209,   210,    -1,    -1,    -1,    -1,     5,
       6,   217,    -1,   219,    -1,   221,   222,    13,    -1,    15,
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
      -1,    -1,   200,    -1,   202,   203,   204,    -1,   206,    -1,
      -1,   209,   210,    -1,    -1,    -1,    -1,     5,     6,   217,
      -1,   219,    -1,   221,   222,    13,    -1,    15,    16,    17,
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
      -1,   219,    -1,   221,   222,    15,    16,    17,    18,    19,
      -1,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   200,    -1,   202,   203,
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
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
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
      19,    -1,    -1,    21,    22,   183,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,   202,   203,   204,    -1,   206,    -1,
      49,   209,   210,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,   219,    -1,   221,   222,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,    -1,    -1,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   173,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    41,   193,    -1,    -1,    -1,    -1,    -1,
      -1,    49,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,   216,   217,
      -1,    -1,   221,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,   173,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,   221,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    21,    22,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
     220,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,   220,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,   220,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,   220,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
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
      -1,   216,   217,   218,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   216,   217,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    21,    22,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    21,    22,   128,   129,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
     144,   145,   146,   147,   148,   149,   150,    -1,   195,    -1,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,   165,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,   185,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    21,
      22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    21,    22,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    -1,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    -1,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    71,
      72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
     158,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    71,    72,    73,   173,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,   129,   130,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    71,    -1,    73,    -1,
      75,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
     121,   122,    -1,   118,   119,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,
     141,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   165,    -1,    -1,    -1,    -1,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,   173,    -1,
     132,    -1,    -1,    -1,   185,   137,    -1,    -1,   140,   141,
      -1,    -1,    -1,   194,    -1,   147,   148,   149,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
     221,   222,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
     140,   141,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,
      -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   221,
     222,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   221,   222
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   225,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   206,   217,   226,   227,   231,
     240,   242,   243,   248,   279,   283,   319,   402,   409,   413,
     420,   467,   472,   477,    19,    20,   173,   271,   272,   273,
     166,   249,   250,   173,   174,   175,   206,   244,   245,   246,
     163,   183,   288,   410,   173,   221,   229,    57,    63,   405,
     405,   405,   143,   173,   305,    34,    63,   107,   136,   210,
     219,   275,   276,   277,   278,   305,   226,     5,     6,     8,
      36,   107,   417,    62,   400,   194,   193,   196,   193,   245,
      22,    57,   205,   216,   247,   405,   406,   408,   406,   400,
     478,   468,   473,   173,   143,   241,   277,   277,   277,   219,
     144,   145,   146,   193,   218,    57,    63,   284,   286,    57,
      63,   411,     5,     6,    57,    63,   418,    57,    63,   401,
      15,    16,   166,   171,   173,   176,   219,   233,   272,   166,
     250,   173,   244,   244,   173,   165,   185,   376,   164,   184,
     289,   406,   226,    57,    63,   228,   173,   173,   173,   173,
     177,   239,   220,   273,   277,   277,   277,   277,   287,   173,
     412,   421,   288,   403,   177,   178,   179,   232,    15,    16,
     166,   171,   173,   233,   269,   270,   247,   407,   165,   376,
     210,   230,   479,   469,   474,   177,   220,    35,    71,    73,
      75,    76,    77,    78,    79,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    93,    94,    95,    98,
      99,   100,   101,   118,   119,   173,   282,   285,   196,   288,
     106,   415,   416,   398,   160,   222,   274,   370,   177,   178,
     179,   193,   220,   194,    66,   196,   431,   288,   288,    21,
      22,    38,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     128,   129,   130,   137,   138,   139,   140,   141,   144,   145,
     146,   147,   148,   149,   150,   195,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   209,   210,   216,   217,
      35,    35,   219,   280,   288,   290,    75,    79,    93,    94,
      98,    99,   100,   101,   425,   404,   173,   422,   289,   399,
     273,   272,   222,   226,   152,   173,   396,   397,   269,    19,
      25,    31,    41,    49,    64,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   156,   221,
     305,   424,   426,   427,   432,   438,   466,    79,    94,    99,
     101,   288,   470,   475,   173,   173,   173,   218,    22,   173,
     218,   155,   220,   370,   381,   382,   383,   196,   281,   293,
     288,   173,   289,   196,   414,   288,   419,   370,   218,   272,
     219,    43,   193,   196,   199,   395,   200,   200,   200,   219,
     200,   200,   219,   431,   200,   200,   200,   200,   200,   219,
     305,    33,    60,    61,   124,   128,   195,   199,   202,   217,
     223,   437,   197,   430,   386,   389,   173,   108,   109,   110,
     111,   112,   113,   114,   115,   137,   147,   148,   149,   219,
       7,    50,   318,   185,   185,   193,   220,   466,     1,     5,
       6,     9,    10,    11,    13,    15,    16,    17,    18,    19,
      25,    26,    27,    28,    29,    31,    38,    39,    42,    44,
      45,    48,    51,    52,    54,    55,    58,    59,    65,    68,
      69,    80,   102,   103,   104,   105,   118,   119,   133,   134,
     135,   151,   152,   153,   154,   155,   157,   159,   160,   161,
     162,   165,   166,   167,   168,   169,   170,   171,   172,   174,
     175,   176,   183,   185,   202,   203,   204,   209,   210,   217,
     219,   221,   222,   238,   240,   251,   252,   255,   258,   259,
     261,   264,   265,   266,   267,   288,   289,   291,   292,   294,
     299,   304,   305,   306,   310,   311,   312,   313,   314,   315,
     316,   317,   319,   323,   324,   331,   334,   337,   342,   345,
     346,   348,   349,   350,   352,   357,   360,   361,   368,   369,
     424,   480,   495,   506,   510,   523,   526,   404,   194,   376,
     305,   377,   397,   218,    65,   104,   174,   299,   361,   173,
     173,   438,   127,   137,   194,   394,   439,   444,   446,   361,
     448,   442,   173,   200,   450,   452,   454,   456,   458,   460,
     462,   464,   361,   200,   219,    33,   199,    33,   199,   217,
     223,   218,   361,   217,   223,   438,   173,   289,   173,   193,
     226,   384,   435,   466,   471,   173,   387,   435,   476,   361,
     152,   173,   391,   392,   423,   383,   383,   383,   200,   200,
     368,   262,   263,   200,   307,   426,   480,   219,   305,   200,
       5,   102,   103,   200,   219,   127,   304,   335,   346,   361,
     368,   290,   200,   219,    61,   368,   219,   368,    19,   173,
     200,   200,   219,   226,   200,   166,    58,   368,   219,   290,
     200,   219,   200,   200,   219,   200,   200,   127,   304,   368,
     361,   361,   222,   290,   337,   341,   341,   341,   219,   219,
     219,   219,   219,   219,    13,   438,    13,   438,    13,   368,
     505,   521,   200,   368,   200,   237,    13,   361,   361,   361,
     361,   361,    13,    49,   295,   335,   361,   335,   222,   226,
     226,   368,    10,    13,   297,   505,   522,    37,   337,   343,
     173,   219,   226,   226,   226,   226,   226,    66,   320,   279,
     132,   226,    21,    22,   116,   117,   118,   119,   120,   123,
     124,   125,   126,   128,   129,   130,   131,   136,   138,   139,
     144,   145,   146,   150,   195,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   216,   217,   108,   109,   110,
     111,   112,   113,   114,   115,   121,   122,   127,   137,   140,
     141,   147,   148,   149,   194,   343,   219,   289,   368,   217,
     279,   289,   378,   376,   200,   220,    43,   226,   394,   304,
     368,   466,   466,   436,   466,   220,   466,   466,   220,   433,
     466,   280,   466,   280,   466,   280,   384,   385,   387,   388,
     220,   441,   295,   218,   218,   361,   165,   193,   194,   429,
     196,   435,   289,   196,   435,   289,   220,   219,    43,   127,
     193,   194,   196,   199,   390,   496,   498,   290,   423,   219,
     308,   219,   305,   200,   219,   332,   200,   200,   200,   517,
     335,   304,   335,   193,   108,   109,   110,   111,   112,   113,
     114,   115,   121,   122,   127,   140,   141,   147,   148,   149,
     194,    14,   438,   297,   368,   361,   290,   173,   194,   325,
     327,   361,   329,   196,   166,   361,   519,   335,   502,   507,
     335,   500,   438,   304,   368,   222,   279,   361,   361,   361,
     361,   361,   361,   423,    53,   158,   173,   202,   217,   219,
     368,   481,   484,   488,   504,   509,   423,   219,   484,   509,
     423,   142,   184,   186,   226,   489,   300,   290,   302,   179,
     180,   232,   219,   219,   423,    13,   218,   193,   525,   525,
     152,   157,   200,   305,   351,   290,   260,   423,   289,   193,
     525,   288,   344,    70,   217,   220,   335,   481,   483,   160,
     219,   322,   397,     4,   160,   340,   341,    19,   158,   173,
     424,    19,   158,   173,   424,   361,   361,   361,   361,   361,
     361,   173,   361,   158,   173,   361,   361,   361,   424,   361,
     361,   361,   361,   361,   361,    22,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   129,   130,   158,
     173,   216,   217,   358,   424,   361,   133,   134,   135,   291,
     347,   368,   347,   368,   347,   368,   347,   368,   347,   368,
     347,   368,   347,   368,   347,   368,   347,   368,   347,   368,
     347,   368,   368,   347,   368,   347,   368,   347,   368,   347,
     368,   347,   368,   347,   368,   220,   335,   376,   278,     8,
     370,   375,   438,   173,   304,   368,   226,   201,   201,   201,
     435,   201,   201,   173,   428,   201,   281,   201,   281,   201,
     281,   201,   435,   201,   435,   298,   466,   220,   218,   368,
     193,   466,   466,   361,   173,   173,   466,   368,   438,   438,
      20,   423,   466,    70,   335,   483,   494,   200,   368,   173,
     368,   466,   511,   513,   515,   438,   525,   361,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   290,   201,   435,   220,   220,
     194,   268,   438,   438,   220,   438,   220,   438,   525,   438,
     385,   525,   388,   201,   340,   220,   220,   220,   220,   220,
     220,    20,   341,   219,   137,   390,   217,   361,   220,   142,
     193,   226,   488,   188,   189,   218,   492,   193,   188,   218,
     226,   491,    20,   220,   488,   184,   187,   490,    20,   368,
     184,   505,   298,   298,   368,   423,   423,    20,   219,   423,
     220,   219,   219,   353,   355,    12,    23,    24,   165,   253,
     254,   368,    20,   505,   293,   279,   173,   220,   483,   481,
     193,   220,   193,   525,   220,   173,   321,   321,   219,   127,
     137,   173,   194,   199,   338,   339,   280,   200,   219,   200,
     219,   219,   219,   218,    19,   158,   173,   424,   196,   158,
     173,   361,   219,   219,   158,   173,   361,     1,   219,   218,
     341,   341,   341,   220,   218,    57,    63,   373,    67,   374,
     226,   201,   226,   440,   445,   447,   466,   449,   443,   201,
     226,   451,   201,   455,   201,   459,   201,   463,   384,   465,
     387,   201,   435,   165,   429,   220,    43,   390,   201,   201,
     335,    20,   201,   483,   220,   220,   220,   173,   220,   201,
     226,   220,   201,   438,   385,   388,   201,   220,   219,   438,
     466,   368,   201,   201,   201,   201,   220,   201,   201,   220,
     201,   340,   280,   219,   335,   361,   368,   368,   484,   488,
     368,   158,   173,   481,   492,   218,   368,   218,   504,   335,
     484,   184,   187,   190,   493,   218,   335,   201,   201,   196,
     235,    20,    20,   335,   423,    20,   361,   361,   438,   280,
     290,   254,   368,    12,   256,   335,   289,   340,   220,   218,
     217,   193,   218,   220,   339,   173,   173,   219,   173,   173,
     193,   218,   281,   362,   361,   364,   361,   220,   335,   361,
     200,   219,   361,   219,   218,   361,   217,   220,   335,   219,
     218,   359,   220,   335,   226,    47,   374,    46,   106,   371,
     376,   340,   434,   173,   453,   457,   461,   219,   466,   173,
     368,   497,   499,   290,   335,   309,   220,   201,   435,   219,
     173,   333,   201,   201,   201,   518,   297,   201,   226,   326,
     328,   330,   520,   503,   508,   501,   219,   343,   281,   220,
     335,   185,   220,   488,   492,   219,   137,   390,   185,   488,
     218,   185,   301,   303,   236,   181,   335,   335,   185,    20,
     335,   220,   220,   201,   281,   290,   257,   226,   185,   280,
     220,   481,   173,   218,   196,   395,   220,   173,   338,   218,
     142,   290,   336,   438,   220,   466,   220,   220,   220,   366,
     361,   361,   220,   481,   220,   361,   220,   376,    33,   372,
     371,   373,   295,   219,   219,   220,   368,   173,   368,   201,
     512,   514,   516,   219,   220,   219,   368,   368,   368,   219,
      70,   494,   219,   219,   220,   361,   336,   220,   361,   137,
     390,   492,   361,   368,   368,   361,   493,   505,   368,   219,
     296,   234,   220,   220,   361,   335,   185,   354,   201,   165,
     254,    26,   105,   258,   311,   312,   313,   315,   368,   505,
     281,   218,   196,   395,   438,   394,   220,   127,   368,   201,
     201,   466,   220,   220,   218,   220,   379,   372,   391,   392,
     393,   220,   494,   494,   290,   220,   201,   220,   219,   219,
     219,   219,   295,   297,   335,   494,   494,   220,   226,   524,
     368,   368,   220,   524,   524,   295,   177,   185,   185,   524,
     220,   361,   351,   356,   254,   127,   127,   368,   524,   290,
     220,   438,   394,   394,   368,   368,   363,   365,   201,   220,
     285,   380,   219,   481,   485,   486,   487,   487,   368,   368,
     494,   494,   481,   482,   220,   220,   525,   487,   482,    53,
     218,   137,   390,   184,   289,   193,   525,   505,   361,   218,
     185,   524,   351,   368,   289,   394,   368,   368,   226,   367,
     226,   285,   481,   193,   525,   220,   220,   220,   220,   487,
     487,   220,   220,   220,   220,   368,   218,   368,   368,   218,
     289,   220,   524,   524,   361,   218,   368,   226,   226,   376,
     290,   220,   219,   220,   220,   184,   218,   524,   226,   376,
     481,   218,   220
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   224,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   226,   226,   227,
     228,   228,   228,   229,   229,   230,   230,   231,   232,   232,
     232,   232,   233,   233,   234,   234,   235,   236,   235,   237,
     237,   237,   238,   239,   239,   241,   240,   242,   243,   244,
     244,   244,   245,   245,   245,   245,   246,   246,   247,   247,
     248,   249,   249,   250,   250,   251,   252,   252,   253,   253,
     254,   254,   254,   254,   255,   255,   256,   257,   256,   258,
     258,   258,   258,   258,   259,   259,   260,   259,   262,   261,
     263,   261,   264,   265,   266,   268,   267,   267,   269,   269,
     269,   269,   269,   269,   270,   270,   271,   271,   271,   272,
     272,   272,   272,   272,   272,   272,   272,   273,   273,   274,
     274,   274,   275,   275,   275,   275,   276,   276,   277,   277,
     277,   277,   277,   277,   277,   278,   278,   279,   279,   280,
     280,   280,   281,   281,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     283,   284,   284,   284,   285,   287,   286,   288,   288,   289,
     289,   290,   290,   291,   291,   291,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   293,   293,
     293,   294,   295,   295,   296,   296,   297,   297,   298,   298,
     300,   301,   299,   302,   303,   299,   304,   304,   304,   304,
     304,   305,   305,   305,   306,   306,   308,   309,   307,   307,
     310,   310,   310,   310,   310,   310,   311,   312,   313,   313,
     313,   314,   314,   314,   315,   315,   316,   316,   316,   317,
     318,   318,   318,   319,   319,   320,   320,   321,   321,   322,
     322,   322,   322,   323,   323,   325,   326,   324,   327,   328,
     324,   329,   330,   324,   332,   333,   331,   334,   334,   334,
     334,   334,   334,   335,   335,   336,   336,   336,   337,   337,
     337,   338,   338,   338,   338,   338,   339,   339,   340,   340,
     340,   341,   341,   342,   344,   343,   345,   345,   345,   345,
     345,   345,   345,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   347,   347,   347,   347,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   349,   349,   350,   350,   351,   351,   352,
     353,   354,   352,   355,   356,   352,   357,   357,   357,   357,
     357,   357,   357,   358,   359,   357,   360,   360,   360,   360,
     360,   360,   360,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   361,   361,   362,
     363,   361,   361,   361,   361,   364,   365,   361,   361,   361,
     366,   367,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   361,   361,   361,   361,   368,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   370,   370,   370,   371,   371,   371,
     372,   372,   373,   373,   373,   374,   374,   375,   376,   376,
     376,   376,   377,   378,   377,   379,   377,   380,   377,   377,
     381,   382,   382,   383,   383,   383,   383,   383,   384,   384,
     385,   385,   386,   386,   386,   387,   388,   388,   389,   389,
     389,   390,   390,   391,   391,   391,   392,   392,   393,   393,
     394,   394,   394,   395,   395,   396,   396,   396,   396,   396,
     397,   397,   397,   397,   397,   397,   398,   399,   398,   400,
     400,   401,   401,   401,   402,   403,   402,   404,   404,   404,
     405,   405,   405,   407,   406,   408,   408,   409,   410,   409,
     411,   411,   411,   412,   413,   413,   414,   414,   415,   415,
     416,   417,   417,   417,   417,   418,   418,   418,   419,   419,
     421,   422,   420,   423,   423,   423,   423,   423,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   425,   425,   425,   425,   425,
     425,   425,   425,   426,   427,   427,   427,   428,   428,   429,
     429,   430,   430,   430,   430,   430,   430,   430,   431,   431,
     431,   431,   431,   433,   434,   432,   435,   435,   436,   436,
     437,   437,   438,   438,   438,   438,   438,   438,   439,   440,
     438,   438,   438,   441,   438,   438,   438,   438,   438,   438,
     438,   438,   438,   438,   438,   438,   438,   442,   443,   438,
     438,   444,   445,   438,   446,   447,   438,   448,   449,   438,
     438,   450,   451,   438,   452,   453,   438,   438,   454,   455,
     438,   456,   457,   438,   438,   458,   459,   438,   460,   461,
     438,   462,   463,   438,   464,   465,   438,   466,   466,   466,
     468,   469,   470,   471,   467,   473,   474,   475,   476,   472,
     478,   479,   477,   480,   480,   480,   480,   480,   481,   481,
     481,   481,   481,   481,   481,   481,   482,   482,   483,   484,
     484,   485,   485,   486,   486,   487,   487,   488,   488,   489,
     489,   490,   490,   491,   491,   492,   492,   492,   493,   493,
     493,   494,   494,   495,   495,   495,   495,   495,   495,   496,
     497,   495,   498,   499,   495,   500,   501,   495,   502,   503,
     495,   504,   504,   504,   505,   505,   506,   507,   508,   506,
     509,   509,   510,   510,   510,   511,   512,   510,   513,   514,
     510,   515,   516,   510,   510,   517,   518,   510,   510,   519,
     520,   510,   521,   521,   522,   522,   523,   523,   523,   523,
     523,   524,   524,   525,   525,   526,   526,   526,   526,   526,
     526,   526,   526,   526
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     2,
       0,     1,     1,     1,     1,     0,     2,     5,     1,     1,
       2,     2,     3,     2,     0,     2,     0,     0,     3,     0,
       2,     5,     3,     1,     2,     0,     4,     2,     2,     1,
       1,     1,     1,     2,     3,     3,     2,     4,     0,     1,
       2,     1,     3,     1,     3,     3,     3,     2,     1,     1,
       0,     2,     4,     5,     1,     1,     0,     0,     3,     1,
       1,     1,     1,     1,     5,     4,     0,     6,     0,     6,
       0,     8,     2,     3,     3,     0,     6,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     5,     1,     3,     2,
       3,     2,     1,     1,     1,     1,     1,     4,     1,     2,
       3,     3,     3,     3,     2,     1,     3,     0,     3,     0,
       2,     3,     0,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       2,     2,     3,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     3,     2,     2,     2,     2,
       2,     3,     3,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     1,     3,     0,     4,     1,     1,     1,
       1,     3,     7,     2,     2,     6,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     2,     2,     0,     2,
       2,     3,     0,     2,     0,     4,     0,     2,     1,     3,
       0,     0,     7,     0,     0,     7,     3,     2,     2,     2,
       1,     1,     3,     2,     2,     3,     0,     0,     5,     1,
       2,     5,     5,     5,     6,     2,     1,     1,     1,     2,
       3,     2,     2,     3,     2,     3,     2,     2,     3,     4,
       1,     1,     0,     1,     1,     1,     0,     1,     3,     9,
       8,     8,     7,     3,     3,     0,     0,     7,     0,     0,
       7,     0,     0,     7,     0,     0,     6,     5,     8,    10,
       5,     8,    10,     1,     3,     1,     2,     3,     1,     1,
       2,     2,     2,     2,     2,     4,     1,     3,     0,     4,
       4,     1,     6,     6,     0,     7,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     8,     5,     6,     1,     4,     3,
       0,     0,     8,     0,     0,     9,     3,     4,     5,     6,
       8,     5,     6,     0,     0,     5,     3,     4,     4,     5,
       4,     3,     4,     1,     1,     1,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     4,     5,     4,     5,
       3,     4,     2,     5,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     1,     4,     4,     7,     8,     3,     0,
       0,     8,     3,     3,     3,     0,     0,     8,     3,     4,
       0,     0,     9,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     2,     4,     1,     1,     4,     4,
       4,     4,     4,     1,     6,     7,     6,     6,     7,     7,
       6,     7,     6,     6,     0,     4,     1,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     1,     5,     1,     1,
       2,     0,     0,     0,     5,     0,    10,     0,    11,     6,
       3,     3,     4,     1,     1,     3,     3,     3,     1,     3,
       1,     3,     0,     2,     3,     3,     1,     3,     0,     2,
       3,     1,     1,     1,     2,     3,     3,     5,     1,     1,
       1,     1,     1,     0,     1,     1,     4,     3,     3,     5,
       4,     6,     5,     5,     4,     4,     0,     0,     5,     0,
       1,     0,     1,     1,     6,     0,     6,     0,     3,     5,
       0,     1,     1,     0,     5,     2,     3,     4,     0,     4,
       0,     1,     1,     1,     7,     9,     0,     2,     0,     1,
       3,     1,     1,     2,     2,     0,     1,     1,     0,     3,
       0,     0,     7,     1,     4,     3,     3,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     1,     3,     1,
       2,     0,     3,     3,     2,     5,     5,     4,     0,     2,
       2,     2,     2,     0,     0,     7,     1,     1,     1,     3,
       3,     4,     1,     1,     1,     1,     2,     3,     0,     0,
       6,     4,     5,     0,     9,     4,     2,     2,     3,     2,
       3,     2,     2,     3,     3,     3,     2,     0,     0,     6,
       2,     0,     0,     6,     0,     0,     6,     0,     0,     6,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     0,     0,     6,     0,     0,     6,     1,     3,     3,
       0,     0,     0,     0,    10,     0,     0,     0,     0,    10,
       0,     0,     9,     1,     1,     1,     1,     1,     3,     3,
       5,     5,     6,     6,     8,     8,     0,     1,     2,     1,
       3,     3,     5,     1,     2,     1,     0,     0,     2,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     2,     1,
       1,     0,     1,     5,     4,     6,     7,     5,     7,     0,
       0,    10,     0,     0,    10,     0,     0,    10,     0,     0,
       7,     1,     3,     3,     3,     1,     5,     0,     0,    10,
       1,     3,     3,     4,     4,     0,     0,    11,     0,     0,
      11,     0,     0,    10,     5,     0,     0,     9,     5,     0,
       0,    10,     1,     3,     1,     3,     3,     3,     4,     7,
       9,     0,     3,     0,     1,     9,    11,    12,    11,    10,
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
            { deleteNameExprList(((*yyvaluep).pNameExprList)); }
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

  case 25: /* optional_not_required: %empty  */
        { (yyval.b) = false; }
    break;

  case 26: /* optional_not_required: '!' "inscope"  */
                        { (yyval.b) = true; }
    break;

  case 27: /* module_declaration: "module" module_name optional_shared optional_public_or_private_module optional_not_required  */
                                                                                                                                    {
        yyextra->g_Program->thisModuleName = *(yyvsp[-3].s);
        yyextra->g_Program->thisModule->isPublic = (yyvsp[-1].b);
        yyextra->g_Program->thisModule->isModule = true;
        yyextra->g_Program->thisModule->visibleEverywhere = (yyvsp[0].b);
        if ( yyextra->g_Program->thisModule->name.empty() ) {
            yyextra->g_Program->library.renameModule(yyextra->g_Program->thisModule.get(),*(yyvsp[-3].s));
        } else if ( yyextra->g_Program->thisModule->name != *(yyvsp[-3].s) ){
            das_yyerror(scanner,"this module already has a name " + yyextra->g_Program->thisModule->name,tokAt(scanner,(yylsp[-3])),
                CompilationError::module_already_has_a_name);
        }
        if ( !yyextra->g_Program->policies.ignore_shared_modules ) {
            yyextra->g_Program->promoteToBuiltin = (yyvsp[-2].b);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 28: /* character_sequence: STRING_CHARACTER  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += (yyvsp[0].ch); }
    break;

  case 29: /* character_sequence: STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = new string(); *(yyval.s) += "\\\\"; }
    break;

  case 30: /* character_sequence: character_sequence STRING_CHARACTER  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += (yyvsp[0].ch); }
    break;

  case 31: /* character_sequence: character_sequence STRING_CHARACTER_ESC  */
                                                                                  { (yyval.s) = (yyvsp[-1].s); *(yyvsp[-1].s) += "\\\\"; }
    break;

  case 32: /* string_constant: "start of the string" character_sequence "end of the string"  */
                                                           { (yyval.s) = (yyvsp[-1].s); }
    break;

  case 33: /* string_constant: "start of the string" "end of the string"  */
                                                           { (yyval.s) = new string(); }
    break;

  case 34: /* format_string: %empty  */
        { (yyval.s) = new string(); }
    break;

  case 35: /* format_string: format_string STRING_CHARACTER  */
                                                 { (yyval.s) = (yyvsp[-1].s); (yyvsp[-1].s)->push_back((yyvsp[0].ch)); }
    break;

  case 36: /* optional_format_string: %empty  */
        { (yyval.s) = new string(""); }
    break;

  case 37: /* $@1: %empty  */
            { das_strfmt(scanner); }
    break;

  case 38: /* optional_format_string: ':' $@1 format_string  */
                                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 39: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 40: /* string_builder_body: string_builder_body character_sequence  */
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

  case 41: /* string_builder_body: string_builder_body "{" expr optional_format_string "}"  */
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

  case 42: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 43: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 44: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 45: /* $@2: %empty  */
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

  case 46: /* expr_reader: '%' name_in_namespace $@2 reader_character_sequence  */
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

  case 47: /* options_declaration: "options" annotation_argument_list  */
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
        for ( auto & opt : *(yyvsp[0].aaList) ) {
            if ( yyextra->g_Access->isOptionAllowed(opt.name, yyextra->g_Program->thisModule->fileName) ) {
                yyextra->g_Program->options.push_back(opt);
            } else {
                das_yyerror(scanner,"option " + opt.name + " is not allowed here",
                    tokAt(scanner,(yylsp[0])), CompilationError::invalid_option);
            }
        }
        delete (yyvsp[0].aaList);
    }
    break;

  case 49: /* keyword_or_name: "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 50: /* keyword_or_name: "keyword"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 51: /* keyword_or_name: "type function"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 52: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 53: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 54: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 55: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 56: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 57: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 58: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 59: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 63: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 64: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 65: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 66: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 67: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 68: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 69: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 70: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 71: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 72: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
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

  case 73: /* expression_else: elif_or_static_elif expr expression_block SEMICOLON expression_else  */
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

  case 74: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 75: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 76: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 77: /* $@3: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 78: /* expression_else_one_liner: "else" $@3 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 79: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 80: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 81: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 82: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 83: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 84: /* expression_if_then_else: if_or_static_if expr expression_block SEMICOLON expression_else  */
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

  case 85: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
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

  case 86: /* $@4: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 87: /* expression_if_then_else: expression_if_one_liner "if" $@4 expr expression_else_one_liner semicolon  */
                                                                                                                                                         {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-2])), (yyvsp[-2].pExpression)->at);

        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),(yyvsp[-2].pExpression),ast_wrapInBlock((yyvsp[-5].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 88: /* $@5: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 89: /* expression_for_loop: "for" $@5 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        format::wrap_par_expr(tokRangeAt(scanner, (yylsp[-3]), (yylsp[-1])), tokRangeAt(scanner, (yylsp[-3]), (yylsp[-1])));
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 90: /* $@6: %empty  */
                     { // Had to add to successfully convert to v2 syntax, just copied from ds2_parser
                             yyextra->das_keyword = true;
     }
    break;

  case 91: /* expression_for_loop: "for" $@6 '(' variable_name_with_pos_list "in" expr_list ')' expression_block  */
                                                                                                  {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 92: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 93: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at);

        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = (yyvsp[-1].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 94: /* expression_with: "with" expr expression_block  */
                                                         {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at);
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = (yyvsp[-1].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 95: /* $@7: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 96: /* expression_with_alias: "assume" "name" '=' $@7 expr semicolon  */
                                                                                                         {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-5])), *(yyvsp[-4].s), ExpressionPtr((yyvsp[-1].pExpression)));
        delete (yyvsp[-4].s);
    }
    break;

  case 97: /* expression_with_alias: "assume" "type" "name" '=' type_declaration  */
                                                                        {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-2].s), TypeDeclPtr((yyvsp[0].pTypeDecl)));
    }
    break;

  case 98: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 99: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 100: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 101: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 102: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 103: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 104: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 105: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 106: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 107: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 108: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 112: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 113: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 114: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 115: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 116: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                               {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]))); delete (yyvsp[-4].s); }
    }
    break;

  case 117: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 118: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 119: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 120: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 121: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 122: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 123: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 124: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 125: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 126: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 127: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 128: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 129: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 130: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 131: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 132: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 133: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 134: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 135: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 136: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 137: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 138: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 139: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 140: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 141: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 142: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 143: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 144: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 145: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 146: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 147: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 148: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 149: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 150: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 151: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 152: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 153: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 154: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 155: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 156: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 157: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 158: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 159: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 160: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 161: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 162: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 163: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 164: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 165: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 166: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 167: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 168: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 169: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 170: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 171: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 172: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 173: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 174: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 175: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 176: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 177: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 178: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 179: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 180: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 181: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 182: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 183: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 184: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 185: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 186: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 187: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 188: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 189: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 190: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 191: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 192: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 193: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 194: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 195: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 196: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 197: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 198: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 199: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 200: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 201: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 202: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 203: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 204: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 205: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 206: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 207: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 208: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 209: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 210: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 211: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 212: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 213: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 214: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 215: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 216: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 217: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 218: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 219: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 220: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 221: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 222: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 223: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 224: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 225: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 226: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 227: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 228: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 229: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 230: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 231: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 232: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 233: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 234: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 235: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 236: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 237: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 238: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 239: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 240: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 241: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 242: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 243: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 244: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 245: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 246: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
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

  case 247: /* open_block: "begin of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 248: /* open_block: "new scope"  */
                        { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 249: /* close_block: "end of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 250: /* close_block: "close scope"  */
                         { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 251: /* expression_block: open_block expressions close_block  */
                                                                  {
        auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui), format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size, format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 252: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
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

  case 253: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 254: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 255: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 256: /* expression_any: SEMICOLON  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 257: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 258: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 259: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 260: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 261: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 262: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 263: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 264: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 265: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 266: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 267: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 268: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 269: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 270: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 271: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 272: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 273: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 274: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 275: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 276: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 277: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 278: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 279: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 280: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 281: /* expr_keyword: "keyword" expr expression_block  */
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

  case 282: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 283: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 284: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 285: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 286: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 287: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 288: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 289: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 290: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 291: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 292: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 293: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 294: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 295: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 296: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 297: /* expr_pipe: "@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }

        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 298: /* expr_pipe: "@@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 299: /* expr_pipe: "$ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "$";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 300: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 301: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 302: /* name_in_namespace: "name" "::" "name"  */
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

  case 303: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 304: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 305: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 306: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 307: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 308: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 309: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 310: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 311: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 312: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 313: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 314: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 315: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 316: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 317: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 318: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 319: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 320: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 321: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 322: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 323: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 324: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 325: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 326: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 327: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 328: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 329: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                              {
        const auto end_block = format::Pos::from_last(tokAt(scanner, (yylsp[-2])));
        const auto start = format::Pos::from(tokAt(scanner, (yylsp[-3])));
        if (format::is_replace_braces()) {
            format::skip_spaces_or_print(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])), yyextra->das_tab_size);
        }

        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 330: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 331: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 332: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 333: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 334: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 335: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 336: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 337: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 338: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 339: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
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

  case 340: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 341: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 342: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 343: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 344: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 345: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 346: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 347: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 348: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 349: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 350: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 351: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 352: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 353: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 354: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 355: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 356: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 357: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 358: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 359: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 360: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 361: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 362: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" semicolon "name" '>' '(' expr ')'  */
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

  case 363: /* expr_list: expr2  */
                       {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 364: /* expr_list: expr_list ',' expr2  */
                                             {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 365: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 366: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 367: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 368: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 369: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 370: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 371: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 372: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 373: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 374: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 375: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 376: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 377: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 378: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 379: /* optional_capture_list: "[[" capture_list ']' ']'  */
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

  case 380: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 381: /* expr_block: expression_block  */
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

  case 382: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 383: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 384: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 385: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 386: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 387: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 388: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 389: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 390: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 391: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 392: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 393: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 394: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 399: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 400: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 401: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 410: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 411: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 412: /* expr_assign_pipe_right: "@ <|" expr_block  */
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

  case 413: /* expr_assign_pipe_right: "@@ <|" expr_block  */
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

  case 414: /* expr_assign_pipe_right: "$ <|" expr_block  */
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

  case 415: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 416: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 420: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 423: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 424: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 425: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 430: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 431: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 432: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 433: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 434: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 435: /* expr_method_call: expr2 "->" "name" '(' ')'  */
                                                          {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 436: /* expr_method_call: expr2 "->" "name" '(' expr_list ')'  */
                                                                               {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 437: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 438: /* func_addr_name: "$i" '(' expr2 ')'  */
                                           {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 439: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 440: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 441: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 442: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 443: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 444: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 445: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
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

  case 446: /* expr_field: expr2 '.' "name"  */
                                               {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 447: /* expr_field: expr2 '.' '.' "name"  */
                                                   {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 448: /* expr_field: expr2 '.' "name" '(' ')'  */
                                                       {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 449: /* expr_field: expr2 '.' "name" '(' expr_list ')'  */
                                                                            {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 450: /* expr_field: expr2 '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 451: /* expr_field: expr2 '.' basic_type_declaration '(' ')'  */
                                                                         {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 452: /* expr_field: expr2 '.' basic_type_declaration '(' expr_list ')'  */
                                                                                              {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 453: /* $@28: %empty  */
                                { yyextra->das_suppress_errors=true; }
    break;

  case 454: /* $@29: %empty  */
                                                                             { yyextra->das_suppress_errors=false; }
    break;

  case 455: /* expr_field: expr2 '.' $@28 error $@29  */
                                                                                                                     {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 456: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 457: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 458: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 459: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 460: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 461: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 462: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 463: /* expr2: name_in_namespace  */
                                              { need_wrap_current_expr = true; (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 464: /* expr2: expr_field  */
                                              { need_wrap_current_expr = true; (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 465: /* expr2: expr_mtag  */
                                              { need_wrap_current_expr = true; (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 466: /* expr2: '!' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"!",(yyvsp[0].pExpression)); }
    break;

  case 467: /* expr2: '~' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"~",(yyvsp[0].pExpression)); }
    break;

  case 468: /* expr2: '+' expr2  */
                                                   { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+",(yyvsp[0].pExpression)); }
    break;

  case 469: /* expr2: '-' expr2  */
                                                   { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"-",(yyvsp[0].pExpression)); }
    break;

  case 470: /* expr2: expr2 "<<" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr2: expr2 ">>" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr2: expr2 "<<<" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr2: expr2 ">>>" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr2: expr2 '+' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr2: expr2 '-' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr2: expr2 '*' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr2: expr2 '/' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr2: expr2 '%' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr2: expr2 '<' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr2: expr2 '>' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr2: expr2 "==" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr2: expr2 "!=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr2: expr2 "<=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr2: expr2 ">=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr2: expr2 '&' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr2: expr2 '|' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr2: expr2 '^' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr2: expr2 "&&" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr2: expr2 "||" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr2: expr2 "^^" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr2: expr2 ".." expr2  */
                                               {
        need_wrap_current_expr = true;
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 492: /* expr2: "++" expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"++", (yyvsp[0].pExpression)); }
    break;

  case 493: /* expr2: "--" expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"--", (yyvsp[0].pExpression)); }
    break;

  case 494: /* expr2: expr2 "++"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 495: /* expr2: expr2 "--"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 496: /* expr2: expr2 '[' expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 497: /* expr2: expr2 '.' '[' expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 498: /* expr2: expr2 "?[" expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 499: /* expr2: expr2 '.' "?[" expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 500: /* expr2: expr2 "?." "name"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 501: /* expr2: expr2 '.' "?." "name"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 502: /* expr2: '*' expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 503: /* expr2: expr2 '?' expr2 ':' expr2  */
                                                             {
        need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp3(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 504: /* expr2: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 505: /* expr2: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 506: /* expr2: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 507: /* expr2: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 508: /* expr2: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 509: /* expr2: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 510: /* expr2: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 511: /* expr2: '(' expr_list optional_comma ')'  */
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

  case 512: /* expr2: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 513: /* expr2: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 514: /* expr2: "deref" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 515: /* expr2: "addr" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 516: /* expr2: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 517: /* expr2: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr2 ')'  */
                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 518: /* expr2: expr2 "??" expr2  */
                                                     { (yyval.pExpression) = new ExprNullCoalescing(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 519: /* $@30: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 520: /* $@31: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 521: /* expr2: expr2 "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                             {
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 522: /* expr2: expr2 "is" basic_type_declaration  */
                                                                {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 523: /* expr2: expr2 "is" "name"  */
                                               {
        (yyval.pExpression) = new ExprIsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 524: /* expr2: expr2 "as" "name"  */
                                               {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 525: /* $@32: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 526: /* $@33: %empty  */
                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 527: /* expr2: expr2 "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                  {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 528: /* expr2: expr2 "as" basic_type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 529: /* expr2: expr2 '?' "as" "name"  */
                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 530: /* $@34: %empty  */
                                                    { yyextra->das_arrow_depth ++; }
    break;

  case 531: /* $@35: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 532: /* expr2: expr2 '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                      {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-8]), (yylsp[-1])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 533: /* expr2: expr2 '?' "as" basic_type_declaration  */
                                                                    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 534: /* expr2: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 535: /* expr2: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 536: /* expr2: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 537: /* expr2: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 538: /* expr2: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); (yyval.pExpression)->at = tokAt(scanner, (yylsp[0])); }
    break;

  case 539: /* expr2: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 540: /* expr2: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 541: /* expr2: expr2 "<|" expr2  */
                                                  { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); }
    break;

  case 542: /* expr2: expr2 "|>" expr2  */
                                                  { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]), (yylsp[0]))); (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])); }
    break;

  case 543: /* expr2: expr2 "|>" basic_type_declaration  */
                                                           {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
    }
    break;

  case 544: /* expr2: name_in_namespace "name"  */
                                                  {
        if (format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << "." << format::get_substring(tokAt(scanner,(yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 545: /* expr2: "unsafe" '(' expr2 ')'  */
                                          {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 546: /* expr2: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 547: /* expr: expr2  */
                                       {
        if (need_wrap_current_expr) {
            format::wrap_par_expr_newline(tokAt(scanner,(yylsp[0])), (yyvsp[0].pExpression)->at);
            need_wrap_current_expr = false;
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 548: /* expr_mtag: "$$" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 549: /* expr_mtag: "$i" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 550: /* expr_mtag: "$v" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 551: /* expr_mtag: "$b" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 552: /* expr_mtag: "$a" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 553: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 554: /* expr_mtag: "$c" '(' expr2 ')' '(' ')'  */
                                                             {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 555: /* expr_mtag: "$c" '(' expr2 ')' '(' expr_list ')'  */
                                                                                 {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 556: /* expr_mtag: expr2 '.' "$f" '(' expr2 ')'  */
                                                                  {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 557: /* expr_mtag: expr2 "?." "$f" '(' expr2 ')'  */
                                                                   {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 558: /* expr_mtag: expr2 '.' '.' "$f" '(' expr2 ')'  */
                                                                      {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 559: /* expr_mtag: expr2 '.' "?." "$f" '(' expr2 ')'  */
                                                                       {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 560: /* expr_mtag: expr2 "as" "$f" '(' expr2 ')'  */
                                                                     {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 561: /* expr_mtag: expr2 '?' "as" "$f" '(' expr2 ')'  */
                                                                         {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 562: /* expr_mtag: expr2 "is" "$f" '(' expr2 ')'  */
                                                                     {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 563: /* expr_mtag: '@' '@' "$c" '(' expr2 ')'  */
                                                          {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 564: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 565: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
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

  case 566: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 567: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 568: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 569: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 570: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 571: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 572: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 573: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 574: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 575: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 576: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 577: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 578: /* opt_sem: SEMICOLON  */
                { (yyval.b) = false; }
    break;

  case 579: /* opt_sem: "end of expression"  */
          { (yyval.b) = true; }
    break;

  case 580: /* opt_sem: "end of expression" SEMICOLON  */
                    { (yyval.b) = true; }
    break;

  case 581: /* opt_sem: %empty  */
                  {(yyval.b) = false; }
    break;

  case 582: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 583: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 584: /* struct_variable_declaration_list: struct_variable_declaration_list $@36 structure_variable_declaration semicolon opt_sem  */
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

  case 585: /* $@37: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 586: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@37 function_declaration_header semicolon opt_sem  */
                                                                  {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-6].b),(yyvsp[-4].b), (yyvsp[-2].pFuncDecl));
            }
    break;

  case 587: /* $@38: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 588: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@38 function_declaration_header expression_block opt_sem  */
                                                                                {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[-1])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 589: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon opt_sem  */
                                                                                               {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-3])), CompilationError::syntax_error);
        delete (yyvsp[-3].faList);
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
    }
    break;

  case 590: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 591: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 592: /* function_argument_declaration_type: "$a" '(' expr2 ')'  */
                                      {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 593: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 594: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 595: /* function_argument_list: function_argument_declaration_no_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 596: /* function_argument_list: function_argument_declaration_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 597: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 598: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 599: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 600: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 601: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 602: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 603: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 604: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 605: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 606: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 607: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 608: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 609: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 610: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 611: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 612: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 613: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 614: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 615: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 616: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 617: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 618: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 619: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 620: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 621: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 622: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 623: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 624: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 625: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 626: /* let_variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 627: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 628: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 629: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 630: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 631: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 632: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 633: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 634: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr  */
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

  case 635: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 636: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 637: /* $@39: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 638: /* global_variable_declaration_list: global_variable_declaration_list $@39 optional_field_annotation let_variable_declaration opt_sem  */
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

  case 639: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 640: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 641: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 642: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 643: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 644: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                                   {
        handle_brace(format::Pos::from(tokAt(scanner, (yylsp[-2]))), (yyvsp[-2].ui),
                     format::get_substring(tokRangeAt(scanner, (yylsp[-2]), (yylsp[0]))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 645: /* $@40: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 646: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@40 optional_field_annotation let_variable_declaration  */
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

  case 647: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 648: /* enum_list: enum_list "name" opt_sem  */
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

  case 649: /* enum_list: enum_list "name" '=' expr opt_sem  */
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

  case 650: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 651: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 652: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 653: /* $@41: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 654: /* single_alias: optional_public_or_private_alias "name" $@41 '=' type_declaration  */
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

  case 655: /* alias_list: single_alias opt_sem  */
                                    {
        (yyval.positions) = new vector<LineInfo>(1, tokAt(scanner, (yylsp[-1])));
    }
    break;

  case 656: /* alias_list: alias_list single_alias opt_sem  */
                                                       {
        (yyvsp[-2].positions)->emplace_back(tokAt(scanner, (yylsp[-1])));
        (yyval.positions) = (yyvsp[-2].positions);
    }
    break;

  case 657: /* alias_declaration: "typedef" open_block alias_list close_block  */
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

  case 658: /* $@42: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 660: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 661: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 662: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 663: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 664: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block enum_list close_block  */
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

  case 665: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block enum_list close_block  */
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

  case 666: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 667: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 668: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 669: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 670: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 671: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 672: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 673: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 674: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 675: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 676: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 677: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 678: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 679: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                                      {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 680: /* $@43: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 681: /* $@44: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 682: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@43 structure_name $@44 optional_struct_variable_declaration_list  */
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

  case 683: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 684: /* variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 685: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 686: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 687: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 688: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 689: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 690: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 691: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 692: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 693: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 694: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 695: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 696: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 697: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 698: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 699: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 700: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 701: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 702: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 703: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 704: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 705: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 706: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 707: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 708: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 709: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 710: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 711: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 712: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 713: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 714: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 715: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 716: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 717: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 718: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 719: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 720: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 721: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 722: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 723: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 724: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 725: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 726: /* auto_type_declaration: "$t" '(' expr2 ')'  */
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

  case 727: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 728: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 731: /* bitfield_alias_bits: %empty  */
       {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 732: /* bitfield_alias_bits: bitfield_alias_bits "name" SEMICOLON  */
                                                       {
        if (format::enum_bitfield_with_comma() && format::is_replace_braces() && format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << ",";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        }
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pNameExprList) = (yyvsp[-2].pNameExprList);
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-1].s),nullptr);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-1].s)->c_str(),atvname);
        }
        delete (yyvsp[-1].s);
    }
    break;

  case 733: /* bitfield_alias_bits: bitfield_alias_bits "name" commas  */
                                                    {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pNameExprList) = (yyvsp[-2].pNameExprList);
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-1].s),nullptr);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-1].s)->c_str(),atvname);
        }
        delete (yyvsp[-1].s);
    }
    break;

  case 734: /* bitfield_alias_bits: bitfield_alias_bits "name"  */
                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.pNameExprList) = (yyvsp[-1].pNameExprList);
        (yyval.pNameExprList)->emplace_back(*(yyvsp[0].s),nullptr);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[0].s)->c_str(),atvname);
        }
        delete (yyvsp[0].s);
    }
    break;

  case 735: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr SEMICOLON  */
                                                                       {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyval.pNameExprList) = (yyvsp[-4].pNameExprList);
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-3].s),(yyvsp[-1].pExpression));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-3].s)->c_str(),atvname);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 736: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr commas  */
                                                                    {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyval.pNameExprList) = (yyvsp[-4].pNameExprList);
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-3].s),(yyvsp[-1].pExpression));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-3].s)->c_str(),atvname);
        }
        delete (yyvsp[-3].s);
    }
    break;

  case 737: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr  */
                                                             {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        (yyval.pNameExprList) = (yyvsp[-3].pNameExprList);
        (yyval.pNameExprList)->emplace_back(*(yyvsp[-2].s),(yyvsp[0].pExpression));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntry((yyvsp[-2].s)->c_str(),atvname);
        }
        delete (yyvsp[-2].s);
    }
    break;

  case 738: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 739: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 740: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 741: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 742: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 743: /* $@45: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 744: /* $@46: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 745: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@45 bitfield_bits '>' $@46  */
                                                                                                                                                             {
            (yyval.pTypeDecl) = new TypeDecl((yyvsp[-5].type));
            (yyval.pTypeDecl)->argNames = *(yyvsp[-2].pNameList);
            auto maxBits = (yyval.pTypeDecl)->maxBitfieldBits();
            if ( (yyval.pTypeDecl)->argNames.size()>maxBits ) {
                das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                    CompilationError::invalid_type);
            }
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
            delete (yyvsp[-2].pNameList);
    }
    break;

  case 748: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 749: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 750: /* dim_list: '[' expr2 ']'  */
                              {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 751: /* dim_list: dim_list '[' expr2 ']'  */
                                             {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 752: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 753: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 754: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 755: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 756: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 757: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 758: /* $@47: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 759: /* $@48: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 760: /* type_declaration_no_options: "type" '<' $@47 type_declaration '>' $@48  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 761: /* type_declaration_no_options: "typedecl" '(' expr2 ')'  */
                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 762: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 763: /* $@49: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 764: /* type_declaration_no_options: '$' name_in_namespace '<' $@49 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 765: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 766: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 767: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 768: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 769: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 770: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 771: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 772: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 773: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 774: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 775: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 776: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 777: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 778: /* $@51: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 779: /* type_declaration_no_options: "smart_ptr" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 780: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 781: /* $@52: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 782: /* $@53: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 783: /* type_declaration_no_options: "array" '<' $@52 type_declaration '>' $@53  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 784: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 785: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 786: /* type_declaration_no_options: "table" '<' $@54 table_type_pair '>' $@55  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 787: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 788: /* $@57: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 789: /* type_declaration_no_options: "iterator" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 790: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 791: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 792: /* $@59: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 793: /* type_declaration_no_options: "block" '<' $@58 type_declaration '>' $@59  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 794: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 795: /* $@61: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 796: /* type_declaration_no_options: "block" '<' $@60 optional_function_argument_list optional_function_type '>' $@61  */
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

  case 797: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 798: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 799: /* $@63: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 800: /* type_declaration_no_options: "function" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 801: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 802: /* $@65: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 803: /* type_declaration_no_options: "function" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
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

  case 804: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 805: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 806: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 807: /* type_declaration_no_options: "lambda" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 808: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 809: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 810: /* type_declaration_no_options: "lambda" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 811: /* $@70: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 812: /* $@71: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 813: /* type_declaration_no_options: "tuple" '<' $@70 tuple_type_list '>' $@71  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 814: /* $@72: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 815: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 816: /* type_declaration_no_options: "variant" '<' $@72 variant_type_list '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 817: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 818: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 819: /* type_declaration: type_declaration '|' '#'  */
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

  case 820: /* $@74: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 821: /* $@75: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 822: /* $@76: %empty  */
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

  case 823: /* $@77: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 824: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@74 "name" $@75 open_block $@76 tuple_alias_type_list $@77 close_block  */
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

  case 825: /* $@78: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 826: /* $@79: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 827: /* $@80: %empty  */
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

  case 828: /* $@81: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 829: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@78 "name" $@79 open_block $@80 variant_alias_type_list $@81 close_block  */
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

  case 830: /* $@82: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 831: /* $@83: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 832: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@82 "name" $@83 bitfield_basic_type_declaration open_block bitfield_alias_bits close_block  */
                                                                                                              {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-5]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
        auto btype = make_smart<TypeDecl>((yyvsp[-3].type));
        btype->alias = *(yyvsp[-5].s);
        btype->at = tokAt(scanner,(yylsp[-5]));
        btype->isPrivateAlias = !(yyvsp[-7].b);
        for ( auto & p : *(yyvsp[-1].pNameExprList) ) {
            if ( !get<1>(p) ) {
                btype->argNames.push_back(get<0>(p));
            }
        }
        auto maxBits = btype->maxBitfieldBits();
        if ( btype->argNames.size()>maxBits ) {
            das_yyerror(scanner,"only " + to_string(maxBits) + " different bits are allowed in a bitfield",tokAt(scanner,(yylsp[-5])),
                CompilationError::invalid_type);
        }
        for ( auto & p : *(yyvsp[-1].pNameExprList) ) {
            if ( get<1>(p) ) {
                ast_globalBitfieldConst ( scanner, btype, (yyvsp[-7].b), get<0>(p), get<1>(p) );
            }
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
        delete (yyvsp[-1].pNameExprList);
    }
    break;

  case 833: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 834: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 835: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 836: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 837: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 838: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 839: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 840: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 841: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 842: /* make_struct_fields: "$f" '(' expr2 ')' copy_or_move expr  */
                                                                            {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 843: /* make_struct_fields: "$f" '(' expr2 ')' ":=" expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner, (yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 844: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' copy_or_move expr  */
                                                                                                        {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 845: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' ":=" expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 846: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 847: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 848: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 849: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 850: /* make_struct_dim: make_struct_dim semicolon make_struct_fields  */
                                                               {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 851: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 852: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 853: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 854: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 855: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 856: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 857: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 858: /* optional_block: "where" expr_block  */
                                  { (yyvsp[0].pExpression)->at = tokAt(scanner, (yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 871: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 872: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 873: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 874: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
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

  case 875: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
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

  case 876: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 877: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 878: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 879: /* $@84: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 880: /* $@85: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 881: /* make_struct_decl: "struct" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 882: /* $@86: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 883: /* $@87: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 884: /* make_struct_decl: "class" '<' $@86 type_declaration_no_options '>' $@87 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 885: /* $@88: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 886: /* $@89: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 887: /* make_struct_decl: "variant" '<' $@88 variant_type_list '>' $@89 '(' use_initializer make_variant_dim ')'  */
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

  case 888: /* $@90: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 889: /* $@91: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 890: /* make_struct_decl: "default" '<' $@90 type_declaration_no_options '>' $@91 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 891: /* make_tuple: expr  */
                  {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 892: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 893: /* make_tuple: make_tuple ',' expr  */
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

  case 894: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 895: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 896: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 897: /* $@92: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 898: /* $@93: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 899: /* make_tuple_call: "tuple" '<' $@92 tuple_type_list '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 900: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 901: /* make_dim: make_dim semicolon make_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 902: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 903: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
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

  case 904: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
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

  case 905: /* $@94: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 906: /* $@95: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 907: /* make_dim_decl: "array" "struct" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 908: /* $@96: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 909: /* $@97: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 910: /* make_dim_decl: "array" "tuple" '<' $@96 tuple_type_list '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 911: /* $@98: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 912: /* $@99: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 913: /* make_dim_decl: "array" "variant" '<' $@98 variant_type_list '>' $@99 '(' make_variant_dim ')'  */
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

  case 914: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 915: /* $@100: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 916: /* $@101: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 917: /* make_dim_decl: "array" '<' $@100 type_declaration_no_options '>' $@101 '(' optional_expr_list ')'  */
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

  case 918: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 919: /* $@102: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 920: /* $@103: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 921: /* make_dim_decl: "fixed_array" '<' $@102 type_declaration_no_options '>' $@103 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 922: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 923: /* make_table: make_table semicolon make_map_tuple  */
                                                      {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 924: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 925: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 926: /* make_table_decl: open_block optional_expr_map_tuple_list close_block  */
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

  case 927: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 928: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 929: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 930: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 931: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 932: /* array_comprehension_where: semicolon "where" expr  */
                                          { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 933: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 934: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 935: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 936: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 937: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                         {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 938: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 939: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                  {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 940: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']' ']'  */
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

  case 941: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where "end of code block" ']'  */
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

  case 942: /* array_comprehension: open_block "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block  */
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

  case 943: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block close_block  */
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


