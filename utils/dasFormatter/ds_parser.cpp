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
  YYSYMBOL_OPEN_BRACE = 163,               /* OPEN_BRACE  */
  YYSYMBOL_CLOSE_BRACE = 164,              /* CLOSE_BRACE  */
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
  YYSYMBOL_format_string = 231,            /* format_string  */
  YYSYMBOL_optional_format_string = 232,   /* optional_format_string  */
  YYSYMBOL_233_1 = 233,                    /* $@1  */
  YYSYMBOL_string_builder_body = 234,      /* string_builder_body  */
  YYSYMBOL_string_builder = 235,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 236, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 237,              /* expr_reader  */
  YYSYMBOL_238_2 = 238,                    /* $@2  */
  YYSYMBOL_options_declaration = 239,      /* options_declaration  */
  YYSYMBOL_require_declaration = 240,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 241,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 242,      /* require_module_name  */
  YYSYMBOL_require_module = 243,           /* require_module  */
  YYSYMBOL_is_public_module = 244,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 245,       /* expect_declaration  */
  YYSYMBOL_expect_list = 246,              /* expect_list  */
  YYSYMBOL_expect_error = 247,             /* expect_error  */
  YYSYMBOL_expression_label = 248,         /* expression_label  */
  YYSYMBOL_expression_goto = 249,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 250,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 251,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 252,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 253, /* expression_else_one_liner  */
  YYSYMBOL_254_3 = 254,                    /* $@3  */
  YYSYMBOL_expression_if_one_liner = 255,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 256,  /* expression_if_then_else  */
  YYSYMBOL_257_4 = 257,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 258,      /* expression_for_loop  */
  YYSYMBOL_259_5 = 259,                    /* $@5  */
  YYSYMBOL_260_6 = 260,                    /* $@6  */
  YYSYMBOL_expression_unsafe = 261,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 262,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 263,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 264,    /* expression_with_alias  */
  YYSYMBOL_265_7 = 265,                    /* $@7  */
  YYSYMBOL_annotation_argument_value = 266, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 267, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 268, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 269,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 270, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 271,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 272, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 273, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 274,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 275,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 276, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 277, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 278,   /* optional_function_type  */
  YYSYMBOL_function_name = 279,            /* function_name  */
  YYSYMBOL_global_function_declaration = 280, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 281, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 282, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 283,     /* function_declaration  */
  YYSYMBOL_284_8 = 284,                    /* $@8  */
  YYSYMBOL_open_block = 285,               /* open_block  */
  YYSYMBOL_close_block = 286,              /* close_block  */
  YYSYMBOL_expression_block = 287,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 288,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 289,           /* expression_any  */
  YYSYMBOL_expressions = 290,              /* expressions  */
  YYSYMBOL_expr_keyword = 291,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 292,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 293, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 294, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 295, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 296,       /* expression_keyword  */
  YYSYMBOL_297_9 = 297,                    /* $@9  */
  YYSYMBOL_298_10 = 298,                   /* $@10  */
  YYSYMBOL_299_11 = 299,                   /* $@11  */
  YYSYMBOL_300_12 = 300,                   /* $@12  */
  YYSYMBOL_expr_pipe = 301,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 302,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 303,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 304,     /* new_type_declaration  */
  YYSYMBOL_305_13 = 305,                   /* $@13  */
  YYSYMBOL_306_14 = 306,                   /* $@14  */
  YYSYMBOL_expr_new = 307,                 /* expr_new  */
  YYSYMBOL_expression_break = 308,         /* expression_break  */
  YYSYMBOL_expression_continue = 309,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 310, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 311,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 312, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 313,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 314,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 315,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 316,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 317,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 318,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 319, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 320,           /* expression_let  */
  YYSYMBOL_expr_cast = 321,                /* expr_cast  */
  YYSYMBOL_322_15 = 322,                   /* $@15  */
  YYSYMBOL_323_16 = 323,                   /* $@16  */
  YYSYMBOL_324_17 = 324,                   /* $@17  */
  YYSYMBOL_325_18 = 325,                   /* $@18  */
  YYSYMBOL_326_19 = 326,                   /* $@19  */
  YYSYMBOL_327_20 = 327,                   /* $@20  */
  YYSYMBOL_expr_type_decl = 328,           /* expr_type_decl  */
  YYSYMBOL_329_21 = 329,                   /* $@21  */
  YYSYMBOL_330_22 = 330,                   /* $@22  */
  YYSYMBOL_expr_type_info = 331,           /* expr_type_info  */
  YYSYMBOL_expr_list = 332,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 333,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 334,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 335,            /* capture_entry  */
  YYSYMBOL_capture_list = 336,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 337,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 338,               /* expr_block  */
  YYSYMBOL_expr_full_block = 339,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 340, /* expr_full_block_assumed_piped  */
  YYSYMBOL_341_23 = 341,                   /* $@23  */
  YYSYMBOL_expr_numeric_const = 342,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 343,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 344,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 345,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 346,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 347,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 348,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 349,           /* func_addr_expr  */
  YYSYMBOL_350_24 = 350,                   /* $@24  */
  YYSYMBOL_351_25 = 351,                   /* $@25  */
  YYSYMBOL_352_26 = 352,                   /* $@26  */
  YYSYMBOL_353_27 = 353,                   /* $@27  */
  YYSYMBOL_expr_field = 354,               /* expr_field  */
  YYSYMBOL_355_28 = 355,                   /* $@28  */
  YYSYMBOL_356_29 = 356,                   /* $@29  */
  YYSYMBOL_expr_call = 357,                /* expr_call  */
  YYSYMBOL_expr_not_wrapped = 358,         /* expr_not_wrapped  */
  YYSYMBOL_expr_wrapped = 359,             /* expr_wrapped  */
  YYSYMBOL_360_30 = 360,                   /* $@30  */
  YYSYMBOL_361_31 = 361,                   /* $@31  */
  YYSYMBOL_362_32 = 362,                   /* $@32  */
  YYSYMBOL_363_33 = 363,                   /* $@33  */
  YYSYMBOL_364_34 = 364,                   /* $@34  */
  YYSYMBOL_365_35 = 365,                   /* $@35  */
  YYSYMBOL_expr2 = 366,                    /* expr2  */
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
#define YYLAST   14309

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  303
/* YYNRULES -- Number of rules.  */
#define YYNRULES  920
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1767

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
       0,   573,   573,   574,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   595,   598,   601,
     607,   608,   609,   613,   614,   618,   636,   637,   638,   639,
     643,   644,   648,   649,   653,   654,   654,   658,   663,   672,
     687,   703,   708,   716,   716,   757,   787,   791,   792,   793,
     797,   800,   804,   810,   819,   822,   828,   829,   833,   837,
     838,   842,   845,   851,   857,   860,   866,   867,   871,   872,
     873,   886,   887,   891,   892,   892,   898,   899,   900,   901,
     902,   906,   916,   916,   924,   924,   928,   928,   937,   945,
     957,   967,   967,   974,   975,   976,   977,   978,   979,   983,
     988,   996,   997,   998,  1002,  1003,  1004,  1005,  1006,  1007,
    1008,  1009,  1015,  1018,  1024,  1027,  1030,  1036,  1037,  1038,
    1039,  1043,  1056,  1074,  1077,  1085,  1096,  1107,  1118,  1121,
    1128,  1132,  1139,  1140,  1144,  1145,  1146,  1150,  1153,  1160,
    1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,
    1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,  1182,  1183,
    1184,  1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,  1193,
    1194,  1195,  1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,
    1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,  1213,
    1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,  1222,  1223,
    1224,  1225,  1226,  1227,  1228,  1229,  1230,  1231,  1232,  1233,
    1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,
    1244,  1245,  1246,  1247,  1252,  1270,  1271,  1272,  1276,  1282,
    1282,  1299,  1300,  1303,  1304,  1307,  1314,  1338,  1356,  1365,
    1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,  1380,
    1381,  1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,  1390,
    1391,  1392,  1396,  1401,  1407,  1413,  1425,  1426,  1430,  1431,
    1435,  1436,  1440,  1444,  1451,  1451,  1451,  1457,  1457,  1457,
    1466,  1500,  1508,  1515,  1522,  1528,  1529,  1540,  1544,  1547,
    1555,  1555,  1555,  1558,  1564,  1567,  1571,  1575,  1582,  1589,
    1595,  1599,  1603,  1606,  1609,  1617,  1620,  1623,  1631,  1634,
    1642,  1645,  1648,  1656,  1668,  1669,  1670,  1674,  1675,  1679,
    1680,  1684,  1689,  1697,  1708,  1714,  1729,  1741,  1744,  1750,
    1750,  1750,  1753,  1753,  1753,  1758,  1758,  1758,  1766,  1766,
    1766,  1772,  1786,  1802,  1820,  1830,  1841,  1856,  1859,  1865,
    1866,  1873,  1884,  1885,  1886,  1890,  1891,  1892,  1893,  1894,
    1898,  1903,  1911,  1912,  1925,  1929,  1939,  1946,  1953,  1953,
    1962,  1963,  1964,  1965,  1966,  1967,  1968,  1972,  1973,  1974,
    1975,  1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,
    1985,  1986,  1987,  1988,  1989,  1990,  1994,  2004,  2013,  2022,
    2027,  2028,  2029,  2030,  2031,  2032,  2033,  2034,  2035,  2036,
    2037,  2038,  2039,  2040,  2041,  2042,  2043,  2048,  2055,  2067,
    2072,  2082,  2086,  2093,  2096,  2096,  2096,  2101,  2101,  2101,
    2114,  2118,  2122,  2127,  2134,  2143,  2148,  2155,  2155,  2155,
    2162,  2166,  2176,  2185,  2194,  2198,  2201,  2207,  2208,  2209,
    2210,  2211,  2212,  2213,  2214,  2215,  2216,  2217,  2218,  2219,
    2220,  2221,  2222,  2223,  2224,  2225,  2226,  2227,  2228,  2229,
    2230,  2231,  2232,  2233,  2234,  2235,  2241,  2242,  2243,  2244,
    2245,  2246,  2247,  2248,  2249,  2250,  2251,  2252,  2258,  2259,
    2260,  2261,  2262,  2263,  2264,  2265,  2279,  2280,  2281,  2282,
    2283,  2286,  2289,  2290,  2290,  2290,  2293,  2298,  2302,  2306,
    2306,  2306,  2311,  2314,  2318,  2318,  2318,  2323,  2326,  2327,
    2328,  2329,  2330,  2331,  2332,  2333,  2334,  2336,  2340,  2348,
    2353,  2357,  2362,  2368,  2377,  2378,  2379,  2380,  2381,  2382,
    2383,  2387,  2391,  2395,  2399,  2403,  2407,  2411,  2415,  2419,
    2426,  2427,  2436,  2440,  2441,  2442,  2446,  2447,  2451,  2452,
    2453,  2457,  2458,  2462,  2473,  2474,  2475,  2478,  2481,  2484,
    2484,  2503,  2502,  2518,  2517,  2531,  2540,  2552,  2561,  2571,
    2572,  2573,  2574,  2575,  2579,  2582,  2591,  2592,  2596,  2599,
    2602,  2618,  2627,  2628,  2632,  2635,  2638,  2652,  2653,  2657,
    2663,  2669,  2678,  2681,  2688,  2691,  2697,  2698,  2699,  2703,
    2704,  2708,  2715,  2720,  2729,  2735,  2746,  2749,  2754,  2759,
    2767,  2778,  2781,  2781,  2801,  2802,  2806,  2807,  2808,  2812,
    2819,  2819,  2838,  2841,  2857,  2877,  2878,  2879,  2884,  2884,
    2914,  2917,  2924,  2934,  2934,  2938,  2939,  2940,  2944,  2954,
    2974,  2997,  2998,  3002,  3003,  3007,  3013,  3014,  3015,  3016,
    3020,  3021,  3022,  3026,  3029,  3040,  3045,  3040,  3065,  3072,
    3077,  3086,  3092,  3103,  3104,  3105,  3106,  3107,  3108,  3109,
    3110,  3111,  3112,  3113,  3114,  3115,  3116,  3117,  3118,  3119,
    3120,  3121,  3122,  3123,  3124,  3125,  3126,  3127,  3128,  3129,
    3133,  3134,  3135,  3136,  3137,  3138,  3139,  3140,  3144,  3155,
    3159,  3166,  3178,  3185,  3195,  3196,  3201,  3206,  3209,  3224,
    3237,  3237,  3237,  3250,  3251,  3255,  3259,  3266,  3270,  3277,
    3278,  3279,  3280,  3281,  3296,  3302,  3302,  3302,  3306,  3311,
    3318,  3318,  3325,  3329,  3333,  3338,  3343,  3348,  3353,  3357,
    3361,  3366,  3370,  3374,  3379,  3379,  3379,  3385,  3392,  3392,
    3392,  3397,  3397,  3397,  3403,  3403,  3403,  3408,  3412,  3412,
    3412,  3417,  3417,  3417,  3426,  3430,  3430,  3430,  3435,  3435,
    3435,  3444,  3448,  3448,  3448,  3453,  3453,  3453,  3462,  3462,
    3462,  3468,  3468,  3468,  3477,  3480,  3491,  3507,  3507,  3512,
    3521,  3507,  3550,  3550,  3555,  3565,  3550,  3594,  3594,  3594,
    3637,  3638,  3639,  3640,  3641,  3645,  3652,  3659,  3665,  3671,
    3678,  3685,  3691,  3700,  3703,  3709,  3717,  3722,  3729,  3734,
    3741,  3746,  3752,  3753,  3757,  3758,  3763,  3764,  3768,  3769,
    3773,  3774,  3778,  3779,  3780,  3784,  3785,  3786,  3790,  3791,
    3795,  3828,  3867,  3886,  3906,  3926,  3947,  3947,  3947,  3955,
    3955,  3955,  3962,  3962,  3962,  3973,  3973,  3973,  3984,  3988,
    3994,  4010,  4016,  4022,  4028,  4028,  4028,  4042,  4047,  4054,
    4074,  4102,  4126,  4126,  4126,  4136,  4136,  4136,  4150,  4150,
    4150,  4164,  4173,  4173,  4173,  4193,  4200,  4200,  4200,  4210,
    4215,  4222,  4225,  4231,  4251,  4270,  4278,  4298,  4323,  4324,
    4328,  4329,  4334,  4344,  4347,  4350,  4353,  4361,  4370,  4382,
    4392
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
  "$@29", "expr_call", "expr_not_wrapped", "expr_wrapped", "$@30", "$@31",
  "$@32", "$@33", "$@34", "$@35", "expr2", "expr", "expr_mtag",
  "optional_field_annotation", "optional_override", "optional_constant",
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

