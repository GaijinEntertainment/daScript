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
  YYSYMBOL_269_8 = 269,                    /* $@8  */
  YYSYMBOL_annotation_argument_value = 270, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 271, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 272, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 273,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 274, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 275,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 276, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 277, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 278,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 279,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 280, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 281, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 282,   /* optional_function_type  */
  YYSYMBOL_function_name = 283,            /* function_name  */
  YYSYMBOL_global_function_declaration = 284, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 285, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 286, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 287,     /* function_declaration  */
  YYSYMBOL_288_9 = 288,                    /* $@9  */
  YYSYMBOL_open_block = 289,               /* open_block  */
  YYSYMBOL_close_block = 290,              /* close_block  */
  YYSYMBOL_expression_block = 291,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 292,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 293,           /* expression_any  */
  YYSYMBOL_expressions = 294,              /* expressions  */
  YYSYMBOL_expr_keyword = 295,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 296,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 297, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 298, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 299, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 300,       /* expression_keyword  */
  YYSYMBOL_301_10 = 301,                   /* $@10  */
  YYSYMBOL_302_11 = 302,                   /* $@11  */
  YYSYMBOL_303_12 = 303,                   /* $@12  */
  YYSYMBOL_304_13 = 304,                   /* $@13  */
  YYSYMBOL_expr_pipe = 305,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 306,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 307,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 308,     /* new_type_declaration  */
  YYSYMBOL_309_14 = 309,                   /* $@14  */
  YYSYMBOL_310_15 = 310,                   /* $@15  */
  YYSYMBOL_expr_new = 311,                 /* expr_new  */
  YYSYMBOL_expression_break = 312,         /* expression_break  */
  YYSYMBOL_expression_continue = 313,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 314, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 315,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 316, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 317,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 318,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 319,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 320,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 321,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 322,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 323, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 324,           /* expression_let  */
  YYSYMBOL_expr_cast = 325,                /* expr_cast  */
  YYSYMBOL_326_16 = 326,                   /* $@16  */
  YYSYMBOL_327_17 = 327,                   /* $@17  */
  YYSYMBOL_328_18 = 328,                   /* $@18  */
  YYSYMBOL_329_19 = 329,                   /* $@19  */
  YYSYMBOL_330_20 = 330,                   /* $@20  */
  YYSYMBOL_331_21 = 331,                   /* $@21  */
  YYSYMBOL_expr_type_decl = 332,           /* expr_type_decl  */
  YYSYMBOL_333_22 = 333,                   /* $@22  */
  YYSYMBOL_334_23 = 334,                   /* $@23  */
  YYSYMBOL_expr_type_info = 335,           /* expr_type_info  */
  YYSYMBOL_expr_list = 336,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 337,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 338,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 339,            /* capture_entry  */
  YYSYMBOL_capture_list = 340,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 341,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 342,               /* expr_block  */
  YYSYMBOL_expr_full_block = 343,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 344, /* expr_full_block_assumed_piped  */
  YYSYMBOL_345_24 = 345,                   /* $@24  */
  YYSYMBOL_expr_numeric_const = 346,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 347,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 348,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 349,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 350,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 351,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 352,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 353,           /* func_addr_expr  */
  YYSYMBOL_354_25 = 354,                   /* $@25  */
  YYSYMBOL_355_26 = 355,                   /* $@26  */
  YYSYMBOL_356_27 = 356,                   /* $@27  */
  YYSYMBOL_357_28 = 357,                   /* $@28  */
  YYSYMBOL_expr_field = 358,               /* expr_field  */
  YYSYMBOL_359_29 = 359,                   /* $@29  */
  YYSYMBOL_360_30 = 360,                   /* $@30  */
  YYSYMBOL_expr_call = 361,                /* expr_call  */
  YYSYMBOL_expr2 = 362,                    /* expr2  */
  YYSYMBOL_363_31 = 363,                   /* $@31  */
  YYSYMBOL_364_32 = 364,                   /* $@32  */
  YYSYMBOL_365_33 = 365,                   /* $@33  */
  YYSYMBOL_366_34 = 366,                   /* $@34  */
  YYSYMBOL_367_35 = 367,                   /* $@35  */
  YYSYMBOL_368_36 = 368,                   /* $@36  */
  YYSYMBOL_expr = 369,                     /* expr  */
  YYSYMBOL_expr_mtag = 370,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 371, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 372,        /* optional_override  */
  YYSYMBOL_optional_constant = 373,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 374, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 375, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 376, /* structure_variable_declaration  */
  YYSYMBOL_opt_sem = 377,                  /* opt_sem  */
  YYSYMBOL_struct_variable_declaration_list = 378, /* struct_variable_declaration_list  */
  YYSYMBOL_379_37 = 379,                   /* $@37  */
  YYSYMBOL_380_38 = 380,                   /* $@38  */
  YYSYMBOL_381_39 = 381,                   /* $@39  */
  YYSYMBOL_382_40 = 382,                   /* $@40  */
  YYSYMBOL_function_argument_declaration_no_type = 383, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 384, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 385,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 386,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 387,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 388,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 389,             /* variant_type  */
  YYSYMBOL_variant_type_list = 390,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 391,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 392,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 393, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 394, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 395,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 396,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 397,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 398, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 399, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 400, /* global_variable_declaration_list  */
  YYSYMBOL_401_41 = 401,                   /* $@41  */
  YYSYMBOL_optional_shared = 402,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 403, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 404,               /* global_let  */
  YYSYMBOL_405_42 = 405,                   /* $@42  */
  YYSYMBOL_enum_list = 406,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 407, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 408,             /* single_alias  */
  YYSYMBOL_409_43 = 409,                   /* $@43  */
  YYSYMBOL_alias_list = 410,               /* alias_list  */
  YYSYMBOL_alias_declaration = 411,        /* alias_declaration  */
  YYSYMBOL_412_44 = 412,                   /* $@44  */
  YYSYMBOL_optional_public_or_private_enum = 413, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 414,                /* enum_name  */
  YYSYMBOL_enum_declaration = 415,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 416, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 417,          /* optional_sealed  */
  YYSYMBOL_structure_name = 418,           /* structure_name  */
  YYSYMBOL_class_or_struct = 419,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 420, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 421, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 422,    /* structure_declaration  */
  YYSYMBOL_423_45 = 423,                   /* $@45  */
  YYSYMBOL_424_46 = 424,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 425, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 426,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 427, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 428, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 429,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 430,            /* bitfield_bits  */
  YYSYMBOL_commas = 431,                   /* commas  */
  YYSYMBOL_bitfield_alias_bits = 432,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_basic_type_declaration = 433, /* bitfield_basic_type_declaration  */
  YYSYMBOL_bitfield_type_declaration = 434, /* bitfield_type_declaration  */
  YYSYMBOL_435_47 = 435,                   /* $@47  */
  YYSYMBOL_436_48 = 436,                   /* $@48  */
  YYSYMBOL_c_or_s = 437,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 438,          /* table_type_pair  */
  YYSYMBOL_dim_list = 439,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 440, /* type_declaration_no_options  */
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
  YYSYMBOL_466_74 = 466,                   /* $@74  */
  YYSYMBOL_467_75 = 467,                   /* $@75  */
  YYSYMBOL_type_declaration = 468,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 469,  /* tuple_alias_declaration  */
  YYSYMBOL_470_76 = 470,                   /* $@76  */
  YYSYMBOL_471_77 = 471,                   /* $@77  */
  YYSYMBOL_472_78 = 472,                   /* $@78  */
  YYSYMBOL_473_79 = 473,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 474, /* variant_alias_declaration  */
  YYSYMBOL_475_80 = 475,                   /* $@80  */
  YYSYMBOL_476_81 = 476,                   /* $@81  */
  YYSYMBOL_477_82 = 477,                   /* $@82  */
  YYSYMBOL_478_83 = 478,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 479, /* bitfield_alias_declaration  */
  YYSYMBOL_480_84 = 480,                   /* $@84  */
  YYSYMBOL_481_85 = 481,                   /* $@85  */
  YYSYMBOL_make_decl = 482,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 483,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 484,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 485,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 486,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 487,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 488,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 489, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 490,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 491, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 492, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 493, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 494, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 495, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 496,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 497,         /* make_struct_decl  */
  YYSYMBOL_498_86 = 498,                   /* $@86  */
  YYSYMBOL_499_87 = 499,                   /* $@87  */
  YYSYMBOL_500_88 = 500,                   /* $@88  */
  YYSYMBOL_501_89 = 501,                   /* $@89  */
  YYSYMBOL_502_90 = 502,                   /* $@90  */
  YYSYMBOL_503_91 = 503,                   /* $@91  */
  YYSYMBOL_504_92 = 504,                   /* $@92  */
  YYSYMBOL_505_93 = 505,                   /* $@93  */
  YYSYMBOL_make_tuple = 506,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 507,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 508,          /* make_tuple_call  */
  YYSYMBOL_509_94 = 509,                   /* $@94  */
  YYSYMBOL_510_95 = 510,                   /* $@95  */
  YYSYMBOL_make_dim = 511,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 512,            /* make_dim_decl  */
  YYSYMBOL_513_96 = 513,                   /* $@96  */
  YYSYMBOL_514_97 = 514,                   /* $@97  */
  YYSYMBOL_515_98 = 515,                   /* $@98  */
  YYSYMBOL_516_99 = 516,                   /* $@99  */
  YYSYMBOL_517_100 = 517,                  /* $@100  */
  YYSYMBOL_518_101 = 518,                  /* $@101  */
  YYSYMBOL_519_102 = 519,                  /* $@102  */
  YYSYMBOL_520_103 = 520,                  /* $@103  */
  YYSYMBOL_521_104 = 521,                  /* $@104  */
  YYSYMBOL_522_105 = 522,                  /* $@105  */
  YYSYMBOL_make_table = 523,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 524,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 525,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 526, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 527,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 528       /* array_comprehension  */
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
#define YYLAST   14335

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  224
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  305
/* YYNRULES -- Number of rules.  */
#define YYNRULES  946
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1798

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
     959,   959,   968,   976,   988,   998,   998,  1002,  1002,  1008,
    1009,  1010,  1011,  1012,  1013,  1017,  1022,  1030,  1031,  1032,
    1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1049,  1052,
    1058,  1061,  1064,  1070,  1071,  1072,  1073,  1077,  1090,  1108,
    1111,  1119,  1130,  1141,  1152,  1155,  1162,  1166,  1173,  1174,
    1178,  1179,  1180,  1184,  1187,  1194,  1198,  1199,  1200,  1201,
    1202,  1203,  1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,
    1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,
    1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,  1231,
    1232,  1233,  1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,
    1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,
    1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,
    1262,  1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,
    1272,  1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,
    1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,
    1292,  1297,  1315,  1316,  1317,  1321,  1327,  1327,  1344,  1345,
    1348,  1349,  1352,  1359,  1383,  1401,  1410,  1416,  1417,  1418,
    1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,
    1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,  1437,  1441,
    1446,  1452,  1458,  1470,  1471,  1475,  1476,  1480,  1481,  1485,
    1489,  1496,  1496,  1496,  1502,  1502,  1502,  1511,  1545,  1553,
    1560,  1567,  1573,  1574,  1585,  1589,  1592,  1600,  1600,  1600,
    1603,  1609,  1612,  1616,  1620,  1627,  1634,  1640,  1644,  1648,
    1651,  1654,  1662,  1665,  1668,  1676,  1679,  1687,  1690,  1693,
    1701,  1713,  1714,  1715,  1719,  1720,  1724,  1725,  1729,  1734,
    1742,  1753,  1759,  1774,  1786,  1789,  1795,  1795,  1795,  1798,
    1798,  1798,  1803,  1803,  1803,  1811,  1811,  1811,  1817,  1831,
    1847,  1865,  1875,  1886,  1901,  1904,  1910,  1911,  1918,  1929,
    1930,  1931,  1935,  1936,  1937,  1938,  1939,  1943,  1948,  1956,
    1957,  1970,  1974,  1984,  1991,  1998,  1998,  2007,  2008,  2009,
    2010,  2011,  2012,  2013,  2017,  2018,  2019,  2020,  2021,  2022,
    2023,  2024,  2025,  2026,  2027,  2028,  2029,  2030,  2031,  2032,
    2033,  2034,  2035,  2039,  2049,  2058,  2067,  2072,  2073,  2074,
    2075,  2076,  2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,
    2085,  2086,  2087,  2088,  2093,  2100,  2112,  2117,  2127,  2131,
    2138,  2141,  2141,  2141,  2146,  2146,  2146,  2159,  2163,  2167,
    2172,  2179,  2188,  2193,  2200,  2200,  2200,  2207,  2211,  2221,
    2230,  2239,  2243,  2246,  2252,  2253,  2254,  2255,  2256,  2257,
    2258,  2259,  2260,  2261,  2262,  2263,  2264,  2265,  2266,  2267,
    2268,  2269,  2270,  2271,  2272,  2273,  2274,  2275,  2276,  2277,
    2278,  2279,  2280,  2287,  2288,  2289,  2290,  2291,  2292,  2293,
    2294,  2295,  2296,  2297,  2298,  2305,  2306,  2307,  2308,  2309,
    2310,  2311,  2312,  2326,  2327,  2328,  2329,  2330,  2333,  2336,
    2337,  2337,  2337,  2340,  2345,  2349,  2353,  2353,  2353,  2358,
    2361,  2365,  2365,  2365,  2370,  2373,  2374,  2375,  2376,  2377,
    2378,  2379,  2380,  2381,  2383,  2387,  2395,  2400,  2404,  2413,
    2414,  2415,  2416,  2417,  2418,  2419,  2423,  2427,  2431,  2435,
    2439,  2443,  2447,  2451,  2455,  2462,  2463,  2472,  2476,  2477,
    2478,  2482,  2483,  2487,  2488,  2489,  2493,  2494,  2498,  2509,
    2510,  2511,  2512,  2517,  2520,  2520,  2524,  2524,  2543,  2542,
    2558,  2557,  2571,  2580,  2592,  2601,  2611,  2612,  2613,  2614,
    2615,  2619,  2622,  2631,  2632,  2636,  2639,  2642,  2658,  2667,
    2668,  2672,  2675,  2678,  2692,  2693,  2697,  2703,  2709,  2718,
    2721,  2728,  2731,  2737,  2738,  2739,  2743,  2744,  2748,  2755,
    2760,  2769,  2775,  2786,  2789,  2794,  2799,  2807,  2817,  2828,
    2831,  2831,  2851,  2852,  2856,  2857,  2858,  2862,  2869,  2869,
    2888,  2891,  2907,  2927,  2928,  2929,  2934,  2934,  2964,  2967,
    2974,  2984,  2984,  2988,  2989,  2990,  2994,  3004,  3024,  3047,
    3048,  3052,  3053,  3057,  3063,  3064,  3065,  3066,  3070,  3071,
    3072,  3076,  3079,  3090,  3095,  3090,  3115,  3122,  3127,  3136,
    3142,  3153,  3154,  3155,  3156,  3157,  3158,  3159,  3160,  3161,
    3162,  3163,  3164,  3165,  3166,  3167,  3168,  3169,  3170,  3171,
    3172,  3173,  3174,  3175,  3176,  3177,  3178,  3179,  3183,  3184,
    3185,  3186,  3187,  3188,  3189,  3190,  3194,  3205,  3209,  3216,
    3228,  3235,  3245,  3246,  3251,  3256,  3270,  3280,  3290,  3300,
    3310,  3323,  3324,  3325,  3326,  3327,  3331,  3331,  3331,  3345,
    3346,  3350,  3354,  3361,  3365,  3372,  3373,  3374,  3375,  3376,
    3391,  3397,  3397,  3397,  3401,  3406,  3413,  3413,  3420,  3424,
    3428,  3433,  3438,  3443,  3448,  3452,  3456,  3461,  3465,  3469,
    3474,  3474,  3474,  3480,  3487,  3487,  3487,  3492,  3492,  3492,
    3498,  3498,  3498,  3503,  3508,  3508,  3508,  3513,  3513,  3513,
    3522,  3527,  3527,  3527,  3532,  3532,  3532,  3541,  3546,  3546,
    3546,  3551,  3551,  3551,  3560,  3560,  3560,  3566,  3566,  3566,
    3575,  3578,  3589,  3605,  3605,  3610,  3619,  3605,  3648,  3648,
    3653,  3663,  3648,  3692,  3692,  3692,  3745,  3746,  3747,  3748,
    3749,  3753,  3760,  3767,  3773,  3779,  3786,  3793,  3799,  3808,
    3811,  3817,  3825,  3830,  3837,  3842,  3849,  3854,  3860,  3861,
    3865,  3866,  3871,  3872,  3876,  3877,  3881,  3882,  3886,  3887,
    3888,  3892,  3893,  3894,  3898,  3899,  3903,  3936,  3975,  3994,
    4014,  4034,  4055,  4055,  4055,  4063,  4063,  4063,  4070,  4070,
    4070,  4081,  4081,  4081,  4092,  4096,  4102,  4118,  4124,  4130,
    4136,  4136,  4136,  4150,  4155,  4162,  4182,  4210,  4234,  4234,
    4234,  4244,  4244,  4244,  4258,  4258,  4258,  4272,  4281,  4281,
    4281,  4301,  4308,  4308,  4308,  4318,  4323,  4330,  4333,  4339,
    4359,  4378,  4386,  4406,  4431,  4432,  4436,  4437,  4442,  4452,
    4455,  4458,  4461,  4469,  4478,  4490,  4500
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
  "expression_with_alias", "$@7", "$@8", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@9", "open_block", "close_block",
  "expression_block", "expr_call_pipe", "expression_any", "expressions",
  "expr_keyword", "optional_expr_list", "optional_expr_list_in_braces",
  "optional_expr_map_tuple_list", "type_declaration_no_options_list",
  "expression_keyword", "$@10", "$@11", "$@12", "$@13", "expr_pipe",
  "name_in_namespace", "expression_delete", "new_type_declaration", "$@14",
  "$@15", "expr_new", "expression_break", "expression_continue",
  "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@16", "$@17", "$@18", "$@19", "$@20",
  "$@21", "expr_type_decl", "$@22", "$@23", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@24", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe_right", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@25", "$@26", "$@27", "$@28", "expr_field", "$@29",
  "$@30", "expr_call", "expr2", "$@31", "$@32", "$@33", "$@34", "$@35",
  "$@36", "expr", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "opt_sem", "struct_variable_declaration_list", "$@37", "$@38", "$@39",
  "$@40", "function_argument_declaration_no_type",
  "function_argument_declaration_type", "function_argument_list",
  "tuple_type", "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration_no_type", "variable_declaration_type",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@41", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@42", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@43", "alias_list",
  "alias_declaration", "$@44", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "optional_structure_parent",
  "optional_sealed", "structure_name", "class_or_struct",
  "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@45", "$@46", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "commas",
  "bitfield_alias_bits", "bitfield_basic_type_declaration",
  "bitfield_type_declaration", "$@47", "$@48", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@49", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60",
  "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69",
  "$@70", "$@71", "$@72", "$@73", "$@74", "$@75", "type_declaration",
  "tuple_alias_declaration", "$@76", "$@77", "$@78", "$@79",
  "variant_alias_declaration", "$@80", "$@81", "$@82", "$@83",
  "bitfield_alias_declaration", "$@84", "$@85", "make_decl",
  "make_struct_fields", "make_variant_dim", "make_struct_single",
  "make_struct_dim", "make_struct_dim_list", "make_struct_dim_decl",
  "optional_make_struct_dim_decl", "optional_block",
  "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@86", "$@87", "$@88", "$@89", "$@90", "$@91", "$@92", "$@93",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@94", "$@95",
  "make_dim", "make_dim_decl", "$@96", "$@97", "$@98", "$@99", "$@100",
  "$@101", "$@102", "$@103", "$@104", "$@105", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1587)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-812)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1587,    66, -1587, -1587,    89,  -103,   571,   178, -1587,   -81,
     190,   190,   190, -1587, -1587,    10,    60, -1587, -1587, -1587,
     633, -1587, -1587, -1587,   330, -1587,    58, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587,   -51, -1587,    31,
     115,    63, -1587, -1587, -1587, -1587,   571, -1587,    25, -1587,
   -1587, -1587,   190,   190, -1587, -1587,    58, -1587, -1587, -1587,
   -1587, -1587,   105,   194, -1587, -1587, -1587, -1587,    60,    60,
      60,   128, -1587,   892,    70, -1587, -1587, -1587, -1587,   770,
     864,   728,   876, -1587,   884,    45,    89,   226,  -103,   210,
     237, -1587,   869,   869, -1587,   260,   671,   238,   633,   891,
     300,   399,   406, -1587,   416,   428, -1587, -1587,   -59,    89,
      60,    60,    60,    60, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587,   464, -1587, -1587, -1587, -1587, -1587, -1587, -1587,   178,
   -1587, -1587, -1587, -1587, -1587,   868,   136, -1587, -1587, -1587,
   -1587,   593, -1587, -1587, -1587, -1587,   463, -1587, -1587, -1587,
   -1587,   671, -1587, -1587, -1587,   449, -1587, -1587, -1587, -1587,
   -1587,   525, -1587,   175, -1587,   813,   613,   892, 14119, -1587,
     -48,   665, -1587,   -86, -1587, -1587, -1587,   885, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587,   252, -1587,   605, -1587, -1587,
     743, -1587,   636,   178,   178, -1587, -1587, 13120, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587,   811,   838, -1587,   678,   178,   721, -1587,
   -1587,   733, -1587,   704,    89,    89,   337,   291, -1587, -1587,
   -1587,   136, -1587, 10639, -1587,   749,   178, -1587, -1587,   764,
     773, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587,   799,   767, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587,   894, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,   814,   779,
   -1587, -1587,   -36,   812, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587,   178,   697,   821,   178, -1587,   -86,
     283, -1587,    89, -1587,   815,   984,   711, -1587, -1587,   829,
     831,   839,   842,   852,   878, -1587, -1587, -1587,   860, -1587,
   -1587, -1587, -1587, -1587,   572, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587,   880, -1587, -1587, -1587,
     881,   882, -1587, -1587, -1587, -1587,   883,   886,   865,    10,
   -1587, -1587, -1587, -1587, -1587,  2633,   888, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,   917,   911,
   -1587,   874, -1587,    97,   910,   276,   887, 10639, -1587,  2931,
   -1587,   455, -1587,    10, -1587, -1587, -1587,   291,   890, -1587,
   10018,   923,   925, 10639, -1587,     7, -1587, -1587, -1587, 10018,
   -1587, -1587,   926,   900,   606,   607,   615, -1587, -1587, 10018,
     -40, -1587, -1587, -1587,    93, -1587, -1587, -1587,    15,  6062,
   -1587,   889, 10385,   698, 10488,   482, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, 10018,
   -1587, -1587,   315,   148,   148,   148, -1587,   888, -1587,   902,
     903, 10018, -1587, -1587,   895, -1587, -1587,   833,   -78,   904,
      54,  3766, -1587, -1587,   178,   374,  6270,   896, -1587, 10018,
     928,   905,   909,   897, -1587,   633,   913,   944,  6478,   328,
     474,   919, -1587,   500,   929,   930,  3974, 10018, 10018,   320,
     320,   320,   898,   906,   907,   912,   914,   916, -1587,  2259,
    2704,  6688, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
    6896,   932, -1587,  1123, -1587, 10018, 10018, 10018, 10018, 10018,
    5646, 10018, -1587,   918, -1587, -1587,   633,   633, 10018,  1128,
   -1587, -1587, -1587, -1587, -1587, -1587,  7106,  1104, -1587, -1587,
   -1587,   748, -1587,   141,   633, -1587,   633,   633,   633, -1587,
     633, -1587, -1587,  1077, -1587, -1587, -1587, -1587,   927, -1587,
   -1587,    61, -1587, -1587, -1587, -1587, -1587, -1587, 13374,  1082,
   -1587,   931, -1587, -1587, -1587, -1587, -1587, -1587,   697, 10018,
   -1587, -1587,    43,   671, -1587,   937,   945,   947, -1587, 10733,
   -1587,  1106,   131, -1587, -1587, -1587,  4390, 10639, 10639, 10639,
   10852, 10639, 10639,   938, -1587, 10639,   678, 10639,   678, 10639,
     678, 10742,   987, 10887, -1587, 10018, -1587, -1587, -1587, -1587,
   -1587,   939, -1587, -1587, 12669, 10018, -1587,  2633,   242, -1587,
     123, -1587, -1587,   350, -1587,   888,   704,   969,   350, -1587,
     704, 10998,   949,  1127, -1587, -1587,   157, -1587, -1587, -1587,
   -1587, -1587,   178,   315,   953, -1587,   956, -1587, -1587,    10,
     604, -1587,   966,   998,   999, -1587, 10018,  4390, -1587,  1007,
    1069,  2024,  1619,  1192, 10639, 10018, 10018, -1587, 10018,  1034,
     178,  1014, -1587, -1587, 10018, -1587, -1587,  1017,  1044, -1587,
   10018, -1587, -1587, 10018, -1587, -1587, 10018, -1587, 10639,  4390,
   -1587,  2773,   321,   321,   992, -1587,   927, -1587, -1587, -1587,
   10018, 10018, 10018, 10018, 10018, 10018,   315,  3139,   315,  3348,
     315,  1073, -1587,   645, -1587,   178, -1587,   835,   997,   321,
     321,   215,   321,   321,   -74,  1204,  1000,  1028, 13374,  1028,
     382, -1587, -1587,   178, -1587,   315,   704, -1587,  1032,   178,
   -1587, -1587, -1587,  4598, -1587, -1587, -1587, -1587, -1587, -1587,
     268,    53,   320, -1587, 13896, 13995, 10018, 10018, -1587, -1587,
   10018, 10018, 10018, 10018,  1054, 10018,   553, 10018, 10018, 10018,
   10018, 10018, 10018, 10018, 10018, 10018,  7314, 10018, 10018, 10018,
   10018, 10018, 10018, 10018, 10018, 10018, 10018, 14057, 10018,  4806,
    4806,  4806,  4806,  4806,  4806,  4806,  4806,  4806,  4806,  4806,
   10018,  4806,  4806,  4806,  4806,  4806,  4806, -1587,  7522, -1587,
     671, -1587,    60,  1220, -1587,   -86, -1587, 10639, -1587,  1059,
   -1587,  4390, -1587,  2773,   737,   774,  1040,   608, -1587,   781,
     782, -1587,  1074,   789,   812,   791,   812,   801,   812, -1587,
     422, -1587,   461, -1587, 10639,  1041, -1587, -1587, 12780, -1587,
   -1587, 10018,  1049, 10639, -1587, -1587, 10639, -1587, -1587, -1587,
   10018,  1078, -1587,  1079, -1587, 10639, -1587, 10018, 10639, 10639,
   -1587,    36,   315, 10639,  5854,  7730,  1080, 10018, 10639, -1587,
   -1587, -1587, 10639,  1028, -1587,  1007, 10018, 10018, 10018, 10018,
   10018, 10018, 10018, 10018, 10018, 10018, 10018, 10018, 10018, 10018,
   10018, 10018, 10018, 10018,   178,  1061,  1035, -1587, 11038,  1063,
   -1587, -1587, 10639, 10639, 11149, 10639, -1587, -1587, 11184, 10639,
    1028, 10639, 10742,  1028,   987,   176, -1587,  2773, -1587,    53,
   11295, 11335, 11446, 11481, 11592, 11632,    47,   320,  1045,    28,
    3557,  5016,  7938,  1117,  1076,   155,   -60,  1081,   385,    51,
    8146,   155,   599,    55, 10018,  1083, -1587, 10018, -1587, 10639,
   -1587, 10639, -1587, 10018,   879,   315,   315,    56,   166, -1587,
   10018, -1587,  1051,  1053,  1058,   616, -1587, -1587,   126, 10018,
      57, -1587, 10018, -1587, -1587,   927,   209,  5226, -1587,   266,
    1087,  1065,  1100,  1100, -1587, -1587,  1067,   319,   678, -1587,
    1088,  1068, -1587, -1587,  1089,  1071, -1587, -1587,  1033,  1033,
    2176,  2176,  1921,  1921,  1072,    69,  1086, -1587, 12820,   198,
     198,   931,  1033,  1033,  1787, 13560, 13671, 13411, 14026, 13263,
    2288, 13708, 13819,  2176,  2176,   661,   661,    69,    69,    69,
     682, 10018,  1090,  1091,   686, 10018,  1280,  1092, 12931,   320,
     320,   320, -1587, -1587,   748, -1587,   748, -1587,   748, -1587,
     748, -1587,   748, -1587,   748, -1587,   748, -1587,   748, -1587,
     748, -1587,   748, -1587,   748, -1587, -1587,   748, -1587,   748,
   -1587,   748, -1587,   748, -1587,   748, -1587,   748, -1587,   278,
   -1587,  1109,   331,   893,  1225,   633,  1120, -1587, -1587, 14113,
   -1587, -1587, -1587, -1587, 10639, -1587, -1587, -1587,   521, -1587,
    1094, -1587,  1095, -1587,  1097, -1587, 10742, -1587,   987,   476,
     888, -1587,  1093, -1587,   193, -1587,   888,   888, 11743, -1587,
    1257,   -55, -1587,  1151,  1329, 10018,    64,   802,   732,   292,
    1096,  1098,  1129,  1103,   523,  1105,   803, 10639, 10742,   987,
    1348,  1107, 13374, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587,  1101, 10639, -1587, -1587, 10639, 10018,  1359,  1648, -1587,
    2057, -1587,  2173,  1108,  2387,   479,  1110,   488,    53,   678,
   -1587, -1587, -1587, -1587, -1587,  1112, 10018, -1587, 10018, 10018,
   10018,  5436, 12669,     5, 10018,   742,   732,   -60, -1587, -1587,
    1099, -1587, 10018, -1587,  1114, 10018, -1587, 10018,   732,   771,
    1116, -1587, -1587, 10018, -1587, -1587, -1587,   491,   536,  1111,
      68,    81, 10018,   315,    82, -1587, 10018, 10018, 10639,   678,
     178, -1587, -1587,   901, 10018, -1587,  1289, 10018, -1587,  2931,
      53,   222, -1587,  1118,   368, 10226, -1587,   742, -1587, -1587,
   -1587,   391,   365,   319,  1133,  1139,  1122,  1141,  1153, -1587,
     556,   812, -1587, 10018, -1587, 10018,  8354, 10018, -1587,  1124,
    1125, -1587, -1587, 10018,  1126, -1587, 12966, 10018,  8562,  1130,
   -1587, 13077, -1587,  8770, -1587, -1587, -1587, -1587, -1587,  1142,
     633, -1587, -1587,   172, -1587,    71,   671,    53, -1587, -1587,
   -1587, -1587,   888, -1587, -1587, -1587,  1166, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587,  1132, 10639, -1587,
   -1587,  1049, -1587,  1169, 10018, -1587, -1587,   255, 10018, -1587,
    1134, -1587, -1587, -1587,   542, -1587,  1136,  1174, -1587, -1587,
    2436,   555,   565, -1587, -1587, 10018,  2448,   403,   633, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,   311,   812,
    8978,   512, 11778, -1587, -1587,   155,   -60, -1587,  1137,   261,
    1076, -1587, -1587, -1587, -1587,  1081,   582,   155,  1143, -1587,
   -1587, -1587, -1587,   664, -1587, -1587, -1587,  1167, 10018, 10018,
     724,    94, 10018, 11889, 11929,  2484,   812, -1587, -1587,   178,
   -1587,   633,   738, -1587,   678, -1587,  1145,  5226,  1184,  1148,
     688,   390, -1587, -1587,  1186, -1587, -1587,   319,  1149,   394,
   10639, 12040, 10639, 12075, -1587,   404, 12186, -1587, 10018, 13522,
   10018, -1587, 12226,  5226, -1587,   458, 10018, -1587, -1587, -1587,
     465, 10639,   671,  1336,    71, -1587, -1587,   893, -1587,  1152,
   -1587, -1587, -1587, -1587, -1587, -1587,   888, -1587, -1587,  1154,
    1157, -1587,   467, -1587, -1587, 10018,  1197, 10018,  1171, -1587,
   -1587, -1587, -1587,  1158,  1159,  1161, -1587, -1587, 10018, 10018,
   10018,  1163,  1308,  1164,  1165,  9186, -1587,   394, -1587,   470,
   10018,   274,   -60, -1587, 10018, 10018, 10018, 10018,   771, -1587,
   10018, 10018,  1132, -1587, -1587,   511,   519, 10018, 10018,   747,
   -1587, -1587, -1587,  1187,   158,  4182, -1587, 10018,   812, -1587,
     577, -1587,   715, 10639,     7, -1587,  1175, -1587, -1587,  9394,
   -1587, -1587,  2532, -1587,   808, -1587, -1587, -1587, 10639, 12337,
   12372, -1587,   588, -1587, 12483, -1587,   403, -1587, -1587, -1587,
    1336,   315,  1308,  1308,   178,  1176,  1190,  1177,  1179,  1180,
    1181,  1182, 10018, -1587, 10018, -1587, -1587, -1587, 10018, -1587,
   -1587,  1308,  1308, -1587, 12523, -1587, -1587, 13225, 10018, 10018,
   -1587, 12634, -1587, -1587, 13225, -1587,   633, -1587, -1587,  1208,
    1209,  1217, 13225,   532, 10018,   221, -1587,   901, -1587,  9602,
    9810, -1587, -1587, -1587, -1587, -1587, -1587,   633,   178,  1183,
   10639,     7,   515, 10018, -1587, 10018, -1587, -1587, -1587,   810,
   -1587, -1587,  1185, -1587,   671, 14119, -1587, -1587, -1587, -1587,
     302,   302, -1587, -1587, 10018, -1587, 10018,  1308,  1308,   732,
    1191,  1194,  1028,   302,   732, -1587,  1353,  1189, -1587, -1587,
     290,  1226,   704, -1587, 10018, 10018,  1199,  1227, 13225, -1587,
     221, -1587, 10018, 10018, -1587,   704, -1587, -1587,   515, 10018,
   10018,   633, -1587, -1587, -1587, -1587, -1587, -1587,   633, 14119,
     732,  1076,  1222, -1587,  1198,  1201,  1202,  1203,   302,   302,
    1076,  1205, -1587, -1587,  1206,  1207,  1210, 10018,  1214, 10018,
   10018,  1215,   704,   633, 13225, -1587, 10018,  1216, -1587, -1587,
   -1587, 10018,   633,   633, -1587, -1587,   671,   178,   543,  1219,
   -1587, -1587, -1587, -1587, -1587,  1221,  1223, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587,  1240,  1224, 13225,
   -1587,   633, -1587, -1587, -1587,   671, -1587,   732, -1587, -1587,
   -1587, -1587,  1229, -1587, -1587,   562, -1587, -1587
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   138,     1,   334,     0,     0,     0,   661,   335,     0,
     653,   653,   653,    17,    18,     0,     0,    16,    15,     3,
       0,    10,     9,     8,     0,     7,   642,     6,    11,     5,
       4,    13,    12,    14,   108,   109,   107,   116,   118,    47,
      63,    60,    61,    49,    50,    51,     0,    52,    58,    48,
     249,   248,   653,   653,    24,    23,   642,   655,   654,   833,
     823,   828,     0,   302,    45,   124,   125,   126,     0,     0,
       0,   127,   129,   136,     0,   123,    19,   675,   674,   242,
     663,     0,   678,   643,   644,     0,     0,     0,     0,    53,
       0,    59,     0,     0,    56,     0,   582,   653,     0,    20,
       0,     0,     0,   304,     0,     0,   135,   130,     0,     0,
       0,     0,     0,     0,   139,   244,   243,   246,   241,   665,
     664,     0,   677,   676,   680,   679,   683,   646,   645,   648,
     114,   115,   112,   113,   111,     0,     0,   110,   119,    64,
      62,    58,    55,    54,   656,   579,   580,   658,   251,   250,
     660,   582,   662,    21,    22,    25,   834,   824,   829,   303,
      43,    46,   134,     0,   131,   132,   133,   137,     0,   666,
       0,   671,   639,   565,    28,    29,    33,     0,   103,   104,
     101,   102,   100,    99,   105,     0,    57,     0,   581,   659,
       0,    27,   741,     0,     0,    44,   128,     0,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,     0,   145,   140,     0,     0,   650,
     672,     0,   684,   640,     0,     0,   567,     0,    30,    31,
      32,     0,   117,     0,    26,     0,     0,   825,   830,   210,
     211,   208,   148,   149,   151,   150,   152,   153,   154,   155,
     181,   182,   179,   180,   172,   183,   184,   173,   170,   171,
     209,   192,     0,   207,   185,   186,   187,   188,   159,   160,
     161,   156,   157,   158,   169,     0,   175,   176,   174,   167,
     168,   163,   162,   164,   165,   166,   147,   146,   191,     0,
     177,   178,   565,   143,   279,   247,   718,   721,   724,   725,
     719,   722,   720,   723,     0,     0,   669,   681,   647,   565,
       0,   120,     0,   122,     0,   628,   626,   649,   106,     0,
       0,     0,     0,     0,     0,   691,   711,   692,   727,   693,
     697,   698,   699,   700,   717,   704,   705,   706,   707,   708,
     709,   710,   712,   713,   714,   715,   793,   696,   703,   716,
     800,   807,   694,   701,   695,   702,     0,     0,     0,     0,
     726,   755,   758,   756,   757,   820,   657,   744,   745,   742,
     743,   734,   605,   611,   212,   213,   206,   190,   214,   193,
     189,     0,   141,   333,   596,   597,     0,     0,   245,     0,
     650,   582,   667,     0,   673,   583,   685,     0,     0,   121,
       0,     0,     0,     0,   627,     0,   761,   784,   787,     0,
     790,   780,     0,     0,   794,   801,   808,   814,   817,     0,
     285,   770,   775,   769,     0,   783,   779,   772,     0,     0,
     774,   759,     0,     0,   826,   831,   215,   195,   196,   198,
     197,   199,   200,   201,   202,   194,   203,   204,   205,     0,
     331,   332,     0,   565,   565,   565,   142,   144,   281,     0,
       0,     0,    74,    75,    88,   510,   511,     0,     0,     0,
       0,   319,   505,   317,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
       0,     0,   717,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   554,     0,
       0,     0,   257,   387,   389,   388,   390,   391,   392,   393,
       0,     0,    39,   248,   258,     0,     0,     0,     0,     0,
     283,     0,   369,   370,   508,   507,     0,     0,     0,     0,
     274,   269,   266,   265,   267,   268,   287,   252,   301,   280,
     260,   547,   259,   464,     0,   538,    82,    83,    80,   272,
      81,   273,   275,   337,   264,   537,   536,   535,   138,   541,
     506,     0,   261,   540,   539,   513,   465,   514,   548,   394,
     466,     0,   509,   836,   840,   837,   838,   839,     0,     0,
     651,   670,   586,   582,   566,     0,     0,     0,   547,     0,
     630,   631,     0,   624,   625,   623,     0,     0,     0,     0,
       0,     0,     0,     0,   746,     0,   140,     0,   140,     0,
     140,     0,     0,     0,   766,   283,   765,   777,   778,   771,
     773,     0,   776,   760,     0,     0,   822,   821,   737,   835,
     302,   749,   750,     0,   606,   601,     0,     0,     0,   612,
       0,     0,     0,   686,   593,   594,   616,   598,   599,   600,
     882,   885,     0,     0,     0,   307,   311,   310,   316,     0,
       0,   355,     0,     0,     0,   918,     0,     0,   323,   320,
       0,   548,   394,     0,     0,   287,     0,   305,     0,     0,
       0,     0,   346,   349,     0,   278,   352,     0,     0,    67,
       0,    92,   922,     0,   891,   900,     0,   888,     0,     0,
     328,   325,   493,   494,   370,   382,   138,   300,   298,   299,
       0,     0,     0,     0,     0,     0,     0,   860,     0,     0,
       0,   898,   925,     0,   291,     0,   294,     0,     0,   470,
     469,   503,   468,   467,     0,     0,     0,   936,   364,   936,
     371,   276,   277,     0,    86,     0,     0,   927,   936,     0,
     385,   255,   545,     0,   263,   270,   271,   322,   327,   336,
       0,   379,     0,   262,     0,     0,     0,     0,   495,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   454,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   254,     0,   668,
     582,   584,     0,     0,   682,   565,   641,     0,   629,     0,
     633,     0,   638,   637,     0,     0,     0,   751,   764,     0,
       0,   728,     0,     0,   143,     0,   143,     0,   143,   603,
       0,   609,     0,   729,     0,   936,   768,   753,     0,   735,
     732,     0,   736,     0,   607,   827,     0,   613,   832,   595,
       0,     0,   615,     0,   614,     0,   617,     0,     0,     0,
      93,     0,     0,     0,   874,     0,     0,     0,     0,   908,
     911,   914,     0,   936,   324,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   306,     0,     0,
      94,    95,     0,     0,     0,     0,    65,    66,     0,     0,
     936,     0,     0,   936,     0,     0,   329,   326,   371,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,     0,     0,   894,   852,   860,     0,   903,     0,     0,
       0,   860,     0,     0,     0,     0,   863,     0,   930,     0,
     282,     0,    42,     0,    40,     0,     0,     0,     0,   905,
     937,   284,     0,     0,     0,   441,   438,   440,    70,     0,
       0,   929,   937,   288,   279,   138,     0,   283,   457,     0,
     936,     0,     0,     0,   345,   344,     0,     0,   140,   297,
       0,     0,   524,   523,     0,     0,   525,   529,   471,   472,
     484,   485,   482,   483,     0,   519,     0,   501,     0,   542,
     543,   544,   473,   474,   489,   490,   491,   492,     0,     0,
     487,   488,   486,   480,   481,   476,   475,   477,   478,   479,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,     0,   416,   425,   404,   426,   405,   428,   407,   427,
     406,   429,   408,   419,   398,   420,   399,   421,   400,   430,
     409,   431,   410,   418,   396,   397,   432,   411,   433,   412,
     422,   401,   423,   402,   424,   403,   417,   395,   462,     0,
     652,     0,     0,   573,   576,     0,     0,   632,   635,   394,
     636,   762,   785,   788,     0,   791,   781,   730,     0,   795,
       0,   802,     0,   809,     0,   815,     0,   818,     0,     0,
     289,   937,     0,   754,   740,   733,   602,   608,     0,   688,
     689,   619,   618,     0,     0,     0,     0,     0,   875,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   365,   404,   405,   407,   406,   408,   398,   399,
     400,   409,   410,   396,   411,   412,   401,   402,   403,   395,
     330,     0,     0,   931,   515,     0,     0,     0,     0,   516,
       0,   546,     0,     0,     0,     0,     0,     0,   379,   140,
     549,   550,   551,   552,   553,     0,     0,   861,     0,     0,
       0,     0,   364,   860,     0,     0,     0,     0,   869,   870,
       0,   877,     0,   867,     0,     0,   906,     0,     0,     0,
       0,   865,   907,     0,   897,   862,   926,     0,     0,    36,
       0,     0,     0,     0,     0,   512,     0,     0,     0,   140,
       0,    68,    69,    70,     0,    85,    76,     0,   928,     0,
     379,     0,   458,     0,     0,     0,   461,   937,   851,   459,
     338,     0,     0,     0,     0,     0,     0,     0,     0,   377,
       0,   143,   520,     0,   526,     0,     0,     0,   499,     0,
       0,   530,   534,     0,     0,   502,     0,     0,     0,     0,
     448,     0,   455,     0,   497,   415,   413,   414,   463,     0,
     139,   574,   575,   576,   577,   568,   582,   379,   634,   763,
     786,   789,   752,   792,   782,   747,     0,   796,   798,   803,
     805,   810,   812,   816,   604,   819,   610,   285,     0,   286,
     738,   739,   687,     0,     0,   883,   886,     0,     0,   308,
       0,   313,   314,   312,     0,   358,     0,     0,   361,   356,
       0,     0,     0,   919,   917,   287,     0,     0,     0,   347,
     350,   353,   923,   921,   892,   901,   899,   889,     0,   143,
       0,     0,     0,   842,   841,   860,     0,   895,     0,     0,
     853,   876,   868,   896,   866,   904,     0,   860,     0,   872,
     873,   880,   864,     0,   292,   295,    37,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,    71,    84,     0,
      77,     0,     0,   253,   140,   460,     0,   283,     0,     0,
     626,     0,   374,   375,     0,   373,   372,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,   531,     0,   504,
       0,   500,     0,   283,   449,     0,     0,   498,   456,   452,
       0,     0,   582,   571,   568,   569,   570,   573,   587,     0,
     748,   731,   799,   806,   813,   767,   290,   690,   620,     0,
       0,    89,     0,   309,   315,     0,     0,     0,     0,   357,
     909,   912,   915,     0,     0,     0,    98,    96,     0,     0,
       0,     0,   874,     0,     0,     0,   256,     0,   555,     0,
       0,     0,     0,   878,     0,     0,     0,     0,     0,   871,
       0,     0,   285,    34,    41,     0,     0,     0,     0,     0,
     439,   564,   442,     0,    70,     0,    87,     0,   143,   434,
       0,   339,   626,     0,     0,   381,     0,   378,   380,     0,
     366,   384,     0,   563,     0,   561,   437,   558,     0,     0,
       0,   557,     0,   450,     0,   453,     0,   592,   572,   588,
     571,     0,   874,   874,     0,     0,     0,     0,     0,     0,
       0,     0,   283,   932,   287,   348,   351,   354,     0,   875,
     893,   874,   874,   517,     0,   383,   556,   934,     0,     0,
     879,     0,   844,   843,   934,   881,   934,   293,   296,    38,
       0,     0,   934,     0,     0,     0,   445,    70,    72,   319,
       0,    78,    82,    83,    80,    81,    79,   934,     0,     0,
       0,     0,     0,     0,   376,     0,   367,   521,   527,     0,
     562,   560,     0,   559,   582,     0,   590,   622,   621,   578,
     859,   859,    91,   359,     0,   362,     0,   874,   874,   849,
       0,     0,   936,   859,   849,   518,     0,     0,   846,   845,
       0,     0,     0,    35,     0,     0,     0,     0,   934,   443,
       0,    73,     0,     0,   325,     0,   386,   435,     0,     0,
       0,     0,   368,   522,   528,   532,   451,   585,     0,     0,
       0,   856,   936,   858,     0,     0,     0,     0,   859,   859,
     850,     0,   920,   933,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   934,   934,   938,     0,     0,   446,   326,
     945,     0,     0,     0,   343,   533,   582,     0,     0,   937,
     857,   884,   887,   360,   363,     0,     0,   916,   924,   902,
     890,   935,   943,   848,   847,   944,   946,     0,     0,   934,
     942,     0,   342,   341,   589,   582,   854,     0,   910,   913,
     941,   939,     0,   340,   591,     0,   940,   855
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1587, -1587,    -1, -1587, -1587, -1587, -1587, -1587,   681,  1360,
   -1587, -1587, -1587, -1587, -1587, -1587,  1443, -1587, -1587, -1587,
     977,  1402, -1587,  1309, -1587, -1587,  1361, -1587, -1587, -1587,
   -1237, -1587, -1587, -1587,   -99, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587,  1211, -1587, -1587,   -41,
     -54, -1587, -1587, -1587,   522,   617,  -550,  -603,  -835, -1587,
   -1587, -1587, -1586, -1587, -1587,    -4,  -226,  -219,  -388, -1587,
     447, -1587,  -629, -1308,  -686,   -27,   777, -1587, -1587, -1587,
   -1587,  -442,     0, -1587, -1587, -1587, -1587, -1587,   -90,   -88,
     -87, -1587,   -85, -1587, -1587, -1587,  1468, -1587,   451, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587,  -135,   -52,   578,    20,   185,  -925,  -467, -1587,
    -544, -1587, -1587,  -387,  1631, -1587, -1587, -1587, -1573, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,  1296, -1587,
   -1587, -1587, -1587, -1587, -1587,   965, -1587,  -147,    -2,  -110,
      -3,   152, -1587,  -149, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587,   602,  -430,  -911, -1587,  -435,  -908, -1587,  -662,  -105,
    -102, -1587,  -611, -1400, -1587,  -372, -1587, -1587,  1432, -1587,
   -1587, -1587,  1102,  1062,   299, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587,  -633,   -23, -1587,  1013, -1587, -1587,   338, -1587,  1147,
   -1587, -1587, -1587,  -364, -1587, -1587,  -389, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587,  -238, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,  1016,  -719,
    -190,  -862,  -717, -1587, -1587, -1201,  -943, -1587, -1587, -1587,
   -1200,   -39, -1188, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587,   253,  -508, -1587, -1587, -1587,   758, -1587, -1587,
   -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587, -1587,
   -1587, -1587, -1422,  -738, -1587
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   652,    18,   155,    56,   191,    19,   177,   183,
    1629,  1427,  1543,   747,   544,   161,   545,   105,    21,    22,
      47,    48,    49,    94,    23,    41,    42,   546,   547,  1274,
    1275,   548,  1441,  1555,   549,   550,  1009,   551,   673,   674,
     552,   553,   554,   555,  1206,   699,   184,   185,    37,    38,
      39,   236,    71,    72,    73,    74,    24,   303,   398,   226,
      25,   117,   227,   118,   168,   556,   150,   725,  1082,   559,
     399,   560,   756,   636,   766,  1149,   608,   989,  1541,   991,
    1542,   562,   563,   564,   676,   903,  1503,   565,   566,   567,
     568,   569,   570,   571,   572,   462,   573,   780,  1291,  1024,
     574,   575,   942,  1518,   943,  1519,   945,  1520,   576,   908,
    1509,   577,   757,  1571,   578,  1299,  1300,  1028,   727,   579,
     837,  1015,   580,   690,  1083,   582,   583,   584,  1007,   585,
    1268,  1635,  1269,  1700,   586,  1076,  1478,   587,   588,  1460,
    1713,  1462,  1714,  1578,  1755,   741,   590,   393,  1487,  1589,
    1333,  1335,  1125,   147,   602,  1121,   845,  1665,  1719,   394,
     395,   396,   869,   870,   444,   871,   872,   445,  1230,   664,
     665,  1669,   616,   415,   326,   327,   233,   319,    84,   129,
      27,   173,   315,    95,    96,   187,    97,    28,    53,   121,
     170,    29,   404,   231,   232,    82,   126,   406,    30,   171,
     317,   666,   591,   314,   372,   373,  1138,   882,   443,   246,
     374,   862,  1490,  1146,   856,   441,   375,   617,  1339,   874,
     622,  1344,   618,  1340,   619,  1341,   621,  1343,   625,  1347,
     626,  1492,   627,  1349,   628,  1493,   629,  1351,   630,  1494,
     631,  1353,   632,  1355,   655,    31,   101,   193,   382,   656,
      32,   102,   194,   383,   660,    33,   100,   192,   592,  1721,
    1731,  1021,   975,  1722,  1723,  1724,   976,   988,  1252,  1246,
    1241,  1421,  1171,   593,   898,  1499,   899,  1500,   954,  1524,
     951,  1522,   977,   767,   594,   952,  1523,   978,   595,  1177,
    1599,  1178,  1600,  1179,  1601,   912,  1513,   949,  1521,   743,
     768,   596,  1687,  1001,   597
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   851,   189,    52,   897,   376,   875,   318,   305,   936,
     658,   558,   581,   742,   653,    64,    75,   771,   974,    76,
     974,  1002,   981,   864,   612,   866,   237,   868,   781,  1140,
    1013,  1142,  1237,  1144,  1219,   603,  1438,  1411,  1249,   688,
     901,  1215,  1170,   728,   729,   138,  1217,    90,   639,  1495,
    1564,  -138,   843,   647,  1020,   163,  1165,  1026,   967,   682,
     130,   131,  1699,    40,   720,    62,     2,  1226,    75,    75,
      75,  1247,   892,     3,   234,  1253,  1262,  1277,   662,  1718,
     654,   659,    91,   841,  1368,   110,   111,   112,  1428,   402,
     784,   785,    54,   558,    65,    63,     4,   152,     5,   663,
       6,  1429,  1432,   966,   460,   979,     7,   983,    34,    35,
      75,    75,    75,    75,  1548,    50,     8,  1485,   558,   391,
      83,   997,     9,    66,   234,   172,   637,  1748,  1238,  1239,
     737,   739,  1010,  1757,   613,    51,   235,  1152,  1270,   894,
      55,   679,   442,    85,   614,   996,    10,   461,   228,  1271,
    1272,   178,   179,    62,  1283,   892,   683,   684,  1240,   467,
     634,   162,  1651,   968,   431,  1229,   229,    67,    11,    12,
    1270,   104,   407,   557,   852,  1181,   959,  1486,  1281,   635,
     320,  1271,  1272,    63,   392,  1020,   235,   788,   789,   247,
     248,   432,   433,   782,   321,   794,    68,   795,   796,   797,
     798,   615,  1691,    62,  1692,   799,  1533,   148,   967,   431,
    1696,   132,  1213,  1027,   640,  1216,   133,   649,   134,  1483,
     371,   135,   894,   304,    86,  1705,    13,   149,   558,   893,
      92,    13,   641,    63,  1628,   323,   432,   433,   642,  1334,
     893,    93,   381,   370,   893,   914,    14,    57,   893,   893,
     893,    14,   600,    58,   685,   434,    88,   893,   613,   435,
     842,   893,    36,   113,   136,   693,   104,  1381,   614,  1166,
      69,  1382,    15,   686,   893,   893,  1747,   956,   103,    70,
     711,   409,  1288,    16,   892,   817,   818,   893,   114,   884,
    1406,  1273,   638,  1398,   887,    57,    13,  1020,  1284,   558,
     434,    58,   180,   391,   435,   935,  1370,   181,   234,   182,
     400,    87,   135,   405,   772,  1029,    14,  1638,   662,   883,
      13,  1777,  1778,  1637,   794,   615,   436,   796,   797,   955,
     437,   558,  1620,   438,  1610,    77,    78,   104,    79,   663,
      14,    50,   784,   785,   796,   797,   689,   109,   439,   892,
     893,   894,    98,   895,   440,  1444,   896,  1792,  1360,  1229,
     773,    51,  1260,  1261,    62,  1264,    80,   968,    86,   430,
     235,   436,   839,  1003,   371,   437,   844,  1218,   438,   854,
     855,   857,  1281,   859,   860,  1263,   880,   863,   892,   865,
     371,   867,   139,   439,    63,   196,   151,   370,  1535,   440,
    1701,   892,   148,   601,  1670,  1671,   759,   879,  1025,  1128,
     141,  1618,  1489,   370,   817,   818,   894,   892,    50,   371,
     324,   371,   149,  1683,  1684,  1301,    93,  1739,  1022,  1282,
     885,   817,   818,   144,   888,   880,   881,    81,    51,   788,
     789,   325,   370,   324,   370,   241,  1294,   794,   916,  1020,
     796,   797,   798,   900,   846,   894,  1295,   799,  1126,  1285,
     968,   464,  1532,   558,   325,  1280,  1459,   662,   894,   465,
    1725,   916,   242,   156,  1538,  1281,    86,   370,   680,  1256,
     304,   940,  1735,    50,   894,   916,  1286,  1023,   663,  1728,
    1729,    50,  1296,  1134,   705,   304,   371,   371,  1328,  1364,
    1227,   408,    13,    51,  1278,   304,   304,   304,  1148,  1163,
    1164,    51,  1371,  1297,   974,    13,  1405,  1410,  1298,   370,
     370,  1720,    14,  1180,   113,    62,   990,  1765,  1766,   974,
    1525,  1417,   542,   724,  1003,    14,  1569,   817,   818,  1004,
    1011,   542,   724,   651,  1008,   761,   762,   710,   431,  1330,
      13,   913,   915,  1207,  1208,    63,  1210,    50,  1448,   322,
    1212,  1235,  1214,   774,  1527,   775,   776,   777,    13,   778,
      14,  1202,   157,  1243,   694,   432,   433,    51,   950,   158,
     783,   953,  1005,  1457,  1448,  1450,  1446,    13,    14,   159,
     106,   107,   108,   695,   371,   371,   371,   916,   371,   371,
     442,  1553,   371,  1244,   371,   160,   371,    14,   371,  1449,
    1565,   850,  1325,  1326,  1327,   651,  1399,   370,   370,   370,
     145,   370,   370,  1145,  1576,   370,    13,   370,   188,   370,
    1431,   370,   164,   165,   166,   167,  1150,   169,  1019,   434,
     146,    13,   613,   435,    13,  1156,    14,    13,  1157,   599,
      91,   916,   614,    13,   651,   657,    13,  1161,   916,   190,
     916,    14,  1147,   916,    14,  1167,  1436,    14,   304,   651,
    1176,   371,   651,    14,   712,   651,    14,  1357,  1583,   905,
    1395,   651,   784,   785,   651,  1585,    13,  1594,    13,  1397,
    1616,  1120,  1424,   713,   370,   371,   304,  1530,  1124,  1514,
     715,    13,   195,  1119,   916,   916,    14,    13,    14,   615,
     436,  1046,   916,  1356,   437,  1200,  1354,   438,   370,   716,
      13,    14,  1345,  1648,  1376,   916,  1047,    14,  1560,   651,
      13,  1630,   439,   122,   123,   651,  1235,  1425,   440,  1631,
      14,   304,   987,  1505,    43,    44,    45,  1536,   651,  1457,
      14,  1150,  1697,  1150,  1582,  1235,  1511,   110,   651,   304,
    1006,  1033,  1037,  1786,    13,  1014,  1512,  1537,   245,  1169,
    1235,   230,  -741,    13,  1458,   916,  1051,    46,   304,   788,
     789,  1235,  1797,  1250,    14,  1358,  1251,   794,  1380,   795,
     796,   797,   798,    14,  1077,  1649,   306,   799,    13,   243,
     307,   651,  -797,  -804,   906,   442,  1662,  -797,  -804,   244,
      13,  -811,  -444,  1386,   308,   309,  -811,  -444,    14,   310,
     311,   312,   313,   907,   371,  -797,  -804,   115,   377,   985,
      14,   986,   245,   116,  -811,  -444,   145,   759,   469,   470,
    1314,  1558,    75,   378,  1319,   759,   300,   370,   379,  1540,
     380,   371,  1130,  1148,  1526,  1315,   146,   916,   480,  1320,
     371,   148,   148,   371,   485,   814,   815,   816,   148,  1619,
     401,   648,   371,   301,   370,   371,   371,   817,   818,  1435,
     371,   149,   149,   370,  1563,   371,   370,   414,   149,   371,
     968,   558,   581,  1358,  1358,   370,  1342,   302,   370,   370,
    1408,   500,   501,   370,   412,  1281,   316,   413,   370,  1547,
     414,  1650,   370,  1270,   414,  1409,   388,   916,  1681,   371,
     371,   119,   371,  1557,  1271,  1272,   371,   120,   371,   371,
     304,   916,  1634,   124,   442,   503,   504,   384,  1131,   125,
     916,   127,   370,   370,  1734,   370,   385,   128,   153,   370,
    1331,   370,   370,  1653,   154,  1418,  1332,   110,  1419,   112,
    1730,  1420,  1257,   304,  1258,  1730,   371,  1387,   371,   542,
     724,   442,   386,  1680,  1236,  1132,    62,  1245,   442,   442,
    1236,  1245,  1135,  1136,  1760,   387,   442,   389,   442,   370,
    1139,   370,  1141,   519,   520,   521,    50,   390,   442,   442,
     442,  1758,  1143,  1369,  1379,   442,    63,   442,   397,  1658,
    1506,  1715,   174,   175,   992,   993,   533,   403,  1148,   447,
     448,   449,   450,   451,   452,   453,   454,   411,  1740,   416,
    1367,   417,  1626,   675,   410,  1312,   110,   111,   112,   418,
    1709,  1710,    43,    44,    45,   174,   175,   176,   455,  1647,
     540,  1437,   420,  1443,   784,   785,   238,   239,   456,   457,
     458,   419,   238,   239,   240,   667,   668,   669,  1795,   142,
     143,  1572,    59,    60,    61,   304,   304,   304,   421,   422,
     424,   425,   426,   427,   429,   442,   428,   726,   726,   726,
     446,  1401,   -79,   459,   431,   463,   610,  1751,   611,   623,
     624,   701,   670,   671,   681,   702,   645,   466,   604,   703,
     707,   371,  1416,   706,   -90,   698,   704,   730,  1423,   714,
    1496,   432,   433,   371,  1336,   731,   732,  1430,  1338,   717,
     718,   733,   746,   734,   370,   735,   748,  1346,   764,   770,
     760,   769,  1442,   779,    16,   847,   370,   744,  1501,   849,
     838,   788,   789,   431,   371,   371,   710,   876,   861,   794,
     657,   795,   796,   797,   798,   886,   909,   770,   890,   799,
     891,  1465,   902,  1377,  1652,   904,   561,   370,   370,   371,
     432,   433,   371,  1475,   431,   434,  1743,  1488,  1480,   435,
     819,   820,   821,   822,   823,   824,   825,   826,   910,   911,
     916,   782,   370,   827,   828,   370,   934,   939,   941,   829,
     947,   432,   433,   946,   958,   984,   995,   998,   999,   830,
    1554,  1000,   831,   832,  1574,  1012,    13,  1044,  1123,   833,
     834,   835,  1127,  1502,  1151,   812,   813,   814,   815,   816,
    1570,  1133,  1155,  1586,   434,   371,    14,  1137,   435,   817,
     818,  1159,  1160,  1174,   651,  1203,   436,  1205,   561,  1234,
     437,  1708,  1201,   438,  1228,  1529,   304,  1255,   370,  1235,
     770,  1265,  1266,  1290,  1242,   434,   836,  1267,   439,   435,
    1287,  1322,  1329,   561,   440,  1289,  1293,  1303,  1302,  1304,
    1305,  1306,  1334,  1545,  1546,  1348,  1350,  1549,  1352,   770,
    1363,  1440,  1374,   542,   724,  1307,  1452,  1426,  1570,  1317,
    1318,  1323,  1453,  1359,  1455,   436,  1372,  1412,  1373,   437,
    1385,  1337,   438,  1375,  1467,  1378,  1456,  1384,  1393,  1482,
    1396,  1400,  1414,  1587,  1422,   371,  1481,   439,  1445,  1491,
    1659,  1454,  1497,   440,  1468,  1470,   436,  1508,  1544,  1476,
     437,   635,  1365,   438,  1504,  1507,  1534,  1561,   370,  1566,
     726,  1539,   431,   304,   589,  1559,  1562,  1568,   439,  1588,
    1596,  1525,  1598,  1592,   440,  1672,  1593,  1602,  1609,  1603,
    1604,   431,  1608,  1611,  1612,  1693,  1516,  1517,  1636,   432,
     433,  1674,   431,   561,  1694,  1654,  1673,  1675,  1676,  1677,
    1678,  1679,  1695,  1707,  1236,  1716,  1737,  1738,   432,   433,
    1741,  1732,  1746,  1633,  1733,  1759,  1236,  1745,  1761,   432,
     433,  1762,  1763,  1764,  1790,  1767,  1768,  1769,   994,  1706,
    1770,   770,  1772,  1775,  1780,   304,   672,   371,  1787,   371,
    1556,  1788,  1791,  1789,    20,   137,   692,  1796,    89,   140,
     186,   697,   328,   434,   700,   304,  1641,   435,   371,  1122,
     370,  1279,   370,   709,   561,  1642,  1742,  1643,  1644,    26,
    1645,   721,   434,  1682,  1292,  1615,   435,  1567,  1451,  1750,
    1666,   370,  1590,   434,  1591,  1484,  1667,   435,    99,  1668,
     677,   423,  1361,   678,  1736,   745,   561,   982,  1415,  1625,
       0,     0,   598,     0,   689,     0,     0,     0,     0,     0,
       0,     0,     0,   763,     0,  1717,  1776,     0,     0,     0,
       0,     0,     0,   304,   436,     0,     0,     0,   437,     0,
    1366,   438,     0,     0,     0,   770,     0,     0,  1785,     0,
     371,     0,     0,   436,     0,   726,   439,   437,     0,  1383,
     438,     0,   440,     0,   436,   371,     0,     0,   437,     0,
    1389,   438,     0,   370,   840,   439,     0,   915,     0,     0,
       0,   440,     0,     0,     0,     0,   439,     0,   370,     0,
       0,   853,   440,     0,     0,  1664,     0,     0,     0,     0,
     304,     0,     0,     0,     0,     0,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,  1784,   561,   561,
     561,   561,   561,   561,     0,     0,  1686,     0,     0,     0,
       0,     0,     0,  1686,     0,  1686,     0,   371,   561,     0,
       0,  1686,     0,     0,     0,  1006,  1794,     0,     0,     0,
       0,     0,     0,     0,   304,     0,  1686,     0,     0,     0,
     370,     0,   692,     0,     0,     0,     0,   726,   726,   726,
       0,   937,   770,     0,   770,     0,   770,     0,   770,     0,
     770,     0,   770,     0,   770,     0,   770,     0,   770,     0,
     770,   431,   770,     0,   957,   770,     0,   770,     0,   770,
       0,   770,     0,   770,     0,   770,     0,  1686,     0,     0,
    1006,     0,   973,     0,   973,     0,   609,   770,   432,   433,
    1754,     0,     0,     0,     0,   620,     0,  1756,     0,     0,
       0,     0,     0,     0,     0,   633,     0,   917,   918,   919,
     920,   921,   922,   923,   924,   644,     0,     0,     0,     0,
     925,   926,  1686,  1686,     0,     0,   927,     0,     0,     0,
       0,  1782,  1783,   304,     0,   661,   830,     0,     0,   928,
     929,     0,     0,     0,     0,     0,   930,   931,   932,     0,
       0,     0,   434,     0,     0,     0,   435,   691,  1686,     0,
    1793,     0,     0,     0,  1084,  1086,  1088,  1090,  1092,  1094,
    1096,  1098,  1100,  1102,  1104,  1105,  1107,  1109,  1111,  1113,
    1115,  1117,     0,   722,   723,     0,     0,     0,   784,   785,
       0,     0,     0,   933,     0,     0,  1129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   749,   750,   751,   752,   753,   758,   758,     0,     0,
     542,   724,     0,   436,     0,     0,  1154,   437,     0,  1390,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1162,     0,     0,   439,     0,     0,     0,     0,
    1173,   440,  1175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1183,  1184,  1185,  1186,  1187,  1188,  1189,  1190,
    1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,     0,
       0,     0,     0,   786,   787,   788,   789,   790,     0,     0,
     791,   792,   793,   794,     0,   795,   796,   797,   798,     0,
       0,     0,     0,   799,     0,   800,   801,     0,     0,     0,
       0,   758,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   878,   784,   785,     0,     0,     0,     0,     0,  1254,
       0,     0,     0,     0,     0,     0,     0,     0,  1259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1276,     0,   770,     0,     0,     0,
       0,     0,   758,   691,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   938,     0,     0,     0,     0,     0,
     944,     0,     0,   817,   818,     0,   948,     0,     0,   758,
       0,     0,   758,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   960,   961,   962,   963,
     964,   965,     0,     0,  -364,     0,     0,   786,   787,   788,
     789,   790,     0,     0,   791,   784,   785,   794,     0,   795,
     796,   797,   798,     0,     0,     0,   561,   799,     0,   800,
     801,     0,     0,     0,     0,     0,     0,     0,     0,   758,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1038,  1039,     0,     0,  1040,  1041,  1042,  1043,
     431,  1045,     0,  1048,  1049,  1050,  1052,  1053,  1054,  1055,
    1056,  1057,  1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,
    1067,  1068,  1069,     0,  1078,     0,     0,   432,   433,     0,
       0,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,   758,     0,     0,   817,   818,     0,
     786,   787,   788,   789,   790,     0,     0,   791,   792,   793,
     794,     0,   795,   796,   797,   798,     0,     0,     0,     0,
     799,     0,   800,   801,     0,     0,     0,     0,   802,   803,
     804,  1388,     0,     0,   805,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,   435,  1158,     0,     0,  -364,
       0,     0,     0,     0,  1403,  1404,     0,   784,   785,  1407,
     758,     0,     0,     0,     0,     0,   431,  1413,     0,  -364,
     973,     0,  1182,     0,     0,     0,     0,  -364,     0,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,     0,     0,   432,   433,     0,     0,     0,     0,  1439,
     817,   818,     0,     0,   589,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,   437,     0,  1391,   438,
       0,     0,     0,     0,     0,     0,   749,  1232,   758,     0,
       0,     0,   736,     0,   439,     0,   758,     0,   329,     0,
     440,     0,     0,     0,   330,     0,     0,     0,     0,     0,
     331,     0,   786,   787,   788,   789,  1182,   434,     0,     0,
     332,   435,   794,     0,   795,   796,   797,   798,   333,   784,
     785,     0,   799,   758,   800,   801,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,     0,     0,  1498,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,     0,     0,     0,     0,  1316,   436,     0,
       0,  1321,   437,     0,  1392,   438,     0,     0,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
     439,     0,   817,   818,     0,     0,   440,     0,     0,     0,
       0,     0,    62,     0,   786,   787,   788,   789,   790,     0,
       0,   791,   792,   793,   794,   368,   795,   796,   797,   798,
     431,     0,     0,     0,   799,     0,   800,   801,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   433,     0,
       0,  1085,  1087,  1089,  1091,  1093,  1095,  1097,  1099,  1101,
    1103,   758,  1106,  1108,  1110,  1112,  1114,  1116,     0,   431,
    1595,     0,  1597,     0,     0,     0,     0,     0,     0,     0,
     369,   431,     0,  1605,  1606,  1607,   808,   809,   810,   811,
     812,   813,   814,   815,   816,     0,   432,   433,     0,     0,
    1622,  1623,     0,     0,   817,   818,  1627,     0,   432,   433,
       0,   434,     0,     0,     0,   435,     0,   431,     0,     0,
    1646,     0,   758,     0,  1402,     0,     0,   758,     0,     0,
       0,     0,     0,     0,  1656,     0,     0,     0,     0,     0,
       0,     0,     0,   758,   432,   433,     0,     0,     0,   758,
       0,     0,     0,     0,     0,     0,     0,     0,   758,     0,
     434,     0,  1433,  1434,   435,   431,     0,     0,     0,     0,
       0,     0,   434,   758,     0,     0,   435,     0,     0,     0,
       0,  1182,   436,  1688,  1689,     0,   437,     0,  1394,   438,
       0,     0,   432,   433,     0,     0,     0,     0,     0,  1461,
       0,  1463,   758,  1466,   439,  1704,     0,     0,   434,  1469,
     440,     0,   435,  1472,   758,     0,     0,     0,  1711,   758,
    1712,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,   437,     0,  1510,   438,  1726,
       0,  1727,     0,   436,     0,     0,     0,   437,     0,  1515,
     438,     0,     0,   439,     0,     0,   434,     0,     0,   440,
     435,     0,     0,     0,   758,   439,   431,     0,  1749,     0,
       0,   440,     0,     0,  1752,  1753,     0,     0,     0,   436,
       0,     0,     0,   437,     0,  1552,   438,     0,     0,     0,
       0,     0,     0,   432,   433,     0,   758,     0,     0,     0,
       0,   439,  1771,     0,  1773,  1774,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,  1781,   738,     0,     0,
       0,     0,     0,   329,   758,   758,     0,   436,   758,   330,
       0,   437,     0,  1657,   438,   331,     0,     0,     0,     0,
       0,     0,     0,   758,     0,   332,     0,     0,     0,   439,
       0,     0,     0,   333,     0,   440,     0,   434,     0,     0,
       0,   435,     0,     0,  1579,     0,  1580,     0,   334,   758,
       0,     0,  1584,     0,     0,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1614,     0,     0,     0,     0,  1617,     0,   436,     0,
    1621,     0,   437,  1624,     0,   438,     0,     0,     0,     0,
       0,     0,     0,  1632,   758,     0,     0,    62,     0,     0,
     439,     0,     0,     0,     0,     0,   440,     0,     0,     0,
     368,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,   917,   918,   919,   920,   921,   922,   923,   924,     0,
       0,     0,     0,     0,   925,   926,     0,     0,   758,     0,
     927,     0,     0,     0,   758,  -394,     0,     0,     0,     0,
     830,     0,     0,   928,   929,     0,     0,     0,     0,     0,
     930,   931,   932,     0,     0,   369,     0,     0,     0,     0,
    1698,     0,   468,     0,     0,   758,   469,   470,     3,     0,
     471,   472,   473,     0,   474,     0,   475,   476,   477,   478,
     479,     0,     0,     0,     0,     0,   480,   481,   482,   483,
     484,     0,   485,     0,     0,     0,     0,   933,     0,   486,
     487,   488,     0,   489,     0,   490,   491,     0,     0,   492,
       0,     8,   493,   494,     0,   495,   496,     0,     0,   497,
     498,  1744,     0,     0,   542,   724,   499,     0,   758,   500,
     501,     0,   335,   336,   337,     0,   339,   340,   341,   342,
     343,   502,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,     0,   357,   358,   359,     0,     0,   362,
     363,   364,   365,   503,   504,   505,   506,     0,     0,     0,
       0,     0,  1779,     0,     0,     0,     0,     0,     0,   507,
     508,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   512,   513,   514,   515,   516,     0,   517,     0,
     518,   519,   520,   521,    50,   148,   522,   523,   524,   525,
     526,   527,   528,   529,    63,   530,   531,   532,     0,     0,
       0,     0,     0,     0,   533,   149,   534,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   535,   536,   537,     0,    15,     0,     0,
     538,   539,     0,     0,   469,   470,     0,     0,   540,     0,
     541,     0,   542,   543,   475,   476,   477,   478,   479,     0,
       0,     0,     0,     0,   480,     0,   482,     0,     0,     0,
     485,     0,   431,     0,     0,     0,     0,     0,   487,     0,
       0,     0,     0,     0,   491,     0,     0,   492,     0,     0,
     493,     0,   967,     0,   496,     0,     0,     0,     0,   432,
     433,     0,     0,     0,   605,     0,     0,   500,   501,     0,
     335,   336,   337,     0,   339,   340,   341,   342,   343,   502,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,     0,   357,   358,   359,     0,     0,   362,   363,   364,
     365,   503,   504,   606,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,     0,
       0,     0,     0,   434,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     512,   513,   514,   515,   516,     0,   517,   968,   518,   519,
     520,   521,    50,     0,     0,   523,   524,   525,   526,   527,
     528,   529,   969,   607,   531,   532,     0,     0,     0,     0,
       0,     0,   533,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,   437,     0,
       0,   970,   536,   537,     0,    15,     0,     0,   538,   539,
       0,     0,     0,   469,   470,     0,   971,     0,   972,     0,
     542,   543,   440,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,   431,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   491,     0,     0,   492,     0,     0,   493,
       0,     0,     0,   496,     0,     0,     0,     0,   432,   433,
       0,     0,     0,   605,     0,     0,   500,   501,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   502,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     503,   504,   606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,   508,     0,     0,
       0,     0,   434,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   512,
     513,   514,   515,   516,     0,   517,   968,   518,   519,   520,
     521,    50,     0,     0,   523,   524,   525,   526,   527,   528,
     529,   969,   607,   531,   532,     0,     0,     0,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,   437,     0,     0,
     970,   536,   537,     0,    15,     0,     0,   538,   539,     0,
       0,     0,   469,   470,     0,   971,     0,   980,     0,   542,
     543,   440,   475,   476,   477,   478,   479,     0,     0,     0,
       0,     0,   480,     0,   482,     0,     0,     0,   485,     0,
     639,     0,     0,     0,     0,     0,   487,     0,     0,     0,
       0,     0,   491,     0,     0,   492,     0,     0,   493,     0,
       0,     0,   496,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   605,     0,     0,   500,   501,     0,   335,   336,
     337,     0,   339,   340,   341,   342,   343,   502,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,     0,
     357,   358,   359,     0,     0,   362,   363,   364,   365,   503,
     504,   606,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   507,   508,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   512,   513,
     514,   515,   516,     0,   517,     0,   518,   519,   520,   521,
      50,     0,     0,   523,   524,   525,   526,   527,   528,   529,
      63,   607,   531,   532,     0,     0,     0,     0,     0,     0,
     533,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   640,     0,     0,   535,
     536,   537,     0,    15,     0,     0,   538,   539,     0,     0,
       0,   469,   470,     0,  1231,     0,   541,     0,   542,   543,
     642,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,     0,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     505,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,   687,     0,     0,     0,     0,     0,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,   469,
     470,     0,     0,   540,     0,   541,     0,   542,   543,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   491,
       0,     0,   492,     0,     0,   493,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   605,
       0,     0,   500,   501,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   502,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   503,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   507,   508,     0,     0,     0,     0,     0,     0,
       0,   719,     0,     0,     0,     0,     0,   509,   510,   511,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   512,   513,   514,   515,   516,
       0,   517,     0,   518,   519,   520,   521,    50,     0,     0,
     523,   524,   525,   526,   527,   528,   529,    63,   607,   531,
     532,     0,     0,     0,     0,     0,     0,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   535,   536,   537,     0,
      15,     0,     0,   538,   539,     0,     0,   469,   470,     0,
       0,   540,     0,   541,     0,   542,   543,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,  1639,   482,
     483,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,   494,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   605,     0,     0,
     500,   501,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   502,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   503,   504,   606,  1640,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   512,   513,   514,   515,   516,     0,   517,
       0,   518,   519,   520,   521,    50,     0,     0,   523,   524,
     525,   526,   527,   528,   529,    63,   607,   531,   532,     0,
       0,     0,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,   536,   537,     0,    15,     0,
       0,   538,   539,     0,     0,   469,   470,     0,     0,   540,
       0,   541,     0,   542,   543,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   491,     0,     0,   492,     0,
       0,   493,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   605,     0,     0,   500,   501,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     502,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   503,   504,   505,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   509,   510,   511,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   512,   513,   514,   515,   516,     0,   517,     0,   518,
     519,   520,   521,    50,     0,     0,   523,   524,   525,   526,
     527,   528,   529,    63,   607,   531,   532,     0,     0,     0,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   535,   536,   537,     0,    15,     0,     0,   538,
     539,     0,     0,   469,   470,     0,     0,   540,     0,   541,
       0,   542,   543,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   491,     0,     0,   492,     0,     0,   493,
       0,     0,     0,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   605,     0,     0,   500,   501,  1016,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   502,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     503,   504,   606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   512,
     513,   514,   515,   516,     0,   517,   968,   518,   519,   520,
     521,    50,     0,     0,   523,   524,   525,   526,   527,   528,
     529,   969,   607,   531,   532,     0,     0,     0,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,   536,   537,     0,    15,     0,     0,   538,   539,     0,
       0,   469,   470,     0,     0,  1017,     0,   541,  1018,   542,
     543,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,     0,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     505,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1079,
    1080,  1081,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,     0,
       0,   469,   470,   540,     0,   541,     0,   542,   543,   754,
       0,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,   755,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,     0,
       0,   469,   470,   540,   643,   541,     0,   542,   543,   754,
       0,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,   755,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,   968,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,   969,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,     0,
       0,   469,   470,   540,     0,   541,     0,   542,   543,   754,
       0,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,   755,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,     0,
       0,   469,   470,   540,   876,   541,     0,   542,   543,   754,
       0,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,   755,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,   469,
     470,     0,     0,   540,     0,   541,     0,   542,   543,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   491,
       0,     0,   492,     0,     0,   493,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   605,
       0,     0,   500,   501,  1168,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   502,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   503,   504,   606,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   512,   513,   514,   515,   516,
       0,   517,   968,   518,   519,   520,   521,    50,     0,     0,
     523,   524,   525,   526,   527,   528,   529,   969,   607,   531,
     532,     0,     0,     0,     0,     0,     0,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   535,   536,   537,     0,
      15,     0,     0,   538,   539,     0,     0,   469,   470,     0,
       0,   540,     0,   541,     0,   542,   543,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   605,     0,     0,
     500,   501,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   502,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   503,   504,   606,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   512,   513,   514,   515,   516,     0,   517,
       0,   518,   519,   520,   521,    50,     0,     0,   523,   524,
     525,   526,   527,   528,   529,    63,   607,   531,   532,     0,
       0,     0,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,   536,   537,     0,    15,     0,
       0,   538,   539,     0,     0,   469,   470,     0,     0,   540,
     643,   541,     0,   542,   543,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   491,     0,     0,   492,     0,
       0,   493,     0,     0,     0,   496,     0,     0,     0,     0,
       0,   696,     0,     0,     0,   605,     0,     0,   500,   501,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     502,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   503,   504,   606,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   512,   513,   514,   515,   516,     0,   517,     0,   518,
     519,   520,   521,    50,     0,     0,   523,   524,   525,   526,
     527,   528,   529,    63,   607,   531,   532,     0,     0,     0,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   535,   536,   537,     0,    15,     0,     0,   538,
     539,     0,     0,   469,   470,     0,     0,   540,     0,   541,
       0,   542,   543,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   491,     0,     0,   492,     0,     0,   493,
       0,     0,     0,   496,     0,     0,   708,     0,     0,     0,
       0,     0,     0,   605,     0,     0,   500,   501,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   502,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     503,   504,   606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   512,
     513,   514,   515,   516,     0,   517,     0,   518,   519,   520,
     521,    50,     0,     0,   523,   524,   525,   526,   527,   528,
     529,    63,   607,   531,   532,     0,     0,     0,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,   536,   537,     0,    15,     0,     0,   538,   539,     0,
       0,     0,     0,   469,   470,   540,     0,   541,     0,   542,
     543,   740,     0,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   491,     0,     0,   492,     0,     0,   493,
       0,     0,     0,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   605,     0,     0,   500,   501,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   502,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     503,   504,   606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   512,
     513,   514,   515,   516,     0,   517,     0,   518,   519,   520,
     521,    50,     0,     0,   523,   524,   525,   526,   527,   528,
     529,    63,   607,   531,   532,     0,     0,     0,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,   536,   537,     0,    15,     0,     0,   538,   539,     0,
       0,   469,   470,     0,     0,   540,     0,   541,     0,   542,
     543,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,     0,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   744,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,     0,
       0,   469,   470,   540,     0,   541,     0,   542,   543,   765,
       0,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,     0,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,   469,
     470,     0,     0,   540,     0,   541,     0,   542,   543,   475,
     476,   477,   478,   479,     0,     0,  1058,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   491,
       0,     0,   492,     0,     0,   493,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   605,
       0,     0,   500,   501,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   502,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   503,   504,   606,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   512,   513,   514,   515,   516,
       0,   517,     0,   518,   519,   520,   521,    50,     0,     0,
     523,   524,   525,   526,   527,   528,   529,    63,   607,   531,
     532,     0,     0,     0,     0,     0,     0,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   535,   536,   537,     0,
      15,     0,     0,   538,   539,     0,     0,   469,   470,     0,
       0,   540,     0,   541,     0,   542,   543,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   605,     0,     0,
     500,   501,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   502,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   503,   504,   606,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   512,   513,   514,   515,   516,     0,   517,
       0,   518,   519,   520,   521,    50,     0,     0,   523,   524,
     525,   526,   527,   528,   529,    63,   607,   531,   532,     0,
       0,     0,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,   536,   537,     0,    15,     0,
       0,   538,   539,     0,     0,   469,   470,     0,     0,   540,
       0,   541,  1118,   542,   543,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   491,     0,     0,   492,     0,
       0,   493,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   605,     0,     0,   500,   501,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     502,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   503,   504,   606,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   512,   513,   514,   515,   516,     0,   517,     0,   518,
     519,   520,   521,    50,     0,     0,   523,   524,   525,   526,
     527,   528,   529,    63,   607,   531,   532,     0,     0,     0,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1172,     0,   535,   536,   537,     0,    15,     0,     0,   538,
     539,     0,     0,   469,   470,     0,     0,   540,     0,   541,
       0,   542,   543,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   491,     0,     0,   492,     0,     0,   493,
       0,     0,     0,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   605,     0,     0,   500,   501,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   502,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     503,   504,   606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   512,
     513,   514,   515,   516,     0,   517,     0,   518,   519,   520,
     521,    50,     0,     0,   523,   524,   525,   526,   527,   528,
     529,    63,   607,   531,   532,     0,     0,     0,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,   536,   537,     0,    15,     0,     0,   538,   539,     0,
       0,   469,   470,     0,     0,   540,     0,   541,  1233,   542,
     543,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,     0,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,   469,
     470,     0,     0,   540,     0,   541,  1248,   542,   543,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   491,
       0,     0,   492,     0,     0,   493,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   605,
       0,     0,   500,   501,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   502,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   503,   504,   606,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   512,   513,   514,   515,   516,
       0,   517,     0,   518,   519,   520,   521,    50,     0,     0,
     523,   524,   525,   526,   527,   528,   529,    63,   607,   531,
     532,     0,     0,     0,     0,     0,     0,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   535,   536,   537,     0,
      15,     0,     0,   538,   539,     0,     0,   469,   470,     0,
       0,   540,     0,   541,  1464,   542,   543,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   605,     0,     0,
     500,   501,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   502,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   503,   504,   606,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   512,   513,   514,   515,   516,     0,   517,
       0,   518,   519,   520,   521,    50,     0,     0,   523,   524,
     525,   526,   527,   528,   529,    63,   607,   531,   532,     0,
       0,     0,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,   536,   537,     0,    15,     0,
       0,   538,   539,     0,     0,   469,   470,     0,     0,  1473,
       0,   541,  1474,   542,   543,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   491,     0,     0,   492,     0,
       0,   493,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   605,     0,     0,   500,   501,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     502,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   503,   504,   606,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   512,   513,   514,   515,   516,     0,   517,     0,   518,
     519,   520,   521,    50,     0,     0,   523,   524,   525,   526,
     527,   528,   529,    63,   607,   531,   532,     0,     0,     0,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   535,   536,   537,     0,    15,     0,     0,   538,
     539,     0,     0,   469,   470,     0,     0,   540,     0,   541,
    1479,   542,   543,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   491,     0,     0,   492,     0,     0,   493,
       0,     0,     0,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   605,     0,     0,   500,   501,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   502,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     503,   504,   606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   512,
     513,   514,   515,   516,     0,   517,     0,   518,   519,   520,
     521,    50,     0,     0,   523,   524,   525,   526,   527,   528,
     529,    63,   607,   531,   532,     0,     0,     0,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,   536,   537,     0,    15,     0,     0,   538,   539,     0,
       0,   469,   470,     0,     0,   540,     0,   541,  1528,   542,
     543,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,     0,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,     0,     0,     0,     0,     0,   533,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,   536,
     537,     0,    15,     0,     0,   538,   539,     0,     0,   469,
     470,     0,     0,   540,     0,   541,  1613,   542,   543,   475,
     476,   477,   478,   479,     0,     0,     0,     0,     0,   480,
       0,   482,     0,     0,     0,   485,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,     0,     0,   491,
       0,     0,   492,     0,     0,   493,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   605,
       0,     0,   500,   501,     0,   335,   336,   337,     0,   339,
     340,   341,   342,   343,   502,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,     0,   357,   358,   359,
       0,     0,   362,   363,   364,   365,   503,   504,   606,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   507,   508,     0,     0,     0,     0,     0,     0,
       0,  1655,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   512,   513,   514,   515,   516,
       0,   517,     0,   518,   519,   520,   521,    50,     0,     0,
     523,   524,   525,   526,   527,   528,   529,    63,   607,   531,
     532,     0,     0,     0,     0,     0,     0,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   535,   536,   537,     0,
      15,     0,     0,   538,   539,     0,     0,   469,   470,     0,
       0,   540,     0,   541,     0,   542,   543,   475,   476,   477,
     478,   479,     0,     0,     0,     0,     0,   480,     0,   482,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,   491,     0,     0,
     492,     0,     0,   493,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   605,     0,     0,
     500,   501,     0,   335,   336,   337,     0,   339,   340,   341,
     342,   343,   502,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,     0,   357,   358,   359,     0,     0,
     362,   363,   364,   365,   503,   504,   606,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     507,   508,     0,     0,     0,     0,     0,     0,     0,  1702,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   512,   513,   514,   515,   516,     0,   517,
       0,   518,   519,   520,   521,    50,     0,     0,   523,   524,
     525,   526,   527,   528,   529,    63,   607,   531,   532,     0,
       0,     0,     0,     0,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,   536,   537,     0,    15,     0,
       0,   538,   539,     0,     0,   469,   470,     0,     0,   540,
       0,   541,     0,   542,   543,   475,   476,   477,   478,   479,
       0,     0,     0,     0,     0,   480,     0,   482,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,     0,     0,     0,   491,     0,     0,   492,     0,
       0,   493,     0,     0,     0,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   605,     0,     0,   500,   501,
       0,   335,   336,   337,     0,   339,   340,   341,   342,   343,
     502,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,     0,   357,   358,   359,     0,     0,   362,   363,
     364,   365,   503,   504,   606,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   507,   508,
       0,     0,     0,     0,     0,     0,     0,  1703,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   512,   513,   514,   515,   516,     0,   517,     0,   518,
     519,   520,   521,    50,     0,     0,   523,   524,   525,   526,
     527,   528,   529,    63,   607,   531,   532,     0,     0,     0,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   535,   536,   537,     0,    15,     0,     0,   538,
     539,     0,     0,   469,   470,     0,     0,   540,     0,   541,
       0,   542,   543,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,   480,     0,   482,     0,     0,     0,   485,
       0,     0,     0,     0,     0,     0,     0,   487,     0,     0,
       0,     0,     0,   491,     0,     0,   492,     0,     0,   493,
       0,     0,     0,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   605,     0,     0,   500,   501,     0,   335,
     336,   337,     0,   339,   340,   341,   342,   343,   502,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
       0,   357,   358,   359,     0,     0,   362,   363,   364,   365,
     503,   504,   606,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   512,
     513,   514,   515,   516,     0,   517,     0,   518,   519,   520,
     521,    50,     0,     0,   523,   524,   525,   526,   527,   528,
     529,    63,   607,   531,   532,     0,     0,     0,     0,     0,
       0,   533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,   536,   537,     0,    15,     0,     0,   538,   539,     0,
       0,   469,   470,     0,     0,   540,     0,   541,     0,   542,
     543,   475,   476,   477,   478,   479,     0,     0,     0,     0,
       0,   480,     0,   482,     0,     0,     0,   485,     0,     0,
       0,     0,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   491,     0,     0,   492,     0,     0,   493,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   605,     0,     0,   500,   501,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   503,   504,
     606,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   512,   513,   514,
     515,   516,     0,   517,     0,   518,   519,   520,   521,    50,
       0,     0,   523,   524,   525,   526,   527,   528,   529,    63,
     607,   531,   532,     0,   329,     0,     0,     0,     0,   533,
     330,     0,     0,     0,     0,     0,   331,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,   535,   536,
     537,     0,    15,     0,   333,   538,   539,     0,     0,     0,
       0,     0,     0,  1447,     0,   541,     0,   542,   543,   334,
       0,     0,     0,     0,     0,     0,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   329,     0,     0,
       0,     0,     0,   330,     0,     0,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,    62,   332,
       0,     0,     0,     0,     0,     0,     0,   333,     0,     0,
       0,   368,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,     0,     0,     0,     0,     0,    63,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   369,     0,   646,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,     0,     0,     0,     0,
       0,     0,     0,    13,     0,     0,     0,     0,   329,     0,
       0,   650,     0,     0,   330,     0,     0,     0,     0,     0,
     331,     0,     0,    14,     0,     0,     0,     0,     0,     0,
     332,   651,     0,     0,     0,     0,     0,     0,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,     0,     0,   369,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   784,   785,     0,     0,     0,     0,
       0,   329,     0,     0,     0,     0,     0,   330,     0,     0,
       0,     0,     0,   331,     0,     0,     0,     0,     0,     0,
       0,     0,    62,   332,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,   368,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,     0,     0,     0,
       0,     0,    63,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,     0,     0,     0,   786,
     787,   788,   789,   790,     0,     0,   791,   792,   793,   794,
     369,   795,   796,   797,   798,     0,     0,     0,     0,   799,
       0,   800,   801,   784,   785,     0,     0,   802,   803,   804,
       0,     0,     0,   805,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   368,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   784,   785,
       0,     0,     0,     0,     0,   650,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,     0,
     807,   808,   809,   810,   811,   812,   813,   814,   815,   816,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   817,
     818,     0,     0,   848,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   369,     0,     0,     0,     0,   786,   787,
     788,   789,   790,     0,     0,   791,   792,   793,   794,     0,
     795,   796,   797,   798,     0,     0,     0,     0,   799,     0,
     800,   801,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   805,   786,   787,   788,   789,   790,     0,     0,
     791,   792,   793,   794,     0,   795,   796,   797,   798,   784,
     785,     0,     0,   799,     0,   800,   801,     0,     0,     0,
       0,   802,   803,   804,     0,     0,     0,   805,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   784,
     785,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,   858,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,   873,     0,     0,
       0,     0,     0,     0,   786,   787,   788,   789,   790,     0,
       0,   791,   792,   793,   794,     0,   795,   796,   797,   798,
       0,     0,     0,     0,   799,     0,   800,   801,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   805,     0,
       0,     0,     0,     0,   786,   787,   788,   789,   790,     0,
       0,   791,   792,   793,   794,     0,   795,   796,   797,   798,
     784,   785,     0,     0,   799,     0,   800,   801,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   805,     0,
       0,     0,     0,   806,     0,   807,   808,   809,   810,   811,
     812,   813,   814,   815,   816,   784,   785,     0,     0,     0,
       0,     0,     0,     0,   817,   818,     0,     0,   889,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   806,     0,   807,   808,   809,   810,   811,
     812,   813,   814,   815,   816,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   817,   818,     0,     0,  1204,     0,
       0,     0,     0,     0,     0,   786,   787,   788,   789,   790,
       0,     0,   791,   792,   793,   794,     0,   795,   796,   797,
     798,     0,     0,     0,     0,   799,     0,   800,   801,     0,
       0,     0,     0,   802,   803,   804,     0,     0,     0,   805,
     786,   787,   788,   789,   790,     0,     0,   791,   792,   793,
     794,     0,   795,   796,   797,   798,   784,   785,     0,     0,
     799,     0,   800,   801,     0,     0,     0,     0,   802,   803,
     804,     0,     0,     0,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,     0,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   784,   785,     0,     0,
       0,     0,     0,     0,     0,   817,   818,     0,     0,  1209,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     817,   818,     0,     0,  1211,     0,     0,     0,     0,     0,
       0,   786,   787,   788,   789,   790,     0,     0,   791,   792,
     793,   794,     0,   795,   796,   797,   798,     0,     0,     0,
       0,   799,     0,   800,   801,     0,     0,     0,     0,   802,
     803,   804,     0,     0,     0,   805,     0,     0,     0,     0,
       0,   786,   787,   788,   789,   790,     0,     0,   791,   792,
     793,   794,     0,   795,   796,   797,   798,   784,   785,     0,
       0,   799,     0,   800,   801,     0,     0,     0,     0,   802,
     803,   804,     0,     0,     0,   805,     0,     0,     0,     0,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,   784,   785,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,  1220,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,  1221,     0,     0,     0,     0,
       0,     0,   786,   787,   788,   789,   790,     0,     0,   791,
     792,   793,   794,     0,   795,   796,   797,   798,     0,     0,
       0,     0,   799,     0,   800,   801,     0,     0,     0,     0,
     802,   803,   804,     0,     0,     0,   805,   786,   787,   788,
     789,   790,     0,     0,   791,   792,   793,   794,     0,   795,
     796,   797,   798,   784,   785,     0,     0,   799,     0,   800,
     801,     0,     0,     0,     0,   802,   803,   804,     0,     0,
       0,   805,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   784,   785,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1222,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,  1223,     0,     0,     0,     0,     0,     0,   786,   787,
     788,   789,   790,     0,     0,   791,   792,   793,   794,     0,
     795,   796,   797,   798,     0,     0,     0,     0,   799,     0,
     800,   801,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   805,     0,     0,     0,     0,     0,   786,   787,
     788,   789,   790,     0,     0,   791,   792,   793,   794,     0,
     795,   796,   797,   798,   784,   785,     0,     0,   799,     0,
     800,   801,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   805,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   784,
     785,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,  1224,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,  1225,     0,     0,     0,     0,     0,     0,   786,
     787,   788,   789,   790,     0,     0,   791,   792,   793,   794,
       0,   795,   796,   797,   798,     0,     0,     0,     0,   799,
       0,   800,   801,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   805,   786,   787,   788,   789,   790,     0,
       0,   791,   792,   793,   794,     0,   795,   796,   797,   798,
     784,   785,     0,     0,   799,     0,   800,   801,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   805,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,     0,
     807,   808,   809,   810,   811,   812,   813,   814,   815,   816,
     784,   785,     0,     0,     0,     0,     0,     0,     0,   817,
     818,     0,     0,  1362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   806,     0,   807,   808,   809,   810,   811,
     812,   813,   814,   815,   816,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   817,   818,     0,     0,  1531,     0,
       0,     0,     0,     0,     0,   786,   787,   788,   789,   790,
       0,     0,   791,   792,   793,   794,     0,   795,   796,   797,
     798,     0,     0,     0,     0,   799,     0,   800,   801,     0,
       0,     0,     0,   802,   803,   804,     0,     0,     0,   805,
       0,     0,     0,     0,     0,   786,   787,   788,   789,   790,
       0,     0,   791,   792,   793,   794,     0,   795,   796,   797,
     798,   784,   785,     0,     0,   799,     0,   800,   801,     0,
       0,     0,     0,   802,   803,   804,     0,     0,     0,   805,
       0,     0,     0,     0,   806,     0,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   784,   785,     0,     0,
       0,     0,     0,     0,     0,   817,   818,     0,     0,  1550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,     0,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   817,   818,     0,     0,  1551,
       0,     0,     0,     0,     0,     0,   786,   787,   788,   789,
     790,     0,     0,   791,   792,   793,   794,     0,   795,   796,
     797,   798,     0,     0,     0,     0,   799,     0,   800,   801,
       0,     0,     0,     0,   802,   803,   804,     0,     0,     0,
     805,   786,   787,   788,   789,   790,     0,     0,   791,   792,
     793,   794,     0,   795,   796,   797,   798,   784,   785,     0,
       0,   799,     0,   800,   801,     0,     0,     0,     0,   802,
     803,   804,     0,     0,     0,   805,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   784,   785,     0,
       0,     0,     0,     0,     0,     0,   817,   818,     0,     0,
    1573,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,  1575,     0,     0,     0,     0,
       0,     0,   786,   787,   788,   789,   790,     0,     0,   791,
     792,   793,   794,     0,   795,   796,   797,   798,     0,     0,
       0,     0,   799,     0,   800,   801,     0,     0,     0,     0,
     802,   803,   804,     0,     0,     0,   805,     0,     0,     0,
       0,     0,   786,   787,   788,   789,   790,     0,     0,   791,
     792,   793,   794,     0,   795,   796,   797,   798,   784,   785,
       0,     0,   799,     0,   800,   801,     0,     0,     0,     0,
     802,   803,   804,     0,     0,     0,   805,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   784,   785,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1577,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1581,     0,     0,     0,
       0,     0,     0,   786,   787,   788,   789,   790,     0,     0,
     791,   792,   793,   794,     0,   795,   796,   797,   798,     0,
       0,     0,     0,   799,     0,   800,   801,     0,     0,     0,
       0,   802,   803,   804,     0,     0,     0,   805,   786,   787,
     788,   789,   790,     0,     0,   791,   792,   793,   794,     0,
     795,   796,   797,   798,   784,   785,     0,     0,   799,     0,
     800,   801,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   805,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   784,   785,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,  1660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,  1661,     0,     0,     0,     0,     0,     0,   786,
     787,   788,   789,   790,     0,     0,   791,   792,   793,   794,
       0,   795,   796,   797,   798,     0,     0,     0,     0,   799,
       0,   800,   801,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   805,     0,     0,     0,     0,     0,   786,
     787,   788,   789,   790,     0,     0,   791,   792,   793,   794,
       0,   795,   796,   797,   798,   784,   785,     0,     0,   799,
       0,   800,   801,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   805,     0,     0,     0,     0,   806,     0,
     807,   808,   809,   810,   811,   812,   813,   814,   815,   816,
     784,   785,     0,     0,     0,     0,     0,     0,     0,   817,
     818,     0,     0,  1663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,     0,
     807,   808,   809,   810,   811,   812,   813,   814,   815,   816,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   817,
     818,     0,     0,  1685,     0,     0,     0,     0,     0,     0,
     786,   787,   788,   789,   790,     0,     0,   791,   792,   793,
     794,     0,   795,   796,   797,   798,     0,     0,     0,     0,
     799,     0,   800,   801,     0,     0,     0,     0,   802,   803,
     804,     0,     0,     0,   805,   786,   787,   788,   789,   790,
       0,     0,   791,   792,   793,   794,     0,   795,   796,   797,
     798,   784,   785,     0,     0,   799,     0,   800,   801,     0,
       0,     0,     0,   802,   803,   804,     0,     0,     0,   805,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,   784,   785,     0,     0,     0,     0,     0,     0,     0,
     817,   818,     0,     0,  1690,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,     0,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   817,   818,   877,     0,     0,
       0,     0,     0,     0,     0,     0,   786,   787,   788,   789,
     790,     0,     0,   791,   792,   793,   794,     0,   795,   796,
     797,   798,     0,     0,     0,     0,   799,     0,   800,   801,
       0,     0,     0,     0,   802,   803,   804,     0,     0,     0,
     805,     0,     0,     0,     0,     0,   786,   787,   788,   789,
     790,     0,     0,   791,   792,   793,   794,     0,   795,   796,
     797,   798,   784,   785,     0,     0,   799,     0,   800,   801,
       0,     0,     0,     0,   802,   803,   804,     0,     0,     0,
     805,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,   784,   785,     0,
       0,     0,     0,     0,     0,     0,   817,   818,  1153,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,   818,  1308,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,   788,
     789,   790,     0,     0,   791,   792,   793,   794,     0,   795,
     796,   797,   798,     0,     0,     0,     0,   799,     0,   800,
     801,     0,     0,     0,     0,   802,   803,   804,     0,     0,
       0,   805,   786,   787,   788,   789,   790,     0,     0,   791,
     792,   793,   794,     0,   795,   796,   797,   798,   784,   785,
       0,     0,   799,     0,   800,   801,     0,     0,     0,     0,
     802,   803,   804,     0,     0,     0,   805,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,   249,   250,     0,     0,     0,     0,   817,   818,  1324,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,  1471,     0,     0,     0,     0,     0,
       0,     0,     0,   786,   787,   788,   789,   790,     0,     0,
     791,   792,   793,   794,     0,   795,   796,   797,   798,     0,
       0,     0,     0,   799,     0,   800,   801,     0,     0,     0,
       0,   802,   803,   804,     0,     0,     0,   805,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   784,   785,   270,   271,
     272,     0,     0,     0,     0,     0,     0,   273,   274,   275,
     276,   277,     0,     0,   278,   279,   280,   281,   282,   283,
     284,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   784,   785,     0,     0,     0,     0,
       0,     0,     0,   817,   818,  1477,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   285,     0,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,     0,     0,   296,
     297,     0,     0,     0,     0,     0,   298,   299,     0,     0,
       0,   786,   787,   788,   789,   790,     0,     0,   791,   792,
     793,   794,     0,   795,   796,   797,   798,     0,     0,     0,
       0,   799,     0,   800,   801,     0,     0,     0,     0,   802,
     803,   804,     0,     0,     0,   805,     0,     0,     0,   786,
     787,   788,   789,   790,     0,     0,   791,   792,   793,   794,
      13,   795,   796,   797,   798,   784,   785,     0,     0,   799,
       0,   800,   801,     0,     0,     0,     0,   802,   803,   804,
      14,     0,     0,   805,     0,     0,     0,     0,     0,     0,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,   784,   785,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,  1313,
     807,   808,   809,   810,   811,   812,   813,   814,   815,   816,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   817,
     818,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,   788,   789,   790,     0,     0,   791,   792,   793,
     794,     0,   795,   796,   797,   798,     0,     0,     0,     0,
     799,     0,   800,   801,     0,     0,     0,     0,   802,   803,
     804,     0,     0,     0,   805,     0,     0,   786,   787,   788,
     789,   790,     0,     0,   791,   792,   793,   794,     0,   795,
     796,   797,   798,   784,   785,     0,     0,   799,     0,   800,
     801,     0,     0,     0,     0,   802,   803,   804,     0,     0,
       0,  -812,     0,     0,     0,     0,     0,     0,     0,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,   784,   785,     0,     0,     0,     0,     0,     0,     0,
     817,   818,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,   787,
     788,   789,   790,     0,     0,   791,   792,   793,   794,     0,
     795,   796,   797,   798,     0,     0,     0,     0,   799,     0,
     800,   801,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,     0,     0,     0,     0,   786,   787,   788,   789,
     790,     0,     0,   791,   792,   793,   794,     0,   795,   796,
     797,   798,   784,   785,     0,     0,   799,     0,   800,   801,
       0,     0,     0,     0,   802,     0,   804,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   784,
     785,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,   818,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,   788,
     789,   790,     0,     0,   791,   792,   793,   794,     0,   795,
     796,   797,   798,     0,     0,     0,     0,   799,     0,   800,
     801,     0,     0,     0,     0,   802,     0,     0,     0,     0,
       0,     0,     0,     0,   786,   787,   788,   789,   790,     0,
       0,   791,   792,   793,   794,     0,   795,   796,   797,   798,
     784,   785,     0,     0,   799,     0,   800,   801,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   809,   810,   811,
     812,   813,   814,   815,   816,  1030,     0,     0,     0,     0,
       0,     0,     0,     0,   817,   818,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,   788,   789,   790,
       0,     0,   791,   792,   793,   794,     0,   795,   796,   797,
     798,     0,     0,     0,     0,   799,     0,   800,   801,     0,
       0,     0,     0,     0,     0,     0,     0,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1034,     0,     0,     0,     0,   810,
     811,   812,   813,   814,   815,   816,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   817,   818,     0,     0,     0,
       0,     0,     0,     0,     0,  1309,     0,     0,     0,     0,
       0,     0,     0,     0,  1031,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   335,   336,   337,  1032,
     339,   340,   341,   342,   343,   502,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,     0,   357,   358,
     359,     0,     0,   362,   363,   364,   365,   335,   336,   337,
       0,   339,   340,   341,   342,   343,   502,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,     0,   357,
     358,   359,     0,     0,   362,   363,   364,   365,   335,   336,
     337,     0,   339,   340,   341,   342,   343,   502,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,     0,
     357,   358,   359,  1035,   197,   362,   363,   364,   365,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1036,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1310,     0,  1070,  1071,     0,     0,
     198,     0,   199,     0,   200,   201,   202,   203,   204,  1311,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,     0,   216,   217,   218,  1072,     0,   219,   220,   221,
     222,   917,   918,   919,   920,   921,   922,   923,   924,     0,
    1073,     0,     0,     0,   925,   926,     0,   223,   224,     0,
     927,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     830,     0,     0,   928,   929,     0,     0,     0,     0,     0,
     930,   931,   932,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1074,  1075,     0,     0,     0,    13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   225,     0,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,     0,     0,   933,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   724
};

static const yytype_int16 yycheck[] =
{
       1,   612,   151,     7,   666,   243,   635,   233,   227,   695,
     445,   399,   399,   521,   444,    15,    16,   561,   737,    20,
     739,   759,   739,   626,   413,   628,   173,   630,   578,   864,
     768,   866,   975,   868,   959,   407,  1273,  1237,   981,   481,
     673,   952,   904,   510,   511,    86,   954,    22,    33,  1357,
    1450,     8,   602,   442,   773,   109,    20,     4,    53,     5,
      15,    16,  1635,   166,   506,   143,     0,    20,    68,    69,
      70,    20,   127,     7,   160,    20,    20,    20,   152,  1665,
     444,   445,    57,    40,    20,   144,   145,   146,    20,   315,
      21,    22,   173,   481,    34,   173,    30,    98,    32,   173,
      34,    20,    20,   736,     7,   738,    40,   740,    19,    20,
     110,   111,   112,   113,    20,   163,    50,    46,   506,   155,
      62,   754,    56,    63,   160,   129,    33,  1700,   188,   189,
     519,   520,   765,  1719,   127,   183,   222,   875,    12,   194,
     221,   219,   197,   194,   137,   219,    80,    50,   196,    23,
      24,    15,    16,   143,  1016,   127,   102,   103,   218,   397,
     200,   220,  1562,   158,    33,   137,   170,   107,   102,   103,
      12,   143,   319,   399,   616,   913,   726,   106,   173,   219,
     234,    23,    24,   173,   220,   904,   222,   118,   119,   193,
     194,    60,    61,   132,   235,   126,   136,   128,   129,   130,
     131,   194,  1624,   143,  1626,   136,  1406,   164,    53,    33,
    1632,   166,   950,   160,   199,   953,   171,   443,   173,    47,
     243,   176,   194,   227,   193,  1647,   165,   184,   616,   193,
     205,   165,   217,   173,  1542,   236,    60,    61,   223,    67,
     193,   216,   246,   243,   193,   687,   185,    57,   193,   193,
     193,   185,   401,    63,   200,   124,   193,   193,   127,   128,
     217,   193,   173,   193,   219,   484,   143,  1178,   137,   902,
     210,  1179,   206,   219,   193,   193,  1698,   719,   173,   219,
     499,   322,  1020,   217,   127,   216,   217,   193,   218,   653,
    1233,   165,   199,  1218,   658,    57,   165,  1016,  1017,   687,
     124,    63,   166,   155,   128,   694,  1168,   171,   160,   173,
     314,   196,   176,   317,   173,   782,   185,  1554,   152,   196,
     165,  1743,  1744,   165,   126,   194,   195,   129,   130,   718,
     199,   719,  1532,   202,  1522,     5,     6,   143,     8,   173,
     185,   163,    21,    22,   129,   130,   481,   219,   217,   127,
     193,   194,    53,   196,   223,  1280,   199,  1779,   165,   137,
     219,   183,   995,   996,   143,   998,    36,   158,   193,   369,
     222,   195,   598,   152,   397,   199,   602,   201,   202,   617,
     618,   619,   173,   621,   622,   219,   193,   625,   127,   627,
     413,   629,   166,   217,   173,   220,    97,   397,   137,   223,
    1637,   127,   164,   403,  1592,  1593,   541,   165,   780,   851,
     173,   137,  1337,   413,   216,   217,   194,   127,   163,   442,
     152,   444,   184,  1611,  1612,  1028,   216,   137,   160,   220,
     656,   216,   217,   173,   660,   193,   194,   107,   183,   118,
     119,   173,   442,   152,   444,   193,   127,   126,   193,  1168,
     129,   130,   131,   672,   603,   194,   137,   136,   847,   193,
     158,   185,  1405,   851,   173,  1015,  1301,   152,   194,   193,
    1671,   193,   220,   173,  1417,   173,   193,   477,   478,   987,
     484,   700,  1683,   163,   194,   193,   220,   219,   173,  1677,
    1678,   163,   173,   857,   495,   499,   519,   520,   220,  1161,
     967,   218,   165,   183,  1012,   509,   510,   511,   872,   898,
     899,   183,   220,   194,  1233,   165,  1233,  1236,   199,   519,
     520,   219,   185,   912,   193,   143,   745,  1728,  1729,  1248,
     219,  1248,   221,   222,   152,   185,   142,   216,   217,   157,
     766,   221,   222,   193,   763,   546,   547,   219,    33,   218,
     165,   686,   687,   942,   943,   173,   945,   163,   193,   222,
     949,   193,   951,   564,  1399,   566,   567,   568,   165,   570,
     185,   935,   173,   188,   200,    60,    61,   183,   713,   173,
     581,   716,   200,   193,   193,   220,   218,   165,   185,   173,
      68,    69,    70,   219,   617,   618,   619,   193,   621,   622,
     197,  1436,   625,   218,   627,   177,   629,   185,   631,   218,
     220,   612,  1079,  1080,  1081,   193,  1219,   617,   618,   619,
     165,   621,   622,   201,   220,   625,   165,   627,   165,   629,
    1263,   631,   110,   111,   112,   113,   874,   173,   773,   124,
     185,   165,   127,   128,   165,   883,   185,   165,   886,   194,
      57,   193,   137,   165,   193,   173,   165,   895,   193,   210,
     193,   185,   201,   193,   185,   903,  1269,   185,   672,   193,
     908,   694,   193,   185,   200,   193,   185,   201,   220,   679,
     201,   193,    21,    22,   193,   220,   165,   220,   165,   201,
     220,   840,   201,   219,   694,   718,   700,   185,   845,  1385,
     200,   165,   177,   838,   193,   193,   185,   165,   185,   194,
     195,   158,   193,  1148,   199,   934,  1146,   202,   718,   219,
     165,   185,   201,  1558,   201,   193,   173,   185,  1447,   193,
     165,   220,   217,     5,     6,   193,   193,   201,   223,   220,
     185,   745,   743,   201,   173,   174,   175,  1409,   193,   193,
     185,   989,   220,   991,  1473,   193,   201,   144,   193,   763,
     760,   784,   785,   220,   165,   769,   201,   185,   196,   904,
     193,   106,   200,   165,   218,   193,   799,   206,   782,   118,
     119,   193,   220,   184,   185,  1149,   187,   126,  1177,   128,
     129,   130,   131,   185,   817,   218,    75,   136,   165,   194,
      79,   193,   196,   196,   200,   197,   218,   201,   201,    66,
     165,   196,   196,  1202,    93,    94,   201,   201,   185,    98,
      99,   100,   101,   219,   847,   219,   219,    57,    79,   184,
     185,   186,   196,    63,   219,   219,   165,   972,     5,     6,
     158,  1444,   842,    94,   158,   980,    35,   847,    99,   185,
     101,   874,   853,  1217,  1398,   173,   185,   193,    25,   173,
     883,   164,   164,   886,    31,   204,   205,   206,   164,  1531,
     173,   173,   895,    35,   874,   898,   899,   216,   217,  1268,
     903,   184,   184,   883,   196,   908,   886,   199,   184,   912,
     158,  1279,  1279,  1257,  1258,   895,  1134,   219,   898,   899,
     158,    68,    69,   903,   193,   173,   173,   196,   908,   185,
     199,   196,   912,    12,   199,   173,    22,   193,  1604,   942,
     943,    57,   945,   185,    23,    24,   949,    63,   951,   952,
     934,   193,   185,    57,   197,   102,   103,   173,   201,    63,
     193,    57,   942,   943,  1682,   945,   173,    63,    57,   949,
      57,   951,   952,  1564,    63,   184,    63,   144,   187,   146,
    1679,   190,   989,   967,   991,  1684,   989,  1205,   991,   221,
     222,   197,   173,  1602,   975,   201,   143,   978,   197,   197,
     981,   982,   201,   201,  1722,   218,   197,   173,   197,   989,
     201,   991,   201,   160,   161,   162,   163,   218,   197,   197,
     197,  1720,   201,   201,   201,   197,   173,   197,   196,   201,
    1374,   201,   177,   178,   179,   180,   183,   196,  1382,   108,
     109,   110,   111,   112,   113,   114,   115,    43,  1690,   200,
    1165,   200,  1540,   200,   219,  1058,   144,   145,   146,   200,
    1651,  1652,   173,   174,   175,   177,   178,   179,   137,  1557,
     217,  1270,   200,  1279,    21,    22,   177,   178,   147,   148,
     149,   219,   177,   178,   179,   463,   464,   465,  1787,    92,
      93,  1460,    10,    11,    12,  1079,  1080,  1081,   200,   219,
     200,   200,   200,   200,   219,   197,   200,   509,   510,   511,
     173,  1226,    10,   219,    33,   185,   173,  1708,   173,   173,
     200,   173,   200,   200,   200,   200,   217,   220,   218,   200,
     166,  1134,  1247,   200,   219,   219,   219,   219,  1253,   200,
    1358,    60,    61,  1146,  1125,   219,   219,  1262,  1129,   200,
     200,   219,   200,   219,  1134,   219,    13,  1138,    10,   561,
     222,    37,  1277,    66,   217,   200,  1146,   200,  1367,    43,
     219,   118,   119,    33,  1177,  1178,   219,   218,   220,   126,
     173,   128,   129,   130,   131,   196,   200,   589,   219,   136,
      43,  1306,   219,  1174,  1563,   219,   399,  1177,  1178,  1202,
      60,    61,  1205,  1318,    33,   124,  1694,  1336,  1323,   128,
     108,   109,   110,   111,   112,   113,   114,   115,   200,   200,
     193,   132,  1202,   121,   122,  1205,    14,   173,   194,   127,
     166,    60,    61,   196,   222,   142,   219,    13,   218,   137,
    1439,   193,   140,   141,  1462,   193,   165,   173,     8,   147,
     148,   149,   173,  1368,   193,   202,   203,   204,   205,   206,
    1459,   201,   193,  1481,   124,  1268,   185,   173,   128,   216,
     217,   173,   173,   173,   193,   220,   195,   194,   481,   142,
     199,  1650,   201,   202,   219,  1400,  1270,   184,  1268,   193,
     692,   220,   219,   173,   193,   124,   194,   219,   217,   128,
     193,     1,   173,   506,   223,   220,   219,   219,   200,   200,
     219,   219,    67,  1428,  1429,   201,   201,  1432,   201,   721,
      43,    12,   173,   221,   222,   219,   173,   196,  1527,   219,
     219,   219,   173,   220,   173,   195,   220,   218,   220,   199,
     219,   201,   202,   220,   200,   220,   173,   220,   220,  1330,
     220,   219,   218,  1482,   218,  1358,   194,   217,   220,   173,
    1578,   219,   173,   223,   219,   219,   195,   173,   181,   219,
     199,   219,   201,   202,   220,   219,   219,   173,  1358,   173,
     782,   218,    33,  1367,   399,   220,   218,   218,   217,    33,
     173,   219,   201,   219,   223,  1594,   219,   219,    70,   220,
     219,    33,   219,   219,   219,   177,  1387,  1388,   201,    60,
      61,   201,    33,   616,   185,   220,   220,   220,   219,   219,
     219,   219,   185,   220,  1405,   220,    53,   218,    60,    61,
     184,   220,   185,  1548,   220,   193,  1417,   218,   220,    60,
      61,   220,   220,   220,   184,   220,   220,   220,   747,  1648,
     220,   853,   218,   218,   218,  1439,   471,  1460,   219,  1462,
    1441,   220,   218,   220,     1,    85,   481,   218,    46,    88,
     141,   486,   241,   124,   489,  1459,  1555,   128,  1481,   842,
    1460,  1014,  1462,   498,   687,  1555,  1692,  1555,  1555,     1,
    1555,   506,   124,  1608,  1023,  1527,   128,  1457,  1293,  1705,
    1590,  1481,  1484,   124,  1487,  1333,  1591,   128,    56,  1591,
     477,   344,  1154,   477,  1684,   530,   719,   739,  1245,  1538,
      -1,    -1,   400,    -1,  1639,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   548,    -1,  1664,  1742,    -1,    -1,    -1,
      -1,    -1,    -1,  1527,   195,    -1,    -1,    -1,   199,    -1,
     201,   202,    -1,    -1,    -1,   957,    -1,    -1,  1757,    -1,
    1563,    -1,    -1,   195,    -1,   967,   217,   199,    -1,   201,
     202,    -1,   223,    -1,   195,  1578,    -1,    -1,   199,    -1,
     201,   202,    -1,  1563,   599,   217,    -1,  1702,    -1,    -1,
      -1,   223,    -1,    -1,    -1,    -1,   217,    -1,  1578,    -1,
      -1,   616,   223,    -1,    -1,  1586,    -1,    -1,    -1,    -1,
    1594,    -1,    -1,    -1,    -1,    -1,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,  1756,   831,   832,
     833,   834,   835,   836,    -1,    -1,  1617,    -1,    -1,    -1,
      -1,    -1,    -1,  1624,    -1,  1626,    -1,  1650,   851,    -1,
      -1,  1632,    -1,    -1,    -1,  1635,  1785,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1648,    -1,  1647,    -1,    -1,    -1,
    1650,    -1,   687,    -1,    -1,    -1,    -1,  1079,  1080,  1081,
      -1,   696,  1084,    -1,  1086,    -1,  1088,    -1,  1090,    -1,
    1092,    -1,  1094,    -1,  1096,    -1,  1098,    -1,  1100,    -1,
    1102,    33,  1104,    -1,   719,  1107,    -1,  1109,    -1,  1111,
      -1,  1113,    -1,  1115,    -1,  1117,    -1,  1698,    -1,    -1,
    1700,    -1,   737,    -1,   739,    -1,   410,  1129,    60,    61,
    1711,    -1,    -1,    -1,    -1,   419,    -1,  1718,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   429,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   439,    -1,    -1,    -1,    -1,
     121,   122,  1743,  1744,    -1,    -1,   127,    -1,    -1,    -1,
      -1,  1752,  1753,  1757,    -1,   459,   137,    -1,    -1,   140,
     141,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,    -1,
      -1,    -1,   124,    -1,    -1,    -1,   128,   481,  1779,    -1,
    1781,    -1,    -1,    -1,   819,   820,   821,   822,   823,   824,
     825,   826,   827,   828,   829,   830,   831,   832,   833,   834,
     835,   836,    -1,   507,   508,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,   194,    -1,    -1,   851,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   535,   536,   537,   538,   539,   540,   541,    -1,    -1,
     221,   222,    -1,   195,    -1,    -1,   881,   199,    -1,   201,
     202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   897,    -1,    -1,   217,    -1,    -1,    -1,    -1,
     905,   223,   907,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   917,   918,   919,   920,   921,   922,   923,   924,
     925,   926,   927,   928,   929,   930,   931,   932,   933,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   635,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   645,    21,    22,    -1,    -1,    -1,    -1,    -1,   984,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   993,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1009,    -1,  1398,    -1,    -1,    -1,
      -1,    -1,   686,   687,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   698,    -1,    -1,    -1,    -1,    -1,
     704,    -1,    -1,   216,   217,    -1,   710,    -1,    -1,   713,
      -1,    -1,   716,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   730,   731,   732,   733,
     734,   735,    -1,    -1,    10,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,    21,    22,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,  1279,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   773,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   786,   787,    -1,    -1,   790,   791,   792,   793,
      33,   795,    -1,   797,   798,   799,   800,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,    -1,   818,    -1,    -1,    60,    61,    -1,
      -1,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,   838,    -1,    -1,   216,   217,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,  1206,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,   128,   890,    -1,    -1,   165,
      -1,    -1,    -1,    -1,  1229,  1230,    -1,    21,    22,  1234,
     904,    -1,    -1,    -1,    -1,    -1,    33,  1242,    -1,   185,
    1245,    -1,   916,    -1,    -1,    -1,    -1,   193,    -1,   195,
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,  1274,
     216,   217,    -1,    -1,  1279,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    -1,    -1,    -1,   199,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,   970,   971,   972,    -1,
      -1,    -1,    13,    -1,   217,    -1,   980,    -1,    19,    -1,
     223,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,   116,   117,   118,   119,  1000,   124,    -1,    -1,
      41,   128,   126,    -1,   128,   129,   130,   131,    49,    21,
      22,    -1,   136,  1017,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,  1364,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,  1071,   195,    -1,
      -1,  1075,   199,    -1,   201,   202,    -1,    -1,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,   216,   217,    -1,    -1,   223,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,   156,   128,   129,   130,   131,
      33,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
      -1,   820,   821,   822,   823,   824,   825,   826,   827,   828,
     829,  1165,   831,   832,   833,   834,   835,   836,    -1,    33,
    1505,    -1,  1507,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     221,    33,    -1,  1518,  1519,  1520,   198,   199,   200,   201,
     202,   203,   204,   205,   206,    -1,    60,    61,    -1,    -1,
    1535,  1536,    -1,    -1,   216,   217,  1541,    -1,    60,    61,
      -1,   124,    -1,    -1,    -1,   128,    -1,    33,    -1,    -1,
    1555,    -1,  1226,    -1,  1228,    -1,    -1,  1231,    -1,    -1,
      -1,    -1,    -1,    -1,  1569,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1247,    60,    61,    -1,    -1,    -1,  1253,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1262,    -1,
     124,    -1,  1266,  1267,   128,    33,    -1,    -1,    -1,    -1,
      -1,    -1,   124,  1277,    -1,    -1,   128,    -1,    -1,    -1,
      -1,  1285,   195,  1618,  1619,    -1,   199,    -1,   201,   202,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,  1303,
      -1,  1305,  1306,  1307,   217,  1640,    -1,    -1,   124,  1313,
     223,    -1,   128,  1317,  1318,    -1,    -1,    -1,  1653,  1323,
    1655,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,    -1,   199,    -1,   201,   202,  1674,
      -1,  1676,    -1,   195,    -1,    -1,    -1,   199,    -1,   201,
     202,    -1,    -1,   217,    -1,    -1,   124,    -1,    -1,   223,
     128,    -1,    -1,    -1,  1368,   217,    33,    -1,  1703,    -1,
      -1,   223,    -1,    -1,  1709,  1710,    -1,    -1,    -1,   195,
      -1,    -1,    -1,   199,    -1,   201,   202,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    -1,  1400,    -1,    -1,    -1,
      -1,   217,  1737,    -1,  1739,  1740,    -1,   223,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1751,    13,    -1,    -1,
      -1,    -1,    -1,    19,  1428,  1429,    -1,   195,  1432,    25,
      -1,   199,    -1,   201,   202,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1447,    -1,    41,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    49,    -1,   223,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,  1468,    -1,  1470,    -1,    64,  1473,
      -1,    -1,  1476,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1525,    -1,    -1,    -1,    -1,  1530,    -1,   195,    -1,
    1534,    -1,   199,  1537,    -1,   202,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1547,  1548,    -1,    -1,   143,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,    -1,   121,   122,    -1,    -1,  1602,    -1,
     127,    -1,    -1,    -1,  1608,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,   140,   141,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,    -1,    -1,   221,    -1,    -1,    -1,    -1,
    1634,    -1,     1,    -1,    -1,  1639,     5,     6,     7,    -1,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    -1,   194,    -1,    38,
      39,    40,    -1,    42,    -1,    44,    45,    -1,    -1,    48,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    -1,    58,
      59,  1695,    -1,    -1,   221,   222,    65,    -1,  1702,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,  1746,    -1,    -1,    -1,    -1,    -1,    -1,   118,
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
     174,   175,   176,    -1,    19,    -1,    -1,    -1,    -1,   183,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,   202,   203,
     204,    -1,   206,    -1,    49,   209,   210,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,   219,    -1,   221,   222,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   173,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   221,    -1,   223,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    19,    -1,
      -1,   173,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      41,   193,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   221,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,   173,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
     221,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    21,    22,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,   116,   117,
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
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,    -1,    -1,
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
     199,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,   216,   217,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,   195,    -1,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    21,    22,   128,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,    -1,   195,    -1,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,   209,
     210,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
     165,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
     185,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
     195,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,   200,   201,
     202,   203,   204,   205,   206,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   200,
     201,   202,   203,   204,   205,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,   173,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,   158,    35,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,   129,   130,    -1,    -1,
      71,    -1,    73,    -1,    75,    76,    77,    78,    79,   173,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,   158,    -1,    98,    99,   100,
     101,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
     173,    -1,    -1,    -1,   121,   122,    -1,   118,   119,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,   140,   141,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   221,   222
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   225,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   206,   217,   226,   227,   231,
     240,   242,   243,   248,   280,   284,   320,   404,   411,   415,
     422,   469,   474,   479,    19,    20,   173,   272,   273,   274,
     166,   249,   250,   173,   174,   175,   206,   244,   245,   246,
     163,   183,   289,   412,   173,   221,   229,    57,    63,   407,
     407,   407,   143,   173,   306,    34,    63,   107,   136,   210,
     219,   276,   277,   278,   279,   306,   226,     5,     6,     8,
      36,   107,   419,    62,   402,   194,   193,   196,   193,   245,
      22,    57,   205,   216,   247,   407,   408,   410,   408,   402,
     480,   470,   475,   173,   143,   241,   278,   278,   278,   219,
     144,   145,   146,   193,   218,    57,    63,   285,   287,    57,
      63,   413,     5,     6,    57,    63,   420,    57,    63,   403,
      15,    16,   166,   171,   173,   176,   219,   233,   273,   166,
     250,   173,   244,   244,   173,   165,   185,   377,   164,   184,
     290,   408,   226,    57,    63,   228,   173,   173,   173,   173,
     177,   239,   220,   274,   278,   278,   278,   278,   288,   173,
     414,   423,   289,   405,   177,   178,   179,   232,    15,    16,
     166,   171,   173,   233,   270,   271,   247,   409,   165,   377,
     210,   230,   481,   471,   476,   177,   220,    35,    71,    73,
      75,    76,    77,    78,    79,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    93,    94,    95,    98,
      99,   100,   101,   118,   119,   173,   283,   286,   196,   289,
     106,   417,   418,   400,   160,   222,   275,   371,   177,   178,
     179,   193,   220,   194,    66,   196,   433,   289,   289,    21,
      22,    38,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     128,   129,   130,   137,   138,   139,   140,   141,   144,   145,
     146,   147,   148,   149,   150,   195,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   209,   210,   216,   217,
      35,    35,   219,   281,   289,   291,    75,    79,    93,    94,
      98,    99,   100,   101,   427,   406,   173,   424,   290,   401,
     274,   273,   222,   226,   152,   173,   398,   399,   270,    19,
      25,    31,    41,    49,    64,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   156,   221,
     306,   426,   428,   429,   434,   440,   468,    79,    94,    99,
     101,   289,   472,   477,   173,   173,   173,   218,    22,   173,
     218,   155,   220,   371,   383,   384,   385,   196,   282,   294,
     289,   173,   290,   196,   416,   289,   421,   371,   218,   273,
     219,    43,   193,   196,   199,   397,   200,   200,   200,   219,
     200,   200,   219,   433,   200,   200,   200,   200,   200,   219,
     306,    33,    60,    61,   124,   128,   195,   199,   202,   217,
     223,   439,   197,   432,   388,   391,   173,   108,   109,   110,
     111,   112,   113,   114,   115,   137,   147,   148,   149,   219,
       7,    50,   319,   185,   185,   193,   220,   468,     1,     5,
       6,     9,    10,    11,    13,    15,    16,    17,    18,    19,
      25,    26,    27,    28,    29,    31,    38,    39,    40,    42,
      44,    45,    48,    51,    52,    54,    55,    58,    59,    65,
      68,    69,    80,   102,   103,   104,   105,   118,   119,   133,
     134,   135,   151,   152,   153,   154,   155,   157,   159,   160,
     161,   162,   165,   166,   167,   168,   169,   170,   171,   172,
     174,   175,   176,   183,   185,   202,   203,   204,   209,   210,
     217,   219,   221,   222,   238,   240,   251,   252,   255,   258,
     259,   261,   264,   265,   266,   267,   289,   290,   292,   293,
     295,   300,   305,   306,   307,   311,   312,   313,   314,   315,
     316,   317,   318,   320,   324,   325,   332,   335,   338,   343,
     346,   347,   349,   350,   351,   353,   358,   361,   362,   369,
     370,   426,   482,   497,   508,   512,   525,   528,   406,   194,
     377,   306,   378,   399,   218,    65,   104,   174,   300,   362,
     173,   173,   440,   127,   137,   194,   396,   441,   446,   448,
     362,   450,   444,   173,   200,   452,   454,   456,   458,   460,
     462,   464,   466,   362,   200,   219,   297,    33,   199,    33,
     199,   217,   223,   218,   362,   217,   223,   440,   173,   290,
     173,   193,   226,   386,   437,   468,   473,   173,   389,   437,
     478,   362,   152,   173,   393,   394,   425,   385,   385,   385,
     200,   200,   369,   262,   263,   200,   308,   428,   482,   219,
     306,   200,     5,   102,   103,   200,   219,   127,   305,   336,
     347,   362,   369,   291,   200,   219,    61,   369,   219,   269,
     369,   173,   200,   200,   219,   226,   200,   166,    58,   369,
     219,   291,   200,   219,   200,   200,   219,   200,   200,   127,
     305,   369,   362,   362,   222,   291,   338,   342,   342,   342,
     219,   219,   219,   219,   219,   219,    13,   440,    13,   440,
      13,   369,   507,   523,   200,   369,   200,   237,    13,   362,
     362,   362,   362,   362,    13,    49,   296,   336,   362,   336,
     222,   226,   226,   369,    10,    13,   298,   507,   524,    37,
     338,   344,   173,   219,   226,   226,   226,   226,   226,    66,
     321,   280,   132,   226,    21,    22,   116,   117,   118,   119,
     120,   123,   124,   125,   126,   128,   129,   130,   131,   136,
     138,   139,   144,   145,   146,   150,   195,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   216,   217,   108,
     109,   110,   111,   112,   113,   114,   115,   121,   122,   127,
     137,   140,   141,   147,   148,   149,   194,   344,   219,   290,
     369,    40,   217,   280,   290,   380,   377,   200,   220,    43,
     226,   396,   305,   369,   468,   468,   438,   468,   220,   468,
     468,   220,   435,   468,   281,   468,   281,   468,   281,   386,
     387,   389,   390,   220,   443,   296,   218,   218,   362,   165,
     193,   194,   431,   196,   437,   290,   196,   437,   290,   220,
     219,    43,   127,   193,   194,   196,   199,   392,   498,   500,
     291,   425,   219,   309,   219,   306,   200,   219,   333,   200,
     200,   200,   519,   336,   305,   336,   193,   108,   109,   110,
     111,   112,   113,   114,   115,   121,   122,   127,   140,   141,
     147,   148,   149,   194,    14,   440,   298,   369,   362,   173,
     291,   194,   326,   328,   362,   330,   196,   166,   362,   521,
     336,   504,   509,   336,   502,   440,   305,   369,   222,   280,
     362,   362,   362,   362,   362,   362,   425,    53,   158,   173,
     202,   217,   219,   369,   483,   486,   490,   506,   511,   425,
     219,   486,   511,   425,   142,   184,   186,   226,   491,   301,
     291,   303,   179,   180,   232,   219,   219,   425,    13,   218,
     193,   527,   527,   152,   157,   200,   306,   352,   291,   260,
     425,   290,   193,   527,   289,   345,    70,   217,   220,   336,
     483,   485,   160,   219,   323,   399,     4,   160,   341,   342,
      19,   158,   173,   426,    19,   158,   173,   426,   362,   362,
     362,   362,   362,   362,   173,   362,   158,   173,   362,   362,
     362,   426,   362,   362,   362,   362,   362,   362,    22,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     129,   130,   158,   173,   216,   217,   359,   426,   362,   133,
     134,   135,   292,   348,   369,   348,   369,   348,   369,   348,
     369,   348,   369,   348,   369,   348,   369,   348,   369,   348,
     369,   348,   369,   348,   369,   369,   348,   369,   348,   369,
     348,   369,   348,   369,   348,   369,   348,   369,   220,   336,
     377,   379,   279,     8,   371,   376,   440,   173,   305,   369,
     226,   201,   201,   201,   437,   201,   201,   173,   430,   201,
     282,   201,   282,   201,   282,   201,   437,   201,   437,   299,
     468,   193,   527,   218,   369,   193,   468,   468,   362,   173,
     173,   468,   369,   440,   440,    20,   425,   468,    70,   336,
     485,   496,   200,   369,   173,   369,   468,   513,   515,   517,
     440,   527,   362,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     291,   201,   437,   220,   220,   194,   268,   440,   440,   220,
     440,   220,   440,   527,   440,   387,   527,   390,   201,   341,
     220,   220,   220,   220,   220,   220,    20,   342,   219,   137,
     392,   217,   362,   220,   142,   193,   226,   490,   188,   189,
     218,   494,   193,   188,   218,   226,   493,    20,   220,   490,
     184,   187,   492,    20,   369,   184,   507,   299,   299,   369,
     425,   425,    20,   219,   425,   220,   219,   219,   354,   356,
      12,    23,    24,   165,   253,   254,   369,    20,   507,   294,
     280,   173,   220,   485,   483,   193,   220,   193,   527,   220,
     173,   322,   322,   219,   127,   137,   173,   194,   199,   339,
     340,   281,   200,   219,   200,   219,   219,   219,   218,    19,
     158,   173,   426,   196,   158,   173,   362,   219,   219,   158,
     173,   362,     1,   219,   218,   342,   342,   342,   220,   173,
     218,    57,    63,   374,    67,   375,   226,   201,   226,   442,
     447,   449,   468,   451,   445,   201,   226,   453,   201,   457,
     201,   461,   201,   465,   386,   467,   389,   201,   437,   220,
     165,   431,   220,    43,   392,   201,   201,   336,    20,   201,
     485,   220,   220,   220,   173,   220,   201,   226,   220,   201,
     440,   387,   390,   201,   220,   219,   440,   468,   369,   201,
     201,   201,   201,   220,   201,   201,   220,   201,   341,   281,
     219,   336,   362,   369,   369,   486,   490,   369,   158,   173,
     483,   494,   218,   369,   218,   506,   336,   486,   184,   187,
     190,   495,   218,   336,   201,   201,   196,   235,    20,    20,
     336,   425,    20,   362,   362,   440,   281,   291,   254,   369,
      12,   256,   336,   290,   341,   220,   218,   217,   193,   218,
     220,   340,   173,   173,   219,   173,   173,   193,   218,   282,
     363,   362,   365,   362,   220,   336,   362,   200,   219,   362,
     219,   218,   362,   217,   220,   336,   219,   218,   360,   220,
     336,   194,   226,    47,   375,    46,   106,   372,   377,   341,
     436,   173,   455,   459,   463,   297,   468,   173,   369,   499,
     501,   291,   336,   310,   220,   201,   437,   219,   173,   334,
     201,   201,   201,   520,   298,   201,   226,   226,   327,   329,
     331,   522,   505,   510,   503,   219,   344,   282,   220,   336,
     185,   220,   490,   494,   219,   137,   392,   185,   490,   218,
     185,   302,   304,   236,   181,   336,   336,   185,    20,   336,
     220,   220,   201,   282,   291,   257,   226,   185,   281,   220,
     483,   173,   218,   196,   397,   220,   173,   339,   218,   142,
     291,   337,   440,   220,   468,   220,   220,   220,   367,   362,
     362,   220,   483,   220,   362,   220,   468,   377,    33,   373,
     372,   374,   219,   219,   220,   369,   173,   369,   201,   514,
     516,   518,   219,   220,   219,   369,   369,   369,   219,    70,
     496,   219,   219,   220,   362,   337,   220,   362,   137,   392,
     494,   362,   369,   369,   362,   495,   507,   369,   297,   234,
     220,   220,   362,   336,   185,   355,   201,   165,   254,    26,
     105,   258,   312,   313,   314,   316,   369,   507,   282,   218,
     196,   397,   440,   396,   220,   127,   369,   201,   201,   468,
     220,   220,   218,   220,   226,   381,   373,   393,   394,   395,
     496,   496,   291,   220,   201,   220,   219,   219,   219,   219,
     296,   298,   336,   496,   496,   220,   226,   526,   369,   369,
     220,   526,   526,   177,   185,   185,   526,   220,   362,   352,
     357,   254,   127,   127,   369,   526,   291,   220,   440,   396,
     396,   369,   369,   364,   366,   201,   220,   377,   286,   382,
     219,   483,   487,   488,   489,   489,   369,   369,   496,   496,
     483,   484,   220,   220,   527,   489,   484,    53,   218,   137,
     392,   184,   290,   507,   362,   218,   185,   526,   352,   369,
     290,   396,   369,   369,   226,   368,   226,   286,   483,   193,
     527,   220,   220,   220,   220,   489,   489,   220,   220,   220,
     220,   369,   218,   369,   369,   218,   290,   526,   526,   362,
     218,   369,   226,   226,   377,   291,   220,   219,   220,   220,
     184,   218,   526,   226,   377,   483,   218,   220
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
     263,   261,   264,   265,   266,   268,   267,   269,   267,   270,
     270,   270,   270,   270,   270,   271,   271,   272,   272,   272,
     273,   273,   273,   273,   273,   273,   273,   273,   274,   274,
     275,   275,   275,   276,   276,   276,   276,   277,   277,   278,
     278,   278,   278,   278,   278,   278,   279,   279,   280,   280,
     281,   281,   281,   282,   282,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   284,   285,   285,   285,   286,   288,   287,   289,   289,
     290,   290,   291,   291,   292,   292,   292,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   294,
     294,   294,   295,   296,   296,   297,   297,   298,   298,   299,
     299,   301,   302,   300,   303,   304,   300,   305,   305,   305,
     305,   305,   306,   306,   306,   307,   307,   309,   310,   308,
     308,   311,   311,   311,   311,   311,   311,   312,   313,   314,
     314,   314,   315,   315,   315,   316,   316,   317,   317,   317,
     318,   319,   319,   319,   320,   320,   321,   321,   322,   322,
     323,   323,   323,   323,   324,   324,   326,   327,   325,   328,
     329,   325,   330,   331,   325,   333,   334,   332,   335,   335,
     335,   335,   335,   335,   336,   336,   337,   337,   337,   338,
     338,   338,   339,   339,   339,   339,   339,   340,   340,   341,
     341,   341,   342,   342,   343,   345,   344,   346,   346,   346,
     346,   346,   346,   346,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   348,   348,   348,   348,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   350,   350,   351,   351,   352,   352,
     353,   354,   355,   353,   356,   357,   353,   358,   358,   358,
     358,   358,   358,   358,   359,   360,   358,   361,   361,   361,
     361,   361,   361,   361,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     363,   364,   362,   362,   362,   362,   365,   366,   362,   362,
     362,   367,   368,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   369,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   371,   371,   371,   372,   372,
     372,   373,   373,   374,   374,   374,   375,   375,   376,   377,
     377,   377,   377,   378,   379,   378,   380,   378,   381,   378,
     382,   378,   378,   383,   384,   384,   385,   385,   385,   385,
     385,   386,   386,   387,   387,   388,   388,   388,   389,   390,
     390,   391,   391,   391,   392,   392,   393,   393,   393,   394,
     394,   395,   395,   396,   396,   396,   397,   397,   398,   398,
     398,   398,   398,   399,   399,   399,   399,   399,   399,   400,
     401,   400,   402,   402,   403,   403,   403,   404,   405,   404,
     406,   406,   406,   407,   407,   407,   409,   408,   410,   410,
     411,   412,   411,   413,   413,   413,   414,   415,   415,   416,
     416,   417,   417,   418,   419,   419,   419,   419,   420,   420,
     420,   421,   421,   423,   424,   422,   425,   425,   425,   425,
     425,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   427,   427,
     427,   427,   427,   427,   427,   427,   428,   429,   429,   429,
     430,   430,   431,   431,   432,   432,   432,   432,   432,   432,
     432,   433,   433,   433,   433,   433,   435,   436,   434,   437,
     437,   438,   438,   439,   439,   440,   440,   440,   440,   440,
     440,   441,   442,   440,   440,   440,   443,   440,   440,   440,
     440,   440,   440,   440,   440,   440,   440,   440,   440,   440,
     444,   445,   440,   440,   446,   447,   440,   448,   449,   440,
     450,   451,   440,   440,   452,   453,   440,   454,   455,   440,
     440,   456,   457,   440,   458,   459,   440,   440,   460,   461,
     440,   462,   463,   440,   464,   465,   440,   466,   467,   440,
     468,   468,   468,   470,   471,   472,   473,   469,   475,   476,
     477,   478,   474,   480,   481,   479,   482,   482,   482,   482,
     482,   483,   483,   483,   483,   483,   483,   483,   483,   484,
     484,   485,   486,   486,   487,   487,   488,   488,   489,   489,
     490,   490,   491,   491,   492,   492,   493,   493,   494,   494,
     494,   495,   495,   495,   496,   496,   497,   497,   497,   497,
     497,   497,   498,   499,   497,   500,   501,   497,   502,   503,
     497,   504,   505,   497,   506,   506,   506,   507,   507,   508,
     509,   510,   508,   511,   511,   512,   512,   512,   513,   514,
     512,   515,   516,   512,   517,   518,   512,   512,   519,   520,
     512,   512,   521,   522,   512,   523,   523,   524,   524,   525,
     525,   525,   525,   525,   526,   526,   527,   527,   528,   528,
     528,   528,   528,   528,   528,   528,   528
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
       0,     8,     2,     3,     3,     0,     6,     0,     6,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     1,     5,     1,     3,
       2,     3,     2,     1,     1,     1,     1,     1,     4,     1,
       2,     3,     3,     3,     3,     2,     1,     3,     0,     3,
       0,     2,     3,     0,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     2,     2,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     3,     2,     2,     2,
       2,     2,     3,     3,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     1,     3,     0,     4,     1,     1,
       1,     1,     3,     7,     2,     2,     6,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     3,     0,     2,     0,     4,     0,     2,     1,
       3,     0,     0,     7,     0,     0,     7,     3,     2,     2,
       2,     1,     1,     3,     2,     2,     3,     0,     0,     5,
       1,     2,     5,     5,     5,     6,     2,     1,     1,     1,
       2,     3,     2,     2,     3,     2,     3,     2,     2,     3,
       4,     1,     1,     0,     1,     1,     1,     0,     1,     3,
       9,     8,     8,     7,     3,     3,     0,     0,     7,     0,
       0,     7,     0,     0,     7,     0,     0,     6,     5,     8,
      10,     5,     8,    10,     1,     3,     1,     2,     3,     1,
       1,     2,     2,     2,     2,     2,     4,     1,     3,     0,
       4,     4,     1,     6,     6,     0,     7,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     6,     8,     5,     6,     1,     4,
       3,     0,     0,     8,     0,     0,     9,     3,     4,     5,
       6,     8,     5,     6,     0,     0,     5,     3,     4,     4,
       5,     4,     3,     4,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     4,     5,     4,
       5,     3,     4,     2,     5,     1,     1,     1,     1,     1,
       1,     1,     4,     1,     1,     4,     4,     7,     8,     3,
       0,     0,     8,     3,     3,     3,     0,     0,     8,     3,
       4,     0,     0,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     2,     4,     1,     1,     4,
       4,     4,     4,     4,     1,     6,     7,     6,     6,     7,
       7,     6,     7,     6,     6,     0,     4,     1,     0,     1,
       1,     0,     1,     0,     1,     1,     0,     1,     5,     1,
       1,     2,     0,     0,     0,     8,     0,     5,     0,    10,
       0,    11,     6,     3,     3,     4,     1,     1,     3,     3,
       3,     1,     3,     1,     3,     0,     2,     3,     3,     1,
       3,     0,     2,     3,     1,     1,     1,     2,     3,     3,
       5,     1,     1,     1,     1,     1,     0,     1,     1,     4,
       3,     3,     5,     4,     6,     5,     5,     4,     4,     0,
       0,     5,     0,     1,     0,     1,     1,     6,     0,     6,
       0,     3,     5,     0,     1,     1,     0,     5,     2,     3,
       4,     0,     4,     0,     1,     1,     1,     7,     9,     0,
       2,     0,     1,     3,     1,     1,     2,     2,     0,     1,
       1,     0,     3,     0,     0,     7,     1,     4,     3,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     1,     2,     0,     3,     3,     2,     5,     5,
       4,     0,     2,     2,     2,     2,     0,     0,     7,     1,
       1,     1,     3,     3,     4,     1,     1,     1,     1,     2,
       3,     0,     0,     6,     4,     3,     0,     7,     4,     2,
       2,     3,     2,     3,     2,     2,     3,     3,     3,     2,
       0,     0,     6,     2,     0,     0,     6,     0,     0,     6,
       0,     0,     6,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     1,     0,     0,
       6,     0,     0,     7,     0,     0,     6,     0,     0,     6,
       1,     3,     3,     0,     0,     0,     0,    10,     0,     0,
       0,     0,    10,     0,     0,     9,     1,     1,     1,     1,
       1,     3,     3,     5,     5,     6,     6,     8,     8,     0,
       1,     2,     1,     3,     3,     5,     1,     2,     1,     0,
       0,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     2,     1,     1,     0,     1,     5,     4,     6,     7,
       5,     7,     0,     0,    10,     0,     0,    10,     0,     0,
      10,     0,     0,     7,     1,     3,     3,     3,     1,     5,
       0,     0,    10,     1,     3,     3,     4,     4,     0,     0,
      11,     0,     0,    11,     0,     0,    10,     5,     0,     0,
       9,     5,     0,     0,    10,     1,     3,     1,     3,     3,
       3,     4,     7,     9,     0,     3,     0,     1,     9,    11,
      12,    11,    10,    10,    10,     9,    10
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

  case 97: /* $@8: %empty  */
                         { yyextra->das_force_oxford_comma=true;}
    break;

  case 98: /* expression_with_alias: "typedef" $@8 "name" '=' type_declaration semicolon  */
                                                                                                                   {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-5])), *(yyvsp[-3].s), TypeDeclPtr((yyvsp[-1].pTypeDecl)));
    }
    break;

  case 99: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 100: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 101: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 102: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 103: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 104: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 105: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 106: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 107: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 108: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 109: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 110: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 112: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 113: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 114: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 115: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 116: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 117: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                               {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]))); delete (yyvsp[-4].s); }
    }
    break;

  case 118: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 119: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 120: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 121: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 122: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 123: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 124: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 125: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 126: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 127: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 128: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 129: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 130: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 131: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 132: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 133: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 134: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 135: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 136: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 137: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 138: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 139: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 140: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 141: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 142: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 143: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 144: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 145: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 146: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 147: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 148: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 149: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 150: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 151: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 152: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 153: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 154: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 155: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 156: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 157: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 158: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 159: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 160: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 161: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 162: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 163: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 164: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 165: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 166: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 167: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 168: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 169: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 170: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 171: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 172: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 173: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 174: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 175: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 176: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 177: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 178: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 179: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 180: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 181: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 182: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 183: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 184: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 185: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 186: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 187: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 188: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 189: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 190: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 191: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 192: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 193: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 194: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 195: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 196: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 197: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 198: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 199: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 200: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 201: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 202: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 203: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 204: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 205: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 206: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 207: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 208: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 209: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 210: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 211: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 212: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 213: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 214: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 215: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 216: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 217: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 218: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 219: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 220: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 221: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 222: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 223: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 224: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 225: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 226: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 227: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 228: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 229: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 230: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 231: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 232: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 233: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 234: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 235: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 236: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 237: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 238: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 239: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 240: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 241: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 242: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 243: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 244: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 245: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 246: /* $@9: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 247: /* function_declaration: optional_public_or_private_function $@9 function_declaration_header expression_block  */
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

  case 248: /* open_block: "begin of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 249: /* open_block: "new scope"  */
                        { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 250: /* close_block: "end of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 251: /* close_block: "close scope"  */
                         { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 252: /* expression_block: open_block expressions close_block  */
                                                                  {
        auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui), format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size, format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 253: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
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

  case 254: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 255: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 256: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 257: /* expression_any: SEMICOLON  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 258: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 259: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 260: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 261: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 262: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 263: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 264: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 265: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 266: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 267: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 268: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 269: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 270: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 271: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 272: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 273: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 274: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 275: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 276: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 277: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 278: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 279: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 280: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 281: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 282: /* expr_keyword: "keyword" expr expression_block  */
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

  case 283: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 284: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 285: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 286: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 287: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 288: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 289: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 290: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 291: /* $@10: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 292: /* $@11: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 293: /* expression_keyword: "keyword" '<' $@10 type_declaration_no_options_list '>' $@11 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 294: /* $@12: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 295: /* $@13: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 296: /* expression_keyword: "type function" '<' $@12 type_declaration_no_options_list '>' $@13 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 297: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 298: /* expr_pipe: "@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }

        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 299: /* expr_pipe: "@@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 300: /* expr_pipe: "$ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "$";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 301: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 302: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 303: /* name_in_namespace: "name" "::" "name"  */
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

  case 304: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 305: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 306: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 307: /* $@14: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 308: /* $@15: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 309: /* new_type_declaration: '<' $@14 type_declaration '>' $@15  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 310: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 311: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 312: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 313: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 314: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 315: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 316: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 317: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 318: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 319: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 320: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 321: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 322: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 323: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 324: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 325: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 326: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 327: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 328: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 329: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 330: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                              {
        const auto end_block = format::Pos::from_last(tokAt(scanner, (yylsp[-2])));
        const auto start = format::Pos::from(tokAt(scanner, (yylsp[-3])));
        if (format::is_replace_braces()) {
            format::skip_spaces_or_print(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])), yyextra->das_tab_size);
        }

        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 331: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 332: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 333: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 334: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 335: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 336: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 337: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 338: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 339: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 340: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
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

  case 341: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 342: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 343: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 344: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 345: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 346: /* $@16: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 347: /* $@17: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 348: /* expr_cast: "cast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 349: /* $@18: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 350: /* $@19: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 351: /* expr_cast: "upcast" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 352: /* $@20: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 353: /* $@21: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 354: /* expr_cast: "reinterpret" '<' $@20 type_declaration_no_options '>' $@21 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 355: /* $@22: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 356: /* $@23: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 357: /* expr_type_decl: "type" '<' $@22 type_declaration '>' $@23  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 358: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 359: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 360: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 361: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 362: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 363: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" semicolon "name" '>' '(' expr ')'  */
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

  case 364: /* expr_list: expr2  */
                       {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 365: /* expr_list: expr_list ',' expr2  */
                                             {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 366: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 367: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 368: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 369: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 370: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 371: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 372: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 373: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 374: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 375: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 376: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 377: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 378: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 379: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 380: /* optional_capture_list: "[[" capture_list ']' ']'  */
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

  case 381: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 382: /* expr_block: expression_block  */
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

  case 383: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 384: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 385: /* $@24: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 386: /* expr_full_block_assumed_piped: block_or_lambda $@24 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 387: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 388: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 389: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 390: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 391: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 392: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 393: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 394: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 395: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 399: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 400: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 401: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 410: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 411: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 412: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 413: /* expr_assign_pipe_right: "@ <|" expr_block  */
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

  case 414: /* expr_assign_pipe_right: "@@ <|" expr_block  */
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

  case 415: /* expr_assign_pipe_right: "$ <|" expr_block  */
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

  case 416: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 417: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 420: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 423: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 424: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 425: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 430: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 431: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 432: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 433: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 434: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 435: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 436: /* expr_method_call: expr2 "->" "name" '(' ')'  */
                                                          {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 437: /* expr_method_call: expr2 "->" "name" '(' expr_list ')'  */
                                                                               {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 438: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 439: /* func_addr_name: "$i" '(' expr2 ')'  */
                                           {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 440: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 441: /* $@25: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 442: /* $@26: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 443: /* func_addr_expr: '@' '@' '<' $@25 type_declaration_no_options '>' $@26 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 444: /* $@27: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 445: /* $@28: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 446: /* func_addr_expr: '@' '@' '<' $@27 optional_function_argument_list optional_function_type '>' $@28 func_addr_name  */
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

  case 447: /* expr_field: expr2 '.' "name"  */
                                               {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 448: /* expr_field: expr2 '.' '.' "name"  */
                                                   {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 449: /* expr_field: expr2 '.' "name" '(' ')'  */
                                                       {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 450: /* expr_field: expr2 '.' "name" '(' expr_list ')'  */
                                                                            {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 451: /* expr_field: expr2 '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 452: /* expr_field: expr2 '.' basic_type_declaration '(' ')'  */
                                                                         {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 453: /* expr_field: expr2 '.' basic_type_declaration '(' expr_list ')'  */
                                                                                              {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 454: /* $@29: %empty  */
                                { yyextra->das_suppress_errors=true; }
    break;

  case 455: /* $@30: %empty  */
                                                                             { yyextra->das_suppress_errors=false; }
    break;

  case 456: /* expr_field: expr2 '.' $@29 error $@30  */
                                                                                                                     {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 457: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 458: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 459: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 460: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 461: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 462: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 463: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 464: /* expr2: name_in_namespace  */
                                              { need_wrap_current_expr = true; (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 465: /* expr2: expr_field  */
                                              { need_wrap_current_expr = true; (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 466: /* expr2: expr_mtag  */
                                              { need_wrap_current_expr = true; (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 467: /* expr2: '!' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"!",(yyvsp[0].pExpression)); }
    break;

  case 468: /* expr2: '~' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"~",(yyvsp[0].pExpression)); }
    break;

  case 469: /* expr2: '+' expr2  */
                                                   { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+",(yyvsp[0].pExpression)); }
    break;

  case 470: /* expr2: '-' expr2  */
                                                   { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"-",(yyvsp[0].pExpression)); }
    break;

  case 471: /* expr2: expr2 "<<" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr2: expr2 ">>" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr2: expr2 "<<<" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr2: expr2 ">>>" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr2: expr2 '+' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr2: expr2 '-' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr2: expr2 '*' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr2: expr2 '/' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr2: expr2 '%' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr2: expr2 '<' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr2: expr2 '>' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr2: expr2 "==" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr2: expr2 "!=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr2: expr2 "<=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr2: expr2 ">=" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr2: expr2 '&' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr2: expr2 '|' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr2: expr2 '^' expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr2: expr2 "&&" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr2: expr2 "||" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr2: expr2 "^^" expr2  */
                                               { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 492: /* expr2: expr2 ".." expr2  */
                                               {
        need_wrap_current_expr = true;
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 493: /* expr2: "++" expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"++", (yyvsp[0].pExpression)); }
    break;

  case 494: /* expr2: "--" expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"--", (yyvsp[0].pExpression)); }
    break;

  case 495: /* expr2: expr2 "++"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 496: /* expr2: expr2 "--"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 497: /* expr2: expr2 '[' expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 498: /* expr2: expr2 '.' '[' expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 499: /* expr2: expr2 "?[" expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 500: /* expr2: expr2 '.' "?[" expr2 ']'  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 501: /* expr2: expr2 "?." "name"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 502: /* expr2: expr2 '.' "?." "name"  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 503: /* expr2: '*' expr2  */
                                                            { need_wrap_current_expr = true; (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 504: /* expr2: expr2 '?' expr2 ':' expr2  */
                                                             {
        need_wrap_current_expr = true; (yyval.pExpression) = new ExprOp3(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 505: /* expr2: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 506: /* expr2: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 507: /* expr2: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 508: /* expr2: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 509: /* expr2: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 510: /* expr2: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 511: /* expr2: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 512: /* expr2: '(' expr_list optional_comma ')'  */
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

  case 513: /* expr2: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 514: /* expr2: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 515: /* expr2: "deref" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 516: /* expr2: "addr" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 517: /* expr2: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 518: /* expr2: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr2 ')'  */
                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 519: /* expr2: expr2 "??" expr2  */
                                                     { (yyval.pExpression) = new ExprNullCoalescing(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 520: /* $@31: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 521: /* $@32: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 522: /* expr2: expr2 "is" "type" '<' $@31 type_declaration_no_options '>' $@32  */
                                                                                                                                                             {
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 523: /* expr2: expr2 "is" basic_type_declaration  */
                                                                {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 524: /* expr2: expr2 "is" "name"  */
                                               {
        (yyval.pExpression) = new ExprIsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 525: /* expr2: expr2 "as" "name"  */
                                               {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 526: /* $@33: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 527: /* $@34: %empty  */
                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 528: /* expr2: expr2 "as" "type" '<' $@33 type_declaration '>' $@34  */
                                                                                                                                                  {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 529: /* expr2: expr2 "as" basic_type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 530: /* expr2: expr2 '?' "as" "name"  */
                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 531: /* $@35: %empty  */
                                                    { yyextra->das_arrow_depth ++; }
    break;

  case 532: /* $@36: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 533: /* expr2: expr2 '?' "as" "type" '<' $@35 type_declaration '>' $@36  */
                                                                                                                                                      {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-8]), (yylsp[-1])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 534: /* expr2: expr2 '?' "as" basic_type_declaration  */
                                                                    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 535: /* expr2: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 536: /* expr2: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 537: /* expr2: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 538: /* expr2: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 539: /* expr2: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); (yyval.pExpression)->at = tokAt(scanner, (yylsp[0])); }
    break;

  case 540: /* expr2: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 541: /* expr2: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 542: /* expr2: expr2 "<|" expr2  */
                                                  { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); }
    break;

  case 543: /* expr2: expr2 "|>" expr2  */
                                                  { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]), (yylsp[0]))); (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])); }
    break;

  case 544: /* expr2: expr2 "|>" basic_type_declaration  */
                                                           {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
    }
    break;

  case 545: /* expr2: name_in_namespace "name"  */
                                                  {
        if (format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << "." << format::get_substring(tokAt(scanner,(yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 546: /* expr2: "unsafe" '(' expr2 ')'  */
                                          {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 547: /* expr2: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 548: /* expr: expr2  */
                                       {
        if (need_wrap_current_expr) {
            format::wrap_par_expr_newline(tokAt(scanner,(yylsp[0])), (yyvsp[0].pExpression)->at);
            need_wrap_current_expr = false;
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 549: /* expr_mtag: "$$" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 550: /* expr_mtag: "$i" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 551: /* expr_mtag: "$v" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 552: /* expr_mtag: "$b" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 553: /* expr_mtag: "$a" '(' expr2 ')'  */
                                                      { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 554: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 555: /* expr_mtag: "$c" '(' expr2 ')' '(' ')'  */
                                                             {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 556: /* expr_mtag: "$c" '(' expr2 ')' '(' expr_list ')'  */
                                                                                 {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 557: /* expr_mtag: expr2 '.' "$f" '(' expr2 ')'  */
                                                                  {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 558: /* expr_mtag: expr2 "?." "$f" '(' expr2 ')'  */
                                                                   {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 559: /* expr_mtag: expr2 '.' '.' "$f" '(' expr2 ')'  */
                                                                      {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 560: /* expr_mtag: expr2 '.' "?." "$f" '(' expr2 ')'  */
                                                                       {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 561: /* expr_mtag: expr2 "as" "$f" '(' expr2 ')'  */
                                                                     {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 562: /* expr_mtag: expr2 '?' "as" "$f" '(' expr2 ')'  */
                                                                         {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 563: /* expr_mtag: expr2 "is" "$f" '(' expr2 ')'  */
                                                                     {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 564: /* expr_mtag: '@' '@' "$c" '(' expr2 ')'  */
                                                          {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 565: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 566: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
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

  case 567: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 568: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 569: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 570: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 571: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 572: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 573: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 574: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 575: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 576: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 577: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 578: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 579: /* opt_sem: SEMICOLON  */
                { (yyval.b) = false; }
    break;

  case 580: /* opt_sem: "end of expression"  */
          { (yyval.b) = true; }
    break;

  case 581: /* opt_sem: "end of expression" SEMICOLON  */
                    { (yyval.b) = true; }
    break;

  case 582: /* opt_sem: %empty  */
                  {(yyval.b) = false; }
    break;

  case 583: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 584: /* $@37: %empty  */
                                                               { yyextra->das_force_oxford_comma=true;}
    break;

  case 585: /* struct_variable_declaration_list: struct_variable_declaration_list "typedef" $@37 "name" '=' type_declaration semicolon opt_sem  */
                                                                                                                                                                 {
        (yyval.pVarDeclList) = (yyvsp[-7].pVarDeclList);
        ast_structureAlias(scanner,(yyvsp[-4].s),(yyvsp[-2].pTypeDecl),tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 586: /* $@38: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 587: /* struct_variable_declaration_list: struct_variable_declaration_list $@38 structure_variable_declaration semicolon opt_sem  */
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

  case 588: /* $@39: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 589: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@39 function_declaration_header semicolon opt_sem  */
                                                                  {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-6].b),(yyvsp[-4].b), (yyvsp[-2].pFuncDecl));
            }
    break;

  case 590: /* $@40: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 591: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@40 function_declaration_header expression_block opt_sem  */
                                                                                {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-10].pVarDeclList),(yyvsp[-9].faList),(yyvsp[-6].b),(yyvsp[-7].b),(yyvsp[-5].i),(yyvsp[-4].b),(yyvsp[-2].pFuncDecl),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-8]),(yylsp[-1])),tokAt(scanner,(yylsp[-9])));
            }
    break;

  case 592: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon opt_sem  */
                                                                                               {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-3])), CompilationError::syntax_error);
        delete (yyvsp[-3].faList);
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
    }
    break;

  case 593: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 594: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 595: /* function_argument_declaration_type: "$a" '(' expr2 ')'  */
                                      {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 596: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 597: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 598: /* function_argument_list: function_argument_declaration_no_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 599: /* function_argument_list: function_argument_declaration_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 600: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 601: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 602: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 603: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 604: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 605: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 606: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 607: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 608: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 609: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 610: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 611: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 612: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 613: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 614: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 615: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 616: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 617: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 618: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 619: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 620: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 621: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 622: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 623: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 624: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 625: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 626: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 627: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 628: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 629: /* let_variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 630: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 631: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 632: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 633: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 634: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 635: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 636: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 637: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr  */
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

  case 638: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 639: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 640: /* $@41: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 641: /* global_variable_declaration_list: global_variable_declaration_list $@41 optional_field_annotation let_variable_declaration opt_sem  */
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

  case 642: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 643: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 644: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 645: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 646: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 647: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                                   {
        handle_brace(format::Pos::from(tokAt(scanner, (yylsp[-2]))), (yyvsp[-2].ui),
                     format::get_substring(tokRangeAt(scanner, (yylsp[-2]), (yylsp[0]))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 648: /* $@42: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 649: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@42 optional_field_annotation let_variable_declaration  */
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

  case 650: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 651: /* enum_list: enum_list "name" opt_sem  */
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

  case 652: /* enum_list: enum_list "name" '=' expr opt_sem  */
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

  case 653: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 654: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 655: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 656: /* $@43: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 657: /* single_alias: optional_public_or_private_alias "name" $@43 '=' type_declaration  */
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

  case 658: /* alias_list: single_alias opt_sem  */
                                    {
        (yyval.positions) = new vector<LineInfo>(1, tokAt(scanner, (yylsp[-1])));
    }
    break;

  case 659: /* alias_list: alias_list single_alias opt_sem  */
                                                       {
        (yyvsp[-2].positions)->emplace_back(tokAt(scanner, (yylsp[-1])));
        (yyval.positions) = (yyvsp[-2].positions);
    }
    break;

  case 660: /* alias_declaration: "typedef" open_block alias_list close_block  */
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

  case 661: /* $@44: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 663: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 664: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 665: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 666: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 667: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block enum_list close_block  */
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

  case 668: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block enum_list close_block  */
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

  case 669: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 670: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 671: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 672: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 673: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 674: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 675: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 676: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 677: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 678: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 679: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 680: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 681: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 682: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                                      {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 683: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 684: /* $@46: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 685: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 686: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 687: /* variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 688: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 689: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 690: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 691: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 692: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 693: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 694: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 695: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 696: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 697: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 698: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 699: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 700: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 701: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 702: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 703: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 704: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 705: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 706: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 707: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 708: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 709: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 710: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 711: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 712: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 713: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 714: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 715: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 716: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 717: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 718: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 719: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 720: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 721: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 722: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 723: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 724: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 725: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 726: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 727: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 728: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 729: /* auto_type_declaration: "$t" '(' expr2 ')'  */
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

  case 730: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 731: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 734: /* bitfield_alias_bits: %empty  */
       {
        auto pSL = new vector<tuple<string,Expression *>>();
        (yyval.pNameExprList) = pSL;

    }
    break;

  case 735: /* bitfield_alias_bits: bitfield_alias_bits "name" SEMICOLON  */
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

  case 736: /* bitfield_alias_bits: bitfield_alias_bits "name" commas  */
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

  case 737: /* bitfield_alias_bits: bitfield_alias_bits "name"  */
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

  case 738: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr SEMICOLON  */
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

  case 739: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr commas  */
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

  case 740: /* bitfield_alias_bits: bitfield_alias_bits "name" '=' expr  */
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

  case 741: /* bitfield_basic_type_declaration: %empty  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 742: /* bitfield_basic_type_declaration: ':' "uint8"  */
                             { (yyval.type) = Type::tBitfield8; }
    break;

  case 743: /* bitfield_basic_type_declaration: ':' "uint16"  */
                             { (yyval.type) = Type::tBitfield16; }
    break;

  case 744: /* bitfield_basic_type_declaration: ':' "uint"  */
                             { (yyval.type) = Type::tBitfield; }
    break;

  case 745: /* bitfield_basic_type_declaration: ':' "uint64"  */
                             { (yyval.type) = Type::tBitfield64; }
    break;

  case 746: /* $@47: %empty  */
                                                                     { yyextra->das_arrow_depth ++; }
    break;

  case 747: /* $@48: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 748: /* bitfield_type_declaration: "bitfield" bitfield_basic_type_declaration '<' $@47 bitfield_bits '>' $@48  */
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

  case 751: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 752: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 753: /* dim_list: '[' expr2 ']'  */
                              {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 754: /* dim_list: dim_list '[' expr2 ']'  */
                                             {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 755: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 756: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 757: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 758: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 759: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 760: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 761: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 762: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 763: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 764: /* type_declaration_no_options: "typedecl" '(' expr2 ')'  */
                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 765: /* type_declaration_no_options: '$' name_in_namespace optional_expr_list_in_braces  */
                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-1]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-1])), *(yyvsp[-1].s)));
        delete (yyvsp[-1].s);
    }
    break;

  case 766: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 767: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' optional_expr_list_in_braces  */
                                                                                                                                                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-5]), (yylsp[0]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-2].pTypeDeclList),(yyvsp[0].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-5])), *(yyvsp[-5].s)));
        delete (yyvsp[-5].s);
    }
    break;

  case 768: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 769: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 770: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 771: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 772: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 773: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 774: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 775: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 776: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 777: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 778: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 779: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 780: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 782: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 783: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 784: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 785: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 786: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 787: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 788: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 789: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 790: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 791: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 792: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 793: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 794: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 795: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 796: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 797: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 798: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 799: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
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

  case 800: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 801: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 802: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 803: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 804: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 805: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 806: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
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

  case 807: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tVoid);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 808: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 809: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 810: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 811: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 812: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 813: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
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

  case 814: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 815: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 816: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 817: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 818: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 819: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 820: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 821: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 822: /* type_declaration: type_declaration '|' '#'  */
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

  case 823: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 824: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 825: /* $@78: %empty  */
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

  case 826: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 827: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 open_block $@78 tuple_alias_type_list $@79 close_block  */
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

  case 828: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 829: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 830: /* $@82: %empty  */
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

  case 831: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 832: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 open_block $@82 variant_alias_type_list $@83 close_block  */
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

  case 833: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 834: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 835: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 bitfield_basic_type_declaration open_block bitfield_alias_bits close_block  */
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

  case 836: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 837: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 838: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 839: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 840: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 841: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 842: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 843: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 844: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 845: /* make_struct_fields: "$f" '(' expr2 ')' copy_or_move expr  */
                                                                            {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 846: /* make_struct_fields: "$f" '(' expr2 ')' ":=" expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner, (yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 847: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' copy_or_move expr  */
                                                                                                        {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 848: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' ":=" expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 849: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 850: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 851: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 852: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 853: /* make_struct_dim: make_struct_dim semicolon make_struct_fields  */
                                                               {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 854: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 855: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 856: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 857: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 858: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 859: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 860: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 861: /* optional_block: "where" expr_block  */
                                  { (yyvsp[0].pExpression)->at = tokAt(scanner, (yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 874: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 875: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 876: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 877: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
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

  case 878: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
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

  case 879: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 880: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 881: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 882: /* $@86: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 883: /* $@87: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 884: /* make_struct_decl: "struct" '<' $@86 type_declaration_no_options '>' $@87 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 885: /* $@88: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 886: /* $@89: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 887: /* make_struct_decl: "class" '<' $@88 type_declaration_no_options '>' $@89 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 888: /* $@90: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 889: /* $@91: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 890: /* make_struct_decl: "variant" '<' $@90 variant_type_list '>' $@91 '(' use_initializer make_variant_dim ')'  */
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

  case 891: /* $@92: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 892: /* $@93: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 893: /* make_struct_decl: "default" '<' $@92 type_declaration_no_options '>' $@93 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 894: /* make_tuple: expr  */
                  {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 895: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 896: /* make_tuple: make_tuple ',' expr  */
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

  case 897: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 898: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 899: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 900: /* $@94: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 901: /* $@95: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 902: /* make_tuple_call: "tuple" '<' $@94 tuple_type_list '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 903: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 904: /* make_dim: make_dim semicolon make_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 905: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 906: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
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

  case 907: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
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

  case 908: /* $@96: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 909: /* $@97: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 910: /* make_dim_decl: "array" "struct" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 911: /* $@98: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 912: /* $@99: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 913: /* make_dim_decl: "array" "tuple" '<' $@98 tuple_type_list '>' $@99 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 914: /* $@100: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 915: /* $@101: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 916: /* make_dim_decl: "array" "variant" '<' $@100 variant_type_list '>' $@101 '(' make_variant_dim ')'  */
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

  case 917: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 918: /* $@102: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 919: /* $@103: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 920: /* make_dim_decl: "array" '<' $@102 type_declaration_no_options '>' $@103 '(' optional_expr_list ')'  */
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

  case 921: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 922: /* $@104: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 923: /* $@105: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 924: /* make_dim_decl: "fixed_array" '<' $@104 type_declaration_no_options '>' $@105 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 925: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 926: /* make_table: make_table semicolon make_map_tuple  */
                                                      {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 927: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 928: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 929: /* make_table_decl: open_block optional_expr_map_tuple_list close_block  */
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

  case 930: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 931: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 932: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 933: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 934: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 935: /* array_comprehension_where: semicolon "where" expr  */
                                          { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 936: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 937: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 938: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 939: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 940: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                         {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 941: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 942: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                  {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 943: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']' ']'  */
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

  case 944: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where "end of code block" ']'  */
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

  case 945: /* array_comprehension: open_block "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block  */
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

  case 946: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block close_block  */
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