#define YYPACT_NINF (-1561)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-909)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1561,   167, -1561, -1561,    99,   -89,   474,   -20, -1561,   -38,
     516,   516,   516, -1561, -1561,   133,    23, -1561, -1561, -1561,
     528, -1561, -1561, -1561,   279, -1561,    66, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561,    -7, -1561,   155,
      48,   233, -1561, -1561, -1561, -1561,   474, -1561,    41, -1561,
   -1561, -1561,   516,   516, -1561, -1561,    66, -1561, -1561, -1561,
   -1561, -1561,   216,   288, -1561, -1561, -1561, -1561,    23,    23,
      23,   259, -1561,   904,   134, -1561, -1561, -1561, -1561,   596,
     835,  1000,   890, -1561,   894,   107,    99,   315,   -89,   296,
     349, -1561,   884,   884, -1561,   360,   528,    16,   528,   907,
     383,   401,   412, -1561,   531,   316, -1561, -1561,     1,    99,
      23,    23,    23,    23, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561,   591, -1561, -1561, -1561, -1561, -1561, -1561, -1561,   -20,
   -1561, -1561, -1561, -1561, -1561,   924,    37, -1561, -1561, -1561,
   -1561,   460, -1561, -1561, -1561,   538, -1561, -1561, -1561,   528,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,   615,
   -1561,    83, -1561,   877,   630,   904, 14067, -1561,   477,   746,
   -1561,   -76, -1561, -1561, -1561,   944, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561,   140, -1561,   682, -1561, -1561, -1561,   538,
     -20,   -20,   -20, -1561, -1561, 13190, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561,   826,   848, -1561,   683,   -20,   816, -1561, -1561,   738,
   -1561,   643,    99,    99,   194,   -12, -1561, -1561, -1561,    37,
   -1561, 10811, -1561, -1561, -1561, -1561,   748,   781, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,   805,   814,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561,   975, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561,   841,   829, -1561, -1561,   145,
     817, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561,   -20,   754,   844,   -20, -1561,   -76,   345, -1561,    99,
   -1561,   852,  1032,   739, -1561, -1561,   883,   887,   893,   860,
     899,   909, -1561, -1561, -1561,   891, -1561, -1561, -1561, -1561,
   -1561,   913, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561,   914, -1561, -1561, -1561,   917,   920, -1561,
   -1561, -1561, -1561,   921,   926,   908,   133, -1561, -1561, -1561,
   -1561, -1561,  1071,   910,   700, -1561, -1561, -1561, -1561, -1561,
   -1561,   957,  1002, -1561,   927, -1561,   108,   952,   515,   923,
   10811, -1561,  3066, -1561,   395, -1561,   133, -1561, -1561, -1561,
     -12,   930, -1561, 10085,   970,   974, 10811, -1561,   191, -1561,
   -1561, -1561, 10085, -1561, -1561,   977, -1561,   620,   669,   702,
   -1561, -1561, 10085,   186, -1561, -1561, -1561,    25, -1561, -1561,
   -1561,    17,  6167, -1561,   936, 10559, -1561,   992,   448, 10662,
     697, -1561, -1561, 10085, -1561, -1561,   294,   266,   266,   266,
   -1561,   910, -1561,   960,   961, 10085, -1561, -1561,   943, -1561,
   -1561,   526,   -67,   963,    39,  3893, -1561, -1561,   -20,   444,
    6373,   945, 10085,   990,   966,   968,   950, -1561,   528,   971,
    1004,  6579,   -63,   499,   976, -1561,   641,   978,   980,  4099,
   10085, 10085,   357,   357,   357,   951,   955,   958,   964,   967,
     969, -1561,  2303, 10456,  6787, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561,  6993,   982, -1561,  1160, -1561, 10085, 10085,
   10085, 10085, 10085,  5755, 10085, -1561,   965, -1561, -1561,   528,
     528, 10085,  1173, -1561, -1561, -1561, -1561, -1561, -1561,  7201,
    1151, -1561, -1561, -1561,   822, -1561,    95,   528, -1561,   528,
     528,   528, -1561,   528, -1561, -1561,  1124, -1561, -1561, -1561,
   -1561,   979, -1561, -1561,   382, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, 13453,  1099, -1561,   972, -1561, -1561, -1561,
   -1561, -1561, -1561,   754, 10085, -1561, -1561,    51,   538, -1561,
     981,   993,   994, -1561,  2074,   -52, -1561,  1150,   657, -1561,
   -1561, -1561,  4511, 10811, 10811, 10811,  2581, 10811, 10811,   984,
    1023, 10811,   683, 10811,   683, 10811,   683, 10914,  1027,  2793,
   -1561, 10085, -1561, -1561, -1561, -1561,   987, -1561, -1561, 12685,
   10085, -1561,  1071, -1561,  1014, -1561, -1561,   198, -1561, -1561,
     675, -1561,   910,   643,  1022,   675, -1561,   643, 10903,  1001,
    1174, -1561, -1561,   -58, -1561, -1561, -1561, -1561, -1561,   -32,
     294,  1005, -1561,  1006, -1561, -1561,   133,   689, -1561,  1021,
    1026,  1030, -1561, 10085,  4511, -1561,  1038,  1098,  1927,  2536,
    1217, 10811, 10085, 10085,   -52, 10085,   -32,  1040, -1561, -1561,
   10085, -1561, -1561,  1039,  1068,   -52, 10085, -1561, -1561, 10085,
   -1561, -1561, 10085, -1561, 10811,  4511, -1561,  2908,   204,   204,
    1015, -1561,   979, -1561, -1561, -1561, 10085, 10085, 10085, 10085,
   10085, 10085,   294,  3272,   294,  3479,   294,   -21, -1561,   407,
   -1561,   -32, -1561,   888,  1020,   204,   204,   269,   204,   204,
     136,  1225,  1025,  1051,  1051,   426, -1561, -1561,   -32, -1561,
     294,   643, -1561,  1052,   -20, -1561, -1561, -1561,  4717, -1561,
   -1561, -1561, -1561, -1561, -1561,   306,    74,   357, -1561, 13906,
   14005, 10085, 10085, -1561, -1561, 10085, 10085, 10085, 10085, 10085,
     119, 10085, 10085, 10085, 10085, 10085, 10085, 10085, 10085, 10085,
    7407, 10085, 10085, 10085, 10085, 10085, 10085, 10085, 10085, 10085,
   10085,   344, 10085,  4923,  4923,  4923,  4923,  4923,  4923,  4923,
    4923,  4923,  4923,  1072,  4923, 10085,  4923,  4923,  4923,  4923,
    4923,  4923,  2757, -1561,  7613, -1561,   182,    23, -1561,  1241,
   -1561,   -76, -1561, 10811, -1561,  1077, -1561,  4511, -1561, 14089,
     270,   729,  1053,   500, -1561,   766,   786, -1561, -1561,   -61,
     813,   817,   820,   817,   823,   817, -1561,   397, -1561,   442,
   -1561, 10811,  1033, -1561, -1561, 12790, 10811, -1561, -1561, 10811,
   -1561, -1561, -1561, 10085,  1080, -1561,  1081, -1561, 10811, -1561,
   10085, 10811, 10811, -1561,    27,   294, 10811,  5961,  7819,  1082,
   10085, 10811, -1561, -1561, -1561, 10811,  1051, -1561,  1038, 10085,
   10085, 10085, 10085, 10085, 10085, 10085, 10085, 10085, 10085, 10085,
   10085, 10085, 10085, 10085, 10085, 10085, 10085,   -20,   879,  1041,
     -52, 10944, -1561, -1561, 10811, 10811, 11055, 10811, -1561, -1561,
   11090, 10811,  1051, 10811, 10914,  1051,  1027,    96, -1561,  2908,
   -1561,    74, 11195, 11239, 11344, 11379, 11484, 11528,    50,   357,
    1043,   250,  3686,  5131,  8025,   -10,  1065,     7,   282,  1066,
     354,    52,  8231,     7,   720,    55, 10085,  1074, -1561, 10085,
   -1561, 10811, -1561, 10811, -1561, 10085,   885,   294,   294,    60,
     265, -1561, 10085, -1561,  1047,  1049,  1055,   703, -1561, -1561,
     538, 10085,    61, -1561, 10085, -1561, -1561,   979,   -66,  5339,
   -1561,   184,  1078,  1056,  1100,  1100, -1561, -1561,  1059,   312,
     683, -1561,  1079,  1061, -1561, -1561,  1084,  1062, -1561, -1561,
     208,   208,  2420,  2420,  1678,  1678,   601,  1063, -1561, 12829,
     269,   269,   972,   208,   208, 10589, 13616, 13709, 13490, 14036,
   13323, 13742, 13835,  1240,  2420,  2420,   544,   544,   601,   601,
     601,   637, 10085,  1067,  1125, 10085, 12934,   357,   357,   357,
   -1561, -1561,   171, -1561,   171, -1561,   171, -1561,   171, -1561,
     171, -1561,   171, -1561,   171, -1561,   171, -1561,   171, -1561,
     171,  1070, -1561,   171,   -52, -1561,   171, -1561,   171, -1561,
     171, -1561,   171, -1561,   171, -1561,   171,  1073,  1112,  1287,
    1083, -1561,   257, -1561,   398,   916,  1222,   528,   775, -1561,
   -1561, 14089, -1561, -1561, -1561, -1561, 10811, -1561, -1561, -1561,
    1121, -1561,  1096, -1561,  1103, -1561,  1104, -1561, 10914, -1561,
    1027,   445,   910, -1561, -1561,   910,   910, 11633, -1561,  1254,
      10,   -52,  1263,  1293, 10085,    62,   825,   670,   262,  1086,
    1087,  1126,   -43,   399,   -35,   837, 10811, 10914,  1027,  1347,
    1088, 13360,   -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,
     -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52,   -52, -1561,
    1090, 10811, -1561, -1561, 10085,  1436,  1461, -1561,  1492, -1561,
    1759,  1092,  1883,   485,  1093,   580,    74,   683, -1561, -1561,
   -1561, -1561, -1561,  1091, 10085, -1561, 10085, 10085, 10085,  5547,
   12645,    11, 10085,   717,   670,   282, -1561, -1561,  1101, -1561,
   10085, -1561,  1109, 10085, -1561, 10085,   670,   759,  1111, -1561,
   -1561, 10085,   -52, -1561, -1561,   602,   612,   187,    67,    69,
   10085,   294,    73, -1561, 10085, 10085, 10811,   683,   634,    34,
   10085, -1561,  3066,    74,   303, -1561,  1094,   523, 10291, -1561,
     717, -1561, -1561, -1561,   535,   417,   312,  1141,  1142,  1114,
    1143,  1147, -1561,   584,   817, -1561, 10085, -1561, 10085, 10085,
   -1561,  1131,  1116, -1561, -1561, 10085,  1117, -1561, 12974, 10085,
    1118, 13079, -1561, -1561, -1561, -1561,  8437,  8643, -1561, -1561,
    8849, -1561,   528, -1561, -1561,   762, -1561,   215,   538,    74,
   -1561, -1561, -1561, -1561,   910, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,  1119,
   10811, -1561,  1164, 10085, -1561, -1561,   454, 10085, -1561,  1127,
   -1561, -1561, -1561,   621, -1561,  1122,  1168, -1561, -1561,  2286,
     633,   656, -1561, -1561, 10085,  2299,   240, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561,   854,   817,  9055,   791,
   11668,   -52,   -52,     7,   282,   -52,  1132,   320,  1065, -1561,
   -1561,   -52, -1561,  1066,   792,     7,  1128, -1561, -1561, -1561,
   -1561,   795, -1561, -1561, -1561,  1161, 10085, 10085,   803,    76,
   10085, 11773, 11817,  2378,   817,   -20, -1561, -1561, 10085, -1561,
   -1561,   528,   808, -1561,   683, -1561,  1130,  5339,  1170,  1134,
     563,   434, -1561, -1561,  1178, -1561, -1561,   312,  1136,   232,
   10811, 11922, 10811, 11957, 12062, -1561, 10085, 13583, 10085, -1561,
   12106, 10085, -1561, -1561,   463,  5339, -1561,   507, -1561, -1561,
     517,   538,  1328,   215, -1561, -1561,   916, -1561,  1145, -1561,
   -1561, -1561, 10085,   910, -1561,   -52,  1149,  1156, -1561,   524,
   -1561, -1561, 10085,  1194, 10085,  1175, -1561, -1561, -1561, -1561,
    1158,  1159,  1165, -1561, 10085, 10085, 10085,  1166,  1311,  1169,
    1171,  9261, -1561,   232, -1561,   543, 10085,   376,   282, -1561,
   10085, 10085, 10085, 10085,   759, -1561, 10085, 10085,  1172, -1561,
   -1561,   565,   571, 10085, 10085,   819, -1561, -1561, -1561,  1186,
   -1561,   -32,  4305, -1561, 10085,   817, -1561,   597, -1561,   798,
   10811,   191, -1561,  1176, -1561, -1561,  9467, -1561, -1561,  2397,
   -1561,   838, -1561, -1561, 10811, 12211, 12246, -1561, 12351, -1561,
     614, -1561, -1561, -1561, -1561, -1561,  1328,   294,  1177,  1311,
    1311,   -20,   -24,  1191,    72,  1179,  1181,  1182,  1183, 10085,
   -1561, 10085,   -52,   -52,   -52, 10085, -1561, -1561,  1311,  1311,
   -1561, 12395, -1561, -1561, 12540, 10085, 10085, -1561, 12500,   -52,
     -52, 13230, -1561,   528,   -52, 10085, -1561,  1227,  1208,  1221,
   12540,   599, 10085,   549, -1561,   634,  9673,  9879, -1561, -1561,
   -1561, -1561, -1561,   -52,   528,   -20,  1196, 10811,   191,   956,
   10085, -1561, 10085,   -52, -1561, -1561,   840, -1561, -1561, -1561,
    1197, 14067, -1561, -1561, -1561, -1561, -1561,   277,   277, -1561,
   -1561, 10085, -1561, 10085,  1311,  1311,   670,  1198,  1201,  1051,
     277,   670, -1561,  1349,  1193,   -52,   -52,   389,  1228,   643,
    1231, -1561, 10085, 10085,  1207,  1239, 12540, -1561,   549, -1561,
   10085, 10085,   -52,   643, -1561, -1561,   956, 10085, 10085,   240,
     -52, -1561, -1561, -1561, -1561,   528, 14067,   670,  1065,  1235,
   -1561,  1209,  1213,   102,   131,   277,   277,  1065,  1215, -1561,
   -1561,  1218,  1230,  1232, 10085,  1219, 10085, 10085,  1229,   643,
   -1561,  1233,   528, 12540, -1561, 10085,  1237, -1561,   -52, -1561,
   10085,   240,   240, -1561, -1561,   538,   -20,   605,  1242, -1561,
   -1561, -1561, -1561, -1561,  1246,  1247, -1561, -1561, -1561, -1561,
     -52, -1561,   -52,   -52, -1561, -1561, -1561,  1250,  1251, 12540,
   -1561,   240, -1561, -1561, -1561, -1561, -1561,   670, -1561, -1561,
   -1561, -1561,  1252, -1561,   608, -1561, -1561
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   132,     1,   317,     0,     0,     0,   643,   318,     0,
     635,   635,   635,    17,    18,     0,     0,    16,    15,     3,
       0,    10,     9,     8,     0,     7,   624,     6,    11,     5,
       4,    13,    12,    14,   102,   103,   101,   110,   112,    45,
      61,    58,    59,    47,    48,    49,     0,    50,    56,    46,
     232,   231,   635,   635,    24,    23,   624,   637,   636,   807,
     797,   802,     0,   285,    43,   118,   119,   120,     0,     0,
       0,   121,   123,   130,     0,   117,    19,   657,   656,   225,
     645,     0,   660,   625,   626,     0,     0,     0,     0,    51,
       0,    57,     0,     0,    54,     0,     0,   635,     0,    20,
       0,     0,     0,   287,     0,     0,   129,   124,     0,     0,
       0,     0,     0,     0,   133,   227,   226,   229,   224,   647,
     646,     0,   659,   658,   662,   661,   665,   628,   627,   630,
     108,   109,   106,   107,   105,     0,     0,   104,   113,    62,
      60,    56,    53,    52,   638,   564,   234,   233,   642,     0,
     644,    21,    22,    25,   808,   798,   803,   286,    41,    44,
     128,     0,   125,   126,   127,   131,     0,   648,     0,   653,
     621,   550,    26,    27,    31,     0,    97,    98,    95,    96,
      94,    93,    99,     0,    55,     0,   565,   566,   640,   564,
       0,     0,     0,    42,   122,     0,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,     0,     0,   139,   134,     0,     0,   632,   654,     0,
     666,   622,     0,     0,   552,     0,    28,    29,    30,     0,
     111,     0,   641,   716,   799,   804,   193,   194,   191,   142,
     143,   145,   144,   146,   147,   148,   149,   175,   176,   173,
     174,   166,   177,   178,   167,   164,   165,   192,   186,     0,
     190,   179,   180,   181,   182,   153,   154,   155,   150,   151,
     152,   163,     0,   169,   170,   168,   161,   162,   157,   156,
     158,   159,   160,   141,   140,   185,     0,   171,   172,   550,
     137,   262,   230,   700,   703,   706,   707,   701,   704,   702,
     705,     0,     0,   651,   663,   629,   550,     0,   114,     0,
     116,     0,   611,   609,   631,   100,     0,     0,     0,     0,
       0,     0,   673,   693,   674,   709,   675,   679,   680,   681,
     682,   699,   686,   687,   688,   689,   690,   691,   692,   694,
     695,   696,   697,   767,   678,   685,   698,   774,   781,   676,
     683,   677,   684,     0,     0,     0,     0,   708,   729,   732,
     730,   731,   794,   639,   714,   588,   594,   195,   196,   189,
     184,   197,   187,   183,     0,   135,   316,   579,   580,     0,
       0,   228,     0,   632,   564,   649,     0,   655,   567,   667,
       0,     0,   115,     0,     0,     0,     0,   610,     0,   735,
     758,   761,     0,   764,   754,     0,   720,   768,   775,   782,
     788,   791,     0,     0,   744,   749,   743,     0,   757,   753,
     746,     0,     0,   748,   733,     0,   717,   714,     0,   800,
     805,   198,   188,     0,   314,   315,     0,   550,   550,   550,
     136,   138,   264,     0,     0,     0,    71,    72,    84,   493,
     494,     0,     0,     0,     0,   302,   488,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,     0,
       0,     0,     0,     0,     0,   699,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   539,     0,     0,     0,   240,   370,   372,   371,   373,
     374,   375,   376,     0,     0,    37,   231,   241,     0,     0,
       0,     0,     0,   266,     0,   352,   353,   491,   490,     0,
       0,     0,     0,   257,   252,   249,   248,   250,   251,   270,
     235,   284,   263,   243,   530,   242,   447,     0,   521,    79,
      80,    77,   255,    78,   256,   258,   320,   247,   520,   519,
     518,   132,   524,   489,     0,   244,   523,   522,   496,   448,
     497,   532,   531,   533,   377,   449,     0,   492,   810,   814,
     811,   812,   813,     0,     0,   633,   652,   569,   564,   551,
       0,     0,     0,   530,   533,     0,   613,   614,     0,   607,
     608,   606,     0,     0,     0,     0,   533,     0,     0,     0,
       0,     0,   134,     0,   134,     0,   134,     0,     0,   533,
     740,   266,   751,   752,   745,   747,     0,   750,   734,   533,
       0,   796,   795,   718,   719,   715,   809,   285,   723,   724,
       0,   589,   584,     0,     0,     0,   595,     0,   533,     0,
     668,   576,   577,   599,   581,   582,   583,   856,   859,     0,
       0,     0,   290,   294,   293,   299,     0,     0,   338,     0,
       0,     0,   892,     0,     0,   306,   303,     0,   533,   377,
       0,     0,   270,     0,   288,     0,     0,     0,   329,   332,
       0,   261,   335,     0,     0,    65,     0,    88,   896,     0,
     865,   874,     0,   862,     0,     0,   311,   308,   476,   477,
     353,   365,   132,   283,   281,   282,     0,     0,     0,     0,
       0,     0,     0,   834,     0,     0,     0,   872,   899,     0,
     274,     0,   277,     0,     0,   453,   452,   486,   451,   450,
       0,     0,     0,   910,   910,   354,   259,   260,     0,    82,
       0,     0,   901,   910,     0,   368,   238,   528,     0,   246,
     253,   254,   305,   310,   319,     0,   362,     0,   245,     0,
       0,     0,     0,   478,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   237,     0,   650,   564,     0,   568,     0,
     664,   550,   623,     0,   612,     0,   616,     0,   620,   377,
       0,     0,     0,   725,   738,     0,     0,   710,   712,     0,
       0,   137,     0,   137,     0,   137,   586,     0,   592,     0,
     711,     0,     0,   742,   727,   533,     0,   590,   801,     0,
     596,   806,   578,     0,     0,   598,     0,   597,     0,   600,
       0,     0,     0,    89,     0,     0,     0,   848,     0,     0,
       0,     0,   882,   885,   888,     0,   910,   307,   304,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     289,   533,    90,    91,     0,     0,   533,     0,    63,    64,
     533,     0,   910,     0,     0,   910,     0,     0,   312,   309,
     354,   362,   533,   533,   533,   533,   533,   533,     0,     0,
       0,   285,     0,     0,     0,   868,   826,   834,     0,   877,
       0,     0,     0,   834,     0,     0,     0,     0,   837,     0,
     904,     0,   265,     0,    40,     0,    38,     0,     0,     0,
       0,   879,   911,   267,     0,     0,     0,   424,   421,   423,
     564,     0,     0,   903,   911,   271,   262,   132,     0,   266,
     440,     0,   910,     0,     0,     0,   328,   327,     0,     0,
     134,   280,     0,     0,   507,   506,     0,     0,   508,   512,
     454,   455,   467,   468,   465,   466,   502,     0,   484,   533,
     525,   526,   527,   456,   457,   472,   473,   474,   475,     0,
     533,   470,   471,   469,   463,   464,   459,   458,   460,   461,
     462,     0,     0,     0,     0,     0,   533,     0,     0,     0,
     399,   408,   387,   409,   388,   411,   390,   410,   389,   412,
     391,   402,   381,   403,   382,   404,   383,   413,   392,   414,
     393,     0,   401,   379,   380,   415,   394,   416,   395,   405,
     384,   406,   385,   407,   386,   400,   378,   430,     0,     0,
       0,   445,     0,   634,     0,   558,   561,     0,     0,   615,
     618,   377,   619,   736,   759,   762,     0,   765,   755,   721,
       0,   769,     0,   776,     0,   783,     0,   789,     0,   792,
       0,     0,   272,   739,   728,   585,   591,   533,   670,   671,
     602,   601,     0,     0,     0,     0,     0,   849,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,   387,   388,   390,   389,   391,   381,   382,   383,
     392,   393,   379,   394,   395,   384,   385,   386,   378,   313,
       0,     0,   905,   498,     0,     0,     0,   499,     0,   529,
       0,     0,     0,     0,     0,     0,   362,   134,   534,   535,
     536,   537,   538,     0,     0,   835,     0,     0,     0,     0,
     347,   834,     0,     0,     0,     0,   843,   844,     0,   851,
       0,   841,     0,     0,   880,     0,     0,     0,     0,   839,
     881,     0,   871,   836,   900,     0,     0,    34,     0,     0,
       0,     0,     0,   495,     0,     0,     0,   134,    68,    73,
       0,   902,     0,   362,     0,   441,     0,     0,     0,   444,
     911,   825,   442,   321,     0,     0,     0,     0,     0,     0,
       0,     0,   360,     0,   137,   503,     0,   509,     0,     0,
     482,     0,     0,   513,   517,     0,     0,   485,   533,     0,
       0,   533,   480,   398,   396,   397,     0,     0,   431,   438,
       0,   446,   133,   559,   560,   561,   562,   553,   564,   362,
     617,   737,   760,   763,   726,   766,   756,   722,   713,   770,
     772,   777,   779,   784,   786,   790,   587,   793,   593,     0,
       0,   669,     0,     0,   857,   860,     0,     0,   291,     0,
     296,   297,   295,     0,   341,     0,     0,   344,   339,     0,
       0,     0,   893,   891,   270,     0,     0,   330,   333,   336,
     897,   895,   866,   875,   873,   863,     0,   137,     0,     0,
     533,   816,   815,   834,     0,   869,     0,     0,   827,   850,
     842,   870,   840,   878,     0,   834,     0,   846,   847,   854,
     838,     0,   275,   278,    35,     0,     0,     0,     0,     0,
       0,   533,   533,     0,   137,     0,    66,    67,     0,    81,
      74,     0,     0,   236,   134,   443,     0,   266,     0,     0,
     609,     0,   357,   358,     0,   356,   355,     0,     0,     0,
       0,   533,     0,   533,   533,   514,     0,   487,     0,   483,
     533,     0,   481,   419,     0,   266,   432,     0,   439,   435,
       0,   564,   556,   553,   554,   555,   558,   570,     0,   773,
     780,   787,   266,   273,   672,   603,     0,     0,    85,     0,
     292,   298,     0,     0,     0,     0,   340,   883,   886,   889,
       0,     0,     0,    92,     0,     0,     0,     0,   848,     0,
       0,     0,   239,     0,   540,     0,     0,     0,     0,   852,
       0,     0,     0,     0,     0,   845,     0,     0,   268,    32,
      39,     0,     0,     0,     0,     0,   422,   549,   425,     0,
      69,     0,     0,    83,     0,   137,   417,     0,   322,   609,
       0,     0,   364,     0,   361,   363,     0,   349,   367,     0,
     548,     0,   546,   543,     0,   533,   533,   542,   533,   420,
       0,   433,   436,   575,   557,   571,   556,     0,     0,   848,
     848,     0,     0,     0,     0,     0,     0,     0,     0,   266,
     906,   270,   331,   334,   337,     0,   849,   867,   848,   848,
     500,   533,   366,   541,   533,     0,     0,   853,   533,   818,
     817,   533,   855,   908,   276,   266,   279,    36,     0,     0,
     533,     0,     0,     0,   428,    68,   302,     0,    75,    79,
      80,    77,    78,    76,   908,     0,     0,     0,     0,     0,
       0,   359,     0,   350,   504,   510,     0,   547,   545,   544,
       0,     0,   573,   605,   604,   563,   741,   833,   833,    87,
     342,     0,   345,     0,   848,   848,   823,     0,     0,   910,
     833,   823,   501,     0,     0,   820,   819,     0,     0,     0,
     910,    33,     0,     0,     0,     0,   533,   426,     0,    70,
       0,     0,   308,     0,   369,   418,     0,     0,     0,     0,
     351,   505,   511,   515,   434,     0,     0,     0,   830,   910,
     832,     0,     0,     0,     0,   833,   833,   824,     0,   894,
     907,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     911,     0,   908,   533,   912,     0,     0,   429,   309,   919,
       0,     0,     0,   326,   516,   564,     0,     0,   911,   831,
     858,   861,   343,   346,     0,     0,   890,   898,   876,   864,
     909,   917,   822,   821,   918,   920,   269,     0,     0,   533,
     916,     0,   325,   324,   572,   574,   828,     0,   884,   887,
     915,   913,     0,   323,     0,   914,   829
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1561, -1561,    -1, -1561, -1561, -1561, -1561,   716,  1373, -1561,
   -1561, -1561, -1561, -1561, -1561,  1465, -1561, -1561, -1561,   995,
    1424, -1561,  1331, -1561, -1561,  1385, -1561, -1561, -1561,  -131,
   -1561, -1561, -1561,   -46, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561,  1238, -1561, -1561,   -44,   -55, -1561,
   -1561, -1561,   642,   653,  -536,  -581,  -829, -1561, -1561, -1561,
   -1560, -1561, -1561,    -4,  -227,  -223,  -375, -1561,   487, -1561,
    -611, -1561,  -677,   -29,  1216, -1561, -1561, -1561, -1561,  -424,
      -2, -1561, -1561, -1561, -1561, -1561,   -41,   -37,   -34, -1561,
     -31, -1561, -1561, -1561,  1497, -1561,   494, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
     443,     8,   508,    75,   234,  -902,  -438, -1561,  -532, -1561,
   -1561,  -381,  1184, -1561, -1561, -1561, -1515, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561,  1463,   948, -1561,  -148,    53,   -51,
      54,   199, -1561,  -188, -1561, -1561, -1561, -1561, -1561, -1561,
     680,  -421,  -894, -1561,  -425,  -893, -1561,  -644,   -50,   -48,
   -1561,  -590, -1359, -1561,  -368, -1561, -1561,  1455, -1561, -1561,
   -1561,  1120,  1123,   251, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
    -623,   165, -1561,  1054, -1561, -1561,  1075, -1561, -1561, -1561,
   -1561,  -391, -1561, -1561,  -390, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561,  -235, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561,  1057,  -696,  -135,  -849,
    -705, -1561, -1561, -1151,  -929, -1561, -1561, -1561, -1179,    15,
   -1217, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
     297,  -497, -1561, -1561, -1561,   799, -1561, -1561, -1561, -1561,
   -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561, -1561,
   -1159,  -723, -1561
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   639,    18,   153,    56,    19,   175,   181,  1597,
    1395,  1509,   733,   527,   159,   528,   105,    21,    22,    47,
      48,    49,    94,    23,    41,    42,   529,   530,  1408,  1409,
     531,  1411,  1522,   532,   533,   991,   534,   660,   661,   535,
     536,   537,   538,  1184,   182,   183,    37,    38,    39,   234,
      71,    72,    73,    74,    24,   300,   391,   224,    25,   117,
     225,   118,   166,   539,   148,   711,  1060,   542,   392,   543,
     742,  1596,   751,  1131,   593,   971,  1507,   973,  1508,   545,
     546,   547,   663,   886,  1470,   548,   549,   550,   551,   552,
     553,   554,   555,   446,   556,   765,  1264,  1006,   557,   558,
     924,  1484,   925,  1485,   927,  1486,   559,   891,  1476,   560,
     743,  1538,   561,  1272,  1273,  1010,   713,   562,   823,   997,
     563,   677,  1061,   565,   566,   567,   989,   568,  1246,  1603,
    1247,  1668,   569,  1099,  1448,   570,   571,   572,  1430,  1681,
    1432,  1682,  1544,  1724,   573,   595,   575,   386,  1456,  1555,
    1305,  1307,  1107,   188,   587,   831,  1631,  1686,   387,   388,
     389,   856,   857,   439,   858,   859,   440,  1208,   651,   652,
    1635,   602,   408,   323,   324,   231,   316,    84,   129,    27,
     171,   312,    95,    96,   185,    97,    28,    53,   121,   168,
      29,   397,   229,   230,    82,   126,   399,    30,   169,   314,
     653,   576,   311,   369,   370,   849,   438,   374,   371,   610,
    1317,  1128,   842,   434,   372,   603,  1311,   861,   608,  1316,
     604,  1312,   605,  1313,   607,  1315,   611,  1319,   612,  1459,
     613,  1321,   614,  1460,   615,  1323,   616,  1461,   617,  1325,
     618,  1327,   642,    31,   101,   191,   375,   643,    32,   102,
     192,   376,   647,    33,   100,   190,   577,  1688,  1698,  1003,
     957,  1689,  1690,  1691,   958,   970,  1230,  1224,  1219,  1389,
    1150,   578,   881,  1466,   882,  1467,   936,  1490,   933,  1488,
     959,   752,   579,   934,  1489,   960,   580,  1156,  1566,  1157,
    1567,  1158,  1568,   895,  1480,   931,  1487,   729,   753,   581,
    1654,   983,   582
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   242,   302,    52,   315,   919,   373,   728,   837,   880,
     862,   564,   756,    64,    75,   645,   598,   541,   640,    76,
     963,   984,  1122,   235,  1124,   766,  1126,   956,  1215,   956,
     995,   851,   588,   853,  1227,   855,  1379,   884,  1149,  1197,
    1193,   675,   138,  1195,   669,   632,  1410,  1144,   641,   646,
     624,   829,   176,   177,   161,   714,   715,    65,   622,  -132,
     949,  1531,  1002,    90,   949,   706,    75,    75,    75,   875,
    1204,  1685,  1225,    57,   813,  1231,    62,    40,  1008,    58,
    1240,  1250,  1337,   813,   232,   395,    66,  1396,  1667,  1397,
     541,   813,   950,  1400,   813,   145,  1514,   150,    91,   948,
      50,   961,   813,   965,    13,   813,    63,  1254,    75,    75,
      75,    75,   723,   725,   541,   444,   813,   979,    34,    35,
      51,   966,   130,   131,    14,   170,  1726,   992,    83,   424,
      67,    50,  1212,   876,   877,    54,   878,   875,  1119,   879,
     321,   670,   671,    50,   233,   110,   111,   112,   189,  1256,
     666,    51,  1255,  1717,   696,   451,   425,   426,   445,    68,
     813,   322,   822,    51,   227,   540,    62,     2,   400,   950,
    1618,   822,    13,  1160,     3,  1344,   941,   317,   838,   822,
     146,    55,   822,  1347,  1254,    85,   243,   244,   245,   318,
     822,  1002,    14,   822,  1640,  1499,    63,     4,   813,     5,
     147,     6,   877,   178,   822,   435,   585,     7,   179,  1191,
     180,   636,  1194,   135,   625,   146,    13,     8,   876,   160,
     427,   301,   623,     9,   428,   769,   770,   541,   813,   769,
     770,    69,   626,   320,  1009,   147,    14,   672,   627,   367,
      70,   876,    87,   876,    92,   680,   876,    10,   822,   867,
     897,   876,   876,   876,   870,    93,   673,   813,   876,   697,
     876,  1454,  1145,  1350,   876,  1351,   827,   876,   757,    11,
      12,  1577,    36,   132,    86,   402,    62,  1027,   133,  1261,
     134,   938,  1374,   135,    77,    78,   822,    79,   649,   429,
    1642,   918,  1028,   430,  1366,  1196,   431,   813,  1339,   541,
     384,   194,  1002,  1257,    98,   232,    63,   393,   813,   650,
     398,   432,   758,   813,   937,    80,   822,   433,   599,  1587,
    1732,  1455,   773,   774,   136,   113,   773,   774,   600,  1011,
     541,   239,    13,   780,   781,   782,   779,   780,   781,   782,
     783,   104,  1637,  1638,   783,   822,    86,   186,   149,  1733,
     114,  1414,    14,   978,  1238,  1239,   825,  1242,   240,    13,
     830,  1650,  1651,   385,   423,   233,   813,   187,   840,   841,
     843,    15,   845,   846,  1536,  1258,   850,   875,   852,    14,
     854,  1394,    16,   601,   620,   822,    81,  1207,   367,   103,
     525,   710,   866,   104,   586,    50,   822,  1007,   780,   781,
     832,   822,  1259,   621,   367,    13,   368,  1458,   796,   797,
     798,   799,   800,  1110,   319,    51,   868,   649,   801,   802,
     871,   384,   801,   802,    88,    14,   232,  1695,  1696,  1274,
     875,   104,  1658,   367,  1659,   950,   883,   367,   650,  1267,
    1207,  1664,   877,  1108,  1498,  1429,   649,   875,   899,  1268,
    1254,  1002,  1116,   899,   822,  1673,  1504,  1501,   321,   367,
     667,  1253,   541,   922,   301,   435,  1004,   650,  1130,  1113,
    1216,  1217,  1234,  1051,  1052,  1301,   109,   691,   301,   322,
    1340,   139,  1241,   801,   802,  1269,   233,  1692,   301,   301,
     301,  1142,  1143,   158,  1687,   877,  1333,  1251,  1218,  1702,
     367,   367,  1053,   875,  1270,  1159,  1373,  1716,   972,  1271,
      93,  1205,   877,  1585,   767,   956,   875,    91,  1378,    13,
      50,  1385,   141,  1005,   993,   990,  1706,  1181,   746,   747,
     956,   453,   454,   144,  1185,  1186,    86,  1188,  1493,    14,
      51,  1190,  1221,  1192,  1734,  1735,   759,    13,   760,   761,
     762,   464,   763,  1747,  1748,   368,   154,   469,  1054,  1055,
     186,   401,    13,   768,    13,   769,   770,    14,   877,    62,
    1222,   368,    13,    57,   155,  1519,   525,   710,   985,    58,
     187,   877,    14,   986,    14,   156,   828,   584,   638,   113,
    1762,   967,    14,   968,   483,   484,  1127,   836,  1345,    63,
     368,   367,   367,   367,   368,   367,   367,    13,  1418,   367,
      13,   367,   146,   367,  1302,   367,  1367,    50,  1399,  1293,
    1294,  1295,   769,   770,   987,  1427,  1132,    14,   486,   487,
      14,  1135,   147,   638,  1136,  1420,   638,    51,  1103,   635,
      50,  1129,   681,  1140,  1329,   899,  1405,    43,    44,    45,
      13,  1146,  1532,   115,   899,   301,  1155,  1406,  1407,   116,
      51,   682,   773,   774,   888,    13,  1404,   368,   368,    62,
      14,   226,   779,   780,   781,   782,   638,  1481,    46,   367,
     783,  1549,   301,  1106,  1363,    14,   502,   503,   504,    50,
     424,   638,    62,    13,  1179,   435,  1615,   698,   899,    63,
     448,   985,   367,   186,   157,  1328,   449,  1326,   899,   516,
     106,   107,   108,    14,  1213,   899,   699,   425,   426,   773,
     774,  1527,    63,   187,   662,  1551,  1418,   301,   969,   779,
     780,   781,   782,  1502,   899,  1552,  1132,   783,  1132,  1416,
    1330,   523,  1561,   988,   301,    13,   798,   799,   800,  1550,
     996,  1419,   162,   163,   164,   165,   899,  1530,   801,   802,
     407,  1583,   899,   301,   167,    14,  1349,    13,   368,   368,
     368,   638,   368,   368,   110,  1427,   368,    13,   368,  1365,
     368,   427,   368,  1598,   599,   428,    13,    14,  1213,  1599,
     899,  1355,   193,   638,   600,  1286,  1213,    14,    13,  1213,
    1428,  1392,  1248,   638,  1130,  1213,    14,   146,   424,  1452,
    1287,  1393,   638,  1616,  -771,   801,   802,  1665,    14,  -771,
    1472,    13,    13,  1756,   638,    75,  1766,   147,   950,  1306,
    1630,   367,  1478,  1525,  1492,   425,   426,  -771,  1112,   701,
      13,    14,    14,  1254,  1330,  1330,   368,   638,  1120,   601,
     429,  1558,   228,  1586,   430,  1479,  1403,   431,   702,   367,
      14,   297,    13,  -778,   367,   436,   638,   367,  -778,   368,
     644,   564,   432,   437,   241,  1376,   367,   541,   433,   367,
     367,  1314,    14,   298,   367,    13,  -778,   889,   638,   367,
    1377,   303,   119,   367,  1648,   304,  -785,  -427,   120,   427,
     299,  -785,  -427,   428,  1228,    14,   890,  1229,   676,   305,
     306,   313,   424,   301,   307,   308,   309,   310,   146,  -785,
    -427,   377,   367,   367,   435,   367,  1701,   394,  1114,   367,
     405,   367,   367,   406,  1015,  1019,   407,  1711,   147,   425,
     426,  1620,  1235,  1386,  1236,   301,  1387,   124,  1032,  1388,
    1697,   127,  1473,   125,   378,  1697,  1214,   128,  1647,  1223,
    1130,   435,  1214,  1223,   151,  1117,  1729,   744,   429,   367,
     152,   367,   430,  1303,  1309,   431,  1496,  1503,   379,  1304,
    1506,   435,   899,   899,  1660,  1118,   899,  1100,  1513,   424,
     432,  1727,  1617,  1524,   899,   407,   433,   381,   368,   899,
     712,   712,   712,   427,  1602,   122,   123,   428,   435,  1593,
     899,   390,  1121,  1707,   382,   435,   425,   426,   435,  1123,
     435,   110,  1125,   112,  1338,  1413,   368,  1614,  1677,  1678,
     380,   368,   435,   435,   368,   435,  1348,  1625,   396,  1683,
    1539,   525,   710,   368,    13,   383,   368,   368,   110,   111,
     112,   368,   755,   301,   301,   301,   368,    43,    44,    45,
     368,  1764,   236,   237,    14,   172,   173,   974,   975,   403,
     638,  1491,   429,   525,   710,   404,   430,   412,  1180,   431,
     427,   409,   755,   599,   428,   410,  1720,   142,   143,   368,
     368,   411,   368,   600,   432,  1463,   368,   413,   368,   368,
     433,   172,   173,   174,   424,   435,  1308,   414,   415,   -76,
    1310,   416,   417,  1468,   367,   418,   896,   898,   419,   420,
    1457,   236,   237,   238,   421,   422,   367,   654,   655,   656,
     441,   425,   426,    59,    60,    61,   368,   447,   368,   442,
    1619,   450,   932,   596,   443,   935,   589,   597,   601,   429,
     609,   630,  1346,   430,   367,   367,   431,   633,   657,   658,
     -86,   668,   685,   687,   688,  1712,   689,   690,   716,   692,
     693,   432,   717,   734,   700,   718,   703,   433,   704,   367,
     732,   719,  1520,   749,   720,   745,   721,   755,   754,   824,
     764,   833,   730,   835,    16,   427,   848,  1541,   696,   428,
     644,  1001,   847,   863,  1284,   635,  1537,   803,   804,   805,
     806,   807,   808,   809,   810,   755,   869,   874,   873,   892,
     811,   812,   885,   887,   893,   813,   814,  1676,   894,   899,
     767,   917,   923,   928,   929,   940,   815,   977,   980,   816,
     817,   981,   982,   994,   367,  1081,   818,   819,   820,  1105,
    1109,  1133,  1115,  1138,  1139,  1153,  1213,  1220,  1233,  1182,
    1206,   769,   770,  1553,   429,  1243,  1244,  1102,   430,  1260,
    1537,   431,  1245,  1263,  1262,   712,  1266,  1275,  1276,  1278,
    1279,   368,  1277,  1290,  1289,  1298,   432,  1296,  1299,  1306,
    1297,   821,   433,   368,  1318,  1320,   424,  1332,  1605,  1343,
    1300,  1451,  1322,  1324,  1341,  1342,  1353,  1354,  1368,  1626,
    1361,  1364,  1415,   822,  1422,  1423,  1425,  1380,   525,   710,
    1426,   368,   368,   425,   426,  1382,   424,  1390,   367,  1435,
    1148,  1424,   301,  1436,  1438,  1441,  1462,  1464,  1639,  1474,
     574,  1475,  1510,  1528,  1505,  1471,   368,   755,  1526,  1500,
    1529,  1533,  1535,   425,   426,  1483,   771,   772,   773,   774,
     775,  1554,  1491,   776,   777,   778,  1559,  1563,   779,   780,
     781,   782,  1214,  1560,  1565,  1569,   783,  1570,   784,   785,
     424,  1576,  1571,  1575,  1214,  1604,  1578,   427,  1579,  1595,
    1641,   428,  1674,  1662,  1621,  1636,  1643,   744,  1644,  1645,
    1646,   301,  1704,   659,  1661,   744,  1663,   425,   426,  1705,
    1523,   368,  1708,   679,  1675,  1684,  1699,   427,   684,  1700,
     686,   428,  1710,  1714,  1715,   301,  1728,  1730,   367,   695,
     367,  1731,  1709,  1736,  1760,  1741,  1737,   707,   794,   795,
     796,   797,   798,   799,   800,  1744,  1719,   755,  1738,   976,
    1739,  1746,   727,  1750,   801,   802,   429,   712,   137,  1757,
     430,   731,  1334,   431,  1758,  1759,    20,  1761,  1765,   424,
      89,   427,   184,   140,  1669,   428,  1608,   325,   432,   748,
    1104,  1609,  1745,  1252,   433,  1610,   429,   727,  1611,   301,
     430,  1612,  1335,   431,   424,   368,   425,   426,    26,  1265,
    1421,  1582,  1534,  1755,  1453,  1632,  1556,  1633,   432,  1634,
    1557,    99,   634,   583,   433,   664,  1703,   301,   665,  1592,
    1383,   425,   426,     0,   964,   424,     0,     0,   367,     0,
       0,     0,   826,     0,     0,     0,     0,  1754,     0,     0,
     429,     0,   367,     0,   430,     0,  1352,   431,     0,     0,
     839,     0,   425,   426,     0,     0,     0,   301,     0,     0,
     427,     0,   432,     0,   428,   712,   712,   712,   433,     0,
     755,     0,   755,     0,   755,     0,   755,     0,   755,     0,
     755,     0,   755,  1653,   755,   427,   755,  1336,   755,   428,
    1653,   755,  1653,     0,   755,   368,   755,   368,   755,  1653,
     755,   988,   755,     0,   755,     0,     0,     0,   544,     0,
       0,   301,     0,  1653,     0,   367,   427,     0,     0,   755,
     428,     0,   679,     0,     0,     0,     0,     0,     0,   429,
     727,   920,     0,   430,     0,  1357,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1369,     0,     0,
       0,   432,     0,   939,   429,     0,     0,   433,   430,     0,
    1358,   431,     0,     0,     0,  1653,   988,     0,  1384,     0,
       0,   955,     0,   955,  1391,     0,   432,     0,  1723,     0,
       0,   544,   433,  1398,  1725,   429,     0,     0,     0,   430,
       0,  1359,   431,  1412,     0,   368,     0,     0,     0,   769,
     770,     0,     0,     0,     0,   544,     0,   432,     0,   368,
       0,  1653,  1653,   433,     0,     0,     0,     0,     0,     0,
    1752,  1753,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1444,
    1447,     0,     0,  1450,     0,     0,     0,     0,  1653,     0,
    1763,  1062,  1064,  1066,  1068,  1070,  1072,  1074,  1076,  1078,
    1080,     0,  1083,  1084,  1086,  1088,  1090,  1092,  1094,  1096,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1469,     0,   368,     0,     0,  1111,     0,     0,     0,     0,
       0,     0,   424,     0,   771,   772,   773,   774,   775,     0,
       0,   776,     0,     0,     0,     0,   779,   780,   781,   782,
       0,  1495,     0,     0,   783,     0,   784,   785,   544,   425,
     426,     0,     0,     0,     0,     0,     0,     0,  1141,     0,
       0,     0,     0,     0,     0,     0,  1152,     0,  1154,  1511,
    1512,     0,     0,  1515,     0,     0,     0,     0,  1162,  1163,
    1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,
    1174,  1175,  1176,  1177,  1178,     0,   594,     0,     0,     0,
       0,     0,     0,     0,   755,   606,   794,   795,   796,   797,
     798,   799,   800,   427,     0,   619,     0,   428,     0,     0,
     544,     0,   801,   802,     0,   629,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   648,     0,     0,     0,
       0,     0,     0,     0,  1232,     0,   424,   727,     0,     0,
       0,   544,     0,  1237,     0,     0,     0,     0,   678,     0,
       0,     0,     0,     0,     0,     0,     0,  -347,     0,  1249,
       0,     0,   727,   425,   426,     0,     0,     0,   769,   770,
       0,     0,   429,   708,   709,     0,   430,  1601,  1360,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
     433,   735,   736,   737,   738,   739,   678,   678,  1063,  1065,
    1067,  1069,  1071,  1073,  1075,  1077,  1079,     0,  1082,     0,
    1085,  1087,  1089,  1091,  1093,  1095,     0,   427,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,  1649,   544,
     544,   544,   544,   544,   544,   544,   544,   544,   544,     0,
     544,     0,   544,   544,   544,   544,   544,   544,     0,     0,
       0,     0,     0,   771,   772,   773,   774,   775,     0,   676,
     776,   777,   778,   544,     0,   779,   780,   781,   782,     0,
       0,     0,     0,   783,     0,   784,   785,     0,     0,     0,
       0,   786,   787,   788,     0,     0,   429,   789,     0,     0,
     430,     0,  1362,   431,   678,     0,     0,     0,     0,     0,
    -347,     0,  -347,   865,     0,   769,   770,     0,   432,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
    -347,     0,  -347,   898,     0,     0,     0,     0,  -347,     0,
     790,     0,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,  1356,     0,     0,     0,   678,   678,     0,     0,
       0,   801,   802,  -347,     0,  -347,     0,     0,   921,     0,
       0,     0,     0,   926,     0,  1371,  1372,     0,     0,   930,
    1375,     0,   678,     0,     0,   678,     0,     0,  1381,     0,
       0,   955,     0,     0,     0,     0,     0,     0,     0,   942,
     943,   944,   945,   946,   947,     0,     0,     0,     0,     0,
     771,   772,   773,   774,   775,     0,     0,   776,   777,   778,
     574,     0,   779,   780,   781,   782,     0,     0,     0,     0,
     783,     0,   784,   785,     0,     0,     0,     0,   786,   787,
     788,   678,     0,     0,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1020,  1021,     0,     0,  1022,  1023,
    1024,  1025,  1026,     0,  1029,  1030,  1031,  1033,  1034,  1035,
    1036,  1037,  1038,  1040,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,  1049,  1050,     0,  1056,     0,   790,     0,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,     0,
       0,  1465,     0,     0,     0,     0,     0,   678,   801,   802,
       0,     0,   834,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   727,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   722,     0,     0,   424,
       0,     0,   326,     0,     0,     0,     0,     0,   327,     0,
       0,     0,   424,     0,   328,     0,  1137,     0,     0,     0,
       0,     0,     0,     0,   329,     0,   425,   426,     0,     0,
     678,     0,   330,     0,     0,     0,  1521,     0,     0,   425,
     426,     0,  1161,     0,     0,     0,     0,   331,     0,     0,
       0,     0,     0,     0,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,     0,     0,     0,
     427,   424,     0,     0,   428,   735,  1210,   678,     0,     0,
    1562,     0,  1564,   427,     0,   678,     0,   428,     0,     0,
     424,     0,  1572,  1573,  1574,     0,     0,     0,   425,   426,
       0,   769,   770,     0,     0,  1161,    62,     0,     0,  1589,
    1590,     0,     0,     0,   727,  1594,     0,   425,   426,   365,
       0,     0,   678,     0,     0,     0,     0,     0,   544,     0,
    1613,     0,   727,     0,     0,     0,    63,     0,     0,   429,
       0,     0,     0,   430,  1623,  1477,   431,     0,     0,     0,
       0,     0,   429,     0,     0,     0,   430,     0,  1482,   431,
       0,   432,   427,     0,     0,     0,   428,   433,     0,     0,
       0,     0,     0,     0,   432,  1288,     0,     0,  1291,   727,
     433,   427,   366,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,  1655,  1656,     0,   771,   772,   773,   774,
       0,     0,     0,     0,     0,     0,     0,     0,   779,   780,
     781,   782,     0,     0,     0,  1672,   783,     0,   784,   785,
       0,     0,     0,     0,     0,     0,     0,     0,  1679,     0,
    1680,   429,     0,     0,     0,   430,     0,  1518,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1693,
     429,  1694,     0,   432,   430,     0,  1624,   431,     0,   433,
       0,     0,   769,   770,     0,     0,     0,   678,     0,     0,
     727,     0,   432,     0,     0,     0,     0,     0,   433,  1718,
     796,   797,   798,   799,   800,  1721,  1722,     0,     0,     0,
       0,     0,     0,     0,   801,   802,     0,     0,     0,     0,
       0,     0,     0,     0,   900,   901,   902,   903,   904,   905,
     906,   907,  1740,     0,  1742,  1743,     0,   908,   909,     0,
       0,     0,   813,   910,     0,     0,     0,   678,  1751,  1370,
       0,     0,   678,   815,     0,     0,   911,   912,     0,     0,
       0,     0,     0,   913,   914,   915,     0,     0,   678,     0,
       0,     0,     0,     0,   678,     0,     0,   771,   772,   773,
     774,   775,     0,   678,   776,   777,   778,  1401,  1402,   779,
     780,   781,   782,   678,     0,     0,     0,   783,     0,   784,
     785,  1161,     0,     0,     0,   786,   787,   788,   916,     0,
       0,   789,     0,     0,     0,     0,     0,     0,     0,  1431,
       0,  1433,  1434,     0,     0,     0,     0,     0,  1437,     0,
     822,     0,  1440,     0,     0,   525,   710,     0,     0,   678,
     678,     0,     0,   678,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   790,     0,   791,   792,   793,   794,
     795,   796,   797,   798,   799,   800,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   801,   802,     0,     0,   844,
     678,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   769,   770,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,   333,
     334,   678,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   678,
     678,     0,     0,   678,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     678,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1545,
       0,  1546,     0,     0,  1548,     0,     0,     0,   678,   771,
     772,   773,   774,   775,     0,     0,   776,   777,   778,     0,
       0,   779,   780,   781,   782,   678,     0,     0,     0,   783,
    1097,   784,   785,     0,     0,     0,     0,   786,   787,   788,
       0,     0,     0,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1581,     0,     0,     0,     0,  1584,
       0,     0,     0,  1588,     0,     0,  1591,     0,     0,     0,
       0,  1098,     0,     0,     0,     0,  1600,   678,     0,     0,
       0,     0,     0,     0,     0,     0,   790,     0,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   801,   802,     0,
       0,   860,     0,     0,     0,     0,   900,   901,   902,   903,
     904,   905,   906,   907,     0,     0,     0,     0,     0,   908,
     909,     0,   678,     0,   813,   910,     0,     0,   678,     0,
    -377,     0,     0,     0,     0,   815,     0,     0,   911,   912,
       0,     0,     0,     0,     0,   913,   914,   915,   678,     0,
       0,     0,     0,     0,     0,  1666,     0,   452,     0,   678,
       0,   453,   454,     3,     0,   455,   456,   457,     0,   458,
       0,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,   464,   465,   466,   467,   468,     0,   469,     0,     0,
     916,     0,     0,     0,   470,   471,     0,     0,   472,     0,
     473,   474,     0,     0,   475,     0,     8,   476,   477,     0,
     478,   479,   822,     0,   480,   481,  1713,   525,   710,     0,
       0,   482,     0,   678,   483,   484,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     488,   489,     0,     0,     0,     0,     0,     0,  1749,     0,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   492,
     493,   494,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,     0,   501,   502,   503,   504,    50,
     146,   505,   506,   507,   508,   509,   510,   511,   512,    63,
     513,   514,   515,     0,     0,     0,     0,     0,     0,   516,
     147,   517,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,   453,   454,     0,
       0,   523,     0,   524,     0,   525,   526,   459,   460,   461,
     462,   463,     0,     0,     0,     0,     0,   464,     0,   466,
       0,     0,     0,   469,     0,   424,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   474,     0,     0,
     475,     0,     0,   476,     0,   949,     0,   479,     0,     0,
       0,     0,   425,   426,     0,     0,     0,   590,     0,     0,
     483,   484,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   486,   487,   591,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,   427,     0,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   495,   496,   497,   498,   499,     0,   500,
     950,   501,   502,   503,   504,    50,     0,     0,   506,   507,
     508,   509,   510,   511,   512,   951,   592,   514,   515,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,   430,
       0,     0,   952,   519,   520,     0,    15,     0,     0,   521,
     522,     0,     0,     0,   453,   454,     0,   953,     0,   954,
       0,   525,   526,   433,   459,   460,   461,   462,   463,     0,
       0,     0,     0,     0,   464,     0,   466,     0,     0,     0,
     469,     0,   424,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   474,     0,     0,   475,     0,     0,
     476,     0,     0,     0,   479,     0,     0,     0,     0,   425,
     426,     0,     0,     0,   590,     0,     0,   483,   484,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   485,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   486,   487,   591,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     495,   496,   497,   498,   499,     0,   500,   950,   501,   502,
     503,   504,    50,     0,     0,   506,   507,   508,   509,   510,
     511,   512,   951,   592,   514,   515,     0,     0,     0,     0,
       0,     0,   516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,   430,     0,     0,   952,
     519,   520,     0,    15,     0,     0,   521,   522,     0,     0,
       0,   453,   454,     0,   953,     0,   962,     0,   525,   526,
     433,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,   464,     0,   466,     0,     0,     0,   469,     0,   624,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   474,     0,     0,   475,     0,     0,   476,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   590,     0,     0,   483,   484,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     591,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,     0,   501,   502,   503,   504,    50,
       0,     0,   506,   507,   508,   509,   510,   511,   512,    63,
     592,   514,   515,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   625,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,     0,   453,   454,
       0,  1209,     0,   524,     0,   525,   526,   627,   459,   460,
     461,   462,   463,     0,     0,     0,     0,     0,   464,     0,
     466,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   474,     0,
       0,   475,     0,     0,   476,     0,     0,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   590,     0,
       0,   483,   484,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   485,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   486,   487,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,     0,
     674,     0,     0,     0,     0,     0,   492,   493,   494,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   495,   496,   497,   498,   499,     0,
     500,     0,   501,   502,   503,   504,    50,     0,     0,   506,
     507,   508,   509,   510,   511,   512,    63,   592,   514,   515,
       0,     0,     0,     0,     0,     0,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,     0,    15,     0,     0,
     521,   522,     0,     0,   453,   454,     0,     0,   523,     0,
     524,     0,   525,   526,   459,   460,   461,   462,   463,     0,
       0,     0,     0,     0,   464,     0,   466,     0,     0,     0,
     469,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   474,     0,     0,   475,     0,     0,
     476,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   590,     0,     0,   483,   484,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   485,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   486,   487,   488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,     0,     0,   705,     0,     0,     0,
       0,     0,   492,   493,   494,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     495,   496,   497,   498,   499,     0,   500,     0,   501,   502,
     503,   504,    50,     0,     0,   506,   507,   508,   509,   510,
     511,   512,    63,   592,   514,   515,     0,     0,     0,     0,
       0,     0,   516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   518,
     519,   520,     0,    15,     0,     0,   521,   522,     0,     0,
     453,   454,     0,     0,   523,     0,   524,     0,   525,   526,
     459,   460,   461,   462,   463,     0,     0,     0,     0,     0,
     464,  1606,   466,   467,     0,     0,   469,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     474,     0,     0,   475,     0,     0,   476,   477,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     590,     0,     0,   483,   484,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   485,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   486,   487,   591,
    1607,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   495,   496,   497,   498,
     499,     0,   500,     0,   501,   502,   503,   504,    50,     0,
       0,   506,   507,   508,   509,   510,   511,   512,    63,   592,
     514,   515,     0,     0,     0,     0,     0,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,     0,    15,
       0,     0,   521,   522,     0,     0,   453,   454,     0,     0,
     523,     0,   524,     0,   525,   526,   459,   460,   461,   462,
     463,     0,     0,     0,     0,     0,   464,     0,   466,     0,
       0,     0,   469,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   474,     0,     0,   475,
       0,     0,   476,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   590,     0,     0,   483,
     484,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   485,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   486,   487,   488,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   492,   493,   494,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,     0,   500,     0,
     501,   502,   503,   504,    50,     0,     0,   506,   507,   508,
     509,   510,   511,   512,    63,   592,   514,   515,     0,     0,
       0,     0,     0,     0,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,    15,     0,     0,   521,   522,
       0,     0,   453,   454,     0,     0,   523,     0,   524,     0,
     525,   526,   459,   460,   461,   462,   463,     0,     0,     0,
       0,     0,   464,     0,   466,     0,     0,     0,   469,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   474,     0,     0,   475,     0,     0,   476,     0,
       0,     0,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   590,     0,     0,   483,   484,   998,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   486,
     487,   591,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   495,   496,
     497,   498,   499,     0,   500,   950,   501,   502,   503,   504,
      50,     0,     0,   506,   507,   508,   509,   510,   511,   512,
     951,   592,   514,   515,     0,     0,     0,     0,     0,     0,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,    15,     0,     0,   521,   522,     0,     0,   453,   454,
       0,     0,   999,     0,   524,  1000,   525,   526,   459,   460,
     461,   462,   463,     0,     0,     0,     0,     0,   464,     0,
     466,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   474,     0,
       0,   475,     0,     0,   476,     0,     0,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   590,     0,
       0,   483,   484,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   485,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   486,   487,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1057,  1058,  1059,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   495,   496,   497,   498,   499,     0,
     500,     0,   501,   502,   503,   504,    50,     0,     0,   506,
     507,   508,   509,   510,   511,   512,    63,   592,   514,   515,
       0,     0,     0,     0,     0,     0,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,     0,    15,     0,     0,
     521,   522,     0,     0,     0,     0,   453,   454,   523,     0,
     524,     0,   525,   526,   740,     0,   459,   460,   461,   462,
     463,     0,     0,     0,     0,     0,   464,     0,   466,     0,
       0,     0,   469,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   474,     0,     0,   475,
     741,     0,   476,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   590,     0,     0,   483,
     484,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   485,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   486,   487,   591,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,     0,   500,     0,
     501,   502,   503,   504,    50,     0,     0,   506,   507,   508,
     509,   510,   511,   512,    63,   592,   514,   515,     0,     0,
       0,     0,     0,     0,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,    15,     0,     0,   521,   522,
       0,     0,     0,     0,   453,   454,   523,   628,   524,     0,
     525,   526,   740,     0,   459,   460,   461,   462,   463,     0,
       0,     0,     0,     0,   464,     0,   466,     0,     0,     0,
     469,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   474,     0,     0,   475,   741,     0,
     476,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   590,     0,     0,   483,   484,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   485,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   486,   487,   591,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     495,   496,   497,   498,   499,     0,   500,   950,   501,   502,
     503,   504,    50,     0,     0,   506,   507,   508,   509,   510,
     511,   512,   951,   592,   514,   515,     0,     0,     0,     0,
       0,     0,   516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   518,
     519,   520,     0,    15,     0,     0,   521,   522,     0,     0,
       0,     0,   453,   454,   523,     0,   524,     0,   525,   526,
     740,     0,   459,   460,   461,   462,   463,     0,     0,     0,
       0,     0,   464,     0,   466,     0,     0,     0,   469,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   474,     0,     0,   475,   741,     0,   476,     0,
       0,     0,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   590,     0,     0,   483,   484,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   486,
     487,   591,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   495,   496,
     497,   498,   499,     0,   500,     0,   501,   502,   503,   504,
      50,     0,     0,   506,   507,   508,   509,   510,   511,   512,
      63,   592,   514,   515,     0,     0,     0,     0,     0,     0,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,    15,     0,     0,   521,   522,     0,     0,     0,     0,
     453,   454,   523,   863,   524,     0,   525,   526,   740,     0,
     459,   460,   461,   462,   463,     0,     0,     0,     0,     0,
     464,     0,   466,     0,     0,     0,   469,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     474,     0,     0,   475,   741,     0,   476,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     590,     0,     0,   483,   484,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   485,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   486,   487,   591,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   495,   496,   497,   498,
     499,     0,   500,     0,   501,   502,   503,   504,    50,     0,
       0,   506,   507,   508,   509,   510,   511,   512,    63,   592,
     514,   515,     0,     0,     0,     0,     0,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,     0,    15,
       0,     0,   521,   522,     0,     0,   453,   454,     0,     0,
     523,     0,   524,     0,   525,   526,   459,   460,   461,   462,
     463,     0,     0,     0,     0,     0,   464,     0,   466,     0,
       0,     0,   469,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   474,     0,     0,   475,
       0,     0,   476,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   590,     0,     0,   483,
     484,  1147,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   485,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   486,   487,   591,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,     0,   500,   950,
     501,   502,   503,   504,    50,     0,     0,   506,   507,   508,
     509,   510,   511,   512,   951,   592,   514,   515,     0,     0,
       0,     0,     0,     0,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,    15,     0,     0,   521,   522,
       0,     0,   453,   454,     0,     0,   523,     0,   524,     0,
     525,   526,   459,   460,   461,   462,   463,     0,     0,     0,
       0,     0,   464,     0,   466,     0,     0,     0,   469,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   474,     0,     0,   475,     0,     0,   476,     0,
       0,     0,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   590,     0,     0,   483,   484,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   486,
     487,   591,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   495,   496,
     497,   498,   499,     0,   500,     0,   501,   502,   503,   504,
      50,     0,     0,   506,   507,   508,   509,   510,   511,   512,
      63,   592,   514,   515,     0,     0,     0,     0,     0,     0,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,    15,     0,     0,   521,   522,     0,     0,   453,   454,
       0,     0,   523,   628,   524,     0,   525,   526,   459,   460,
     461,   462,   463,     0,     0,     0,     0,     0,   464,     0,
     466,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   474,     0,
       0,   475,     0,     0,   476,     0,     0,     0,   479,     0,
       0,     0,     0,     0,   683,     0,     0,     0,   590,     0,
       0,   483,   484,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   485,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   486,   487,   591,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   495,   496,   497,   498,   499,     0,
     500,     0,   501,   502,   503,   504,    50,     0,     0,   506,
     507,   508,   509,   510,   511,   512,    63,   592,   514,   515,
       0,     0,     0,     0,     0,     0,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,     0,    15,     0,     0,
     521,   522,     0,     0,   453,   454,     0,     0,   523,     0,
     524,     0,   525,   526,   459,   460,   461,   462,   463,     0,
       0,     0,     0,     0,   464,     0,   466,     0,     0,     0,
     469,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   474,     0,     0,   475,     0,     0,
     476,     0,     0,     0,   479,     0,     0,   694,     0,     0,
       0,     0,     0,     0,   590,     0,     0,   483,   484,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   485,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   486,   487,   591,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     495,   496,   497,   498,   499,     0,   500,     0,   501,   502,
     503,   504,    50,     0,     0,   506,   507,   508,   509,   510,
     511,   512,    63,   592,   514,   515,     0,     0,     0,     0,
       0,     0,   516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   518,
     519,   520,     0,    15,     0,     0,   521,   522,     0,     0,
       0,     0,   453,   454,   523,     0,   524,     0,   525,   526,
     726,     0,   459,   460,   461,   462,   463,     0,     0,     0,
       0,     0,   464,     0,   466,     0,     0,     0,   469,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   474,     0,     0,   475,     0,     0,   476,     0,
       0,     0,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   590,     0,     0,   483,   484,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   486,
     487,   591,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   495,   496,
     497,   498,   499,     0,   500,     0,   501,   502,   503,   504,
      50,     0,     0,   506,   507,   508,   509,   510,   511,   512,
      63,   592,   514,   515,     0,     0,     0,     0,     0,     0,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,    15,     0,     0,   521,   522,     0,     0,   453,   454,
       0,     0,   523,     0,   524,     0,   525,   526,   459,   460,
     461,   462,   463,     0,     0,     0,     0,     0,   464,     0,
     466,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   474,     0,
       0,   475,     0,     0,   476,     0,     0,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   590,     0,
       0,   483,   484,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   485,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   486,   487,   591,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   495,   496,   497,   498,   499,     0,
     500,     0,   501,   502,   503,   504,    50,     0,     0,   506,
     507,   508,   509,   510,   511,   512,    63,   592,   514,   515,
       0,     0,     0,     0,     0,     0,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   730,     0,   518,   519,   520,     0,    15,     0,     0,
     521,   522,     0,     0,     0,     0,   453,   454,   523,     0,
     524,     0,   525,   526,   750,     0,   459,   460,   461,   462,
     463,     0,     0,     0,     0,     0,   464,     0,   466,     0,
       0,     0,   469,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   474,     0,     0,   475,
       0,     0,   476,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   590,     0,     0,   483,
     484,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   485,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   486,   487,   591,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,     0,   500,     0,
     501,   502,   503,   504,    50,     0,     0,   506,   507,   508,
     509,   510,   511,   512,    63,   592,   514,   515,     0,     0,
       0,     0,     0,     0,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,    15,     0,     0,   521,   522,
       0,     0,   453,   454,     0,     0,   523,     0,   524,     0,
     525,   526,   459,   460,   461,   462,   463,     0,     0,  1039,
       0,     0,   464,     0,   466,     0,     0,     0,   469,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   474,     0,     0,   475,     0,     0,   476,     0,
       0,     0,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   590,     0,     0,   483,   484,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   486,
     487,   591,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   495,   496,
     497,   498,   499,     0,   500,     0,   501,   502,   503,   504,
      50,     0,     0,   506,   507,   508,   509,   510,   511,   512,
      63,   592,   514,   515,     0,     0,     0,     0,     0,     0,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,    15,     0,     0,   521,   522,     0,     0,   453,   454,
       0,     0,   523,     0,   524,     0,   525,   526,   459,   460,
     461,   462,   463,     0,     0,     0,     0,     0,   464,     0,
     466,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   474,     0,
       0,   475,     0,     0,   476,     0,     0,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   590,     0,
       0,   483,   484,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   485,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   486,   487,   591,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   495,   496,   497,   498,   499,     0,
     500,     0,   501,   502,   503,   504,    50,     0,     0,   506,
     507,   508,   509,   510,   511,   512,    63,   592,   514,   515,
       0,     0,     0,     0,     0,     0,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,     0,    15,     0,     0,
     521,   522,     0,     0,   453,   454,     0,     0,   523,     0,
     524,  1101,   525,   526,   459,   460,   461,   462,   463,     0,
       0,     0,     0,     0,   464,     0,   466,     0,     0,     0,
     469,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   474,     0,     0,   475,     0,     0,
     476,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   590,     0,     0,   483,   484,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   485,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   486,   487,   591,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     495,   496,   497,   498,   499,     0,   500,     0,   501,   502,
     503,   504,    50,     0,     0,   506,   507,   508,   509,   510,
     511,   512,    63,   592,   514,   515,     0,     0,     0,     0,
       0,     0,   516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1151,     0,   518,
     519,   520,     0,    15,     0,     0,   521,   522,     0,     0,
     453,   454,     0,     0,   523,     0,   524,     0,   525,   526,
     459,   460,   461,   462,   463,     0,     0,     0,     0,     0,
     464,     0,   466,     0,     0,     0,   469,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     474,     0,     0,   475,     0,     0,   476,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     590,     0,     0,   483,   484,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   485,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   486,   487,   591,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   495,   496,   497,   498,
     499,     0,   500,     0,   501,   502,   503,   504,    50,     0,
       0,   506,   507,   508,   509,   510,   511,   512,    63,   592,
     514,   515,     0,     0,     0,     0,     0,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,     0,    15,
       0,     0,   521,   522,     0,     0,   453,   454,     0,     0,
     523,     0,   524,  1211,   525,   526,   459,   460,   461,   462,
     463,     0,     0,     0,     0,     0,   464,     0,   466,     0,
       0,     0,   469,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   474,     0,     0,   475,
       0,     0,   476,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   590,     0,     0,   483,
     484,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   485,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   486,   487,   591,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,     0,   500,     0,
     501,   502,   503,   504,    50,     0,     0,   506,   507,   508,
     509,   510,   511,   512,    63,   592,   514,   515,     0,     0,
       0,     0,     0,     0,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,    15,     0,     0,   521,   522,
       0,     0,   453,   454,     0,     0,   523,     0,   524,  1226,
     525,   526,   459,   460,   461,   462,   463,     0,     0,     0,
       0,     0,   464,     0,   466,     0,     0,     0,   469,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   474,     0,     0,   475,     0,     0,   476,     0,
       0,     0,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   590,     0,     0,   483,   484,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   486,
     487,   591,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   495,   496,
     497,   498,   499,     0,   500,     0,   501,   502,   503,   504,
      50,     0,     0,   506,   507,   508,   509,   510,   511,   512,
      63,   592,   514,   515,     0,     0,     0,     0,     0,     0,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,    15,     0,     0,   521,   522,     0,     0,   453,   454,
       0,     0,   523,     0,   524,  1443,   525,   526,   459,   460,
     461,   462,   463,     0,     0,     0,     0,     0,   464,     0,
     466,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   474,     0,
       0,   475,     0,     0,   476,     0,     0,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   590,     0,
       0,   483,   484,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   485,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   486,   487,   591,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   495,   496,   497,   498,   499,     0,
     500,     0,   501,   502,   503,   504,    50,     0,     0,   506,
     507,   508,   509,   510,   511,   512,    63,   592,   514,   515,
       0,     0,     0,     0,     0,     0,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,     0,    15,     0,     0,
     521,   522,     0,     0,   453,   454,     0,     0,  1445,     0,
     524,  1446,   525,   526,   459,   460,   461,   462,   463,     0,
       0,     0,     0,     0,   464,     0,   466,     0,     0,     0,
     469,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   474,     0,     0,   475,     0,     0,
     476,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   590,     0,     0,   483,   484,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   485,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   486,   487,   591,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     495,   496,   497,   498,   499,     0,   500,     0,   501,   502,
     503,   504,    50,     0,     0,   506,   507,   508,   509,   510,
     511,   512,    63,   592,   514,   515,     0,     0,     0,     0,
       0,     0,   516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   518,
     519,   520,     0,    15,     0,     0,   521,   522,     0,     0,
     453,   454,     0,     0,   523,     0,   524,  1449,   525,   526,
     459,   460,   461,   462,   463,     0,     0,     0,     0,     0,
     464,     0,   466,     0,     0,     0,   469,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     474,     0,     0,   475,     0,     0,   476,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     590,     0,     0,   483,   484,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   485,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   486,   487,   591,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   495,   496,   497,   498,
     499,     0,   500,     0,   501,   502,   503,   504,    50,     0,
       0,   506,   507,   508,   509,   510,   511,   512,    63,   592,
     514,   515,     0,     0,     0,     0,     0,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,     0,    15,
       0,     0,   521,   522,     0,     0,   453,   454,     0,     0,
     523,     0,   524,  1494,   525,   526,   459,   460,   461,   462,
     463,     0,     0,     0,     0,     0,   464,     0,   466,     0,
       0,     0,   469,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   474,     0,     0,   475,
       0,     0,   476,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   590,     0,     0,   483,
     484,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   485,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   486,   487,   591,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,     0,   500,     0,
     501,   502,   503,   504,    50,     0,     0,   506,   507,   508,
     509,   510,   511,   512,    63,   592,   514,   515,     0,     0,
       0,     0,     0,     0,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,    15,     0,     0,   521,   522,
       0,     0,   453,   454,     0,     0,   523,     0,   524,  1580,
     525,   526,   459,   460,   461,   462,   463,     0,     0,     0,
       0,     0,   464,     0,   466,     0,     0,     0,   469,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   474,     0,     0,   475,     0,     0,   476,     0,
       0,     0,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   590,     0,     0,   483,   484,     0,   332,   333,
     334,     0,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,   486,
     487,   591,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,     0,     0,  1622,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   495,   496,
     497,   498,   499,     0,   500,     0,   501,   502,   503,   504,
      50,     0,     0,   506,   507,   508,   509,   510,   511,   512,
      63,   592,   514,   515,     0,     0,     0,     0,     0,     0,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,    15,     0,     0,   521,   522,     0,     0,   453,   454,
       0,     0,   523,     0,   524,     0,   525,   526,   459,   460,
     461,   462,   463,     0,     0,     0,     0,     0,   464,     0,
     466,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   474,     0,
       0,   475,     0,     0,   476,     0,     0,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   590,     0,
       0,   483,   484,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   485,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,   486,   487,   591,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,     0,
    1670,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   495,   496,   497,   498,   499,     0,
     500,     0,   501,   502,   503,   504,    50,     0,     0,   506,
     507,   508,   509,   510,   511,   512,    63,   592,   514,   515,
       0,     0,     0,     0,     0,     0,   516,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,     0,    15,     0,     0,
     521,   522,     0,     0,   453,   454,     0,     0,   523,     0,
     524,     0,   525,   526,   459,   460,   461,   462,   463,     0,
       0,     0,     0,     0,   464,     0,   466,     0,     0,     0,
     469,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   474,     0,     0,   475,     0,     0,
     476,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   590,     0,     0,   483,   484,     0,
     332,   333,   334,     0,   336,   337,   338,   339,   340,   485,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,     0,   354,   355,   356,     0,     0,   359,   360,   361,
     362,   486,   487,   591,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,     0,     0,  1671,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     495,   496,   497,   498,   499,     0,   500,     0,   501,   502,
     503,   504,    50,     0,     0,   506,   507,   508,   509,   510,
     511,   512,    63,   592,   514,   515,     0,     0,     0,     0,
       0,     0,   516,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   518,
     519,   520,     0,    15,     0,     0,   521,   522,     0,     0,
     453,   454,     0,     0,   523,     0,   524,     0,   525,   526,
     459,   460,   461,   462,   463,     0,     0,     0,     0,     0,
     464,     0,   466,     0,     0,     0,   469,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     474,     0,     0,   475,     0,     0,   476,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     590,     0,     0,   483,   484,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   485,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   486,   487,   591,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   495,   496,   497,   498,
     499,     0,   500,     0,   501,   502,   503,   504,    50,     0,
       0,   506,   507,   508,   509,   510,   511,   512,    63,   592,
     514,   515,     0,     0,     0,     0,     0,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,     0,    15,
       0,     0,   521,   522,     0,     0,   453,   454,     0,     0,
     523,     0,   524,     0,   525,   526,   459,   460,   461,   462,
     463,     0,     0,     0,     0,     0,   464,     0,   466,     0,
       0,     0,   469,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   474,     0,     0,   475,
       0,     0,   476,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   590,     0,     0,   483,
     484,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   485,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   486,   487,   591,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,     0,   500,     0,
     501,   502,   503,   504,    50,     0,     0,   506,   507,   508,
     509,   510,   511,   512,    63,   592,   514,   515,     0,   724,
       0,     0,     0,     0,   516,   326,     0,     0,     0,     0,
       0,   327,     0,     0,     0,     0,     0,   328,     0,     0,
       0,   518,   519,   520,     0,    15,     0,   329,   521,   522,
       0,     0,     0,     0,     0,   330,  1417,     0,   524,     0,
     525,   526,     0,     0,     0,     0,     0,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   326,     0,
       0,     0,     0,     0,   327,     0,     0,     0,     0,     0,
     328,     0,     0,     0,     0,     0,     0,     0,     0,    62,
     329,     0,     0,     0,     0,     0,     0,     0,   330,     0,
     769,   770,   365,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   331,     0,     0,     0,     0,     0,    63,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,     0,     0,     0,     0,
       0,   326,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,     0,   328,     0,     0,     0,     0,     0,     0,
       0,     0,    62,   329,     0,   771,   772,   773,   774,   775,
       0,   330,   776,   777,   778,   365,     0,   779,   780,   781,
     782,     0,     0,     0,     0,   783,   331,   784,   785,     0,
       0,     0,    63,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
     631,     0,     0,     0,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   801,   802,    62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     0,
       0,     0,     0,     0,     0,     0,     0,    13,     0,     0,
     326,     0,     0,     0,     0,   637,   327,     0,     0,     0,
       0,     0,   328,     0,     0,     0,     0,    14,     0,     0,
       0,     0,   329,   638,     0,     0,     0,     0,     0,     0,
     330,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   331,     0,     0,     0,     0,
       0,   366,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   769,   770,     0,     0,     0,     0,
       0,     0,     0,   326,     0,     0,     0,     0,     0,   327,
       0,     0,     0,     0,     0,   328,     0,     0,     0,     0,
       0,     0,     0,     0,    62,   329,     0,     0,     0,     0,
       0,     0,     0,   330,     0,   769,   770,   365,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   331,     0,
       0,     0,     0,     0,    63,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,     0,   771,
     772,   773,   774,   775,     0,     0,   776,   777,   778,     0,
     366,   779,   780,   781,   782,     0,     0,     0,     0,   783,
       0,   784,   785,     0,     0,     0,     0,   786,   787,   788,
       0,     0,     0,   789,     0,     0,     0,    62,     0,     0,
     771,   772,   773,   774,   775,     0,     0,   776,   777,   778,
     365,     0,   779,   780,   781,   782,   769,   770,     0,     0,
     783,     0,   784,   785,     0,     0,     0,   637,   786,   787,
     788,     0,     0,     0,   789,     0,   790,     0,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,     0,     0,
       0,   769,   770,     0,     0,     0,     0,   801,   802,     0,
       0,   872,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   366,     0,     0,     0,   790,     0,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   801,   802,
       0,     0,  1183,     0,     0,     0,     0,     0,     0,     0,
       0,   771,   772,   773,   774,   775,     0,     0,   776,   777,
     778,     0,     0,   779,   780,   781,   782,     0,     0,     0,
       0,   783,     0,   784,   785,     0,     0,     0,     0,   786,
     787,   788,     0,     0,     0,   789,   771,   772,   773,   774,
     775,     0,     0,   776,   777,   778,   769,   770,   779,   780,
     781,   782,     0,     0,     0,     0,   783,     0,   784,   785,
       0,     0,     0,     0,   786,   787,   788,     0,     0,     0,
     789,     0,     0,     0,     0,     0,     0,     0,   790,     0,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
     769,   770,     0,     0,     0,     0,     0,     0,     0,   801,
     802,     0,     0,  1187,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,     0,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   801,   802,     0,     0,  1189,     0,
       0,   771,   772,   773,   774,   775,     0,     0,   776,   777,
     778,     0,     0,   779,   780,   781,   782,     0,     0,     0,
       0,   783,     0,   784,   785,     0,     0,     0,     0,   786,
     787,   788,     0,     0,     0,   789,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   771,   772,   773,   774,   775,
       0,     0,   776,   777,   778,   769,   770,   779,   780,   781,
     782,     0,     0,     0,     0,   783,     0,   784,   785,     0,
       0,     0,     0,   786,   787,   788,     0,     0,   790,   789,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
     769,   770,     0,     0,     0,     0,     0,     0,     0,   801,
     802,     0,     0,  1198,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   790,     0,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   801,   802,     0,     0,  1199,     0,     0,
     771,   772,   773,   774,   775,     0,     0,   776,   777,   778,
       0,     0,   779,   780,   781,   782,     0,     0,     0,     0,
     783,     0,   784,   785,     0,     0,     0,     0,   786,   787,
     788,     0,     0,     0,   789,   771,   772,   773,   774,   775,
       0,     0,   776,   777,   778,   769,   770,   779,   780,   781,
     782,     0,     0,     0,     0,   783,     0,   784,   785,     0,
       0,     0,     0,   786,   787,   788,     0,     0,     0,   789,
       0,     0,     0,     0,     0,     0,     0,   790,     0,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,   769,
     770,     0,     0,     0,     0,     0,     0,     0,   801,   802,
       0,     0,  1200,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   790,     0,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   801,   802,     0,     0,  1201,     0,     0,
     771,   772,   773,   774,   775,     0,     0,   776,   777,   778,
       0,     0,   779,   780,   781,   782,     0,     0,     0,     0,
     783,     0,   784,   785,     0,     0,     0,     0,   786,   787,
     788,     0,     0,     0,   789,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   771,   772,   773,   774,   775,     0,
       0,   776,   777,   778,   769,   770,   779,   780,   781,   782,
       0,     0,     0,     0,   783,     0,   784,   785,     0,     0,
       0,     0,   786,   787,   788,     0,     0,   790,   789,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,   769,
     770,     0,     0,     0,     0,     0,     0,     0,   801,   802,
       0,     0,  1202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   790,     0,   791,   792,   793,   794,   795,   796,   797,
     798,   799,   800,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   801,   802,     0,     0,  1203,     0,     0,   771,
     772,   773,   774,   775,     0,     0,   776,   777,   778,     0,
       0,   779,   780,   781,   782,     0,     0,     0,     0,   783,
       0,   784,   785,     0,     0,     0,     0,   786,   787,   788,
       0,     0,     0,   789,   771,   772,   773,   774,   775,     0,
       0,   776,   777,   778,   769,   770,   779,   780,   781,   782,
       0,     0,     0,     0,   783,     0,   784,   785,     0,     0,
       0,     0,   786,   787,   788,     0,     0,     0,   789,     0,
       0,     0,     0,     0,     0,     0,   790,     0,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,   769,   770,
       0,     0,     0,     0,     0,     0,     0,   801,   802,     0,
       0,  1331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   790,     0,   791,   792,   793,   794,   795,   796,   797,
     798,   799,   800,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   801,   802,     0,     0,  1497,     0,     0,   771,
     772,   773,   774,   775,     0,     0,   776,   777,   778,     0,
       0,   779,   780,   781,   782,     0,     0,     0,     0,   783,
       0,   784,   785,     0,     0,     0,     0,   786,   787,   788,
       0,     0,     0,   789,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   771,   772,   773,   774,   775,     0,     0,
     776,   777,   778,   769,   770,   779,   780,   781,   782,     0,
       0,     0,     0,   783,     0,   784,   785,     0,     0,     0,
       0,   786,   787,   788,     0,     0,   790,   789,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,   769,   770,
       0,     0,     0,     0,     0,     0,     0,   801,   802,     0,
       0,  1516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     790,     0,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   801,   802,     0,     0,  1517,     0,     0,   771,   772,
     773,   774,   775,     0,     0,   776,   777,   778,     0,     0,
     779,   780,   781,   782,     0,     0,     0,     0,   783,     0,
     784,   785,     0,     0,     0,     0,   786,   787,   788,     0,
       0,     0,   789,   771,   772,   773,   774,   775,     0,     0,
     776,   777,   778,   769,   770,   779,   780,   781,   782,     0,
       0,     0,     0,   783,     0,   784,   785,     0,     0,     0,
       0,   786,   787,   788,     0,     0,     0,   789,     0,     0,
       0,     0,     0,     0,     0,   790,     0,   791,   792,   793,
     794,   795,   796,   797,   798,   799,   800,   769,   770,     0,
       0,     0,     0,     0,     0,     0,   801,   802,     0,     0,
    1540,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     790,     0,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   801,   802,     0,     0,  1542,     0,     0,   771,   772,
     773,   774,   775,     0,     0,   776,   777,   778,     0,     0,
     779,   780,   781,   782,     0,     0,     0,     0,   783,     0,
     784,   785,     0,     0,     0,     0,   786,   787,   788,     0,
       0,     0,   789,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   771,   772,   773,   774,   775,     0,     0,   776,
     777,   778,   769,   770,   779,   780,   781,   782,     0,     0,
       0,     0,   783,     0,   784,   785,     0,     0,     0,     0,
     786,   787,   788,     0,     0,   790,   789,   791,   792,   793,
     794,   795,   796,   797,   798,   799,   800,   769,   770,     0,
       0,     0,     0,     0,     0,     0,   801,   802,     0,     0,
    1543,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
       0,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     801,   802,     0,     0,  1547,     0,     0,   771,   772,   773,
     774,   775,     0,     0,   776,   777,   778,     0,     0,   779,
     780,   781,   782,     0,     0,     0,     0,   783,     0,   784,
     785,     0,     0,     0,     0,   786,   787,   788,     0,     0,
       0,   789,   771,   772,   773,   774,   775,     0,     0,   776,
     777,   778,   769,   770,   779,   780,   781,   782,     0,     0,
       0,     0,   783,     0,   784,   785,     0,     0,     0,     0,
     786,   787,   788,     0,     0,     0,   789,     0,     0,     0,
       0,     0,     0,     0,   790,     0,   791,   792,   793,   794,
     795,   796,   797,   798,   799,   800,   769,   770,     0,     0,
       0,     0,     0,     0,     0,   801,   802,     0,     0,  1627,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   790,
       0,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     801,   802,     0,     0,  1628,     0,     0,   771,   772,   773,
     774,   775,     0,     0,   776,   777,   778,     0,     0,   779,
     780,   781,   782,     0,     0,     0,     0,   783,     0,   784,
     785,     0,     0,     0,     0,   786,   787,   788,     0,     0,
       0,   789,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   771,   772,   773,   774,   775,     0,     0,   776,   777,
     778,   769,   770,   779,   780,   781,   782,     0,     0,     0,
       0,   783,     0,   784,   785,     0,     0,     0,     0,   786,
     787,   788,     0,     0,   790,   789,   791,   792,   793,   794,
     795,   796,   797,   798,   799,   800,     0,     0,     0,     0,
       0,   769,   770,     0,     0,   801,   802,     0,     0,  1629,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,     0,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   801,
     802,     0,     0,  1652,     0,     0,   771,   772,   773,   774,
     775,     0,     0,   776,   777,   778,     0,     0,   779,   780,
     781,   782,     0,     0,     0,     0,   783,     0,   784,   785,
       0,     0,     0,     0,   786,   787,   788,     0,     0,     0,
     789,     0,     0,     0,     0,     0,   771,   772,   773,   774,
     775,     0,     0,   776,   777,   778,   769,   770,   779,   780,
     781,   782,     0,     0,     0,     0,   783,     0,   784,   785,
       0,     0,     0,     0,   786,   787,   788,     0,     0,     0,
     789,     0,     0,   790,     0,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,    13,   769,   770,     0,     0,
       0,     0,     0,     0,   801,   802,     0,     0,  1657,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,   790,     0,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   801,   802,  -908,     0,     0,     0,
       0,   771,   772,   773,   774,   775,     0,     0,   776,   777,
     778,  -533,     0,   779,   780,   781,   782,     0,     0,     0,
       0,   783,     0,   784,   785,     0,     0,     0,     0,   786,
     787,   788,     0,     0,     0,   789,     0,     0,     0,     0,
       0,   771,   772,   773,   774,   775,     0,     0,   776,   777,
     778,   769,   770,   779,   780,   781,   782,     0,     0,     0,
       0,   783,     0,   784,   785,     0,     0,     0,     0,   786,
     787,   788,     0,     0,     0,   789,     0,     0,   790,     0,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
     769,   770,     0,     0,     0,     0,     0,     0,     0,   801,
     802,   864,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   790,     0,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   801,
     802,   864,     0,     0,     0,     0,   771,   772,   773,   774,
     775,     0,     0,   776,   777,   778,     0,     0,   779,   780,
     781,   782,     0,     0,     0,     0,   783,     0,   784,   785,
       0,     0,     0,     0,   786,   787,   788,     0,     0,     0,
     789,     0,     0,     0,     0,   771,   772,   773,   774,   775,
       0,     0,   776,   777,   778,   769,   770,   779,   780,   781,
     782,     0,     0,     0,     0,   783,     0,   784,   785,     0,
       0,     0,     0,   786,   787,   788,     0,     0,     0,   789,
       0,     0,     0,   790,     0,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,   769,   770,     0,     0,     0,
       0,     0,     0,     0,   801,   802,  1134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   790,     0,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   801,   802,  1280,     0,     0,     0,     0,
     771,   772,   773,   774,   775,     0,     0,   776,   777,   778,
       0,     0,   779,   780,   781,   782,     0,     0,     0,     0,
     783,     0,   784,   785,     0,     0,     0,     0,   786,   787,
     788,     0,     0,     0,   789,     0,     0,     0,     0,     0,
     771,   772,   773,   774,   775,     0,     0,   776,   777,   778,
     769,   770,   779,   780,   781,   782,     0,     0,     0,     0,
     783,     0,   784,   785,     0,     0,     0,     0,   786,   787,
     788,     0,     0,     0,   789,     0,     0,   790,     0,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   801,   802,
    1292,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   790,     0,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   801,   802,
    1439,     0,     0,     0,     0,   771,   772,   773,   774,   775,
       0,     0,   776,   777,   778,     0,     0,   779,   780,   781,
     782,   246,   247,     0,     0,   783,     0,   784,   785,     0,
       0,     0,     0,   786,   787,   788,     0,     0,   248,   789,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   769,   770,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   790,     0,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   801,   802,  1442,     0,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,     0,     0,   267,   268,
     269,     0,     0,     0,     0,     0,     0,   270,   271,   272,
     273,   274,     0,     0,   275,   276,   277,   278,   279,   280,
     281,     0,     0,     0,   769,   770,   771,   772,   773,   774,
     775,     0,     0,   776,   777,   778,     0,     0,   779,   780,
     781,   782,     0,     0,     0,     0,   783,     0,   784,   785,
       0,     0,     0,     0,   786,   787,   788,     0,     0,     0,
     789,   769,   770,   282,     0,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,    13,     0,   293,   294,     0,
       0,     0,     0,     0,   295,   296,     0,     0,     0,     0,
       0,     0,     0,     0,  -908,    14,     0,     0,     0,     0,
       0,     0,     0,   790,     0,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,     0,     0,     0,     0,   771,
     772,   773,   774,   775,   801,   802,   776,   777,   778,     0,
       0,   779,   780,   781,   782,     0,     0,     0,     0,   783,
       0,   784,   785,     0,     0,     0,     0,   786,   787,   788,
       0,     0,     0,   789,   769,   770,   771,   772,   773,   774,
     775,     0,     0,   776,   777,   778,  -533,     0,   779,   780,
     781,   782,     0,     0,     0,     0,   783,     0,   784,   785,
       0,     0,     0,     0,   786,   787,   788,     0,     0,     0,
     789,   769,   770,     0,     0,     0,   790,  1285,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   801,   802,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,     0,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,     0,     0,     0,     0,   771,
     772,   773,   774,   775,   801,   802,   776,   777,   778,     0,
       0,   779,   780,   781,   782,     0,     0,     0,     0,   783,
       0,   784,   785,     0,     0,     0,     0,   786,   787,   788,
       0,     0,     0,   789,   769,   770,   771,   772,   773,   774,
     775,     0,     0,   776,   777,   778,     0,     0,   779,   780,
     781,   782,     0,     0,     0,     0,   783,     0,   784,   785,
       0,     0,     0,     0,   786,   787,   788,   769,   770,     0,
    -909,     0,     0,     0,     0,     0,   790,     0,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   801,   802,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   790,     0,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,     0,     0,     0,     0,   771,
     772,   773,   774,   775,   801,   802,   776,   777,   778,     0,
       0,   779,   780,   781,   782,     0,     0,     0,     0,   783,
       0,   784,   785,     0,     0,     0,     0,   786,   787,   788,
     769,   770,   771,   772,   773,   774,   775,     0,     0,   776,
     777,   778,     0,     0,   779,   780,   781,   782,     0,     0,
       0,     0,   783,     0,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   769,   770,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   790,     0,   791,   792,
     793,   794,   795,   796,   797,   798,   799,   800,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   801,   802,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,     0,     0,     0,     0,   771,   772,   773,   774,   775,
     801,   802,   776,   777,   778,     0,     0,   779,   780,   781,
     782,     0,     0,     0,     0,   783,     0,   784,   785,     0,
       0,     0,     0,   786,     0,     0,   769,   770,   771,   772,
     773,   774,   775,     0,     0,   776,   777,   778,     0,     0,
     779,   780,   781,   782,     0,     0,     0,     0,   783,     0,
     784,   785,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   801,   802,  1012,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   792,   793,
     794,   795,   796,   797,   798,   799,   800,     0,     0,     0,
       0,   771,   772,   773,   774,   775,   801,   802,   776,   777,
     778,     0,     0,   779,   780,   781,   782,     0,     0,     0,
       0,   783,     0,   784,   785,     0,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1016,     0,     0,     0,     0,     0,
       0,     0,   793,   794,   795,   796,   797,   798,   799,   800,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   801,
     802,     0,     0,     0,     0,  1281,     0,     0,     0,     0,
       0,     0,     0,     0,  1013,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,   333,   334,  1014,
     336,   337,   338,   339,   340,   485,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,   195,   359,   360,   361,   362,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   196,     0,
     197,     0,   198,   199,   200,   201,   202,     0,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,     0,
     214,   215,   216,  1017,     0,   217,   218,   219,   220,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1018,     0,
       0,     0,     0,     0,     0,   221,   222,     0,     0,     0,
       0,     0,     0,     0,  1282,     0,     0,   900,   901,   902,
     903,   904,   905,   906,   907,     0,     0,     0,     0,  1283,
     908,   909,     0,     0,     0,   813,   910,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   815,     0,     0,   911,
     912,     0,     0,     0,     0,     0,   913,   914,   915,     0,
     223,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,   916,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   822,     0,     0,     0,     0,   525,   710
};

static const yytype_int16 yycheck[] =
{
       1,   189,   225,     7,   231,   682,   241,   504,   598,   653,
     621,   392,   544,    15,    16,   440,   406,   392,   439,    20,
     725,   744,   851,   171,   853,   561,   855,   723,   957,   725,
     753,   612,   400,   614,   963,   616,  1215,   660,   887,   941,
     934,   465,    86,   936,     5,   435,    12,    20,   439,   440,
      33,   587,    15,    16,   109,   493,   494,    34,    33,     8,
      53,  1420,   758,    22,    53,   489,    68,    69,    70,   127,
      20,  1631,    20,    57,   126,    20,   143,   166,     4,    63,
      20,    20,    20,   126,   160,   312,    63,    20,  1603,    20,
     465,   126,   158,    20,   126,    96,    20,    98,    57,   722,
     163,   724,   126,   726,   165,   126,   173,   173,   110,   111,
     112,   113,   502,   503,   489,     7,   126,   740,    19,    20,
     183,   142,    15,    16,   185,   129,  1686,   750,    62,    33,
     107,   163,   142,   191,   192,   173,   194,   127,   199,   197,
     152,   102,   103,   163,   220,   144,   145,   146,   149,   998,
     217,   183,   218,  1668,   217,   390,    60,    61,    50,   136,
     126,   173,   214,   183,   168,   392,   143,     0,   316,   158,
    1529,   214,   165,   896,     7,   218,   712,   232,   602,   214,
     164,   219,   214,   218,   173,   192,   190,   191,   192,   233,
     214,   887,   185,   214,   218,  1374,   173,    30,   126,    32,
     184,    34,   192,   166,   214,   195,   394,    40,   171,   932,
     173,   438,   935,   176,   197,   164,   165,    50,   191,   218,
     124,   225,   197,    56,   128,    21,    22,   602,   126,    21,
      22,   208,   215,   234,   160,   184,   185,   198,   221,   241,
     217,   191,   194,   191,   203,   468,   191,    80,   214,   640,
     674,   191,   191,   191,   645,   214,   217,   126,   191,   482,
     191,    46,   885,  1157,   191,  1158,   215,   191,   173,   102,
     103,  1488,   173,   166,   191,   319,   143,   158,   171,  1002,
     173,   705,  1211,   176,     5,     6,   214,     8,   152,   193,
     218,   681,   173,   197,  1196,   199,   200,   126,  1147,   674,
     155,   218,   998,   999,    53,   160,   173,   311,   126,   173,
     314,   215,   217,   126,   704,    36,   214,   221,   127,  1498,
     218,   106,   118,   119,   217,   191,   118,   119,   137,   767,
     705,   191,   165,   129,   130,   131,   128,   129,   130,   131,
     136,   143,  1559,  1560,   136,   214,   191,   165,    97,   218,
     216,  1253,   185,   217,   977,   978,   583,   980,   218,   165,
     587,  1578,  1579,   218,   366,   220,   126,   185,   603,   604,
     605,   204,   607,   608,   142,   191,   611,   127,   613,   185,
     615,   194,   215,   192,   198,   214,   107,   137,   390,   173,
     219,   220,   194,   143,   396,   163,   214,   765,   129,   130,
     588,   214,   218,   217,   406,   165,   241,  1309,   200,   201,
     202,   203,   204,   837,   220,   183,   643,   152,   214,   215,
     647,   155,   214,   215,   191,   185,   160,  1644,  1645,  1010,
     127,   143,  1591,   435,  1593,   158,   659,   439,   173,   127,
     137,  1600,   192,   833,  1373,  1274,   152,   127,   191,   137,
     173,  1147,   843,   191,   214,  1614,  1385,   137,   152,   461,
     462,   997,   837,   686,   468,   195,   160,   173,   859,   199,
     188,   189,   969,   129,   130,   218,   217,   478,   482,   173,
     218,   166,   217,   214,   215,   173,   220,  1638,   492,   493,
     494,   881,   882,   177,   217,   192,  1140,   994,   216,  1650,
     502,   503,   158,   127,   192,   895,  1211,  1666,   731,   197,
     214,   949,   192,   137,   132,  1211,   127,    57,  1214,   165,
     163,  1226,   173,   217,   751,   748,   137,   918,   529,   530,
    1226,     5,     6,   173,   924,   925,   191,   927,  1367,   185,
     183,   931,   188,   933,  1695,  1696,   547,   165,   549,   550,
     551,    25,   553,  1712,  1713,   390,   173,    31,   214,   215,
     165,   216,   165,   564,   165,    21,    22,   185,   192,   143,
     216,   406,   165,    57,   173,  1404,   219,   220,   152,    63,
     185,   192,   185,   157,   185,   173,   587,   192,   191,   191,
    1749,   184,   185,   186,    68,    69,   199,   598,   199,   173,
     435,   603,   604,   605,   439,   607,   608,   165,   191,   611,
     165,   613,   164,   615,   216,   617,  1197,   163,  1241,  1057,
    1058,  1059,    21,    22,   198,   191,   861,   185,   102,   103,
     185,   866,   184,   191,   869,   218,   191,   183,   826,   191,
     163,   199,   198,   878,   199,   191,    12,   173,   174,   175,
     165,   886,   218,    57,   191,   659,   891,    23,    24,    63,
     183,   217,   118,   119,   666,   165,  1247,   502,   503,   143,
     185,   194,   128,   129,   130,   131,   191,  1354,   204,   681,
     136,   218,   686,   831,   199,   185,   160,   161,   162,   163,
      33,   191,   143,   165,   917,   195,  1525,   198,   191,   173,
     185,   152,   704,   165,   173,  1130,   191,  1128,   191,   183,
      68,    69,    70,   185,   191,   191,   217,    60,    61,   118,
     119,  1417,   173,   185,   198,   218,   191,   731,   729,   128,
     129,   130,   131,  1377,   191,   218,   971,   136,   973,   216,
    1131,   215,   218,   745,   748,   165,   202,   203,   204,  1445,
     754,   216,   110,   111,   112,   113,   191,   194,   214,   215,
     197,   218,   191,   767,   173,   185,  1156,   165,   603,   604,
     605,   191,   607,   608,   144,   191,   611,   165,   613,   199,
     615,   124,   617,   218,   127,   128,   165,   185,   191,   218,
     191,  1181,   177,   191,   137,   158,   191,   185,   165,   191,
     216,   199,   990,   191,  1195,   191,   185,   164,    33,    47,
     173,   199,   191,   216,   194,   214,   215,   218,   185,   199,
     199,   165,   165,   218,   191,   827,   218,   184,   158,    67,
     216,   833,   199,  1414,  1366,    60,    61,   217,   839,   198,
     165,   185,   185,   173,  1235,  1236,   681,   191,   849,   192,
     193,  1462,   106,  1497,   197,   199,  1246,   200,   217,   861,
     185,    35,   165,   194,   866,   165,   191,   869,   199,   704,
     173,  1252,   215,   173,   192,   158,   878,  1252,   221,   881,
     882,  1116,   185,    35,   886,   165,   217,   198,   191,   891,
     173,    75,    57,   895,  1571,    79,   194,   194,    63,   124,
     217,   199,   199,   128,   184,   185,   217,   187,   465,    93,
      94,   173,    33,   917,    98,    99,   100,   101,   164,   217,
     217,   173,   924,   925,   195,   927,  1649,   173,   199,   931,
     191,   933,   934,   194,   769,   770,   197,  1660,   184,    60,
      61,  1531,   971,   184,   973,   949,   187,    57,   783,   190,
    1646,    57,  1343,    63,   173,  1651,   957,    63,  1569,   960,
    1351,   195,   963,   964,    57,   199,  1689,   524,   193,   971,
      63,   973,   197,    57,   199,   200,   185,   185,   173,    63,
     185,   195,   191,   191,  1595,   199,   191,   822,   185,    33,
     215,  1687,   194,   185,   191,   197,   221,    22,   833,   191,
     492,   493,   494,   124,   185,     5,     6,   128,   195,  1506,
     191,   194,   199,  1657,   173,   195,    60,    61,   195,   199,
     195,   144,   199,   146,   199,  1252,   861,  1524,  1618,  1619,
     216,   866,   195,   195,   869,   195,   199,   199,   194,   199,
    1430,   219,   220,   878,   165,   216,   881,   882,   144,   145,
     146,   886,   544,  1057,  1058,  1059,   891,   173,   174,   175,
     895,  1757,   177,   178,   185,   177,   178,   179,   180,   217,
     191,   217,   193,   219,   220,    43,   197,   217,   199,   200,
     124,   198,   574,   127,   128,   198,  1676,    92,    93,   924,
     925,   198,   927,   137,   215,  1330,   931,   198,   933,   934,
     221,   177,   178,   179,    33,   195,  1107,   198,   217,    10,
    1111,   198,   198,  1336,  1116,   198,   673,   674,   198,   198,
    1308,   177,   178,   179,   198,   217,  1128,   447,   448,   449,
     173,    60,    61,    10,    11,    12,   971,   185,   973,   137,
    1530,   218,   699,   173,   217,   702,   216,   173,   192,   193,
     173,   215,  1153,   197,  1156,  1157,   200,   165,   198,   198,
     217,   198,   217,   173,   198,  1662,   198,   217,   217,   198,
     166,   215,   217,    13,   198,   217,   198,   221,   198,  1181,
     198,   217,  1405,    10,   217,   220,   217,   679,    37,   217,
      66,   198,   198,    43,   215,   124,   173,  1432,   217,   128,
     173,   758,   218,   216,  1039,   191,  1429,   108,   109,   110,
     111,   112,   113,   114,   115,   707,   194,    43,   217,   198,
     121,   122,   217,   217,   198,   126,   127,  1617,   198,   191,
     132,    14,   192,   194,   166,   220,   137,   217,    13,   140,
     141,   216,   191,   191,  1246,   173,   147,   148,   149,     8,
     173,   218,   199,   173,   173,   173,   191,   191,   184,   218,
     217,    21,    22,  1451,   193,   218,   217,   824,   197,   191,
    1493,   200,   217,   173,   218,   767,   217,   198,   217,   217,
     217,  1116,   198,   158,   217,   173,   215,   217,     1,    67,
     217,   192,   221,  1128,   173,   199,    33,    43,  1521,   173,
     217,  1302,   199,   199,   218,   218,   218,   217,   217,  1544,
     218,   218,   218,   214,   173,   173,   173,   216,   219,   220,
     173,  1156,  1157,    60,    61,   216,    33,   216,  1330,   198,
     887,   217,  1336,   217,   217,   217,   217,   173,  1561,   217,
     392,   173,   181,   173,   216,   218,  1181,   839,   218,   217,
     216,   173,   216,    60,    61,  1356,   116,   117,   118,   119,
     120,    33,   217,   123,   124,   125,   217,   173,   128,   129,
     130,   131,  1373,   217,   199,   217,   136,   218,   138,   139,
      33,    70,   217,   217,  1385,   199,   217,   124,   217,   217,
     199,   128,  1615,   185,   218,   218,   217,   954,   217,   217,
     217,  1405,    53,   455,   177,   962,   185,    60,    61,   216,
    1411,  1246,   184,   465,   218,   218,   218,   124,   470,   218,
     472,   128,   191,   216,   185,  1429,   191,   218,  1430,   481,
    1432,   218,  1659,   218,   184,   216,   218,   489,   198,   199,
     200,   201,   202,   203,   204,   216,  1673,   939,   218,   733,
     218,   218,   504,   216,   214,   215,   193,   949,    85,   217,
     197,   513,   199,   200,   218,   218,     1,   216,   216,    33,
      46,   124,   141,    88,  1605,   128,  1522,   239,   215,   531,
     827,  1522,  1709,   996,   221,  1522,   193,   539,  1522,  1493,
     197,  1522,   199,   200,    33,  1330,    60,    61,     1,  1005,
    1266,  1493,  1427,  1726,  1305,  1556,  1453,  1557,   215,  1557,
    1456,    56,   437,   393,   221,   461,  1651,  1521,   461,  1504,
    1223,    60,    61,    -1,   725,    33,    -1,    -1,  1530,    -1,
      -1,    -1,   584,    -1,    -1,    -1,    -1,  1725,    -1,    -1,
     193,    -1,  1544,    -1,   197,    -1,   199,   200,    -1,    -1,
     602,    -1,    60,    61,    -1,    -1,    -1,  1561,    -1,    -1,
     124,    -1,   215,    -1,   128,  1057,  1058,  1059,   221,    -1,
    1062,    -1,  1064,    -1,  1066,    -1,  1068,    -1,  1070,    -1,
    1072,    -1,  1074,  1584,  1076,   124,  1078,  1144,  1080,   128,
    1591,  1083,  1593,    -1,  1086,  1430,  1088,  1432,  1090,  1600,
    1092,  1603,  1094,    -1,  1096,    -1,    -1,    -1,   392,    -1,
      -1,  1615,    -1,  1614,    -1,  1617,   124,    -1,    -1,  1111,
     128,    -1,   674,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     682,   683,    -1,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1204,    -1,    -1,
      -1,   215,    -1,   705,   193,    -1,    -1,   221,   197,    -1,
     199,   200,    -1,    -1,    -1,  1666,  1668,    -1,  1225,    -1,
      -1,   723,    -1,   725,  1231,    -1,   215,    -1,  1679,    -1,
      -1,   465,   221,  1240,  1685,   193,    -1,    -1,    -1,   197,
      -1,   199,   200,  1250,    -1,  1530,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,   489,    -1,   215,    -1,  1544,
      -1,  1712,  1713,   221,    -1,    -1,    -1,    -1,    -1,    -1,
    1721,  1722,  1726,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1296,
    1297,    -1,    -1,  1300,    -1,    -1,    -1,    -1,  1749,    -1,
    1751,   803,   804,   805,   806,   807,   808,   809,   810,   811,
     812,    -1,   814,   815,   816,   817,   818,   819,   820,   821,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1337,    -1,  1617,    -1,    -1,   837,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,   128,   129,   130,   131,
      -1,  1368,    -1,    -1,   136,    -1,   138,   139,   602,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   880,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   888,    -1,   890,  1396,
    1397,    -1,    -1,  1400,    -1,    -1,    -1,    -1,   900,   901,
     902,   903,   904,   905,   906,   907,   908,   909,   910,   911,
     912,   913,   914,   915,   916,    -1,   403,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1366,   412,   198,   199,   200,   201,
     202,   203,   204,   124,    -1,   422,    -1,   128,    -1,    -1,
     674,    -1,   214,   215,    -1,   432,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   443,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   966,    -1,    33,   969,    -1,    -1,
      -1,   705,    -1,   975,    -1,    -1,    -1,    -1,   465,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,   991,
      -1,    -1,   994,    60,    61,    -1,    -1,    -1,    21,    22,
      -1,    -1,   193,   490,   491,    -1,   197,  1514,   199,   200,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
     221,   518,   519,   520,   521,   522,   523,   524,   804,   805,
     806,   807,   808,   809,   810,   811,   812,    -1,   814,    -1,
     816,   817,   818,   819,   820,   821,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,  1575,   803,
     804,   805,   806,   807,   808,   809,   810,   811,   812,    -1,
     814,    -1,   816,   817,   818,   819,   820,   821,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,  1606,
     123,   124,   125,   837,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,   193,   150,    -1,    -1,
     197,    -1,   199,   200,   621,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,   630,    -1,    21,    22,    -1,   215,    -1,
      -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,    -1,
     183,    -1,   185,  1670,    -1,    -1,    -1,    -1,   191,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,  1184,    -1,    -1,    -1,   673,   674,    -1,    -1,
      -1,   214,   215,   216,    -1,   218,    -1,    -1,   685,    -1,
      -1,    -1,    -1,   690,    -1,  1207,  1208,    -1,    -1,   696,
    1212,    -1,   699,    -1,    -1,   702,    -1,    -1,  1220,    -1,
      -1,  1223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   716,
     717,   718,   719,   720,   721,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
    1252,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,   758,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   771,   772,    -1,    -1,   775,   776,
     777,   778,   779,    -1,   781,   782,   783,   784,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,    -1,   802,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,  1333,    -1,    -1,    -1,    -1,    -1,   824,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    33,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    33,    -1,    31,    -1,   873,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    60,    61,    -1,    -1,
     887,    -1,    49,    -1,    -1,    -1,  1408,    -1,    -1,    60,
      61,    -1,   899,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
     124,    33,    -1,    -1,   128,   952,   953,   954,    -1,    -1,
    1472,    -1,  1474,   124,    -1,   962,    -1,   128,    -1,    -1,
      33,    -1,  1484,  1485,  1486,    -1,    -1,    -1,    60,    61,
      -1,    21,    22,    -1,    -1,   982,   143,    -1,    -1,  1501,
    1502,    -1,    -1,    -1,  1506,  1507,    -1,    60,    61,   156,
      -1,    -1,   999,    -1,    -1,    -1,    -1,    -1,  1252,    -1,
    1522,    -1,  1524,    -1,    -1,    -1,   173,    -1,    -1,   193,
      -1,    -1,    -1,   197,  1536,   199,   200,    -1,    -1,    -1,
      -1,    -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,
      -1,   215,   124,    -1,    -1,    -1,   128,   221,    -1,    -1,
      -1,    -1,    -1,    -1,   215,  1052,    -1,    -1,  1055,  1571,
     221,   124,   219,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1585,  1586,    -1,   116,   117,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,  1607,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1620,    -1,
    1622,   193,    -1,    -1,    -1,   197,    -1,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1641,
     193,  1643,    -1,   215,   197,    -1,   199,   200,    -1,   221,
      -1,    -1,    21,    22,    -1,    -1,    -1,  1144,    -1,    -1,
    1662,    -1,   215,    -1,    -1,    -1,    -1,    -1,   221,  1671,
     200,   201,   202,   203,   204,  1677,  1678,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,  1704,    -1,  1706,  1707,    -1,   121,   122,    -1,
      -1,    -1,   126,   127,    -1,    -1,    -1,  1204,  1720,  1206,
      -1,    -1,  1209,   137,    -1,    -1,   140,   141,    -1,    -1,
      -1,    -1,    -1,   147,   148,   149,    -1,    -1,  1225,    -1,
      -1,    -1,    -1,    -1,  1231,    -1,    -1,   116,   117,   118,
     119,   120,    -1,  1240,   123,   124,   125,  1244,  1245,   128,
     129,   130,   131,  1250,    -1,    -1,    -1,   136,    -1,   138,
     139,  1258,    -1,    -1,    -1,   144,   145,   146,   192,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1276,
      -1,  1278,  1279,    -1,    -1,    -1,    -1,    -1,  1285,    -1,
     214,    -1,  1289,    -1,    -1,   219,   220,    -1,    -1,  1296,
    1297,    -1,    -1,  1300,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
    1337,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,  1368,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,  1396,
    1397,    -1,    -1,  1400,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1417,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1436,
      -1,  1438,    -1,    -1,  1441,    -1,    -1,    -1,  1445,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,  1462,    -1,    -1,    -1,   136,
     173,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1491,    -1,    -1,    -1,    -1,  1496,
      -1,    -1,    -1,  1500,    -1,    -1,  1503,    -1,    -1,    -1,
      -1,   214,    -1,    -1,    -1,    -1,  1513,  1514,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,   121,
     122,    -1,  1569,    -1,   126,   127,    -1,    -1,  1575,    -1,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,   141,
      -1,    -1,    -1,    -1,    -1,   147,   148,   149,  1595,    -1,
      -1,    -1,    -1,    -1,    -1,  1602,    -1,     1,    -1,  1606,
      -1,     5,     6,     7,    -1,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    -1,    31,    -1,    -1,
     192,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,    -1,
      44,    45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,
      54,    55,   214,    -1,    58,    59,  1663,   219,   220,    -1,
      -1,    65,    -1,  1670,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,  1715,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,    -1,   219,   220,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,     5,     6,    -1,   215,    -1,   217,
      -1,   219,   220,   221,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,   193,    -1,    -1,    -1,   197,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,     5,     6,    -1,   215,    -1,   217,    -1,   219,   220,
     221,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,    33,
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
      -1,    -1,    -1,   197,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,    -1,     5,     6,
      -1,   215,    -1,   217,    -1,   219,   220,   221,    15,    16,
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
     127,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
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
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,    -1,   219,   220,    15,    16,    17,    18,
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
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,   218,   219,   220,    15,    16,
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
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,    -1,    -1,     5,     6,   215,    -1,
     217,    -1,   219,   220,    13,    -1,    15,    16,    17,    18,
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
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,    -1,    -1,     5,     6,   215,   216,   217,    -1,
     219,   220,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
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
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,    -1,     5,     6,   215,    -1,   217,    -1,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,    -1,
       5,     6,   215,   216,   217,    -1,   219,   220,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,    -1,   219,   220,    15,    16,    17,    18,
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
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,   216,   217,    -1,   219,   220,    15,    16,
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
     157,    -1,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,    -1,     5,     6,   215,    -1,   217,    -1,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
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
      -1,   198,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,    -1,    -1,     5,     6,   215,    -1,
     217,    -1,   219,   220,    13,    -1,    15,    16,    17,    18,
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
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    22,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
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
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,   218,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,   218,   219,   220,    15,    16,    17,    18,
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
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,   218,   219,   220,    15,    16,
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
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,   218,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,   218,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,   218,   219,   220,    15,    16,    17,    18,
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
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
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
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,
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
      -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,    -1,
     215,    -1,   217,    -1,   219,   220,    15,    16,    17,    18,
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
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    13,
      -1,    -1,    -1,    -1,   183,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    41,   207,   208,
      -1,    -1,    -1,    -1,    -1,    49,   215,    -1,   217,    -1,
     219,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      21,    22,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   173,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    41,    -1,   116,   117,   118,   119,   120,
      -1,    49,   123,   124,   125,   156,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    64,   138,   139,    -1,
      -1,    -1,   173,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   219,    -1,
     221,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      19,    -1,    -1,    -1,    -1,   173,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,   185,    -1,    -1,
      -1,    -1,    41,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   219,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    21,    22,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,   173,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
     219,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,   143,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     156,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,   173,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   219,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    -1,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    21,    22,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    -1,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    21,    22,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,   193,   150,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    21,    22,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,    21,    22,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,   193,   150,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,    21,    22,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,    21,    22,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,   193,   150,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,    21,    22,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,   218,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,    21,    22,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,   193,   150,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,    21,    22,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    21,    22,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,   193,   150,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    21,    22,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   165,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,    21,    22,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,   216,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    21,    22,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      -1,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
      21,    22,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    -1,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    38,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,   216,    -1,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,    -1,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   165,    -1,   207,   208,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,    -1,    -1,    -1,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    21,    22,    -1,
     150,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,    -1,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      21,    22,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,    -1,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,    -1,   146,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     214,   215,   123,   124,   125,    -1,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,    -1,    -1,    21,    22,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,    -1,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   214,   215,   123,   124,
     125,    -1,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,   173,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    35,    98,    99,   100,   101,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    71,    -1,
      73,    -1,    75,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,   158,    -1,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   173,
     121,   122,    -1,    -1,    -1,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,
     141,    -1,    -1,    -1,    -1,    -1,   147,   148,   149,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,   219,   220
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   225,   228,
     237,   239,   240,   245,   276,   280,   316,   401,   408,   412,
     419,   465,   470,   475,    19,    20,   173,   268,   269,   270,
     166,   246,   247,   173,   174,   175,   204,   241,   242,   243,
     163,   183,   285,   409,   173,   219,   227,    57,    63,   404,
     404,   404,   143,   173,   302,    34,    63,   107,   136,   208,
     217,   272,   273,   274,   275,   302,   224,     5,     6,     8,
      36,   107,   416,    62,   399,   192,   191,   194,   191,   242,
      22,    57,   203,   214,   244,   404,   405,   407,   405,   399,
     476,   466,   471,   173,   143,   238,   274,   274,   274,   217,
     144,   145,   146,   191,   216,    57,    63,   281,   283,    57,
      63,   410,     5,     6,    57,    63,   417,    57,    63,   400,
      15,    16,   166,   171,   173,   176,   217,   230,   269,   166,
     247,   173,   241,   241,   173,   224,   164,   184,   286,   405,
     224,    57,    63,   226,   173,   173,   173,   173,   177,   236,
     218,   270,   274,   274,   274,   274,   284,   173,   411,   420,
     285,   402,   177,   178,   179,   229,    15,    16,   166,   171,
     173,   230,   266,   267,   244,   406,   165,   185,   375,   224,
     477,   467,   472,   177,   218,    35,    71,    73,    75,    76,
      77,    78,    79,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    93,    94,    95,    98,    99,   100,
     101,   118,   119,   173,   279,   282,   194,   285,   106,   414,
     415,   397,   160,   220,   271,   369,   177,   178,   179,   191,
     218,   192,   375,   285,   285,   285,    21,    22,    38,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   128,   129,   130,
     137,   138,   139,   140,   141,   144,   145,   146,   147,   148,
     149,   150,   193,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   207,   208,   214,   215,    35,    35,   217,
     277,   285,   287,    75,    79,    93,    94,    98,    99,   100,
     101,   424,   403,   173,   421,   286,   398,   270,   269,   220,
     224,   152,   173,   395,   396,   266,    19,    25,    31,    41,
      49,    64,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   156,   219,   302,   423,   425,
     426,   430,   436,   464,   429,   468,   473,   173,   173,   173,
     216,    22,   173,   216,   155,   218,   369,   380,   381,   382,
     194,   278,   290,   285,   173,   286,   194,   413,   285,   418,
     369,   216,   269,   217,    43,   191,   194,   197,   394,   198,
     198,   198,   217,   198,   198,   217,   198,   198,   198,   198,
     198,   198,   217,   302,    33,    60,    61,   124,   128,   193,
     197,   200,   215,   221,   435,   195,   165,   173,   428,   385,
     388,   173,   137,   217,     7,    50,   315,   185,   185,   191,
     218,   464,     1,     5,     6,     9,    10,    11,    13,    15,
      16,    17,    18,    19,    25,    26,    27,    28,    29,    31,
      38,    39,    42,    44,    45,    48,    51,    52,    54,    55,
      58,    59,    65,    68,    69,    80,   102,   103,   104,   105,
     118,   119,   133,   134,   135,   151,   152,   153,   154,   155,
     157,   159,   160,   161,   162,   165,   166,   167,   168,   169,
     170,   171,   172,   174,   175,   176,   183,   185,   200,   201,
     202,   207,   208,   215,   217,   219,   220,   235,   237,   248,
     249,   252,   255,   256,   258,   261,   262,   263,   264,   285,
     286,   288,   289,   291,   296,   301,   302,   303,   307,   308,
     309,   310,   311,   312,   313,   314,   316,   320,   321,   328,
     331,   334,   339,   342,   343,   345,   346,   347,   349,   354,
     357,   358,   359,   366,   367,   368,   423,   478,   493,   504,
     508,   521,   524,   403,   192,   375,   302,   376,   396,   216,
      65,   104,   174,   296,   366,   367,   173,   173,   436,   127,
     137,   192,   393,   437,   442,   444,   366,   446,   440,   173,
     431,   448,   450,   452,   454,   456,   458,   460,   462,   366,
     198,   217,    33,   197,    33,   197,   215,   221,   216,   366,
     215,   221,   436,   165,   428,   191,   286,   173,   191,   224,
     383,   433,   464,   469,   173,   386,   433,   474,   366,   152,
     173,   390,   391,   422,   382,   382,   382,   198,   198,   367,
     259,   260,   198,   304,   425,   478,   217,   302,   198,     5,
     102,   103,   198,   217,   127,   301,   332,   343,   366,   367,
     287,   198,   217,    61,   367,   217,   367,   173,   198,   198,
     217,   224,   198,   166,    58,   367,   217,   287,   198,   217,
     198,   198,   217,   198,   198,   127,   301,   367,   366,   366,
     220,   287,   334,   338,   338,   338,   217,   217,   217,   217,
     217,   217,    13,   436,    13,   436,    13,   367,   503,   519,
     198,   367,   198,   234,    13,   366,   366,   366,   366,   366,
      13,    49,   292,   332,   332,   220,   224,   224,   367,    10,
      13,   294,   503,   520,    37,   334,   340,   173,   217,   224,
     224,   224,   224,   224,    66,   317,   276,   132,   224,    21,
      22,   116,   117,   118,   119,   120,   123,   124,   125,   128,
     129,   130,   131,   136,   138,   139,   144,   145,   146,   150,
     193,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   214,   215,   108,   109,   110,   111,   112,   113,   114,
     115,   121,   122,   126,   127,   137,   140,   141,   147,   148,
     149,   192,   214,   340,   217,   286,   367,   215,   224,   276,
     286,   377,   375,   198,   218,    43,   224,   393,   301,   367,
     464,   464,   434,   464,   218,   464,   464,   218,   173,   427,
     464,   277,   464,   277,   464,   277,   383,   384,   386,   387,
     218,   439,   292,   216,   216,   366,   194,   433,   286,   194,
     433,   286,   218,   217,    43,   127,   191,   192,   194,   197,
     389,   494,   496,   287,   422,   217,   305,   217,   302,   198,
     217,   329,   198,   198,   198,   515,   332,   301,   332,   191,
     108,   109,   110,   111,   112,   113,   114,   115,   121,   122,
     127,   140,   141,   147,   148,   149,   192,    14,   436,   294,
     367,   366,   287,   192,   322,   324,   366,   326,   194,   166,
     366,   517,   332,   500,   505,   332,   498,   436,   301,   367,
     220,   276,   366,   366,   366,   366,   366,   366,   422,    53,
     158,   173,   200,   215,   217,   367,   479,   482,   486,   502,
     507,   422,   217,   482,   507,   422,   142,   184,   186,   224,
     487,   297,   287,   299,   179,   180,   229,   217,   217,   422,
      13,   216,   191,   523,   523,   152,   157,   198,   302,   348,
     287,   257,   422,   286,   191,   523,   285,   341,    70,   215,
     218,   332,   479,   481,   160,   217,   319,   396,     4,   160,
     337,   338,    19,   158,   173,   423,    19,   158,   173,   423,
     366,   366,   366,   366,   366,   366,   366,   158,   173,   366,
     366,   366,   423,   366,   366,   366,   366,   366,   366,    22,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   129,   130,   158,   214,   215,   366,   133,   134,   135,
     288,   344,   367,   344,   367,   344,   367,   344,   367,   344,
     367,   344,   367,   344,   367,   344,   367,   344,   367,   344,
     367,   173,   344,   367,   367,   344,   367,   344,   367,   344,
     367,   344,   367,   344,   367,   344,   367,   173,   214,   355,
     423,   218,   332,   375,   275,     8,   369,   374,   436,   173,
     301,   367,   224,   199,   199,   199,   433,   199,   199,   199,
     224,   199,   278,   199,   278,   199,   278,   199,   433,   199,
     433,   295,   464,   218,   216,   464,   464,   366,   173,   173,
     464,   367,   436,   436,    20,   422,   464,    70,   332,   481,
     492,   198,   367,   173,   367,   464,   509,   511,   513,   436,
     523,   366,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   287,
     199,   433,   218,   218,   265,   436,   436,   218,   436,   218,
     436,   523,   436,   384,   523,   387,   199,   337,   218,   218,
     218,   218,   218,   218,    20,   338,   217,   137,   389,   215,
     366,   218,   142,   191,   224,   486,   188,   189,   216,   490,
     191,   188,   216,   224,   489,    20,   218,   486,   184,   187,
     488,    20,   367,   184,   503,   295,   295,   367,   422,   422,
      20,   217,   422,   218,   217,   217,   350,   352,   375,   367,
      20,   503,   290,   276,   173,   218,   481,   479,   191,   218,
     191,   523,   218,   173,   318,   318,   217,   127,   137,   173,
     192,   197,   335,   336,   277,   198,   217,   198,   217,   217,
     216,    19,   158,   173,   423,   194,   158,   173,   366,   217,
     158,   366,   216,   338,   338,   338,   217,   217,   173,     1,
     217,   218,   216,    57,    63,   372,    67,   373,   224,   199,
     224,   438,   443,   445,   464,   447,   441,   432,   173,   449,
     199,   453,   199,   457,   199,   461,   383,   463,   386,   199,
     433,   218,    43,   389,   199,   199,   332,    20,   199,   481,
     218,   218,   218,   173,   218,   199,   224,   218,   199,   436,
     384,   387,   199,   218,   217,   436,   367,   199,   199,   199,
     199,   218,   199,   199,   218,   199,   337,   277,   217,   332,
     366,   367,   367,   482,   486,   367,   158,   173,   479,   490,
     216,   367,   216,   502,   332,   482,   184,   187,   190,   491,
     216,   332,   199,   199,   194,   232,    20,    20,   332,   422,
      20,   366,   366,   436,   277,    12,    23,    24,   250,   251,
      12,   253,   332,   286,   337,   218,   216,   215,   191,   216,
     218,   336,   173,   173,   217,   173,   173,   191,   216,   278,
     360,   366,   362,   366,   366,   198,   217,   366,   217,   216,
     366,   217,   216,   218,   332,   215,   218,   332,   356,   218,
     332,   224,    47,   373,    46,   106,   370,   375,   337,   451,
     455,   459,   217,   464,   173,   367,   495,   497,   287,   332,
     306,   218,   199,   433,   217,   173,   330,   199,   199,   199,
     516,   294,   199,   224,   323,   325,   327,   518,   501,   506,
     499,   217,   340,   278,   218,   332,   185,   218,   486,   490,
     217,   137,   389,   185,   486,   216,   185,   298,   300,   233,
     181,   332,   332,   185,    20,   332,   218,   218,   199,   278,
     287,   367,   254,   224,   185,   277,   218,   479,   173,   216,
     194,   394,   218,   173,   335,   216,   142,   287,   333,   436,
     218,   464,   218,   218,   364,   366,   366,   218,   366,   218,
     479,   218,   218,   375,    33,   371,   370,   372,   292,   217,
     217,   218,   367,   173,   367,   199,   510,   512,   514,   217,
     218,   217,   367,   367,   367,   217,    70,   492,   217,   217,
     218,   366,   333,   218,   366,   137,   389,   490,   366,   367,
     367,   366,   491,   503,   367,   217,   293,   231,   218,   218,
     366,   332,   185,   351,   199,   287,    26,   105,   255,   308,
     309,   310,   312,   367,   503,   278,   216,   194,   394,   436,
     393,   218,   127,   367,   199,   199,   464,   218,   218,   218,
     216,   378,   371,   390,   391,   392,   218,   492,   492,   287,
     218,   199,   218,   217,   217,   217,   217,   292,   294,   332,
     492,   492,   218,   224,   522,   367,   367,   218,   522,   522,
     292,   177,   185,   185,   522,   218,   366,   348,   353,   251,
     127,   127,   367,   522,   287,   218,   436,   393,   393,   367,
     367,   361,   363,   199,   218,   282,   379,   217,   479,   483,
     484,   485,   485,   367,   367,   492,   492,   479,   480,   218,
     218,   523,   485,   480,    53,   216,   137,   389,   184,   286,
     191,   523,   503,   366,   216,   185,   522,   348,   367,   286,
     393,   367,   367,   224,   365,   224,   282,   479,   191,   523,
     218,   218,   218,   218,   485,   485,   218,   218,   218,   218,
     367,   216,   367,   367,   216,   286,   218,   522,   522,   366,
     216,   367,   224,   224,   375,   287,   218,   217,   218,   218,
     184,   216,   522,   224,   479,   216,   218
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   222,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   224,   224,   225,
     226,   226,   226,   227,   227,   228,   229,   229,   229,   229,
     230,   230,   231,   231,   232,   233,   232,   234,   234,   234,
     235,   236,   236,   238,   237,   239,   240,   241,   241,   241,
     242,   242,   242,   242,   243,   243,   244,   244,   245,   246,
     246,   247,   247,   248,   249,   249,   250,   250,   251,   251,
     251,   252,   252,   253,   254,   253,   255,   255,   255,   255,
     255,   256,   257,   256,   259,   258,   260,   258,   261,   262,
     263,   265,   264,   266,   266,   266,   266,   266,   266,   267,
     267,   268,   268,   268,   269,   269,   269,   269,   269,   269,
     269,   269,   270,   270,   271,   271,   271,   272,   272,   272,
     272,   273,   273,   274,   274,   274,   274,   274,   274,   274,
     275,   275,   276,   276,   277,   277,   277,   278,   278,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   280,   281,   281,   281,   282,   284,
     283,   285,   285,   286,   286,   287,   287,   288,   288,   288,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   290,   290,   290,   291,   292,   292,   293,   293,
     294,   294,   295,   295,   297,   298,   296,   299,   300,   296,
     301,   301,   301,   301,   301,   302,   302,   302,   303,   303,
     305,   306,   304,   304,   307,   307,   307,   307,   307,   307,
     308,   309,   310,   310,   310,   311,   311,   311,   312,   312,
     313,   313,   313,   314,   315,   315,   315,   316,   316,   317,
     317,   318,   318,   319,   319,   319,   319,   320,   320,   322,
     323,   321,   324,   325,   321,   326,   327,   321,   329,   330,
     328,   331,   331,   331,   331,   331,   331,   332,   332,   333,
     333,   333,   334,   334,   334,   335,   335,   335,   335,   335,
     336,   336,   337,   337,   337,   338,   338,   339,   341,   340,
     342,   342,   342,   342,   342,   342,   342,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   344,   344,   344,   344,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   346,   346,   347,
     347,   348,   348,   349,   350,   351,   349,   352,   353,   349,
     354,   354,   354,   354,   354,   354,   354,   355,   356,   354,
     357,   357,   357,   357,   357,   357,   357,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   360,   361,   359,   359,   359,   359,   362,
     363,   359,   359,   359,   364,   365,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   366,   366,   367,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     369,   369,   369,   370,   370,   370,   371,   371,   372,   372,
     372,   373,   373,   374,   375,   375,   375,   376,   376,   377,
     376,   378,   376,   379,   376,   376,   380,   381,   381,   382,
     382,   382,   382,   382,   383,   383,   384,   384,   385,   385,
     385,   386,   387,   387,   388,   388,   388,   389,   389,   390,
     390,   390,   391,   391,   392,   392,   393,   393,   393,   394,
     394,   395,   395,   395,   395,   395,   396,   396,   396,   396,
     396,   397,   398,   397,   399,   399,   400,   400,   400,   401,
     402,   401,   403,   403,   403,   404,   404,   404,   406,   405,
     407,   407,   408,   409,   408,   410,   410,   410,   411,   412,
     412,   413,   413,   414,   414,   415,   416,   416,   416,   416,
     417,   417,   417,   418,   418,   420,   421,   419,   422,   422,
     422,   422,   422,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     424,   424,   424,   424,   424,   424,   424,   424,   425,   426,
     426,   426,   427,   427,   428,   428,   429,   429,   429,   429,
     431,   432,   430,   433,   433,   434,   434,   435,   435,   436,
     436,   436,   436,   436,   436,   437,   438,   436,   436,   436,
     439,   436,   436,   436,   436,   436,   436,   436,   436,   436,
     436,   436,   436,   436,   440,   441,   436,   436,   442,   443,
     436,   444,   445,   436,   446,   447,   436,   436,   448,   449,
     436,   450,   451,   436,   436,   452,   453,   436,   454,   455,
     436,   436,   456,   457,   436,   458,   459,   436,   460,   461,
     436,   462,   463,   436,   464,   464,   464,   466,   467,   468,
     469,   465,   471,   472,   473,   474,   470,   476,   477,   475,
     478,   478,   478,   478,   478,   479,   479,   479,   479,   479,
     479,   479,   479,   480,   480,   481,   482,   482,   483,   483,
     484,   484,   485,   485,   486,   486,   487,   487,   488,   488,
     489,   489,   490,   490,   490,   491,   491,   491,   492,   492,
     493,   493,   493,   493,   493,   493,   494,   495,   493,   496,
     497,   493,   498,   499,   493,   500,   501,   493,   502,   502,
     502,   503,   503,   504,   505,   506,   504,   507,   507,   508,
     508,   508,   509,   510,   508,   511,   512,   508,   513,   514,
     508,   508,   515,   516,   508,   508,   517,   518,   508,   519,
     519,   520,   520,   521,   521,   521,   521,   521,   522,   522,
     523,   523,   524,   524,   524,   524,   524,   524,   524,   524,
     524
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
       4,     1,     1,     0,     0,     3,     1,     1,     1,     1,
       1,     5,     0,     6,     0,     6,     0,     8,     2,     3,
       3,     0,     6,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     5,     1,     3,     2,     3,     2,     1,     1,     1,
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
       4,     1,     1,     1,     1,     3,     7,     2,     2,     6,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     2,
       2,     2,     0,     2,     2,     3,     0,     2,     0,     4,
       0,     2,     1,     3,     0,     0,     7,     0,     0,     7,
       3,     2,     2,     2,     1,     1,     3,     2,     2,     3,
       0,     0,     5,     1,     2,     5,     5,     5,     6,     2,
       1,     1,     1,     2,     3,     2,     2,     3,     2,     3,
       2,     2,     3,     4,     1,     1,     0,     1,     1,     1,
       0,     1,     3,     9,     8,     8,     7,     3,     3,     0,
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
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       4,     5,     4,     5,     3,     4,     2,     5,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     1,     4,     4,
       7,     8,     3,     0,     0,     8,     3,     3,     3,     0,
       0,     8,     3,     4,     0,     0,     9,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     2,     4,
       1,     1,     1,     1,     4,     4,     4,     4,     4,     1,
       6,     7,     6,     6,     7,     7,     6,     7,     6,     6,
       0,     4,     1,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     0,     1,     1,     0,     2,     0,
       5,     0,    10,     0,    10,     6,     3,     3,     4,     1,
       1,     3,     3,     3,     1,     3,     1,     3,     0,     2,
       3,     3,     1,     3,     0,     2,     3,     1,     1,     1,
       2,     3,     3,     5,     1,     1,     1,     1,     1,     0,
       1,     1,     4,     3,     3,     5,     4,     6,     5,     5,
       4,     0,     0,     5,     0,     1,     0,     1,     1,     6,
       0,     6,     0,     3,     5,     0,     1,     1,     0,     5,
       3,     4,     4,     0,     4,     0,     1,     1,     1,     7,
       9,     0,     2,     0,     1,     3,     1,     1,     2,     2,
       0,     1,     1,     0,     3,     0,     0,     7,     1,     4,
       3,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     1,     3,     0,     2,     0,     2,     3,     3,
       0,     0,     6,     1,     1,     1,     3,     3,     4,     1,
       1,     1,     1,     2,     3,     0,     0,     6,     4,     5,
       0,     9,     4,     2,     2,     3,     2,     3,     2,     2,
       3,     3,     3,     2,     0,     0,     6,     2,     0,     0,
       6,     0,     0,     6,     0,     0,     6,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     0,     0,
       6,     0,     0,     6,     1,     3,     3,     0,     0,     0,
       0,    10,     0,     0,     0,     0,    10,     0,     0,     9,
       1,     1,     1,     1,     1,     3,     3,     5,     5,     6,
       6,     8,     8,     0,     1,     2,     1,     3,     3,     5,
       1,     2,     1,     0,     0,     2,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     2,     1,     1,     0,     1,
       5,     4,     6,     7,     5,     7,     0,     0,    10,     0,
       0,    10,     0,     0,    10,     0,     0,     7,     1,     3,
       3,     3,     1,     5,     0,     0,    10,     1,     3,     3,
       4,     4,     0,     0,    11,     0,     0,    11,     0,     0,
      10,     5,     0,     0,     9,     5,     0,     0,    10,     1,
       3,     1,     3,     3,     3,     4,     7,     9,     0,     3,
       0,     1,     9,    11,    12,    11,    10,    10,    10,     9,
      10
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

    case YYSYMBOL_expr_not_wrapped: /* expr_not_wrapped  */
            { delete ((*yyvaluep).pExpression); }
        break;

    case YYSYMBOL_expr_wrapped: /* expr_wrapped  */
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

  case 71: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 72: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 73: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 74: /* $@3: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 75: /* expression_else_one_liner: "else" $@3 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 76: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 77: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 78: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 79: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 80: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 81: /* expression_if_then_else: if_or_static_if expr expression_block opt_sem expression_else  */
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

  case 82: /* $@4: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 83: /* expression_if_then_else: expression_if_one_liner "if" $@4 expr expression_else_one_liner semicolon  */
                                                                                                                                                         {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-2])), (yyvsp[-2].pExpression)->at);

        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),(yyvsp[-2].pExpression),ast_wrapInBlock((yyvsp[-5].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 84: /* $@5: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 85: /* expression_for_loop: "for" $@5 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        format::wrap_par_expr(tokRangeAt(scanner, (yylsp[-3]), (yylsp[-1])), tokRangeAt(scanner, (yylsp[-3]), (yylsp[-1])));
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 86: /* $@6: %empty  */
                     { // Had to add to successfully convert to v2 syntax, just copied from ds2_parser
                             yyextra->das_keyword = true;
     }
    break;

  case 87: /* expression_for_loop: "for" $@6 '(' variable_name_with_pos_list "in" expr_list ')' expression_block  */
                                                                                                  {
        yyextra->das_keyword = false;
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-4].pNameWithPosList),(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-7])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 88: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 89: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at);

        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = (yyvsp[-1].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 90: /* expression_with: "with" expr expression_block  */
                                                         {
        format::wrap_par_expr(tokAt(scanner,(yylsp[-1])), (yyvsp[-1].pExpression)->at);
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = (yyvsp[-1].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 91: /* $@7: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 92: /* expression_with_alias: "assume" "name" '=' $@7 expr semicolon  */
                                                                                                         {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-5])), *(yyvsp[-4].s), (yyvsp[-1].pExpression) );
        delete (yyvsp[-4].s);
    }
    break;

  case 93: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 94: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 95: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 96: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 97: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 98: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 99: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 100: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 101: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 102: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 103: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 104: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 105: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 106: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 107: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 108: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); delete (yyvsp[-2].s); }
    break;

  case 110: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 111: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                               {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokRangeAt(scanner,(yylsp[-4]),(yylsp[0]))); delete (yyvsp[-4].s); }
    }
    break;

  case 112: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 113: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 114: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 115: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 116: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 117: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 118: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 119: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 120: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 121: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 122: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 123: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 124: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 125: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 126: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 127: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 128: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 129: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 130: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 131: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 132: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 133: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 134: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 135: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 136: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 137: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 138: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 139: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 140: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 141: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 142: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 143: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 144: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 145: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 146: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 147: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 148: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 149: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 150: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 151: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 152: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 153: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 154: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 155: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 156: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 157: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 158: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 159: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 160: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 161: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 162: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 163: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 164: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 165: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 166: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 167: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 168: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 169: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 170: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 171: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 172: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 173: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 174: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 175: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 176: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 177: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 178: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 179: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 180: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 181: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 182: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 183: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 184: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 185: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 186: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 187: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 188: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 189: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 190: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 191: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 192: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 193: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 194: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 195: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 196: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 197: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 198: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 199: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 200: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 201: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 202: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 203: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 204: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 205: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 206: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 207: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 208: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 209: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 210: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 211: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 212: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 213: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 214: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 215: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 216: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 217: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 218: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 219: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 220: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 221: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 222: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 223: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 224: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 225: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 226: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 227: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 228: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 229: /* $@8: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 230: /* function_declaration: optional_public_or_private_function $@8 function_declaration_header expression_block  */
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

  case 231: /* open_block: "begin of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 232: /* open_block: OPEN_BRACE  */
                        { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 233: /* close_block: "end of code block"  */
          { (yyval.ui) = 0xdeadbeef; }
    break;

  case 234: /* close_block: CLOSE_BRACE  */
                         { (yyval.ui) = (yyvsp[0].i); }
    break;

  case 235: /* expression_block: open_block expressions close_block  */
                                                                  {
        auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui), format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size, format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 236: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
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

  case 237: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 238: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 239: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 240: /* expression_any: SEMICOLON  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 241: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 242: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 244: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 245: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 246: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 247: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 249: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 250: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 251: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 252: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 254: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 255: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 257: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 258: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 259: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 260: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 261: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 262: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 263: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 264: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 265: /* expr_keyword: "keyword" expr expression_block  */
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

  case 266: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 267: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 268: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 269: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 270: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 271: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 272: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 273: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 274: /* $@9: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 275: /* $@10: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 276: /* expression_keyword: "keyword" '<' $@9 type_declaration_no_options_list '>' $@10 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 277: /* $@11: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 278: /* $@12: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 279: /* expression_keyword: "type function" '<' $@11 type_declaration_no_options_list '>' $@12 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 280: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 281: /* expr_pipe: "@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }

        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 282: /* expr_pipe: "@@ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "@@";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 283: /* expr_pipe: "$ <|" expr_block  */
                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner, (yylsp[-1]))))) {
            format::get_writer() << "$";
            format::finish_rule(format::Pos::from_last(tokAt(scanner, (yylsp[-1]))));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 284: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 285: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 286: /* name_in_namespace: "name" "::" "name"  */
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

  case 287: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 288: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 289: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 290: /* $@13: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 291: /* $@14: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 292: /* new_type_declaration: '<' $@13 type_declaration '>' $@14  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 293: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 294: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 295: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 296: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 297: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 298: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 299: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 300: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 301: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 302: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 303: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 304: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 305: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 306: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 307: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 308: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 309: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 310: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 311: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 312: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 313: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                              {
        const auto end_block = format::Pos::from_last(tokAt(scanner, (yylsp[-2])));
        const auto start = format::Pos::from(tokAt(scanner, (yylsp[-3])));
        if (format::is_replace_braces()) {
            format::skip_spaces_or_print(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])), yyextra->das_tab_size);
        }

        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 314: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 315: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 316: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 317: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 318: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 319: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 320: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 321: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 322: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 323: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
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

  case 324: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 325: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 326: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 327: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 328: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 329: /* $@15: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 330: /* $@16: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 331: /* expr_cast: "cast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 332: /* $@17: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 333: /* $@18: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 334: /* expr_cast: "upcast" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 335: /* $@19: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 336: /* $@20: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 337: /* expr_cast: "reinterpret" '<' $@19 type_declaration_no_options '>' $@20 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 338: /* $@21: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 339: /* $@22: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 340: /* expr_type_decl: "type" '<' $@21 type_declaration '>' $@22  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 341: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 342: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 343: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 344: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 345: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 346: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" semicolon "name" '>' '(' expr ')'  */
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

  case 347: /* expr_list: expr2  */
                       {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 348: /* expr_list: expr_list ',' expr2  */
                                             {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 349: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 350: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 351: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 352: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 353: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 354: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 355: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 356: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 357: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 358: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 359: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 360: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 361: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 362: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 363: /* optional_capture_list: "[[" capture_list ']' ']'  */
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

  case 364: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 365: /* expr_block: expression_block  */
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

  case 366: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 367: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 368: /* $@23: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 369: /* expr_full_block_assumed_piped: block_or_lambda $@23 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 370: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 371: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 372: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 373: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 374: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 375: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 376: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 377: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 378: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 379: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 380: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 381: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 382: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 383: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 384: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 385: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 386: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 387: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 388: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 389: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 390: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 391: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 392: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 393: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 394: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign_pipe_right: "@ <|" expr_block  */
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

  case 397: /* expr_assign_pipe_right: "@@ <|" expr_block  */
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

  case 398: /* expr_assign_pipe_right: "$ <|" expr_block  */
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

  case 399: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 400: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 401: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 410: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 411: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 412: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 413: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 414: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 415: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 416: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 418: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 419: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 420: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 421: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 422: /* func_addr_name: "$i" '(' expr2 ')'  */
                                           {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 423: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 424: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 425: /* $@25: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 426: /* func_addr_expr: '@' '@' '<' $@24 type_declaration_no_options '>' $@25 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 427: /* $@26: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 428: /* $@27: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 429: /* func_addr_expr: '@' '@' '<' $@26 optional_function_argument_list optional_function_type '>' $@27 func_addr_name  */
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

  case 430: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 431: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 432: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 433: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 434: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 435: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 436: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 437: /* $@28: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 438: /* $@29: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 439: /* expr_field: expr '.' $@28 error $@29  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 440: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 441: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 442: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 443: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
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

  case 444: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 445: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 446: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 447: /* expr_not_wrapped: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 448: /* expr_not_wrapped: expr_field  */
                                              { (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 449: /* expr_not_wrapped: expr_mtag  */
                                              { (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 450: /* expr_not_wrapped: '!' expr2  */
                                               { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"!",(yyvsp[0].pExpression)); }
    break;

  case 451: /* expr_not_wrapped: '~' expr2  */
                                               { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"~",(yyvsp[0].pExpression)); }
    break;

  case 452: /* expr_not_wrapped: '+' expr2  */
                                                   { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+",(yyvsp[0].pExpression)); }
    break;

  case 453: /* expr_not_wrapped: '-' expr2  */
                                                   { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"-",(yyvsp[0].pExpression)); }
    break;

  case 454: /* expr_not_wrapped: expr2 "<<" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 455: /* expr_not_wrapped: expr2 ">>" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 456: /* expr_not_wrapped: expr2 "<<<" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr_not_wrapped: expr2 ">>>" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr_not_wrapped: expr2 '+' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr_not_wrapped: expr2 '-' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr_not_wrapped: expr2 '*' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr_not_wrapped: expr2 '/' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr_not_wrapped: expr2 '%' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr_not_wrapped: expr2 '<' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr_not_wrapped: expr2 '>' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr_not_wrapped: expr2 "==" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr_not_wrapped: expr2 "!=" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr_not_wrapped: expr2 "<=" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr_not_wrapped: expr2 ">=" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_not_wrapped: expr2 '&' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_not_wrapped: expr2 '|' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_not_wrapped: expr2 '^' expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_not_wrapped: expr2 "&&" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr_not_wrapped: expr2 "||" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr_not_wrapped: expr2 "^^" expr2  */
                                               { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr_not_wrapped: expr2 ".." expr2  */
                                               {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 476: /* expr_not_wrapped: "++" expr2  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"++", (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr_not_wrapped: "--" expr2  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"--", (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr_not_wrapped: expr2 "++"  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 479: /* expr_not_wrapped: expr2 "--"  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 480: /* expr_not_wrapped: expr2 '[' expr2 ']'  */
                                                        { (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 481: /* expr_not_wrapped: expr2 '.' '[' expr2 ']'  */
                                                            { (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 482: /* expr_not_wrapped: expr2 "?[" expr2 ']'  */
                                                        { (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 483: /* expr_not_wrapped: expr2 '.' "?[" expr2 ']'  */
                                                            { (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 484: /* expr_not_wrapped: expr2 "?." "name"  */
                                                  { (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 485: /* expr_not_wrapped: expr2 '.' "?." "name"  */
                                                      { (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 486: /* expr_not_wrapped: '*' expr2  */
                                                    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 487: /* expr_not_wrapped: expr2 '?' expr2 ':' expr2  */
                                                             {
            (yyval.pExpression) = new ExprOp3(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 488: /* expr_wrapped: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 489: /* expr_wrapped: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 490: /* expr_wrapped: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 491: /* expr_wrapped: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 492: /* expr_wrapped: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 493: /* expr_wrapped: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 494: /* expr_wrapped: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 495: /* expr_wrapped: '(' expr_list optional_comma ')'  */
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

  case 496: /* expr_wrapped: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 497: /* expr_wrapped: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 498: /* expr_wrapped: "deref" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 499: /* expr_wrapped: "addr" '(' expr2 ')'  */
                                                    { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 500: /* expr_wrapped: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 501: /* expr_wrapped: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr2 ')'  */
                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 502: /* expr_wrapped: expr2 "??" expr2  */
                                                     { (yyval.pExpression) = new ExprNullCoalescing(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 503: /* $@30: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 504: /* $@31: %empty  */
                                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 505: /* expr_wrapped: expr2 "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                             {
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 506: /* expr_wrapped: expr2 "is" basic_type_declaration  */
                                                                {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 507: /* expr_wrapped: expr2 "is" "name"  */
                                               {
        (yyval.pExpression) = new ExprIsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 508: /* expr_wrapped: expr2 "as" "name"  */
                                               {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 509: /* $@32: %empty  */
                                                { yyextra->das_arrow_depth ++; }
    break;

  case 510: /* $@33: %empty  */
                                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 511: /* expr_wrapped: expr2 "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                  {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 512: /* expr_wrapped: expr2 "as" basic_type_declaration  */
                                                                {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 513: /* expr_wrapped: expr2 '?' "as" "name"  */
                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 514: /* $@34: %empty  */
                                                    { yyextra->das_arrow_depth ++; }
    break;

  case 515: /* $@35: %empty  */
                                                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 516: /* expr_wrapped: expr2 '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                      {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-8]), (yylsp[-1])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 517: /* expr_wrapped: expr2 '?' "as" basic_type_declaration  */
                                                                    {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 518: /* expr_wrapped: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 519: /* expr_wrapped: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 520: /* expr_wrapped: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 521: /* expr_wrapped: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 522: /* expr_wrapped: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); (yyval.pExpression)->at = tokAt(scanner, (yylsp[0])); }
    break;

  case 523: /* expr_wrapped: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr_wrapped: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 525: /* expr_wrapped: expr2 "<|" expr2  */
                                                  { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); }
    break;

  case 526: /* expr_wrapped: expr2 "|>" expr2  */
                                                  { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]), (yylsp[0]))); (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])); }
    break;

  case 527: /* expr_wrapped: expr2 "|>" basic_type_declaration  */
                                                           {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
    }
    break;

  case 528: /* expr_wrapped: name_in_namespace "name"  */
                                                  {
        if (format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << "." << format::get_substring(tokAt(scanner,(yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 529: /* expr_wrapped: "unsafe" '(' expr2 ')'  */
                                          {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 530: /* expr_wrapped: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 531: /* expr2: expr_wrapped  */
                       {
        // Bison is bottom up grammar. Top expression will be parsed latest.
        need_wrap_current_expr = false;
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 532: /* expr2: expr_not_wrapped  */
                           {
        need_wrap_current_expr = true;
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
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

  case 567: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 568: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
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

  case 574: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@38 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
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

  case 620: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 621: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 622: /* $@39: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 623: /* global_variable_declaration_list: global_variable_declaration_list $@39 optional_field_annotation let_variable_declaration opt_sem  */
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

  case 624: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 625: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 626: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 627: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 628: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 629: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                                   {
        handle_brace(format::Pos::from(tokAt(scanner, (yylsp[-2]))), (yyvsp[-2].ui),
                     format::get_substring(tokRangeAt(scanner, (yylsp[-2]), (yylsp[0]))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 630: /* $@40: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 631: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@40 optional_field_annotation let_variable_declaration  */
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

  case 632: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 633: /* enum_list: enum_list "name" opt_sem  */
                                              {
        format::skip_token(true, tokAt(scanner,(yylsp[0])));
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

  case 634: /* enum_list: enum_list "name" '=' expr opt_sem  */
                                                              {
        format::skip_token(true, tokAt(scanner,(yylsp[0])));
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

  case 635: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 636: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 637: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 638: /* $@41: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 639: /* single_alias: optional_public_or_private_alias "name" $@41 '=' type_declaration  */
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

  case 640: /* alias_list: single_alias semicolon opt_sem  */
                                                   {
        (yyval.positions) = new vector<LineInfo>(1, tokAt(scanner, (yylsp[-2])));
    }
    break;

  case 641: /* alias_list: alias_list single_alias semicolon opt_sem  */
                                                                      {
        (yyvsp[-3].positions)->emplace_back(tokAt(scanner, (yylsp[-2])));
        (yyval.positions) = (yyvsp[-3].positions);
    }
    break;

  case 642: /* alias_declaration: "typedef" open_block alias_list close_block  */
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

  case 643: /* $@42: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 645: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 646: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 647: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 648: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 649: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block enum_list close_block  */
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

  case 650: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block enum_list close_block  */
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

  case 651: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 652: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 653: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 654: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 655: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 656: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 657: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 658: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 659: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 660: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 661: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 662: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 663: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 664: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                                      {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 665: /* $@43: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 666: /* $@44: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 667: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@43 structure_name $@44 optional_struct_variable_declaration_list  */
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

  case 668: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 669: /* variable_name_with_pos_list: "$i" '(' expr2 ')'  */
                                      {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 670: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 671: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 672: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 673: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 674: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 675: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 676: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 677: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 678: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 679: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 680: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 681: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 682: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 683: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 684: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 685: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 686: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 687: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 688: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 689: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 690: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 691: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 692: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 693: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 694: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 695: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 696: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 697: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 698: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 699: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 700: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 701: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 702: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 703: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 704: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 705: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 706: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 707: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 708: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 709: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 710: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 711: /* auto_type_declaration: "$t" '(' expr2 ')'  */
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

  case 712: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 713: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 716: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 717: /* bitfield_alias_bits: bitfield_alias_bits SEMICOLON  */
                                            {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
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

  case 720: /* $@45: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 721: /* $@46: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 722: /* bitfield_type_declaration: "bitfield" '<' $@45 bitfield_bits '>' $@46  */
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

  case 725: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 726: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 727: /* dim_list: '[' expr2 ']'  */
                              {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 728: /* dim_list: dim_list '[' expr2 ']'  */
                                             {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 729: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 730: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 731: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 732: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 733: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 734: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 735: /* $@47: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 736: /* $@48: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 737: /* type_declaration_no_options: "type" '<' $@47 type_declaration '>' $@48  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 738: /* type_declaration_no_options: "typedecl" '(' expr2 ')'  */
                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 739: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 740: /* $@49: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 741: /* type_declaration_no_options: '$' name_in_namespace '<' $@49 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 742: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 743: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 744: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 745: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 746: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 747: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 748: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 749: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 750: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 751: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 752: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 753: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 754: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 755: /* $@51: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 756: /* type_declaration_no_options: "smart_ptr" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 757: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 758: /* $@52: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 759: /* $@53: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 760: /* type_declaration_no_options: "array" '<' $@52 type_declaration '>' $@53  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 761: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 762: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 763: /* type_declaration_no_options: "table" '<' $@54 table_type_pair '>' $@55  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 764: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 765: /* $@57: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 766: /* type_declaration_no_options: "iterator" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 767: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 768: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 769: /* $@59: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 770: /* type_declaration_no_options: "block" '<' $@58 type_declaration '>' $@59  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 771: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 772: /* $@61: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 773: /* type_declaration_no_options: "block" '<' $@60 optional_function_argument_list optional_function_type '>' $@61  */
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

  case 774: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 775: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 776: /* $@63: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 777: /* type_declaration_no_options: "function" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 778: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 779: /* $@65: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 780: /* type_declaration_no_options: "function" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
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

  case 781: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 782: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 783: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 784: /* type_declaration_no_options: "lambda" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 785: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 786: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 787: /* type_declaration_no_options: "lambda" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 788: /* $@70: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 789: /* $@71: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 790: /* type_declaration_no_options: "tuple" '<' $@70 tuple_type_list '>' $@71  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 791: /* $@72: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 792: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 793: /* type_declaration_no_options: "variant" '<' $@72 variant_type_list '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 794: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 795: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 796: /* type_declaration: type_declaration '|' '#'  */
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

  case 797: /* $@74: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 798: /* $@75: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 799: /* $@76: %empty  */
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

  case 800: /* $@77: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 801: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@74 "name" $@75 open_block $@76 tuple_alias_type_list $@77 close_block  */
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

  case 802: /* $@78: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 803: /* $@79: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 804: /* $@80: %empty  */
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

  case 805: /* $@81: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 806: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@78 "name" $@79 open_block $@80 variant_alias_type_list $@81 close_block  */
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

  case 807: /* $@82: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 808: /* $@83: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 809: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@82 "name" $@83 open_block bitfield_alias_bits commas close_block  */
                                                                          {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-3])));
        handle_brace(prev_loc, (yyvsp[-3].ui),
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

  case 810: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 811: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 812: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 813: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 814: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 815: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 816: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 817: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 818: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 819: /* make_struct_fields: "$f" '(' expr2 ')' copy_or_move expr  */
                                                                            {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 820: /* make_struct_fields: "$f" '(' expr2 ')' ":=" expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner, (yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 821: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' copy_or_move expr  */
                                                                                                        {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 822: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr2 ')' ":=" expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 823: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 824: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 825: /* make_struct_single: make_struct_fields optional_comma  */
                                               {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 826: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 827: /* make_struct_dim: make_struct_dim semicolon make_struct_fields  */
                                                               {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 828: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 829: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 830: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 831: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 832: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 833: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 834: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 835: /* optional_block: "where" expr_block  */
                                  { (yyvsp[0].pExpression)->at = tokAt(scanner, (yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 848: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 849: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 850: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 851: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
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

  case 852: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
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

  case 853: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 854: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 855: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 856: /* $@84: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 857: /* $@85: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 858: /* make_struct_decl: "struct" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 859: /* $@86: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 860: /* $@87: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 861: /* make_struct_decl: "class" '<' $@86 type_declaration_no_options '>' $@87 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 862: /* $@88: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 863: /* $@89: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 864: /* make_struct_decl: "variant" '<' $@88 variant_type_list '>' $@89 '(' use_initializer make_variant_dim ')'  */
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

  case 865: /* $@90: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 866: /* $@91: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 867: /* make_struct_decl: "default" '<' $@90 type_declaration_no_options '>' $@91 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 868: /* make_tuple: expr  */
                  {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 869: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 870: /* make_tuple: make_tuple ',' expr  */
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

  case 871: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 872: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 873: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 874: /* $@92: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 875: /* $@93: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 876: /* make_tuple_call: "tuple" '<' $@92 tuple_type_list '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 877: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 878: /* make_dim: make_dim semicolon make_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 879: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 880: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
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

  case 881: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
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

  case 882: /* $@94: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 883: /* $@95: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 884: /* make_dim_decl: "array" "struct" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 885: /* $@96: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 886: /* $@97: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 887: /* make_dim_decl: "array" "tuple" '<' $@96 tuple_type_list '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 888: /* $@98: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 889: /* $@99: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 890: /* make_dim_decl: "array" "variant" '<' $@98 variant_type_list '>' $@99 '(' make_variant_dim ')'  */
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

  case 891: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 892: /* $@100: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 893: /* $@101: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 894: /* make_dim_decl: "array" '<' $@100 type_declaration_no_options '>' $@101 '(' optional_expr_list ')'  */
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

  case 895: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 896: /* $@102: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 897: /* $@103: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 898: /* make_dim_decl: "fixed_array" '<' $@102 type_declaration_no_options '>' $@103 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 899: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 900: /* make_table: make_table semicolon make_map_tuple  */
                                                      {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 901: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 902: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 903: /* make_table_decl: open_block optional_expr_map_tuple_list close_block  */
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

  case 904: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 905: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 906: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 907: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 908: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 909: /* array_comprehension_where: semicolon "where" expr  */
                                          { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 910: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 911: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 912: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                     {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 913: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 914: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                         {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 915: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 916: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']'  */
                                                                                                                                                                  {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 917: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where ']' ']'  */
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

  case 918: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr2 array_comprehension_where "end of code block" ']'  */
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

  case 919: /* array_comprehension: open_block "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block  */
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

  case 920: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block close_block  */
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


