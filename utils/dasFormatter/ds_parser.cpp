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
  YYSYMBOL_expr = 359,                     /* expr  */
  YYSYMBOL_360_30 = 360,                   /* $@30  */
  YYSYMBOL_361_31 = 361,                   /* $@31  */
  YYSYMBOL_362_32 = 362,                   /* $@32  */
  YYSYMBOL_363_33 = 363,                   /* $@33  */
  YYSYMBOL_364_34 = 364,                   /* $@34  */
  YYSYMBOL_365_35 = 365,                   /* $@35  */
  YYSYMBOL_expr_mtag = 366,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 367, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 368,        /* optional_override  */
  YYSYMBOL_optional_constant = 369,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 370, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 371, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 372, /* structure_variable_declaration  */
  YYSYMBOL_opt_sem = 373,                  /* opt_sem  */
  YYSYMBOL_struct_variable_declaration_list = 374, /* struct_variable_declaration_list  */
  YYSYMBOL_375_36 = 375,                   /* $@36  */
  YYSYMBOL_376_37 = 376,                   /* $@37  */
  YYSYMBOL_377_38 = 377,                   /* $@38  */
  YYSYMBOL_function_argument_declaration_no_type = 378, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 379, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 380,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 381,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 382,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 383,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 384,             /* variant_type  */
  YYSYMBOL_variant_type_list = 385,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 386,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 387,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 388, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 389, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 390,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 391,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 392,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 393, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 394, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 395, /* global_variable_declaration_list  */
  YYSYMBOL_396_39 = 396,                   /* $@39  */
  YYSYMBOL_optional_shared = 397,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 398, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 399,               /* global_let  */
  YYSYMBOL_400_40 = 400,                   /* $@40  */
  YYSYMBOL_enum_list = 401,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 402, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 403,             /* single_alias  */
  YYSYMBOL_404_41 = 404,                   /* $@41  */
  YYSYMBOL_alias_list = 405,               /* alias_list  */
  YYSYMBOL_alias_declaration = 406,        /* alias_declaration  */
  YYSYMBOL_407_42 = 407,                   /* $@42  */
  YYSYMBOL_optional_public_or_private_enum = 408, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 409,                /* enum_name  */
  YYSYMBOL_enum_declaration = 410,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 411, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 412,          /* optional_sealed  */
  YYSYMBOL_structure_name = 413,           /* structure_name  */
  YYSYMBOL_class_or_struct = 414,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 415, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 416, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 417,    /* structure_declaration  */
  YYSYMBOL_418_43 = 418,                   /* $@43  */
  YYSYMBOL_419_44 = 419,                   /* $@44  */
  YYSYMBOL_variable_name_with_pos_list = 420, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 421,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 422, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 423, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 424,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 425,            /* bitfield_bits  */
  YYSYMBOL_commas = 426,                   /* commas  */
  YYSYMBOL_bitfield_alias_bits = 427,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 428, /* bitfield_type_declaration  */
  YYSYMBOL_429_45 = 429,                   /* $@45  */
  YYSYMBOL_430_46 = 430,                   /* $@46  */
  YYSYMBOL_c_or_s = 431,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 432,          /* table_type_pair  */
  YYSYMBOL_dim_list = 433,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 434, /* type_declaration_no_options  */
  YYSYMBOL_435_47 = 435,                   /* $@47  */
  YYSYMBOL_436_48 = 436,                   /* $@48  */
  YYSYMBOL_437_49 = 437,                   /* $@49  */
  YYSYMBOL_438_50 = 438,                   /* $@50  */
  YYSYMBOL_439_51 = 439,                   /* $@51  */
  YYSYMBOL_440_52 = 440,                   /* $@52  */
  YYSYMBOL_441_53 = 441,                   /* $@53  */
  YYSYMBOL_442_54 = 442,                   /* $@54  */
  YYSYMBOL_443_55 = 443,                   /* $@55  */
  YYSYMBOL_444_56 = 444,                   /* $@56  */
  YYSYMBOL_445_57 = 445,                   /* $@57  */
  YYSYMBOL_446_58 = 446,                   /* $@58  */
  YYSYMBOL_447_59 = 447,                   /* $@59  */
  YYSYMBOL_448_60 = 448,                   /* $@60  */
  YYSYMBOL_449_61 = 449,                   /* $@61  */
  YYSYMBOL_450_62 = 450,                   /* $@62  */
  YYSYMBOL_451_63 = 451,                   /* $@63  */
  YYSYMBOL_452_64 = 452,                   /* $@64  */
  YYSYMBOL_453_65 = 453,                   /* $@65  */
  YYSYMBOL_454_66 = 454,                   /* $@66  */
  YYSYMBOL_455_67 = 455,                   /* $@67  */
  YYSYMBOL_456_68 = 456,                   /* $@68  */
  YYSYMBOL_457_69 = 457,                   /* $@69  */
  YYSYMBOL_458_70 = 458,                   /* $@70  */
  YYSYMBOL_459_71 = 459,                   /* $@71  */
  YYSYMBOL_460_72 = 460,                   /* $@72  */
  YYSYMBOL_461_73 = 461,                   /* $@73  */
  YYSYMBOL_type_declaration = 462,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 463,  /* tuple_alias_declaration  */
  YYSYMBOL_464_74 = 464,                   /* $@74  */
  YYSYMBOL_465_75 = 465,                   /* $@75  */
  YYSYMBOL_466_76 = 466,                   /* $@76  */
  YYSYMBOL_467_77 = 467,                   /* $@77  */
  YYSYMBOL_variant_alias_declaration = 468, /* variant_alias_declaration  */
  YYSYMBOL_469_78 = 469,                   /* $@78  */
  YYSYMBOL_470_79 = 470,                   /* $@79  */
  YYSYMBOL_471_80 = 471,                   /* $@80  */
  YYSYMBOL_472_81 = 472,                   /* $@81  */
  YYSYMBOL_bitfield_alias_declaration = 473, /* bitfield_alias_declaration  */
  YYSYMBOL_474_82 = 474,                   /* $@82  */
  YYSYMBOL_475_83 = 475,                   /* $@83  */
  YYSYMBOL_make_decl = 476,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 477,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 478,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 479,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 480,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 481,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 482,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 483, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 484,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 485, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 486, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 487, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 488, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 489, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 490,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 491,         /* make_struct_decl  */
  YYSYMBOL_492_84 = 492,                   /* $@84  */
  YYSYMBOL_493_85 = 493,                   /* $@85  */
  YYSYMBOL_494_86 = 494,                   /* $@86  */
  YYSYMBOL_495_87 = 495,                   /* $@87  */
  YYSYMBOL_496_88 = 496,                   /* $@88  */
  YYSYMBOL_497_89 = 497,                   /* $@89  */
  YYSYMBOL_498_90 = 498,                   /* $@90  */
  YYSYMBOL_499_91 = 499,                   /* $@91  */
  YYSYMBOL_make_tuple = 500,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 501,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 502,          /* make_tuple_call  */
  YYSYMBOL_503_92 = 503,                   /* $@92  */
  YYSYMBOL_504_93 = 504,                   /* $@93  */
  YYSYMBOL_make_dim = 505,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 506,            /* make_dim_decl  */
  YYSYMBOL_507_94 = 507,                   /* $@94  */
  YYSYMBOL_508_95 = 508,                   /* $@95  */
  YYSYMBOL_509_96 = 509,                   /* $@96  */
  YYSYMBOL_510_97 = 510,                   /* $@97  */
  YYSYMBOL_511_98 = 511,                   /* $@98  */
  YYSYMBOL_512_99 = 512,                   /* $@99  */
  YYSYMBOL_513_100 = 513,                  /* $@100  */
  YYSYMBOL_514_101 = 514,                  /* $@101  */
  YYSYMBOL_515_102 = 515,                  /* $@102  */
  YYSYMBOL_516_103 = 516,                  /* $@103  */
  YYSYMBOL_make_table = 517,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 518,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 519,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 520, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 521,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 522       /* array_comprehension  */
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
#define YYLAST   14986

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  301
/* YYNRULES -- Number of rules.  */
#define YYNRULES  918
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1760

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
       0,   571,   571,   572,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   593,   596,   599,
     605,   606,   607,   611,   612,   616,   634,   635,   636,   637,
     641,   642,   646,   647,   651,   652,   652,   656,   661,   670,
     685,   701,   706,   714,   714,   755,   785,   789,   790,   791,
     795,   798,   802,   808,   817,   820,   826,   827,   831,   835,
     836,   840,   843,   849,   855,   858,   864,   865,   869,   870,
     871,   884,   885,   889,   890,   890,   896,   897,   898,   899,
     900,   904,   914,   914,   922,   922,   926,   926,   935,   943,
     955,   965,   965,   972,   973,   974,   975,   976,   977,   981,
     986,   994,   995,   996,  1000,  1001,  1002,  1003,  1004,  1005,
    1006,  1007,  1013,  1016,  1022,  1025,  1028,  1034,  1035,  1036,
    1037,  1041,  1054,  1072,  1075,  1083,  1094,  1105,  1116,  1119,
    1126,  1130,  1137,  1138,  1142,  1143,  1144,  1148,  1151,  1158,
    1162,  1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,
    1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,  1183,  1184,  1185,  1186,  1187,  1188,  1189,  1190,  1191,
    1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,  1200,  1201,
    1202,  1203,  1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,
    1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,  1221,
    1222,  1223,  1224,  1225,  1226,  1227,  1228,  1229,  1230,  1231,
    1232,  1233,  1234,  1235,  1236,  1237,  1238,  1239,  1240,  1241,
    1242,  1243,  1244,  1245,  1250,  1268,  1269,  1270,  1274,  1280,
    1280,  1297,  1298,  1301,  1302,  1305,  1312,  1336,  1354,  1363,
    1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,
    1379,  1380,  1381,  1382,  1383,  1384,  1385,  1386,  1387,  1388,
    1389,  1390,  1394,  1399,  1405,  1411,  1422,  1423,  1427,  1428,
    1432,  1433,  1437,  1441,  1448,  1448,  1448,  1454,  1454,  1454,
    1463,  1497,  1505,  1512,  1519,  1525,  1526,  1537,  1541,  1544,
    1552,  1552,  1552,  1555,  1561,  1564,  1568,  1572,  1579,  1586,
    1592,  1596,  1600,  1603,  1606,  1614,  1617,  1620,  1628,  1631,
    1639,  1642,  1645,  1653,  1665,  1666,  1667,  1671,  1672,  1676,
    1677,  1681,  1686,  1694,  1705,  1711,  1726,  1738,  1741,  1747,
    1747,  1747,  1750,  1750,  1750,  1755,  1755,  1755,  1763,  1763,
    1763,  1769,  1783,  1799,  1817,  1827,  1838,  1853,  1856,  1862,
    1863,  1870,  1881,  1882,  1883,  1887,  1888,  1889,  1890,  1891,
    1895,  1900,  1908,  1909,  1919,  1923,  1933,  1940,  1947,  1947,
    1956,  1957,  1958,  1959,  1960,  1961,  1962,  1966,  1967,  1968,
    1969,  1970,  1971,  1972,  1973,  1974,  1975,  1976,  1977,  1978,
    1979,  1980,  1981,  1982,  1983,  1984,  1988,  1998,  2007,  2016,
    2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,  2029,  2030,
    2031,  2032,  2033,  2034,  2035,  2036,  2037,  2042,  2049,  2061,
    2066,  2076,  2080,  2087,  2090,  2090,  2090,  2095,  2095,  2095,
    2108,  2112,  2116,  2121,  2128,  2137,  2142,  2149,  2149,  2149,
    2156,  2160,  2170,  2179,  2188,  2192,  2195,  2201,  2202,  2203,
    2204,  2205,  2206,  2207,  2208,  2209,  2210,  2211,  2212,  2213,
    2214,  2215,  2216,  2217,  2218,  2219,  2220,  2221,  2222,  2223,
    2224,  2225,  2226,  2227,  2228,  2229,  2235,  2236,  2237,  2238,
    2239,  2240,  2241,  2242,  2243,  2244,  2245,  2246,  2252,  2253,
    2261,  2262,  2263,  2264,  2265,  2266,  2267,  2281,  2282,  2283,
    2284,  2285,  2288,  2291,  2292,  2292,  2292,  2295,  2300,  2304,
    2308,  2308,  2308,  2313,  2316,  2320,  2320,  2320,  2325,  2328,
    2329,  2330,  2331,  2332,  2333,  2334,  2335,  2336,  2338,  2342,
    2350,  2355,  2359,  2360,  2361,  2362,  2363,  2364,  2365,  2369,
    2373,  2377,  2381,  2385,  2389,  2393,  2397,  2401,  2408,  2409,
    2418,  2422,  2423,  2424,  2428,  2429,  2433,  2434,  2435,  2439,
    2440,  2444,  2455,  2456,  2457,  2460,  2463,  2466,  2466,  2485,
    2484,  2500,  2499,  2513,  2522,  2534,  2543,  2553,  2554,  2555,
    2556,  2557,  2561,  2564,  2573,  2574,  2578,  2581,  2584,  2600,
    2609,  2610,  2614,  2617,  2620,  2634,  2635,  2639,  2645,  2651,
    2660,  2663,  2670,  2673,  2679,  2680,  2681,  2685,  2686,  2690,
    2697,  2702,  2711,  2717,  2728,  2731,  2736,  2741,  2749,  2760,
    2763,  2763,  2783,  2784,  2788,  2789,  2790,  2794,  2801,  2801,
    2820,  2823,  2838,  2857,  2858,  2859,  2864,  2864,  2894,  2897,
    2904,  2914,  2914,  2918,  2919,  2920,  2924,  2934,  2954,  2977,
    2978,  2982,  2983,  2987,  2993,  2994,  2995,  2996,  3000,  3001,
    3002,  3006,  3009,  3020,  3025,  3020,  3045,  3052,  3057,  3066,
    3072,  3083,  3084,  3085,  3086,  3087,  3088,  3089,  3090,  3091,
    3092,  3093,  3094,  3095,  3096,  3097,  3098,  3099,  3100,  3101,
    3102,  3103,  3104,  3105,  3106,  3107,  3108,  3109,  3113,  3114,
    3115,  3116,  3117,  3118,  3119,  3120,  3124,  3135,  3139,  3146,
    3158,  3165,  3175,  3176,  3181,  3186,  3189,  3204,  3217,  3217,
    3217,  3230,  3231,  3235,  3239,  3246,  3250,  3257,  3258,  3259,
    3260,  3261,  3276,  3282,  3282,  3282,  3286,  3291,  3298,  3298,
    3305,  3309,  3313,  3318,  3323,  3328,  3333,  3337,  3341,  3346,
    3350,  3354,  3359,  3359,  3359,  3365,  3372,  3372,  3372,  3377,
    3377,  3377,  3383,  3383,  3383,  3388,  3392,  3392,  3392,  3397,
    3397,  3397,  3406,  3410,  3410,  3410,  3415,  3415,  3415,  3424,
    3428,  3428,  3428,  3433,  3433,  3433,  3442,  3442,  3442,  3448,
    3448,  3448,  3457,  3460,  3471,  3487,  3487,  3492,  3501,  3487,
    3530,  3530,  3535,  3545,  3530,  3574,  3574,  3574,  3617,  3618,
    3619,  3620,  3621,  3625,  3632,  3639,  3645,  3651,  3658,  3665,
    3671,  3680,  3683,  3689,  3697,  3702,  3709,  3714,  3721,  3726,
    3732,  3733,  3737,  3738,  3743,  3744,  3748,  3749,  3753,  3754,
    3758,  3759,  3760,  3764,  3765,  3766,  3770,  3771,  3775,  3808,
    3848,  3867,  3887,  3907,  3928,  3928,  3928,  3936,  3936,  3936,
    3943,  3943,  3943,  3954,  3954,  3954,  3965,  3969,  3975,  3991,
    3997,  4003,  4009,  4009,  4009,  4023,  4028,  4035,  4055,  4083,
    4107,  4107,  4107,  4117,  4117,  4117,  4131,  4131,  4131,  4145,
    4154,  4154,  4154,  4174,  4181,  4181,  4181,  4191,  4196,  4203,
    4206,  4212,  4232,  4251,  4259,  4279,  4304,  4305,  4309,  4310,
    4315,  4325,  4328,  4331,  4334,  4342,  4351,  4363,  4373
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
  "$@29", "expr_call", "expr_not_wrapped", "expr", "$@30", "$@31", "$@32",
  "$@33", "$@34", "$@35", "expr_mtag", "optional_field_annotation",
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

#define YYPACT_NINF (-1448)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-784)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1448,    94, -1448, -1448,    61,  -101,   360,   271, -1448,   -59,
     457,   457,   457, -1448, -1448,   124,   267, -1448, -1448, -1448,
     261, -1448, -1448, -1448,   266, -1448,    89, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448,    -7, -1448,    17,
      23,   114, -1448, -1448, -1448, -1448,   360, -1448,    22, -1448,
   -1448, -1448,   457,   457, -1448, -1448,    89, -1448, -1448, -1448,
   -1448, -1448,   149,   217, -1448, -1448, -1448, -1448,   267,   267,
     267,   148, -1448,   849,    78, -1448, -1448, -1448, -1448,   773,
     795,   799,   804, -1448,   843,    34,    61,   212,  -101,   157,
     247, -1448,   830,   830, -1448,   272,   261,    27,   261,   845,
     348,   353,   369, -1448,   398,   399, -1448, -1448,   -46,    61,
     267,   267,   267,   267, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448,   415, -1448, -1448, -1448, -1448, -1448, -1448, -1448,   271,
   -1448, -1448, -1448, -1448, -1448,   831,    97, -1448, -1448, -1448,
   -1448,   565, -1448, -1448, -1448,   405, -1448, -1448, -1448,   261,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,   459,
   -1448,    64, -1448,   428,   523,   849, 14813, -1448,   303,   606,
   -1448,   -45, -1448, -1448, -1448,   848, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448,   166, -1448,   496, -1448, -1448, -1448,   405,
     271,   271,   271, -1448, -1448, 13700, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448,   679,   693, -1448,   502,   271,   686, -1448, -1448,   576,
   -1448,   574,    61,    61,   -27,   -33, -1448, -1448, -1448,    97,
   -1448, 10480, -1448, -1448, -1448, -1448,   591,   603, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,   618,   602,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448,   809, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448,   697,   666, -1448, -1448,   132,
     690, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448,   271,   629,   698,   271, -1448,   -45,    92, -1448,    61,
   -1448,   682,   869,   671, -1448, -1448,   749,   751,   786,   709,
     793,   801, -1448, -1448, -1448,   747, -1448, -1448, -1448, -1448,
   -1448,   803, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448,   813, -1448, -1448, -1448,   817,   818, -1448,
   -1448, -1448, -1448,   824,   825,   819,   124, -1448, -1448, -1448,
   -1448, -1448,   120,   839,   655, -1448, -1448, -1448, -1448, -1448,
   -1448,   862,   901, -1448,   822, -1448,   128,   812,   731,   827,
   10480, -1448,  2735, -1448,   291, -1448,   124, -1448, -1448, -1448,
     -33,   826, -1448,  9754,   867,   870, 10480, -1448,     2, -1448,
   -1448, -1448,  9754, -1448, -1448,   875, -1448,   549,   573,   579,
   -1448, -1448,  9754,   165, -1448, -1448, -1448,    19, -1448, -1448,
   -1448,    41,  5836, -1448,   834, 10228, -1448,   885,   308, 10331,
     624, -1448, -1448,  9754, -1448, -1448,   113,    60,    60,    60,
   -1448,   839, -1448,   855,   856,  9754, -1448, -1448,   838, -1448,
   -1448,  1101,   -70,   858,    43,  3562, -1448, -1448,   271,   215,
    6042,   841,  9754,   887,   863,   864,   850, -1448,   261,   877,
     902,  6248,   256,   605,   879, -1448,   634,   881,   882,  3768,
    9754,  9754,   -78,   -78,   -78,   853,   861,   865,   866,   868,
     871, -1448,  2103, 10125,  6456, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448,  6662,   883, -1448,  1060, -1448,  9754,  9754,
    9754,  9754,  9754,  5424,  9754, -1448,   874, -1448, -1448,   261,
     261,  9754,  1074, -1448, -1448, -1448, -1448, -1448, -1448,  6870,
    1049, -1448, -1448, -1448,   695, -1448,   -85,   261, -1448,   261,
     261,   261, -1448,   261, -1448, -1448,  1023, -1448, -1448, -1448,
   -1448,   884, -1448, -1448,   263, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448,  2251, -1448,   873, -1448, -1448, -1448, -1448, -1448,
   -1448,   629,  9754, -1448, -1448,    39,   405, -1448,   878,   894,
     898, -1448, 10823, -1448,  1054,   991, -1448, -1448, -1448,  4180,
   10480, 10480, 10480, 10934, 10480, 10480,   886,   927, 10480,   502,
   10480,   502, 10480,   502, 10583,   930, 10972, -1448,  9754, -1448,
   -1448, -1448, -1448,   895, -1448, -1448, 13183,  9754, -1448,   120,
   -1448,   919, -1448, -1448,   168, -1448, -1448,   499, -1448,   839,
     574,   918,   499, -1448,   574, 11083,   897,  1077, -1448, -1448,
     -35, -1448, -1448, -1448, -1448, -1448, 13742,   113,   900, -1448,
     905, -1448, -1448,   124,   637, -1448,   925,   926,   931, -1448,
    9754,  4180, -1448,   934,   998, 10788,  1117, 10480,  9754,  9754,
   14226,  9754, 13742,   942, -1448, -1448,  9754, -1448, -1448,   941,
     970, 14226,  9754, -1448, -1448,  9754, -1448, -1448,  9754, -1448,
   10480,  4180, -1448, 10788,   681,   681,   917, -1448,   884, -1448,
   -1448, -1448,  9754,  9754,  9754,  9754,  9754,  9754,   113,  2941,
     113,  3148,   113, 13966, -1448,   664, -1448, 13742, -1448,   810,
     921,   681,   681,   -60,   681,   681,   180,  1127,   928,   951,
   14226,   951,   284, -1448, -1448, 13742, -1448,   113,   574, -1448,
     952,   271, -1448, -1448, -1448,  4386, -1448, -1448, -1448, -1448,
   -1448, -1448,   -56,    72,   -78, -1448, 14598, 14653,  4592,  4592,
    4592,  4592,  4592,  4592,  4592,  4592,  9754,  9754, -1448, -1448,
    9754,  4592,  4592,  9754,  9754,  9754,   972,  4592,  9754,   156,
    9754,  9754,  9754,  9754,  9754,  9754,  4592,  4592,  9754,  9754,
    9754,  4592,  4592,  4592,  9754,  4592,  7076,  9754,  9754,  9754,
    9754,  9754,  9754,  9754,  9754,  9754,  9754, 14752,  9754, -1448,
    7282, -1448, 13835,   267, -1448,  1140, -1448,   -45, -1448, 10480,
   -1448,   978, -1448,  4180, -1448, 10667,  -112,   582,   953,   402,
   -1448,   593,   658, -1448, -1448,   -63,   684,   690,   692,   690,
     771,   690, -1448,   345, -1448,   392, -1448, 10480,   935, -1448,
   -1448, 13294, 10480, -1448, -1448, 10480, -1448, -1448, -1448,  9754,
     981, -1448,   982, -1448, 10480, -1448,  9754, 10480, 10480, -1448,
      35,   113, 10480,  5630,  7488,   984,  9754, 10480, -1448, -1448,
   -1448, 10480,   951, -1448,   934,  9754,  9754,  9754,  9754,  9754,
    9754,  9754,  9754,  9754,  9754,  9754,  9754,  9754,  9754,  9754,
    9754,  9754,  9754,   271,   872,   945, 14226, 11118, -1448, -1448,
   10480, 10480, 11229, 10480, -1448, -1448, 11267, 10480,   951, 10480,
   10583,   951,   930,   518, -1448, 10788, -1448,    72, 11378, 11413,
   11524, 11562, 11673, 11708,    36,   -78,   943,     0,  3355,  4800,
    7694, 14059,   968,    24,   242,   973,   264,    37,  7900,    24,
     654,    38,  9754,   987, -1448,  9754, -1448, 10480, -1448, 10480,
   -1448,  9754,   752,   113,   113,    40,   185, -1448,  9754, -1448,
     948,   955,   956,   607, -1448, -1448,   405,  9754,    44, -1448,
    9754, -1448, -1448,   884,   -42,  5008, -1448,   190,   968,   957,
    1005,  1005, -1448, -1448,   962,   151,   502, -1448,   983,   963,
   -1448, -1448,   988,   965, -1448, -1448,   -78,   -78,   -78, -1448,
   -1448,  2476, -1448,  2476, -1448,  2476, -1448,  2476, -1448,  2476,
   -1448,  2476, -1448,  2476, -1448,  2476,  1434,  1434,  1484, -1448,
    2476, -1448,  2476,  1484, 14505, 14505,   976, -1448,  2476,   540,
     977, -1448, 13332,   210,   210,   873, 14226,  1434,  1434, -1448,
    2476, -1448,  2476,  1250,  1804, 10624, -1448,  2476, -1448,  2476,
   -1448,  2476, 14319, -1448,  2476, 14697, 14096, 14356, 14412, 14449,
    1484,  1484,   197,   197,   540,   540,   540,   408,  9754,   979,
     980,   446,  9754,  1184,   985, 13443, -1448,   205, -1448,   288,
     860,  1120,   261,   553, -1448, -1448, 10667, -1448, -1448, -1448,
   -1448, 10480, -1448, -1448, -1448,  1016, -1448,   996, -1448,   999,
   -1448,  1000, -1448, 10583, -1448,   930,   424,   839, -1448, -1448,
     839,   839, 11819, -1448,  1147,     6, 14226,  1198,  1286,  9754,
      51,   779,   477,   253,   989,   990,  1032, 11857,   338, 11968,
     780, 10480, 10583,   930,  1454,   992, 14226, 14226, 14226, 14226,
   14226, 14226, 14226, 14226, 14226, 14226, 14226, 14226, 14226, 14226,
   14226, 14226, 14226, 14226, -1448,   994, 10480, -1448, -1448,  9754,
    1857,  2217, -1448,  2295, -1448,  2430,  1003,  2510,   440,  1004,
     488,    72,   502, -1448, -1448, -1448, -1448, -1448,  1006,  9754,
   -1448,  9754,  9754,  9754,  5216, 13183,     9,  9754,   547,   477,
     242, -1448, -1448,   993, -1448,  9754, -1448,  1008,  9754, -1448,
    9754,   477,   687,  1009, -1448, -1448,  9754, 14226, -1448, -1448,
     498,   500, 14189,    52,    55,  9754,   113,    58, -1448,  9754,
    9754, 10480,   502,   802,  1606,  9754, -1448,  2735,    72,   162,
   -1448,  1011,   316,  9960, -1448, -1448, -1448,   349,   290,   151,
    1057,  1061,  1018,  1063,  1064, -1448,   505,   690, -1448,  9754,
   -1448,  9754, -1448, -1448, -1448,  8106,  9754, -1448,  1035,  1026,
   -1448, -1448,  9754,  1028, -1448, 13478,  9754,  8312,  1034, -1448,
   13589, -1448,  8518, -1448, -1448,   261, -1448, -1448,   728, -1448,
      77,   405,    72, -1448, -1448, -1448, -1448,   839, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448,  1036, 10480, -1448,  1065,  9754, -1448, -1448,    67,
    9754, -1448,  1031, -1448, -1448, -1448,   560, -1448,  1037,  1079,
   -1448, -1448,  4589,   571,   572, -1448, -1448,  9754,  4797, 13928,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,   623,
     690,  8724,   753, 12003, 14226, 14226,    24,   242, 14226,  1038,
     194,   968, -1448, -1448, 14226, -1448,   973,   757,    24,  1044,
   -1448, -1448, -1448, -1448,   765, -1448, -1448, -1448,  1085,  9754,
    9754,   766,    62,  9754, 12114, 12152,  5005,   690,   271, -1448,
   -1448,  9754, -1448, -1448,   261,   777, -1448,   502, -1448,  1051,
    5008,  1097,  1059,   672,   307, -1448, -1448,  1100, -1448, -1448,
     151,  1066,   226, 10480, 12263, 10480, 12298, -1448,   377, 12409,
   -1448,  9754,  2096,  9754, -1448, 12447,  5008, -1448,   420,  9754,
   -1448, -1448, -1448,   436,   405,  1243,    77, -1448, -1448,   860,
   -1448,  1069, -1448, -1448, -1448,  9754,   839, -1448, 14226,  1070,
    1071, -1448,   438, -1448, -1448,  9754,  1104,  9754,  1081, -1448,
   -1448, -1448, -1448,  1072,  1067,  1073, -1448,  9754,  9754,  9754,
    1075,  1211,  1078,  1080,  8930, -1448,   226, -1448,   442,  9754,
     224,   242, -1448,  9754,  9754,  9754,  9754,   687, -1448,  9754,
    9754,  1083, -1448, -1448,   454,   456,  9754,  9754,   778, -1448,
   -1448, -1448,  1084, -1448, 13742,  3974, -1448,  9754,   690, -1448,
     513, -1448,   707, 10480,     2, -1448,  1086, -1448, -1448,  9136,
   -1448, -1448,  5213, -1448,   782, -1448, -1448, -1448, 10480, 12558,
   12593, -1448,   519, -1448, 12704, -1448, -1448, -1448, -1448,  1243,
     113,  1087,  1211,  1211,   271, 12742,  1092, 12853,  1089,  1090,
    1091,  1093,  9754, -1448,  9754,  1484,  1484,  1484,  9754, -1448,
   -1448,  1211,  1211, -1448, 12888, -1448, -1448, 13928,  9754,  9754,
   -1448, 12999, 14226, 14226, 13928, -1448,   261,  1484,  9754, -1448,
    1119,  1113,  1116, 13928,   495,  9754,   342, -1448,   802,  9342,
    9548, -1448, -1448, -1448, -1448, -1448, 14226,   261,   271,  1094,
   10480,     2,  1207,  9754, -1448,  9754, 14319, -1448, -1448,   787,
   -1448, -1448,  1096, -1448, 14813, -1448, -1448, -1448, -1448, -1448,
     176,   176, -1448, -1448,  9754, -1448,  9754,  1211,  1211,   477,
    1099,  1102,   951,   176,   477, -1448,  1249,  1095, 14226, 14226,
     229,  1125,   574,  1112, -1448,  9754,  9754,  1107,  1130, 13928,
   -1448,   342, -1448,  9754,  9754, 14226,   574, -1448, -1448,  1207,
    9754,  9754, 13928,  2096, -1448, -1448, -1448, -1448,   261, 14813,
     477,   968,  1133, -1448,  1109,  1110, 13037, 13148,   176,   176,
     968,  1111, -1448, -1448,  1114,  1118,  1123,  9754,  1121,  9754,
    9754,  1122,   574, -1448,  1124,   261, 13928, -1448,  9754,  1129,
   -1448, 14226, -1448,  9754, 13928, 13928, -1448, -1448,   405,   271,
     509,  1126, -1448, -1448, -1448, -1448, -1448,  1131,  1132, -1448,
   -1448, -1448, -1448, 14226, -1448, 14226, 14226, -1448, -1448, -1448,
    1134,  1135, 13928, -1448, 13928, -1448, -1448, -1448, -1448, -1448,
     477, -1448, -1448, -1448, -1448,  1136, -1448,   516, -1448, -1448
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   132,     1,   317,     0,     0,     0,   641,   318,     0,
     633,   633,   633,    17,    18,     0,     0,    16,    15,     3,
       0,    10,     9,     8,     0,     7,   622,     6,    11,     5,
       4,    13,    12,    14,   102,   103,   101,   110,   112,    45,
      61,    58,    59,    47,    48,    49,     0,    50,    56,    46,
     232,   231,   633,   633,    24,    23,   622,   635,   634,   805,
     795,   800,     0,   285,    43,   118,   119,   120,     0,     0,
       0,   121,   123,   130,     0,   117,    19,   655,   654,   225,
     643,     0,   658,   623,   624,     0,     0,     0,     0,    51,
       0,    57,     0,     0,    54,     0,     0,   633,     0,    20,
       0,     0,     0,   287,     0,     0,   129,   124,     0,     0,
       0,     0,     0,     0,   133,   227,   226,   229,   224,   645,
     644,     0,   657,   656,   660,   659,   663,   626,   625,   628,
     108,   109,   106,   107,   105,     0,     0,   104,   113,    62,
      60,    56,    53,    52,   636,   562,   234,   233,   640,     0,
     642,    21,    22,    25,   806,   796,   801,   286,    41,    44,
     128,     0,   125,   126,   127,   131,     0,   646,     0,   651,
     619,   548,    26,    27,    31,     0,    97,    98,    95,    96,
      94,    93,    99,     0,    55,     0,   563,   564,   638,   562,
       0,     0,     0,    42,   122,     0,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,     0,     0,   139,   134,     0,     0,   630,   652,     0,
     664,   620,     0,     0,   550,     0,    28,    29,    30,     0,
     111,     0,   639,   714,   797,   802,   193,   194,   191,   142,
     143,   145,   144,   146,   147,   148,   149,   175,   176,   173,
     174,   166,   177,   178,   167,   164,   165,   192,   186,     0,
     190,   179,   180,   181,   182,   153,   154,   155,   150,   151,
     152,   163,     0,   169,   170,   168,   161,   162,   157,   156,
     158,   159,   160,   141,   140,   185,     0,   171,   172,   548,
     137,   262,   230,   698,   701,   704,   705,   699,   702,   700,
     703,     0,     0,   649,   661,   627,   548,     0,   114,     0,
     116,     0,   609,   607,   629,   100,     0,     0,     0,     0,
       0,     0,   671,   691,   672,   707,   673,   677,   678,   679,
     680,   697,   684,   685,   686,   687,   688,   689,   690,   692,
     693,   694,   695,   765,   676,   683,   696,   772,   779,   674,
     681,   675,   682,     0,     0,     0,     0,   706,   727,   730,
     728,   729,   792,   637,   712,   586,   592,   195,   196,   189,
     184,   197,   187,   183,     0,   135,   316,   577,   578,     0,
       0,   228,     0,   630,   562,   647,     0,   653,   565,   665,
       0,     0,   115,     0,     0,     0,     0,   608,     0,   733,
     756,   759,     0,   762,   752,     0,   718,   766,   773,   780,
     786,   789,     0,     0,   742,   747,   741,     0,   755,   751,
     744,     0,     0,   746,   731,     0,   715,   712,     0,   798,
     803,   198,   188,     0,   314,   315,     0,   548,   548,   548,
     136,   138,   264,     0,     0,     0,    71,    72,    84,   494,
     495,     0,     0,     0,     0,   302,   488,   300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,     0,
       0,     0,     0,     0,     0,   697,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   537,     0,     0,     0,   240,   370,   372,   371,   373,
     374,   375,   376,     0,     0,    37,   231,   241,     0,     0,
       0,     0,     0,   266,     0,   352,   353,   492,   491,     0,
       0,     0,     0,   257,   252,   249,   248,   250,   251,   270,
     235,   284,   263,   243,   531,   242,   447,     0,   522,    79,
      80,    77,   255,    78,   256,   258,   320,   247,   521,   520,
     519,   132,   525,   490,     0,   244,   524,   523,   497,   448,
     498,   489,   377,   449,     0,   493,   808,   812,   809,   810,
     811,     0,     0,   631,   650,   567,   562,   549,     0,     0,
       0,   531,     0,   611,   612,     0,   605,   606,   604,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
       0,   134,     0,   134,     0,     0,     0,   738,   266,   749,
     750,   743,   745,     0,   748,   732,     0,     0,   794,   793,
     716,   717,   713,   807,   285,   721,   722,     0,   587,   582,
       0,     0,     0,   593,     0,     0,     0,   666,   574,   575,
     597,   579,   580,   581,   854,   857,     0,     0,     0,   290,
     294,   293,   299,     0,     0,   338,     0,     0,     0,   890,
       0,     0,   306,   303,     0,   347,     0,     0,   270,     0,
     288,     0,     0,     0,   329,   332,     0,   261,   335,     0,
       0,    65,     0,    88,   894,     0,   863,   872,     0,   860,
       0,     0,   311,   308,   476,   477,   353,   365,   132,   283,
     281,   282,     0,     0,     0,     0,     0,     0,     0,   832,
       0,     0,     0,   870,   897,     0,   274,     0,   277,     0,
       0,   453,   452,   486,   451,   450,     0,     0,     0,   908,
     347,   908,   354,   259,   260,     0,    82,     0,     0,   899,
     908,     0,   368,   238,   529,     0,   246,   253,   254,   305,
     310,   319,     0,   362,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,     0,   237,
       0,   648,   562,     0,   566,     0,   662,   548,   621,     0,
     610,     0,   614,     0,   618,   377,     0,     0,     0,   723,
     736,     0,     0,   708,   710,     0,     0,   137,     0,   137,
       0,   137,   584,     0,   590,     0,   709,     0,     0,   740,
     725,     0,     0,   588,   799,     0,   594,   804,   576,     0,
       0,   596,     0,   595,     0,   598,     0,     0,     0,    89,
       0,     0,     0,   846,     0,     0,     0,     0,   880,   883,
     886,     0,   908,   307,   304,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,     0,    90,    91,
       0,     0,     0,     0,    63,    64,     0,     0,   908,     0,
       0,   908,     0,     0,   312,   309,   354,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   285,     0,     0,
       0,   866,   824,   832,     0,   875,     0,     0,     0,   832,
       0,     0,     0,     0,   835,     0,   902,     0,   265,     0,
      40,     0,    38,     0,     0,     0,     0,   877,   909,   267,
       0,     0,     0,   424,   421,   423,   562,     0,     0,   901,
     909,   271,   262,   132,     0,   266,   440,     0,   823,     0,
       0,     0,   328,   327,     0,     0,   134,   280,     0,     0,
     508,   507,     0,     0,   509,   513,     0,     0,     0,   399,
     408,   387,   409,   388,   411,   390,   410,   389,   412,   391,
     402,   381,   403,   382,   404,   383,   454,   455,   467,   413,
     392,   414,   393,   468,   465,   466,     0,   401,   379,   503,
       0,   484,     0,   526,   527,   528,   380,   456,   457,   415,
     394,   416,   395,   472,   473,   474,   405,   384,   406,   385,
     407,   386,   475,   400,   378,     0,     0,   470,   471,   469,
     463,   464,   459,   458,   460,   461,   462,     0,     0,     0,
     430,     0,     0,     0,     0,     0,   445,     0,   632,     0,
     556,   559,     0,     0,   613,   616,   377,   617,   734,   757,
     760,     0,   763,   753,   719,     0,   767,     0,   774,     0,
     781,     0,   787,     0,   790,     0,     0,   272,   737,   726,
     583,   589,     0,   668,   669,   600,   599,     0,     0,     0,
       0,     0,   847,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,   387,   388,   390,
     389,   391,   381,   382,   383,   392,   393,   379,   394,   395,
     384,   385,   386,   378,   313,     0,     0,   903,   499,     0,
       0,     0,   500,     0,   530,     0,     0,     0,     0,     0,
       0,   362,   134,   532,   533,   534,   535,   536,     0,     0,
     833,     0,     0,     0,     0,   347,   832,     0,     0,     0,
       0,   841,   842,     0,   849,     0,   839,     0,     0,   878,
       0,     0,     0,     0,   837,   879,     0,   869,   834,   898,
       0,     0,    34,     0,     0,     0,     0,     0,   496,     0,
       0,     0,   134,    68,    73,     0,   900,     0,   362,     0,
     441,     0,     0,     0,   444,   442,   321,     0,     0,     0,
       0,     0,     0,     0,     0,   360,     0,   137,   504,     0,
     510,     0,   398,   396,   397,     0,     0,   482,     0,     0,
     514,   518,     0,     0,   485,     0,     0,     0,     0,   431,
       0,   438,     0,   480,   446,   133,   557,   558,   559,   560,
     551,   562,   362,   615,   735,   758,   761,   724,   764,   754,
     720,   711,   768,   770,   775,   777,   782,   784,   788,   585,
     791,   591,     0,     0,   667,     0,     0,   855,   858,     0,
       0,   291,     0,   296,   297,   295,     0,   341,     0,     0,
     344,   339,     0,     0,     0,   891,   889,   270,     0,     0,
     330,   333,   336,   895,   893,   864,   873,   871,   861,     0,
     137,     0,     0,     0,   814,   813,   832,     0,   867,     0,
       0,   825,   848,   840,   868,   838,   876,     0,   832,     0,
     844,   845,   852,   836,     0,   275,   278,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   137,     0,    66,
      67,     0,    81,    74,     0,     0,   236,   134,   443,     0,
     266,     0,     0,   607,     0,   357,   358,     0,   356,   355,
       0,     0,     0,     0,     0,     0,     0,   419,     0,     0,
     515,     0,   487,     0,   483,     0,   266,   432,     0,     0,
     481,   439,   435,     0,   562,   554,   551,   552,   553,   556,
     568,     0,   771,   778,   785,   266,   273,   670,   601,     0,
       0,    85,     0,   292,   298,     0,     0,     0,     0,   340,
     881,   884,   887,     0,     0,     0,    92,     0,     0,     0,
       0,   846,     0,     0,     0,   239,     0,   538,     0,     0,
       0,     0,   850,     0,     0,     0,     0,     0,   843,     0,
       0,   268,    32,    39,     0,     0,     0,     0,     0,   422,
     547,   425,     0,    69,     0,     0,    83,     0,   137,   417,
       0,   322,   607,     0,     0,   364,     0,   361,   363,     0,
     349,   367,     0,   546,     0,   544,   420,   541,     0,     0,
       0,   540,     0,   433,     0,   436,   573,   555,   569,   554,
       0,     0,   846,   846,     0,     0,     0,     0,     0,     0,
       0,     0,   266,   904,   270,   331,   334,   337,     0,   847,
     865,   846,   846,   501,     0,   366,   539,   906,     0,     0,
     851,     0,   816,   815,   906,   853,   906,   276,   266,   279,
      36,     0,     0,   906,     0,     0,     0,   428,    68,   302,
       0,    75,    79,    80,    77,    78,    76,   906,     0,     0,
       0,     0,     0,     0,   359,     0,   350,   505,   511,     0,
     545,   543,     0,   542,     0,   571,   603,   602,   561,   739,
     831,   831,    87,   342,     0,   345,     0,   846,   846,   821,
       0,     0,   908,   831,   821,   502,     0,     0,   818,   817,
       0,     0,     0,   908,    33,     0,     0,     0,     0,   906,
     426,     0,    70,     0,     0,   308,     0,   369,   418,     0,
       0,     0,     0,   351,   506,   512,   516,   434,     0,     0,
       0,   828,   908,   830,     0,     0,     0,     0,   831,   831,
     822,     0,   892,   905,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   909,     0,   906,   906,   910,     0,     0,
     429,   309,   917,     0,     0,     0,   326,   517,   562,     0,
       0,   909,   829,   856,   859,   343,   346,     0,     0,   888,
     896,   874,   862,   907,   915,   820,   819,   916,   918,   269,
       0,     0,   906,   914,     0,   325,   324,   570,   572,   826,
       0,   882,   885,   913,   911,     0,   323,     0,   912,   827
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1448, -1448,    -1, -1448, -1448, -1448, -1448,   601,  1268, -1448,
   -1448, -1448, -1448, -1448, -1448,  1353, -1448, -1448, -1448,   880,
    1309, -1448,  1215, -1448, -1448,  1269, -1448, -1448, -1448,  -240,
   -1448, -1448, -1448,  -156, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448,  1143, -1448, -1448,   -43,   -55, -1448,
   -1448, -1448,   443,   537,  -524,  -591,  -817, -1448, -1448, -1448,
   -1402, -1448, -1448,    -4,  -226,  -221,  -359, -1448,   370, -1448,
    -608, -1448,  -667,  -326,   445, -1448, -1448, -1448, -1448,  -426,
      -2, -1448, -1448, -1448, -1448, -1448,  -154,  -152,  -151, -1448,
    -143, -1448, -1448, -1448,  1382, -1448,   383, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
    -228,   -96,  1052,   -28,   126,  -910,  -458, -1448,  -536, -1448,
   -1448,  -383,  1240, -1448, -1448, -1448, -1447, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448,  1068, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448,  -145,   -53,  -153,   -48,   104,
   -1448,  -188, -1448, -1448, -1448, -1448, -1448, -1448,   581,  -414,
    -888, -1448,  -417,  -887, -1448,  -644,  -144,  -142, -1448,  -593,
   -1354, -1448,  -371, -1448, -1448,  1349, -1448, -1448, -1448,  1019,
    1021,   293, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,  -629,    16,
   -1448,   950, -1448, -1448,   995, -1448, -1448, -1448, -1448,  -270,
   -1448, -1448,  -382, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
    -225, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448,   954,  -704,  -235,  -842,  -690, -1448,
   -1448, -1226,  -913, -1448, -1448, -1448, -1172,   -81, -1268, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448,   199,  -497,
   -1448, -1448, -1448,   699, -1448, -1448, -1448, -1448, -1448, -1448,
   -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1448, -1011,  -729,
   -1448
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   636,    18,   153,    56,    19,   175,   181,  1590,
    1388,  1502,   729,   527,   159,   528,   105,    21,    22,    47,
      48,    49,    94,    23,    41,    42,   529,   530,  1401,  1402,
     531,  1404,  1515,   532,   533,   987,   534,   657,   658,   535,
     536,   537,   538,  1179,   182,   183,    37,    38,    39,   234,
      71,    72,    73,    74,    24,   300,   391,   224,    25,   117,
     225,   118,   166,   539,   148,   707,  1019,   542,   392,   543,
     738,  1589,   748,  1126,   591,   967,  1500,   969,  1501,   545,
     546,   547,   660,   882,  1463,   548,   549,   550,   551,   552,
     553,   554,   555,   446,   556,   762,  1257,  1002,   557,   558,
     920,  1477,   921,  1478,   923,  1479,   559,   887,  1469,   560,
     739,  1531,   561,  1265,  1266,  1006,   709,   562,   819,   993,
     563,   674,  1020,   565,   566,   567,   985,   568,  1241,  1596,
    1242,  1661,   569,  1093,  1441,   570,   571,   740,  1423,  1674,
    1425,  1675,  1538,  1717,   573,   386,  1449,  1548,  1298,  1300,
    1102,   188,   585,   827,  1624,  1679,   387,   388,   389,   852,
     853,   439,   854,   855,   440,  1203,   648,   649,  1628,   599,
     408,   323,   324,   231,   316,    84,   129,    27,   171,   312,
      95,    96,   185,    97,    28,    53,   121,   168,    29,   397,
     229,   230,    82,   126,   399,    30,   169,   314,   650,   574,
     311,   369,   370,   845,   438,   374,   371,   607,  1310,  1123,
     838,   434,   372,   600,  1304,   857,   605,  1309,   601,  1305,
     602,  1306,   604,  1308,   608,  1312,   609,  1452,   610,  1314,
     611,  1453,   612,  1316,   613,  1454,   614,  1318,   615,  1320,
     639,    31,   101,   191,   375,   640,    32,   102,   192,   376,
     644,    33,   100,   190,   575,  1681,  1691,   999,   953,  1682,
    1683,  1684,   954,   966,  1225,  1219,  1214,  1382,  1145,   576,
     877,  1459,   878,  1460,   932,  1483,   929,  1481,   955,   749,
     577,   930,  1482,   956,   578,  1151,  1559,  1152,  1560,  1153,
    1561,   891,  1473,   927,  1480,   725,   750,   579,  1647,   979,
     580
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   242,   833,    52,   302,   315,   876,   724,   753,   564,
     858,   915,   980,    64,    75,   952,   373,   952,   847,    76,
     849,   991,   851,   642,   595,   637,   235,  1192,   880,   586,
    1117,   959,  1119,   541,  1121,   710,   711,   763,  1372,   672,
    1210,  1144,  1188,   138,    90,  1190,  1222,  -132,   666,   130,
     131,   998,   619,   629,   161,  1139,  1199,  1220,  1226,  1524,
    1235,   825,   945,   702,  1245,    40,    75,    75,    75,   789,
     790,  1330,  1389,    62,   621,  1390,  1004,   945,  1393,    91,
      34,    35,  1507,   435,    57,    50,   395,  1108,   754,   944,
      58,   957,   871,   961,     2,   145,   321,   150,   110,   111,
     112,     3,    13,    63,  1000,    51,   541,   975,    75,    75,
      75,    75,   176,   177,    54,   232,   946,   322,   988,   321,
     719,   721,    14,  1447,     4,   170,     5,   871,     6,   596,
     541,  1249,   755,   871,     7,   444,  1114,  1202,    13,   597,
     322,   525,   706,   104,     8,   667,   668,   663,   189,  1660,
       9,    83,  1251,   424,   817,   818,   872,   873,    14,   874,
      55,  1001,   875,  1155,   227,   451,   540,   946,  1611,   638,
     643,   400,   160,   834,    10,   233,  1250,   317,   445,   998,
     425,   426,  1249,  1448,   937,    85,   243,   244,   245,    13,
     318,   146,   873,   319,   598,  1492,    11,    12,   873,  1186,
     132,   435,  1189,   146,    13,   133,   583,   134,    86,    14,
     135,   147,   633,  1570,  1710,   384,   620,    87,   766,   767,
     232,   301,  1678,   147,    14,    92,   872,   872,   872,   872,
      50,   872,  1005,   320,    36,   872,    93,   673,   622,   367,
     541,   669,   872,   872,   427,   893,   872,   676,   428,   872,
      51,   136,  1140,   872,   823,    86,   623,   368,   895,    13,
     670,   693,   624,   178,  1343,   646,  1344,    62,   179,   113,
     180,    77,    78,   135,    79,   934,   402,  1719,  1260,    14,
     233,  1359,   194,    86,  1630,  1631,   647,   384,  1261,   871,
     998,  1252,   232,  1367,   114,   914,   741,    63,    15,  1202,
    1332,    65,    80,  1643,  1644,    88,  1007,   393,   401,    16,
     398,   104,   541,   429,  1050,   778,   779,   430,   933,  1580,
     431,   871,   103,   786,  1262,   788,   789,   790,   791,  1051,
      66,  1494,   646,   792,   946,   432,   786,   646,  1407,   789,
     790,   433,   541,  1263,  1233,  1234,    98,  1237,  1264,  1249,
     385,   871,   233,   647,   873,   821,   871,   239,   647,   826,
     104,  1578,   862,   617,   423,   109,  1699,   863,  1529,  1688,
    1689,    93,   866,    81,    67,   836,   837,   839,   139,   841,
     842,  1253,   618,   846,   240,   848,   873,   850,   367,    50,
     149,  1003,  1451,  1680,   584,   764,   895,   974,   828,   814,
     815,   816,  1236,    68,   367,  1685,   368,  1105,  1254,    51,
      62,   817,   818,   677,   864,  1267,   873,  1695,   867,    50,
     141,   873,   368,  1294,   817,   818,    13,    62,    13,    13,
    1211,  1212,   678,   367,    50,   879,   981,   367,   998,    51,
      63,   982,   892,   894,   895,   144,    14,  1103,    14,    14,
    1422,   368,  1216,  1491,    51,   368,   186,    63,  1213,   367,
     664,   918,  1727,  1728,   301,  1497,    50,   928,  1229,  1248,
     931,  1333,   146,   692,   541,    69,   187,   687,   301,   113,
    1217,  1411,   983,   582,    70,    62,    51,  1200,   301,   301,
     301,  1326,   147,  1246,   981,  1137,  1138,   226,  1420,   632,
     367,   367,   952,    13,  1295,  1371,   968,  1208,  1413,  1154,
      13,   106,   107,   108,    57,    63,  1366,   952,   368,   368,
      58,   154,   989,    14,   986,  1525,   155,   997,   743,   744,
      14,  1378,  1409,    43,    44,    45,   635,  1338,  1180,  1181,
    1411,  1183,   156,  1486,  1122,  1185,   756,  1187,   757,   758,
     759,   424,   760,   162,   163,   164,   165,    13,  1272,  1273,
    1274,   766,   767,   765,    46,  1412,  1283,    13,   895,  1111,
     186,   157,   110,  1651,   112,  1652,   158,    14,   425,   426,
    1512,  1284,  1657,   635,   824,  1125,   424,    14,   167,    13,
     187,  1124,  1097,   635,   832,  1536,  1666,   435,   367,   367,
     367,  1360,   367,   367,  1288,    13,   367,  1392,   367,    14,
     367,   895,   367,   425,   426,   635,   368,   368,   368,  1289,
     368,   368,    91,  1322,   368,    14,   368,   895,   368,   895,
     368,   635,  1127,   895,  1098,   946,   193,  1130,  1543,  1356,
    1131,  1230,   427,  1231,  1176,   895,   428,   895,  1709,  1135,
    1249,  1397,   301,    13,  1545,  1143,  1554,  1141,   778,   779,
    1576,   884,  1150,    13,    13,    13,   786,   110,   788,   789,
     790,   791,  1591,    14,  1592,   367,   792,   427,   301,   635,
    1474,   428,  1101,    14,    14,    14,   895,  1358,   241,   635,
     635,   635,  1174,   368,  1740,  1741,  1420,  1385,   367,  1386,
    1208,  1608,   766,   767,  1208,  1369,  1520,  1208,  1321,  1319,
    1208,   429,   228,  1658,   297,   430,   368,  1191,   431,   299,
    1370,  1421,   741,   301,   965,    13,  1495,  1749,   298,  1609,
     741,  1755,  1542,   432,  1759,  1622,    13,    13,   146,   433,
     984,   301,  1127,  -769,  1127,    14,   429,   992,  -769,   313,
     430,   635,  1302,   431,   817,   818,    14,    14,   147,  1465,
     301,   303,   635,   635,   377,   304,  -769,  -776,   432,  1342,
    1471,  1472,  -776,  -783,   433,  1445,   378,   435,  -783,   305,
     306,  1109,  1011,  1015,   307,   308,   309,   310,   435,    13,
    -776,   379,  1112,   146,  1348,  1299,  -783,   641,  1243,   778,
     779,  -427,   394,   694,   122,   123,  -427,   786,  1055,    14,
     789,   790,   791,   147,  1398,   635,  1518,   792,   380,    13,
     436,    75,   695,  1485,  -427,  1399,  1400,   367,   437,    13,
     115,   381,   697,  1094,  1107,   885,   116,   544,  1223,    14,
    1484,  1224,   525,   706,  1115,   368,  1579,  1551,   963,    14,
     964,   698,   119,   435,   886,   367,  1323,  1113,   120,  1396,
     367,   124,   405,   367,   564,   406,  1523,   125,   407,   407,
     382,  1379,   367,   368,  1380,   367,   367,  1381,   368,   435,
     367,   368,   383,  1116,   390,   367,  1307,   435,   541,   367,
     368,  1118,   396,   368,   368,   817,   818,  1641,   368,   403,
     127,  1610,   151,   368,   407,   424,   128,   368,   152,   301,
     544,  1329,   404,  1694,   525,   706,   448,  1296,   367,   367,
    1125,   367,   449,  1297,  1704,   367,   412,   367,   367,   236,
     237,  1613,   425,   426,   544,  1690,   368,   368,  1489,   368,
    1690,   301,  1496,   368,   895,   368,   368,   409,   895,   410,
    1499,  1506,  1209,  1722,  1640,  1218,   895,   895,  1209,  1218,
    1323,  1323,  1517,  1595,   415,   367,   435,   367,   895,   895,
    1120,  1362,   142,   143,   435,   435,  1720,   435,  1331,  1341,
    1653,  1618,   435,   368,   411,   368,  1676,   172,   173,   970,
     971,   413,  1377,   110,   111,   112,   427,   447,  1384,   414,
     428,   416,  1586,    43,    44,    45,  1700,  1391,   172,   173,
     174,   417,   301,   301,   301,   418,   419,  1405,  1670,  1671,
    1607,  1406,   420,   421,   424,   236,   237,   238,   651,   652,
     653,    59,    60,    61,   435,   441,   422,    13,   442,   443,
     593,  1532,   587,   594,   544,   450,  1757,  1428,   606,   627,
     630,   425,   426,   654,   655,   -86,   665,    14,   681,  1438,
     683,   684,   685,   635,  1443,   429,  1466,   686,   689,   430,
     712,  1175,   431,   730,  1125,   688,  1713,   696,   713,   699,
     700,   728,   714,   715,   746,   716,   751,   432,   717,   761,
     820,  1281,   829,   433,   742,   692,   726,   831,  1456,    16,
     844,  1301,  1462,   641,   843,  1303,   453,   454,  1461,   367,
     632,   859,   865,  1450,   869,   427,   544,   881,   596,   428,
     870,   367,   883,   888,   889,   895,   464,   368,   597,   890,
     764,   913,   469,  1488,   919,   924,   925,   936,   973,   368,
     976,  1612,   978,   990,   977,  1046,   544,  1339,  1100,   367,
     367,  1104,  1110,  1128,  1133,  1134,    13,  1148,  1705,  1208,
    1201,  1504,  1505,  1177,  1215,  1508,  1238,   368,   368,   483,
     484,  1228,  1239,  1240,   367,  1255,    14,  1513,  1256,  1259,
    1269,  1268,  1271,   598,   429,  1291,  1270,  1299,   430,  1311,
    1325,   431,   368,  1275,  1276,  1313,  1286,  1287,  1315,  1317,
    1534,  1530,  1292,   486,   487,  1336,   432,  1334,  1335,  1373,
    1346,  1347,   433,   544,   544,   544,   544,   544,   544,   544,
     544,  1354,  1357,  1361,  1375,  1383,   544,   544,  1669,  1408,
    1415,   424,   544,  1430,  1416,  1417,  1418,  1419,  1457,   367,
     424,   544,   544,  1431,    62,  1433,   544,   544,   544,  1464,
     544,  1439,  1468,  1455,  1467,  1493,  1546,   368,   425,   426,
    1498,   502,   503,   504,    50,  1530,  1503,   425,   426,  1519,
    1521,   766,   767,  1526,    63,  1522,  1547,  1556,   544,  1594,
    1558,  1569,  1528,  1597,   516,  1563,  1484,  1552,  1553,  1562,
    1564,  1634,  1568,  1598,  1444,  1571,  1654,  1572,  1655,   659,
    1588,  1656,  1697,  1703,  1614,  1629,  1636,  1637,  1638,  1701,
    1639,  1698,  1668,  1619,  1677,  1708,   523,  1692,  1753,   424,
    1693,   367,   427,  1707,  1721,   301,   428,  1723,  1724,  1729,
     972,   427,  1730,  1632,   596,   428,  1731,  1734,  1737,   368,
    1642,  1732,  1739,  1750,   597,  1743,   425,   426,  1476,  1751,
    1752,  1754,  1758,   137,    20,    89,   184,   140,  1662,  1601,
    1099,  1602,  1247,  1603,  1604,  1209,   776,   777,   778,   779,
     780,   673,  1605,   783,   784,   785,   786,  1209,   788,   789,
     790,   791,   325,    26,  1258,  1414,   792,  1667,   794,   795,
    1575,   429,  1527,  1549,   301,   430,  1625,  1327,   431,   598,
     429,  1550,  1446,  1516,   430,    99,  1626,   431,  1627,  1696,
     427,   661,   581,   432,   428,   662,  1585,  1376,   301,   433,
     960,   367,   432,   367,     0,     0,  1702,     0,   433,     0,
       0,     0,   631,     0,     0,   894,     0,     0,     0,   368,
    1712,   368,     0,     0,     0,   807,   808,   809,   810,   811,
     812,   813,   814,   815,   816,   766,   767,     0,     0,     0,
     572,     0,     0,     0,   817,   818,     0,     0,     0,     0,
       0,   592,     0,     0,     0,     0,  1738,     0,     0,   429,
     603,     0,   301,   430,     0,  1328,   431,   424,     0,     0,
     616,     0,     0,     0,     0,     0,     0,     0,  1748,     0,
     626,   432,     0,     0,     0,   766,   767,   433,     0,     0,
     301,   645,     0,     0,   425,   426,     0,     0,     0,     0,
       0,   367,     0,   656,     0,     0,     0,     0,     0,     0,
    1747,     0,     0,   675,     0,     0,   367,     0,   680,   368,
     682,     0,     0,     0,   708,   708,   708,     0,     0,   691,
     301,     0,   778,   779,   368,     0,     0,   703,   704,   705,
     786,     0,   788,   789,   790,   791,     0,     0,     0,     0,
     792,     0,   723,     0,     0,     0,  1646,     0,   427,     0,
       0,   727,   428,  1646,     0,  1646,   731,   732,   733,   734,
     735,     0,  1646,     0,   984,     0,   752,     0,     0,   745,
     776,   777,   778,   779,   301,     0,  1646,   723,   367,     0,
     786,     0,   788,   789,   790,   791,     0,     0,  1403,     0,
     792,     0,   794,   795,   752,     0,   368,   766,   767,     0,
       0,     0,     0,     0,   812,   813,   814,   815,   816,     0,
       0,     0,     0,     0,     0,     0,     0,   429,   817,   818,
     822,   430,     0,  1345,   431,     0,     0,     0,  1646,   984,
       0,     0,     0,     0,     0,     0,     0,   835,     0,   432,
       0,  1716,     0,     0,     0,   433,     0,  1718,     0,     0,
       0,     0,     0,     0,   812,   813,   814,   815,   816,     0,
       0,     0,   544,     0,     0,   861,     0,     0,   817,   818,
       0,     0,     0,     0,  1646,  1646,     0,     0,     0,     0,
       0,     0,     0,  1745,  1746,   301,     0,     0,     0,     0,
       0,     0,   776,   777,   778,   779,   780,   752,     0,   783,
     784,   785,   786,     0,   788,   789,   790,   791,     0,   675,
       0,  1646,   792,  1756,   794,   795,   723,   916,     0,   917,
     798,   799,   800,     0,   922,   752,   804,     0,     0,     0,
     926,     0,     0,     0,     0,     0,     0,     0,     0,   935,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     938,   939,   940,   941,   942,   943,     0,   951,     0,   951,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,     0,     0,     0,     0,     0,   708,     0,     0,     0,
     817,   818,     0,     0,     0,   766,   767,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1021,  1023,  1025,  1027,
    1029,  1031,  1033,  1035,  1036,  1037,     0,     0,  1038,  1040,
    1042,  1043,  1044,  1045,     0,  1048,  1049,     0,  1052,  1053,
    1054,  1056,  1057,  1058,  1060,  1062,  1063,  1064,  1065,  1067,
    1069,  1071,  1072,  1074,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,     0,  1095,   752,     0,     0,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   425,   426,     0,
     776,   777,   778,   779,   780,     0,     0,   783,   784,   785,
     786,     0,   788,   789,   790,   791,     0,  1132,     0,     0,
     792,     0,   794,   795,  1136,     0,     0,     0,   798,     0,
     800,     0,  1147,     0,  1149,     0,     0,     0,     0,     0,
       0,     0,     0,  1156,  1157,  1158,  1159,  1160,  1161,  1162,
    1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,  1172,
    1173,   427,     0,     0,     0,   428,     0,   752,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   708,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,  1022,
    1024,  1026,  1028,  1030,  1032,  1034,   731,  1205,   817,   818,
       0,  1039,  1041,     0,     0,     0,     0,  1047,     0,     0,
    1227,     0,     0,   723,     0,     0,  1059,  1061,     0,  1232,
       0,  1066,  1068,  1070,     0,  1073,  1156,     0,     0,     0,
     429,     0,     0,     0,   430,  1244,  1350,   431,   723,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   708,   708,
     708,     0,   432,   752,     0,   752,     0,   752,   433,   752,
       0,   752,     0,   752,     0,   752,     0,   752,     0,     0,
       0,     0,   752,     0,   752,     0,     0,     0,     0,     0,
     752,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,   752,     0,   718,   766,   767,   752,
       0,   752,   326,   752,     0,     0,   752,     0,   327,     0,
       0,     0,     0,     0,   328,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   329,     0,     0,     0,     0,     0,
       0,     0,   330,     0,     0,     0,  1285,     0,   752,     0,
    1290,     0,     0,     0,     0,     0,     0,   331,     0,     0,
       0,     0,     0,     0,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,     0,     0,     0,
       0,     0,   776,   777,   778,   779,   780,     0,     0,   783,
     784,   785,   786,     0,   788,   789,   790,   791,     0,     0,
       0,     0,   792,     0,   794,   795,     0,     0,     0,     0,
     798,   799,   800,     0,     0,     0,    62,  1349,     0,     0,
     424,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,   -76,     0,     0,     0,     0,     0,     0,     0,  1363,
    1364,  1365,   766,   767,     0,  1368,    63,   425,   426,     0,
       0,     0,     0,  1374,     0,     0,   951,     0,     0,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,     0,     0,     0,     0,     0,     0,  1394,  1395,     0,
     817,   818,     0,     0,     0,   572,     0,     0,     0,     0,
       0,  1156,   366,     0,     0,     0,     0,     0,   424,     0,
       0,     0,     0,     0,     0,     0,     0,  1424,     0,  1426,
       0,   427,     0,     0,  1429,   428,     0,     0,     0,     0,
    1432,     0,     0,     0,  1435,   425,   426,     0,     0,   768,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,   786,   787,   788,
     789,   790,   791,     0,     0,     0,     0,   792,   793,   794,
     795,   796,   797,     0,  1458,   798,   799,   800,   801,   802,
     803,   804,     0,     0,     0,     0,     0,     0,     0,     0,
     429,   752,     0,     0,   430,   723,  1351,   431,     0,   427,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,   805,   806,     0,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,     0,     0,     0,     0,
       0,     0,     0,   424,     0,   817,   818,     0,     0,  1514,
     525,   706,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,     0,
     425,   426,   430,     0,  1352,   431,     0,   766,   767,  1539,
       0,  1540,     0,     0,     0,     0,     0,  1544,     0,     0,
     432,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1555,     0,  1557,     0,     0,     0,     0,
       0,     0,     0,   424,     0,  1565,  1566,  1567,     0,     0,
       0,     0,  1574,     0,   427,     0,     0,  1577,   428,     0,
       0,  1581,  1582,  1583,  1584,     0,     0,   723,  1587,     0,
     425,   426,     0,     0,  1593,     0,     0,     0,     0,     0,
       0,     0,     0,  1606,     0,   723,     0,     0,     0,     0,
       0,     0,   776,   777,   778,   779,   780,  1616,     0,   783,
     784,   785,   786,     0,   788,   789,   790,   791,     0,     0,
       0,     0,   792,     0,   794,   795,     0,     0,     0,     0,
     798,   799,   800,   429,     0,     0,   804,   430,     0,  1353,
     431,     0,   723,     0,   427,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,   432,  1648,  1649,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1659,     0,     0,     0,     0,  1665,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,  1672,     0,  1673,     0,     0,     0,     0,     0,     0,
     817,   818,     0,     0,     0,   525,   706,     0,     0,     0,
       0,     0,  1686,   429,  1687,     0,     0,   430,     0,  1355,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   723,  1706,   432,     0,     0,     0,     0,
       0,   433,  1711,     0,     0,     0,   452,     0,  1714,  1715,
     453,   454,     3,     0,   455,   456,   457,     0,   458,     0,
     459,   460,   461,   462,   463,     0,     0,     0,     0,     0,
     464,   465,   466,   467,   468,  1733,   469,  1735,  1736,     0,
       0,     0,     0,   470,   471,     0,  1742,   472,     0,   473,
     474,  1744,     0,   475,     0,     8,   476,   477,     0,   478,
     479,     0,     0,   480,   481,     0,     0,     0,     0,     0,
     482,     0,     0,   483,   484,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   485,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   486,   487,   488,
     489,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   492,   493,
     494,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   495,   496,   497,   498,
     499,     0,   500,     0,   501,   502,   503,   504,    50,   146,
     505,   506,   507,   508,   509,   510,   511,   512,    63,   513,
     514,   515,     0,     0,     0,     0,     0,     0,   516,   147,
     517,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,     0,    15,
       0,     0,   521,   522,     0,     0,   453,   454,     0,     0,
     523,     0,   524,     0,   525,   526,   459,   460,   461,   462,
     463,     0,     0,     0,     0,     0,   464,     0,   466,     0,
       0,     0,   469,     0,   424,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   474,     0,     0,   475,
       0,     0,   476,     0,   945,     0,   479,     0,     0,     0,
       0,   425,   426,     0,     0,     0,   588,     0,     0,   483,
     484,     0,   332,   333,   334,     0,   336,   337,   338,   339,
     340,   485,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,     0,   354,   355,   356,     0,     0,   359,
     360,   361,   362,   486,   487,   589,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,   427,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   495,   496,   497,   498,   499,     0,   500,   946,
     501,   502,   503,   504,    50,     0,     0,   506,   507,   508,
     509,   510,   511,   512,   947,   590,   514,   515,     0,     0,
       0,     0,     0,     0,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,   430,     0,
       0,   948,   519,   520,     0,    15,     0,     0,   521,   522,
       0,     0,     0,   453,   454,     0,   949,     0,   950,     0,
     525,   526,   433,   459,   460,   461,   462,   463,     0,     0,
       0,     0,     0,   464,     0,   466,     0,     0,     0,   469,
       0,   424,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   474,     0,     0,   475,     0,     0,   476,
       0,     0,     0,   479,     0,     0,     0,     0,   425,   426,
       0,     0,     0,   588,     0,     0,   483,   484,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   485,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     486,   487,   589,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,   427,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   495,
     496,   497,   498,   499,     0,   500,   946,   501,   502,   503,
     504,    50,     0,     0,   506,   507,   508,   509,   510,   511,
     512,   947,   590,   514,   515,     0,     0,     0,     0,     0,
       0,   516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,   430,     0,     0,   948,   519,
     520,     0,    15,     0,     0,   521,   522,     0,     0,     0,
     453,   454,     0,   949,     0,   958,     0,   525,   526,   433,
     459,   460,   461,   462,   463,     0,     0,     0,     0,     0,
     464,     0,   466,     0,     0,     0,   469,     0,   621,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     474,     0,     0,   475,     0,     0,   476,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     588,     0,     0,   483,   484,     0,   332,   333,   334,     0,
     336,   337,   338,   339,   340,   485,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,     0,   354,   355,
     356,     0,     0,   359,   360,   361,   362,   486,   487,   589,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   495,   496,   497,   498,
     499,     0,   500,     0,   501,   502,   503,   504,    50,     0,
       0,   506,   507,   508,   509,   510,   511,   512,    63,   590,
     514,   515,     0,     0,     0,     0,     0,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   622,     0,     0,   518,   519,   520,     0,    15,
       0,     0,   521,   522,     0,     0,     0,   453,   454,     0,
    1204,     0,   524,     0,   525,   526,   624,   459,   460,   461,
     462,   463,     0,     0,     0,     0,     0,   464,     0,   466,
       0,     0,     0,   469,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   474,     0,     0,
     475,     0,     0,   476,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   588,     0,     0,
     483,   484,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   486,   487,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,     0,     0,   671,
       0,     0,     0,     0,     0,   492,   493,   494,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   495,   496,   497,   498,   499,     0,   500,
       0,   501,   502,   503,   504,    50,     0,     0,   506,   507,
     508,   509,   510,   511,   512,    63,   590,   514,   515,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,     0,    15,     0,     0,   521,
     522,     0,     0,   453,   454,     0,     0,   523,     0,   524,
       0,   525,   526,   459,   460,   461,   462,   463,     0,     0,
       0,     0,     0,   464,     0,   466,     0,     0,     0,   469,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   474,     0,     0,   475,     0,     0,   476,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   588,     0,     0,   483,   484,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   485,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     486,   487,   488,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,     0,   701,     0,     0,     0,     0,
       0,   492,   493,   494,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   495,
     496,   497,   498,   499,     0,   500,     0,   501,   502,   503,
     504,    50,     0,     0,   506,   507,   508,   509,   510,   511,
     512,    63,   590,   514,   515,     0,     0,     0,     0,     0,
       0,   516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,    15,     0,     0,   521,   522,     0,     0,   453,
     454,     0,     0,   523,     0,   524,     0,   525,   526,   459,
     460,   461,   462,   463,     0,     0,     0,     0,     0,   464,
    1599,   466,   467,     0,     0,   469,     0,     0,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   474,
       0,     0,   475,     0,     0,   476,   477,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   588,
       0,     0,   483,   484,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   485,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   486,   487,   589,  1600,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   495,   496,   497,   498,   499,
       0,   500,     0,   501,   502,   503,   504,    50,     0,     0,
     506,   507,   508,   509,   510,   511,   512,    63,   590,   514,
     515,     0,     0,     0,     0,     0,     0,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,     0,    15,     0,
       0,   521,   522,     0,     0,   453,   454,     0,     0,   523,
       0,   524,     0,   525,   526,   459,   460,   461,   462,   463,
       0,     0,     0,     0,     0,   464,     0,   466,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   474,     0,     0,   475,     0,
       0,   476,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   588,     0,     0,   483,   484,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     485,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   486,   487,   488,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   492,   493,   494,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   495,   496,   497,   498,   499,     0,   500,     0,   501,
     502,   503,   504,    50,     0,     0,   506,   507,   508,   509,
     510,   511,   512,    63,   590,   514,   515,     0,     0,     0,
       0,     0,     0,   516,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,     0,    15,     0,     0,   521,   522,     0,
       0,   453,   454,     0,     0,   523,     0,   524,     0,   525,
     526,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,   464,     0,   466,     0,     0,     0,   469,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   474,     0,     0,   475,     0,     0,   476,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   588,     0,     0,   483,   484,   994,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,   946,   501,   502,   503,   504,    50,
       0,     0,   506,   507,   508,   509,   510,   511,   512,   947,
     590,   514,   515,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,   453,   454,     0,
       0,   995,     0,   524,   996,   525,   526,   459,   460,   461,
     462,   463,     0,     0,     0,     0,     0,   464,     0,   466,
       0,     0,   424,   469,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   474,     0,     0,
     475,     0,     0,   476,     0,     0,     0,   479,     0,   425,
     426,     0,     0,     0,     0,     0,     0,   588,     0,     0,
     483,   484,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   486,   487,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,   427,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,  1016,  1017,  1018,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   495,   496,   497,   498,   499,     0,   500,
       0,   501,   502,   503,   504,    50,     0,     0,   506,   507,
     508,   509,   510,   511,   512,    63,   590,   514,   515,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,   430,     0,  1470,   431,
       0,     0,   518,   519,   520,     0,    15,     0,     0,   521,
     522,     0,     0,     0,   432,   453,   454,   523,     0,   524,
     433,   525,   526,   736,     0,   459,   460,   461,   462,   463,
       0,     0,     0,     0,     0,   464,     0,   466,     0,     0,
     424,   469,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   474,     0,     0,   475,   737,
       0,   476,     0,     0,     0,   479,     0,   425,   426,     0,
       0,     0,     0,     0,     0,   588,     0,     0,   483,   484,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     485,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   486,   487,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
       0,   427,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   495,   496,   497,   498,   499,     0,   500,     0,   501,
     502,   503,   504,    50,     0,     0,   506,   507,   508,   509,
     510,   511,   512,    63,   590,   514,   515,     0,     0,     0,
       0,     0,     0,   516,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,   430,     0,  1475,   431,     0,     0,
     518,   519,   520,     0,    15,     0,     0,   521,   522,     0,
       0,     0,   432,   453,   454,   523,   625,   524,   433,   525,
     526,   736,     0,   459,   460,   461,   462,   463,     0,     0,
       0,     0,     0,   464,     0,   466,     0,     0,   424,   469,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   474,     0,     0,   475,   737,     0,   476,
       0,     0,     0,   479,     0,   425,   426,     0,     0,     0,
       0,     0,     0,   588,     0,     0,   483,   484,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   485,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     486,   487,   589,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,   427,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   495,
     496,   497,   498,   499,     0,   500,   946,   501,   502,   503,
     504,    50,     0,     0,   506,   507,   508,   509,   510,   511,
     512,   947,   590,   514,   515,     0,     0,     0,     0,     0,
       0,   516,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,   430,     0,  1511,   431,     0,     0,   518,   519,
     520,     0,    15,     0,     0,   521,   522,     0,     0,     0,
     432,   453,   454,   523,     0,   524,   433,   525,   526,   736,
       0,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,   464,     0,   466,     0,     0,   424,   469,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   474,     0,     0,   475,   737,     0,   476,     0,     0,
       0,   479,     0,   425,   426,     0,     0,     0,     0,     0,
       0,   588,     0,     0,   483,   484,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,   427,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,     0,   501,   502,   503,   504,    50,
       0,     0,   506,   507,   508,   509,   510,   511,   512,    63,
     590,   514,   515,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
     430,     0,  1617,   431,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,     0,   432,   453,
     454,   523,   859,   524,   433,   525,   526,   736,     0,   459,
     460,   461,   462,   463,     0,     0,     0,     0,     0,   464,
       0,   466,     0,     0,     0,   469,     0,     0,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   474,
       0,     0,   475,   737,     0,   476,     0,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   588,
       0,     0,   483,   484,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   485,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   486,   487,   589,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   495,   496,   497,   498,   499,
       0,   500,     0,   501,   502,   503,   504,    50,     0,     0,
     506,   507,   508,   509,   510,   511,   512,    63,   590,   514,
     515,     0,     0,     0,     0,     0,     0,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,     0,    15,     0,
       0,   521,   522,     0,     0,   453,   454,     0,     0,   523,
       0,   524,     0,   525,   526,   459,   460,   461,   462,   463,
       0,     0,     0,     0,     0,   464,     0,   466,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   474,     0,     0,   475,     0,
       0,   476,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   588,     0,     0,   483,   484,
    1142,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     485,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   486,   487,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   495,   496,   497,   498,   499,     0,   500,   946,   501,
     502,   503,   504,    50,     0,     0,   506,   507,   508,   509,
     510,   511,   512,   947,   590,   514,   515,     0,     0,     0,
       0,     0,     0,   516,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,     0,    15,     0,     0,   521,   522,     0,
       0,   453,   454,     0,     0,   523,     0,   524,     0,   525,
     526,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,   464,     0,   466,     0,     0,     0,   469,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   474,     0,     0,   475,     0,     0,   476,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   588,     0,     0,   483,   484,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,     0,   501,   502,   503,   504,    50,
       0,     0,   506,   507,   508,   509,   510,   511,   512,    63,
     590,   514,   515,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,   453,   454,     0,
       0,   523,   625,   524,     0,   525,   526,   459,   460,   461,
     462,   463,     0,     0,     0,     0,     0,   464,     0,   466,
       0,     0,     0,   469,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   474,     0,     0,
     475,     0,     0,   476,     0,     0,     0,   479,     0,     0,
       0,     0,     0,   679,     0,     0,     0,   588,     0,     0,
     483,   484,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   486,   487,   589,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   495,   496,   497,   498,   499,     0,   500,
       0,   501,   502,   503,   504,    50,     0,     0,   506,   507,
     508,   509,   510,   511,   512,    63,   590,   514,   515,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,     0,    15,     0,     0,   521,
     522,     0,     0,   453,   454,     0,     0,   523,     0,   524,
       0,   525,   526,   459,   460,   461,   462,   463,     0,     0,
       0,     0,     0,   464,     0,   466,     0,     0,     0,   469,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   474,     0,     0,   475,     0,     0,   476,
       0,     0,     0,   479,     0,     0,   690,     0,     0,     0,
       0,     0,     0,   588,     0,     0,   483,   484,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   485,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     486,   487,   589,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   495,
     496,   497,   498,   499,     0,   500,     0,   501,   502,   503,
     504,    50,     0,     0,   506,   507,   508,   509,   510,   511,
     512,    63,   590,   514,   515,     0,     0,     0,     0,     0,
       0,   516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,    15,     0,     0,   521,   522,     0,     0,     0,
       0,   453,   454,   523,     0,   524,     0,   525,   526,   722,
       0,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,   464,     0,   466,     0,     0,     0,   469,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   474,     0,     0,   475,     0,     0,   476,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   588,     0,     0,   483,   484,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,     0,   501,   502,   503,   504,    50,
       0,     0,   506,   507,   508,   509,   510,   511,   512,    63,
     590,   514,   515,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,   453,   454,     0,
       0,   523,     0,   524,     0,   525,   526,   459,   460,   461,
     462,   463,     0,     0,     0,     0,     0,   464,     0,   466,
       0,     0,     0,   469,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   474,     0,     0,
     475,     0,     0,   476,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   588,     0,     0,
     483,   484,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   486,   487,   589,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   495,   496,   497,   498,   499,     0,   500,
       0,   501,   502,   503,   504,    50,     0,     0,   506,   507,
     508,   509,   510,   511,   512,    63,   590,   514,   515,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,     0,   518,   519,   520,     0,    15,     0,     0,   521,
     522,     0,     0,     0,     0,   453,   454,   523,     0,   524,
       0,   525,   526,   747,     0,   459,   460,   461,   462,   463,
       0,     0,     0,     0,     0,   464,     0,   466,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   474,     0,     0,   475,     0,
       0,   476,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   588,     0,     0,   483,   484,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     485,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   486,   487,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   495,   496,   497,   498,   499,     0,   500,     0,   501,
     502,   503,   504,    50,     0,     0,   506,   507,   508,   509,
     510,   511,   512,    63,   590,   514,   515,     0,     0,     0,
       0,     0,     0,   516,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,     0,    15,     0,     0,   521,   522,     0,
       0,   453,   454,     0,     0,   523,     0,   524,     0,   525,
     526,   459,   460,   461,   462,   463,     0,     0,  1075,     0,
       0,   464,     0,   466,     0,     0,     0,   469,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   474,     0,     0,   475,     0,     0,   476,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   588,     0,     0,   483,   484,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,     0,   501,   502,   503,   504,    50,
       0,     0,   506,   507,   508,   509,   510,   511,   512,    63,
     590,   514,   515,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,   453,   454,     0,
       0,   523,     0,   524,     0,   525,   526,   459,   460,   461,
     462,   463,     0,     0,     0,     0,     0,   464,     0,   466,
       0,     0,     0,   469,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   474,     0,     0,
     475,     0,     0,   476,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   588,     0,     0,
     483,   484,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   486,   487,   589,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   495,   496,   497,   498,   499,     0,   500,
       0,   501,   502,   503,   504,    50,     0,     0,   506,   507,
     508,   509,   510,   511,   512,    63,   590,   514,   515,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,     0,    15,     0,     0,   521,
     522,     0,     0,   453,   454,     0,     0,   523,     0,   524,
    1096,   525,   526,   459,   460,   461,   462,   463,     0,     0,
       0,     0,     0,   464,     0,   466,     0,     0,     0,   469,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   474,     0,     0,   475,     0,     0,   476,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   588,     0,     0,   483,   484,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   485,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     486,   487,   589,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   495,
     496,   497,   498,   499,     0,   500,     0,   501,   502,   503,
     504,    50,     0,     0,   506,   507,   508,   509,   510,   511,
     512,    63,   590,   514,   515,     0,     0,     0,     0,     0,
       0,   516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1146,     0,   518,   519,
     520,     0,    15,     0,     0,   521,   522,     0,     0,   453,
     454,     0,     0,   523,     0,   524,     0,   525,   526,   459,
     460,   461,   462,   463,     0,     0,     0,     0,     0,   464,
       0,   466,     0,     0,     0,   469,     0,     0,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   474,
       0,     0,   475,     0,     0,   476,     0,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   588,
       0,     0,   483,   484,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   485,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   486,   487,   589,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   495,   496,   497,   498,   499,
       0,   500,     0,   501,   502,   503,   504,    50,     0,     0,
     506,   507,   508,   509,   510,   511,   512,    63,   590,   514,
     515,     0,     0,     0,     0,     0,     0,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,     0,    15,     0,
       0,   521,   522,     0,     0,   453,   454,     0,     0,   523,
       0,   524,  1206,   525,   526,   459,   460,   461,   462,   463,
       0,     0,     0,     0,     0,   464,     0,   466,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   474,     0,     0,   475,     0,
       0,   476,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   588,     0,     0,   483,   484,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     485,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   486,   487,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   495,   496,   497,   498,   499,     0,   500,     0,   501,
     502,   503,   504,    50,     0,     0,   506,   507,   508,   509,
     510,   511,   512,    63,   590,   514,   515,     0,     0,     0,
       0,     0,     0,   516,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,     0,    15,     0,     0,   521,   522,     0,
       0,   453,   454,     0,     0,   523,     0,   524,  1221,   525,
     526,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,   464,     0,   466,     0,     0,     0,   469,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   474,     0,     0,   475,     0,     0,   476,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   588,     0,     0,   483,   484,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,     0,   501,   502,   503,   504,    50,
       0,     0,   506,   507,   508,   509,   510,   511,   512,    63,
     590,   514,   515,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,   453,   454,     0,
       0,   523,     0,   524,  1427,   525,   526,   459,   460,   461,
     462,   463,     0,     0,     0,     0,     0,   464,     0,   466,
       0,     0,     0,   469,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   474,     0,     0,
     475,     0,     0,   476,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   588,     0,     0,
     483,   484,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   486,   487,   589,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   495,   496,   497,   498,   499,     0,   500,
       0,   501,   502,   503,   504,    50,     0,     0,   506,   507,
     508,   509,   510,   511,   512,    63,   590,   514,   515,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,     0,    15,     0,     0,   521,
     522,     0,     0,   453,   454,     0,     0,  1436,     0,   524,
    1437,   525,   526,   459,   460,   461,   462,   463,     0,     0,
       0,     0,     0,   464,     0,   466,     0,     0,     0,   469,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   474,     0,     0,   475,     0,     0,   476,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   588,     0,     0,   483,   484,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   485,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     486,   487,   589,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   495,
     496,   497,   498,   499,     0,   500,     0,   501,   502,   503,
     504,    50,     0,     0,   506,   507,   508,   509,   510,   511,
     512,    63,   590,   514,   515,     0,     0,     0,     0,     0,
       0,   516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,    15,     0,     0,   521,   522,     0,     0,   453,
     454,     0,     0,   523,     0,   524,  1442,   525,   526,   459,
     460,   461,   462,   463,     0,     0,     0,     0,     0,   464,
       0,   466,     0,     0,     0,   469,     0,     0,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   474,
       0,     0,   475,     0,     0,   476,     0,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   588,
       0,     0,   483,   484,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   485,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   486,   487,   589,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   495,   496,   497,   498,   499,
       0,   500,     0,   501,   502,   503,   504,    50,     0,     0,
     506,   507,   508,   509,   510,   511,   512,    63,   590,   514,
     515,     0,     0,     0,     0,     0,     0,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,     0,    15,     0,
       0,   521,   522,     0,     0,   453,   454,     0,     0,   523,
       0,   524,  1487,   525,   526,   459,   460,   461,   462,   463,
       0,     0,     0,     0,     0,   464,     0,   466,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   474,     0,     0,   475,     0,
       0,   476,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   588,     0,     0,   483,   484,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     485,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   486,   487,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   495,   496,   497,   498,   499,     0,   500,     0,   501,
     502,   503,   504,    50,     0,     0,   506,   507,   508,   509,
     510,   511,   512,    63,   590,   514,   515,     0,     0,     0,
       0,     0,     0,   516,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,     0,    15,     0,     0,   521,   522,     0,
       0,   453,   454,     0,     0,   523,     0,   524,  1573,   525,
     526,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,   464,     0,   466,     0,     0,     0,   469,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   474,     0,     0,   475,     0,     0,   476,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   588,     0,     0,   483,   484,     0,   332,   333,   334,
       0,   336,   337,   338,   339,   340,   485,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,     0,   354,
     355,   356,     0,     0,   359,   360,   361,   362,   486,   487,
     589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,     0,  1615,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   495,   496,   497,
     498,   499,     0,   500,     0,   501,   502,   503,   504,    50,
       0,     0,   506,   507,   508,   509,   510,   511,   512,    63,
     590,   514,   515,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
      15,     0,     0,   521,   522,     0,     0,   453,   454,     0,
       0,   523,     0,   524,     0,   525,   526,   459,   460,   461,
     462,   463,     0,     0,     0,     0,     0,   464,     0,   466,
       0,     0,     0,   469,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   474,     0,     0,
     475,     0,     0,   476,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   588,     0,     0,
     483,   484,     0,   332,   333,   334,     0,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,     0,     0,
     359,   360,   361,   362,   486,   487,   589,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,     0,     0,  1663,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   495,   496,   497,   498,   499,     0,   500,
       0,   501,   502,   503,   504,    50,     0,     0,   506,   507,
     508,   509,   510,   511,   512,    63,   590,   514,   515,     0,
       0,     0,     0,     0,     0,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,     0,    15,     0,     0,   521,
     522,     0,     0,   453,   454,     0,     0,   523,     0,   524,
       0,   525,   526,   459,   460,   461,   462,   463,     0,     0,
       0,     0,     0,   464,     0,   466,     0,     0,     0,   469,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   474,     0,     0,   475,     0,     0,   476,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   588,     0,     0,   483,   484,     0,   332,
     333,   334,     0,   336,   337,   338,   339,   340,   485,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
     486,   487,   589,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,     0,  1664,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   495,
     496,   497,   498,   499,     0,   500,     0,   501,   502,   503,
     504,    50,     0,     0,   506,   507,   508,   509,   510,   511,
     512,    63,   590,   514,   515,     0,     0,     0,     0,     0,
       0,   516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,    15,     0,     0,   521,   522,     0,     0,   453,
     454,     0,     0,   523,     0,   524,     0,   525,   526,   459,
     460,   461,   462,   463,     0,     0,     0,     0,     0,   464,
       0,   466,     0,     0,     0,   469,     0,     0,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   474,
       0,     0,   475,     0,     0,   476,     0,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   588,
       0,     0,   483,   484,     0,   332,   333,   334,     0,   336,
     337,   338,   339,   340,   485,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,     0,   354,   355,   356,
       0,     0,   359,   360,   361,   362,   486,   487,   589,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   495,   496,   497,   498,   499,
       0,   500,     0,   501,   502,   503,   504,    50,     0,     0,
     506,   507,   508,   509,   510,   511,   512,    63,   590,   514,
     515,     0,     0,     0,     0,     0,     0,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,     0,    15,     0,
       0,   521,   522,     0,     0,   453,   454,     0,     0,   523,
       0,   524,     0,   525,   526,   459,   460,   461,   462,   463,
       0,     0,     0,     0,     0,   464,     0,   466,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   474,     0,     0,   475,     0,
       0,   476,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   588,     0,     0,   483,   484,
       0,   332,   333,   334,     0,   336,   337,   338,   339,   340,
     485,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,     0,   354,   355,   356,     0,     0,   359,   360,
     361,   362,   486,   487,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   495,   496,   497,   498,   499,     0,   500,     0,   501,
     502,   503,   504,    50,     0,     0,   506,   507,   508,   509,
     510,   511,   512,    63,   590,   514,   515,     0,   720,     0,
       0,     0,     0,   516,   326,     0,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,   328,     0,     0,     0,
     518,   519,   520,     0,    15,     0,   329,   521,   522,     0,
       0,     0,     0,     0,   330,  1410,     0,   524,     0,   525,
     526,     0,     0,     0,     0,     0,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   326,     0,     0,
       0,     0,     0,   327,     0,     0,     0,     0,     0,   328,
       0,     0,     0,     0,     0,     0,     0,     0,    62,   329,
       0,     0,     0,     0,     0,     0,     0,   330,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,    63,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
     326,     0,     0,     0,     0,     0,   327,     0,     0,     0,
       0,     0,   328,     0,     0,     0,     0,     0,     0,     0,
       0,    62,   329,     0,     0,     0,     0,     0,     0,     0,
     330,     0,     0,     0,   365,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   331,     0,     0,     0,     0,
       0,    63,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,   628,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
       0,     0,     0,     0,     0,     0,    13,     0,     0,   326,
       0,     0,     0,     0,   634,   327,     0,     0,     0,     0,
       0,   328,     0,     0,     0,     0,    14,     0,     0,     0,
       0,   329,   635,     0,     0,     0,     0,     0,     0,   330,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   331,     0,     0,     0,     0,     0,
     366,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,     0,     0,     0,     0,     0,   327,     0,
       0,     0,     0,     0,   328,     0,     0,     0,     0,     0,
       0,     0,     0,    62,   329,     0,     0,     0,     0,     0,
       0,     0,   330,     0,     0,     0,   365,     0,     0,     0,
       0,     0,     0,     0,     0,   766,   767,   331,     0,     0,
       0,     0,     0,    63,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,     0,   766,   767,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
     776,   777,   778,   779,   780,     0,     0,   783,   784,   785,
     786,     0,   788,   789,   790,   791,   634,     0,     0,     0,
     792,     0,   794,   795,     0,     0,     0,     0,   798,     0,
       0,     0,     0,     0,     0,   896,   897,   898,   899,   900,
     901,   902,   903,   776,   777,   778,   779,   780,   904,   905,
     783,   784,   785,   786,   906,   788,   789,   790,   791,     0,
       0,     0,   366,   792,   793,   794,   795,   907,   908,   766,
     767,   798,   799,   800,   909,   910,   911,   804,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,     0,
       0,     0,    13,     0,     0,     0,     0,     0,   817,   818,
       0,     0,     0,     0,   766,   767,     0,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,   912,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,     0,   525,   706,     0,     0,
       0,     0,     0,     0,     0,     0,   896,   897,   898,   899,
     900,   901,   902,   903,   776,   777,   778,   779,   780,   904,
     905,   783,   784,   785,   786,   906,   788,   789,   790,   791,
    -377,     0,     0,     0,   792,   793,   794,   795,   907,   908,
       0,     0,   798,   799,   800,   909,   910,   911,   804,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,   766,   767,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,     0,     0,     0,     0,     0,     0,
     912,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   766,   767,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,     0,   525,   706,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,   830,     0,     0,     0,     0,     0,     0,     0,     0,
     776,   777,   778,   779,   780,     0,     0,   783,   784,   785,
     786,     0,   788,   789,   790,   791,     0,     0,     0,     0,
     792,     0,   794,   795,     0,     0,     0,     0,   798,   799,
     800,     0,     0,     0,   804,     0,     0,     0,   776,   777,
     778,   779,   780,     0,     0,   783,   784,   785,   786,     0,
     788,   789,   790,   791,   766,   767,     0,     0,   792,     0,
     794,   795,     0,     0,     0,     0,   798,   799,   800,     0,
       0,     0,   804,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   766,
     767,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,   840,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,   818,     0,     0,
     856,     0,     0,     0,     0,     0,     0,     0,     0,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,     0,     0,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
     766,   767,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,   766,   767,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,   868,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1178,     0,     0,     0,
       0,     0,     0,     0,     0,   776,   777,   778,   779,   780,
       0,     0,   783,   784,   785,   786,     0,   788,   789,   790,
     791,     0,     0,     0,     0,   792,     0,   794,   795,     0,
       0,     0,     0,   798,   799,   800,     0,     0,     0,   804,
       0,     0,     0,   776,   777,   778,   779,   780,     0,     0,
     783,   784,   785,   786,     0,   788,   789,   790,   791,   766,
     767,     0,     0,   792,     0,   794,   795,     0,     0,     0,
       0,   798,   799,   800,     0,     0,     0,   804,     0,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   766,   767,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,  1182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,  1184,     0,     0,     0,     0,
       0,     0,     0,     0,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
       0,     0,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,   766,   767,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   766,   767,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1193,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,  1194,     0,     0,     0,     0,     0,     0,     0,     0,
     776,   777,   778,   779,   780,     0,     0,   783,   784,   785,
     786,     0,   788,   789,   790,   791,     0,     0,     0,     0,
     792,     0,   794,   795,     0,     0,     0,     0,   798,   799,
     800,     0,     0,     0,   804,     0,     0,     0,   776,   777,
     778,   779,   780,     0,     0,   783,   784,   785,   786,     0,
     788,   789,   790,   791,   766,   767,     0,     0,   792,     0,
     794,   795,     0,     0,     0,     0,   798,   799,   800,     0,
       0,     0,   804,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   766,
     767,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,  1195,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,   818,     0,     0,
    1196,     0,     0,     0,     0,     0,     0,     0,     0,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,     0,     0,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
     766,   767,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,   766,   767,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,  1197,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1198,     0,     0,     0,
       0,     0,     0,     0,     0,   776,   777,   778,   779,   780,
       0,     0,   783,   784,   785,   786,     0,   788,   789,   790,
     791,     0,     0,     0,     0,   792,     0,   794,   795,     0,
       0,     0,     0,   798,   799,   800,     0,     0,     0,   804,
       0,     0,     0,   776,   777,   778,   779,   780,     0,     0,
     783,   784,   785,   786,     0,   788,   789,   790,   791,   766,
     767,     0,     0,   792,     0,   794,   795,     0,     0,     0,
       0,   798,   799,   800,     0,     0,     0,   804,     0,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   766,   767,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,  1324,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,  1337,     0,     0,     0,     0,
       0,     0,     0,     0,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
       0,     0,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,   766,   767,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   766,   767,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,  1490,     0,     0,     0,     0,     0,     0,     0,     0,
     776,   777,   778,   779,   780,     0,     0,   783,   784,   785,
     786,     0,   788,   789,   790,   791,     0,     0,     0,     0,
     792,     0,   794,   795,     0,     0,     0,     0,   798,   799,
     800,     0,     0,     0,   804,     0,     0,     0,   776,   777,
     778,   779,   780,     0,     0,   783,   784,   785,   786,     0,
     788,   789,   790,   791,   766,   767,     0,     0,   792,     0,
     794,   795,     0,     0,     0,     0,   798,   799,   800,     0,
       0,     0,   804,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   766,
     767,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,  1509,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,   818,     0,     0,
    1510,     0,     0,     0,     0,     0,     0,     0,     0,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,     0,     0,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
     766,   767,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,   766,   767,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,  1533,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1535,     0,     0,     0,
       0,     0,     0,     0,     0,   776,   777,   778,   779,   780,
       0,     0,   783,   784,   785,   786,     0,   788,   789,   790,
     791,     0,     0,     0,     0,   792,     0,   794,   795,     0,
       0,     0,     0,   798,   799,   800,     0,     0,     0,   804,
       0,     0,     0,   776,   777,   778,   779,   780,     0,     0,
     783,   784,   785,   786,     0,   788,   789,   790,   791,   766,
     767,     0,     0,   792,     0,   794,   795,     0,     0,     0,
       0,   798,   799,   800,     0,     0,     0,   804,     0,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   766,   767,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,  1537,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,  1541,     0,     0,     0,     0,
       0,     0,     0,     0,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
       0,     0,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,   766,   767,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   766,   767,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1620,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,  1621,     0,     0,     0,     0,     0,     0,     0,     0,
     776,   777,   778,   779,   780,     0,     0,   783,   784,   785,
     786,     0,   788,   789,   790,   791,     0,     0,     0,     0,
     792,     0,   794,   795,     0,     0,     0,     0,   798,   799,
     800,     0,     0,     0,   804,     0,     0,     0,   776,   777,
     778,   779,   780,     0,     0,   783,   784,   785,   786,     0,
     788,   789,   790,   791,   766,   767,     0,     0,   792,     0,
     794,   795,     0,     0,     0,     0,   798,   799,   800,     0,
       0,     0,   804,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   766,
     767,     0,     0,     0,     0,     0,     0,     0,   817,   818,
       0,     0,  1623,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,   818,     0,     0,
    1633,     0,     0,     0,     0,     0,     0,     0,     0,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,     0,     0,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
     766,   767,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,   766,   767,
       0,     0,     0,     0,     0,     0,     0,   817,   818,     0,
       0,  1635,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1645,     0,     0,     0,
       0,     0,     0,     0,     0,   776,   777,   778,   779,   780,
       0,     0,   783,   784,   785,   786,     0,   788,   789,   790,
     791,     0,     0,     0,     0,   792,     0,   794,   795,     0,
       0,     0,     0,   798,   799,   800,     0,     0,     0,   804,
       0,     0,     0,   776,   777,   778,   779,   780,     0,     0,
     783,   784,   785,   786,     0,   788,   789,   790,   791,   766,
     767,     0,     0,   792,     0,   794,   795,     0,     0,     0,
       0,   798,   799,   800,     0,     0,     0,   804,     0,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,   766,   767,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,  1650,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,     0,   807,   808,   809,   810,   811,   812,   813,   814,
     815,   816,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   817,   818,     0,     0,  1725,     0,     0,     0,     0,
       0,     0,     0,     0,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
       0,     0,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,   766,   767,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   766,   767,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,  1726,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,   860,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     776,   777,   778,   779,   780,     0,     0,   783,   784,   785,
     786,     0,   788,   789,   790,   791,     0,     0,     0,     0,
     792,     0,   794,   795,     0,     0,     0,     0,   798,   799,
     800,     0,     0,     0,   804,     0,     0,     0,   776,   777,
     778,   779,   780,     0,     0,   783,   784,   785,   786,     0,
     788,   789,   790,   791,   766,   767,     0,     0,   792,     0,
     794,   795,     0,     0,     0,     0,   798,   799,   800,     0,
       0,     0,   804,     0,     0,     0,     0,   806,     0,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   766,
     767,     0,     0,     0,     0,     0,     0,     0,   817,   818,
    1129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   817,   818,  1277,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   776,
     777,   778,   779,   780,     0,     0,   783,   784,   785,   786,
       0,   788,   789,   790,   791,     0,     0,     0,     0,   792,
       0,   794,   795,     0,     0,     0,     0,   798,   799,   800,
       0,     0,     0,   804,   776,   777,   778,   779,   780,     0,
       0,   783,   784,   785,   786,     0,   788,   789,   790,   791,
     766,   767,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,     0,
       0,     0,     0,     0,     0,     0,   806,     0,   807,   808,
     809,   810,   811,   812,   813,   814,   815,   816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   817,   818,  1293,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,  1434,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   776,   777,   778,   779,   780,
       0,     0,   783,   784,   785,   786,     0,   788,   789,   790,
     791,   246,   247,     0,     0,   792,     0,   794,   795,     0,
       0,     0,     0,   798,   799,   800,     0,     0,   248,   804,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   766,   767,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,   818,  1440,     0,     0,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,     0,     0,   267,   268,
     269,     0,     0,     0,     0,     0,     0,   270,   271,   272,
     273,   274,     0,     0,   275,   276,   277,   278,   279,   280,
     281,     0,     0,     0,     0,     0,   766,   767,   776,   777,
     778,   779,   780,     0,     0,   783,   784,   785,   786,     0,
     788,   789,   790,   791,     0,     0,     0,     0,   792,     0,
     794,   795,     0,     0,     0,     0,   798,   799,   800,     0,
       0,     0,   804,   282,     0,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,    50,     0,   293,   294,     0,
       0,     0,     0,     0,   295,   296,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,     0,   807,   808,   809,
     810,   811,   812,   813,   814,   815,   816,     0,     0,   766,
     767,   776,   777,   778,   779,   780,   817,   818,   783,   784,
     785,   786,     0,   788,   789,   790,   791,     0,     0,     0,
       0,   792,     0,   794,   795,     0,     0,     0,     0,   798,
     799,   800,     0,     0,     0,   804,     0,   766,   767,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,     0,     0,     0,     0,     0,     0,     0,   806,     0,
     807,   808,   809,   810,   811,   812,   813,   814,   815,   816,
       0,     0,     0,     0,   776,   777,   778,   779,   780,   817,
     818,   783,   784,   785,   786,     0,   788,   789,   790,   791,
       0,     0,     0,     0,   792,     0,   794,   795,     0,     0,
       0,     0,   798,   799,   800,     0,     0,     0,   804,     0,
     766,   767,   776,   777,   778,   779,   780,     0,     0,   783,
     784,   785,   786,    13,   788,   789,   790,   791,     0,     0,
       0,     0,   792,     0,   794,   795,     0,     0,   962,     0,
     798,   799,   800,    14,     0,     0,   804,   766,   767,     0,
       0,   806,     0,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   817,   818,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,     0,     0,     0,     0,   776,   777,   778,   779,   780,
     817,   818,   783,   784,   785,   786,     0,   788,   789,   790,
     791,     0,     0,     0,     0,   792,     0,   794,   795,     0,
       0,  1207,     0,   798,   799,   800,     0,     0,     0,   804,
     766,   767,   776,   777,   778,   779,   780,     0,     0,   783,
     784,   785,   786,     0,   788,   789,   790,   791,     0,     0,
       0,     0,   792,     0,   794,   795,     0,     0,     0,     0,
     798,   799,   800,     0,     0,     0,   804,   766,   767,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
    1282,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,     0,     0,     0,     0,   776,   777,   778,   779,   780,
     817,   818,   783,   784,   785,   786,     0,   788,   789,   790,
     791,     0,     0,     0,     0,   792,     0,   794,   795,     0,
       0,     0,     0,   798,   799,   800,     0,     0,     0,   804,
     766,   767,   776,   777,   778,   779,   780,     0,     0,   783,
     784,   785,   786,     0,   788,   789,   790,   791,     0,     0,
       0,     0,   792,     0,   794,   795,     0,     0,     0,     0,
     798,   799,   800,     0,     0,     0,   804,   766,   767,     0,
       0,     0,   806,  1387,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
       0,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,     0,     0,   766,   767,   776,   777,   778,   779,   780,
     817,   818,   783,   784,   785,   786,     0,   788,   789,   790,
     791,     0,     0,     0,     0,   792,     0,   794,   795,     0,
       0,     0,     0,   798,   799,   800,     0,     0,     0,  -784,
     766,   767,   776,   777,   778,   779,   780,     0,     0,   783,
     784,   785,   786,     0,   788,   789,   790,   791,     0,     0,
       0,     0,   792,     0,   794,   795,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   806,     0,   807,   808,   809,   810,   811,   812,
     813,   814,   815,   816,     0,     0,   766,   767,   776,   777,
     778,   779,   780,   817,   818,   783,   784,   785,   786,     0,
     788,   789,   790,   791,     0,     0,     0,     0,   792,     0,
     794,   795,   808,   809,   810,   811,   812,   813,   814,   815,
     816,     0,     0,     0,     0,   776,   777,   778,   779,   780,
     817,   818,   783,   784,   785,   786,     0,   788,   789,   790,
     791,     0,     0,     0,     0,   792,     0,   794,   795,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   809,
     810,   811,   812,   813,   814,   815,   816,  1008,     0,     0,
       0,   776,   777,   778,   779,   780,   817,   818,   783,     0,
       0,   786,     0,   788,   789,   790,   791,     0,     0,     0,
       0,   792,     0,   794,   795,     0,     0,   810,   811,   812,
     813,   814,   815,   816,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   817,   818,     0,     0,     0,     0,   332,
     333,   334,  1012,   336,   337,   338,   339,   340,   485,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
       0,   354,   355,   356,     0,     0,   359,   360,   361,   362,
       0,     0,     0,   810,   811,   812,   813,   814,   815,   816,
       0,     0,     0,     0,     0,     0,  1278,     0,     0,   817,
     818,     0,     0,     0,   332,   333,   334,     0,   336,   337,
     338,   339,   340,   485,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,     0,   354,   355,   356,     0,
       0,   359,   360,   361,   362,     0,  1009,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,   333,
     334,  1010,   336,   337,   338,   339,   340,   485,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,     0,
     354,   355,   356,     0,     0,   359,   360,   361,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1013,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,   333,   334,  1014,   336,   337,   338,
     339,   340,   485,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,     0,   354,   355,   356,   195,     0,
     359,   360,   361,   362,     0,  1279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1280,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1087,  1088,     0,   196,     0,   197,     0,   198,   199,
     200,   201,   202,     0,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,     0,   214,   215,   216,     0,
    1089,   217,   218,   219,   220,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1090,     0,     0,     0,     0,
       0,   221,   222,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1091,  1092,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   223
};

static const yytype_int16 yycheck[] =
{
       1,   189,   595,     7,   225,   231,   650,   504,   544,   392,
     618,   678,   741,    15,    16,   719,   241,   721,   609,    20,
     611,   750,   613,   440,   406,   439,   171,   937,   657,   400,
     847,   721,   849,   392,   851,   493,   494,   561,  1210,   465,
     953,   883,   930,    86,    22,   932,   959,     8,     5,    15,
      16,   755,    33,   435,   109,    20,    20,    20,    20,  1413,
      20,   585,    53,   489,    20,   166,    68,    69,    70,   129,
     130,    20,    20,   143,    33,    20,     4,    53,    20,    57,
      19,    20,    20,   195,    57,   163,   312,   199,   173,   718,
      63,   720,   127,   722,     0,    96,   152,    98,   144,   145,
     146,     7,   165,   173,   160,   183,   465,   736,   110,   111,
     112,   113,    15,    16,   173,   160,   158,   173,   747,   152,
     502,   503,   185,    46,    30,   129,    32,   127,    34,   127,
     489,   173,   217,   127,    40,     7,   199,   137,   165,   137,
     173,   219,   220,   143,    50,   102,   103,   217,   149,  1596,
      56,    62,   994,    33,   214,   215,   191,   192,   185,   194,
     219,   217,   197,   892,   168,   390,   392,   158,  1522,   439,
     440,   316,   218,   599,    80,   220,   218,   232,    50,   883,
      60,    61,   173,   106,   708,   192,   190,   191,   192,   165,
     233,   164,   192,   220,   192,  1367,   102,   103,   192,   928,
     166,   195,   931,   164,   165,   171,   394,   173,   191,   185,
     176,   184,   438,  1481,  1661,   155,   197,   194,    21,    22,
     160,   225,  1624,   184,   185,   203,   191,   191,   191,   191,
     163,   191,   160,   234,   173,   191,   214,   465,   197,   241,
     599,   198,   191,   191,   124,   671,   191,   468,   128,   191,
     183,   217,   881,   191,   215,   191,   215,   241,   191,   165,
     217,   482,   221,   166,  1152,   152,  1153,   143,   171,   191,
     173,     5,     6,   176,     8,   701,   319,  1679,   127,   185,
     220,  1191,   218,   191,  1552,  1553,   173,   155,   137,   127,
     994,   995,   160,  1206,   216,   677,   524,   173,   204,   137,
    1142,    34,    36,  1571,  1572,   191,   764,   311,   216,   215,
     314,   143,   671,   193,   158,   118,   119,   197,   700,  1491,
     200,   127,   173,   126,   173,   128,   129,   130,   131,   173,
      63,   137,   152,   136,   158,   215,   126,   152,  1248,   129,
     130,   221,   701,   192,   973,   974,    53,   976,   197,   173,
     218,   127,   220,   173,   192,   581,   127,   191,   173,   585,
     143,   137,   194,   198,   366,   217,   137,   637,   142,  1637,
    1638,   214,   642,   107,   107,   600,   601,   602,   166,   604,
     605,   191,   217,   608,   218,   610,   192,   612,   390,   163,
      97,   762,  1302,   217,   396,   132,   191,   217,   586,   202,
     203,   204,   217,   136,   406,  1631,   390,   833,   218,   183,
     143,   214,   215,   198,   640,  1006,   192,  1643,   644,   163,
     173,   192,   406,   218,   214,   215,   165,   143,   165,   165,
     188,   189,   217,   435,   163,   656,   152,   439,  1142,   183,
     173,   157,   670,   671,   191,   173,   185,   829,   185,   185,
    1267,   435,   188,  1366,   183,   439,   165,   173,   216,   461,
     462,   682,  1688,  1689,   468,  1378,   163,   695,   965,   993,
     698,   218,   164,   217,   833,   208,   185,   478,   482,   191,
     216,   191,   198,   192,   217,   143,   183,   945,   492,   493,
     494,  1135,   184,   990,   152,   877,   878,   194,   191,   191,
     502,   503,  1206,   165,   216,  1209,   727,   191,   218,   891,
     165,    68,    69,    70,    57,   173,  1206,  1221,   502,   503,
      63,   173,   748,   185,   745,   218,   173,   755,   529,   530,
     185,  1221,   216,   173,   174,   175,   191,   199,   920,   921,
     191,   923,   173,  1360,   199,   927,   547,   929,   549,   550,
     551,    33,   553,   110,   111,   112,   113,   165,  1016,  1017,
    1018,    21,    22,   564,   204,   216,   158,   165,   191,   839,
     165,   173,   144,  1584,   146,  1586,   177,   185,    60,    61,
    1397,   173,  1593,   191,   585,   855,    33,   185,   173,   165,
     185,   199,   820,   191,   595,   218,  1607,   195,   600,   601,
     602,  1192,   604,   605,   158,   165,   608,  1236,   610,   185,
     612,   191,   614,    60,    61,   191,   600,   601,   602,   173,
     604,   605,    57,   199,   608,   185,   610,   191,   612,   191,
     614,   191,   857,   191,   822,   158,   177,   862,   218,   199,
     865,   967,   124,   969,   914,   191,   128,   191,  1659,   874,
     173,  1242,   656,   165,   218,   883,   218,   882,   118,   119,
     218,   663,   887,   165,   165,   165,   126,   144,   128,   129,
     130,   131,   218,   185,   218,   677,   136,   124,   682,   191,
    1347,   128,   827,   185,   185,   185,   191,   199,   192,   191,
     191,   191,   913,   677,  1705,  1706,   191,   199,   700,   199,
     191,  1518,    21,    22,   191,   158,  1410,   191,  1125,  1123,
     191,   193,   106,   218,    35,   197,   700,   199,   200,   217,
     173,   216,   950,   727,   725,   165,  1370,   218,    35,   216,
     958,  1742,  1436,   215,   218,   216,   165,   165,   164,   221,
     742,   745,   967,   194,   969,   185,   193,   751,   199,   173,
     197,   191,   199,   200,   214,   215,   185,   185,   184,   199,
     764,    75,   191,   191,   173,    79,   217,   194,   215,  1151,
     199,   199,   199,   194,   221,    47,   173,   195,   199,    93,
      94,   199,   766,   767,    98,    99,   100,   101,   195,   165,
     217,   173,   199,   164,  1176,    67,   217,   173,   986,   118,
     119,   194,   173,   198,     5,     6,   199,   126,   792,   185,
     129,   130,   131,   184,    12,   191,  1407,   136,   216,   165,
     165,   823,   217,  1359,   217,    23,    24,   829,   173,   165,
      57,    22,   198,   817,   835,   198,    63,   392,   184,   185,
     217,   187,   219,   220,   845,   829,  1490,  1455,   184,   185,
     186,   217,    57,   195,   217,   857,  1126,   199,    63,  1241,
     862,    57,   191,   865,  1247,   194,   194,    63,   197,   197,
     173,   184,   874,   857,   187,   877,   878,   190,   862,   195,
     882,   865,   216,   199,   194,   887,  1111,   195,  1247,   891,
     874,   199,   194,   877,   878,   214,   215,  1564,   882,   217,
      57,   194,    57,   887,   197,    33,    63,   891,    63,   913,
     465,  1139,    43,  1642,   219,   220,   185,    57,   920,   921,
    1190,   923,   191,    63,  1653,   927,   217,   929,   930,   177,
     178,  1524,    60,    61,   489,  1639,   920,   921,   185,   923,
    1644,   945,   185,   927,   191,   929,   930,   198,   191,   198,
     185,   185,   953,  1682,  1562,   956,   191,   191,   959,   960,
    1230,  1231,   185,   185,   217,   967,   195,   969,   191,   191,
     199,  1199,    92,    93,   195,   195,  1680,   195,   199,   199,
    1588,   199,   195,   967,   198,   969,   199,   177,   178,   179,
     180,   198,  1220,   144,   145,   146,   124,   185,  1226,   198,
     128,   198,  1499,   173,   174,   175,  1650,  1235,   177,   178,
     179,   198,  1016,  1017,  1018,   198,   198,  1245,  1611,  1612,
    1517,  1247,   198,   198,    33,   177,   178,   179,   447,   448,
     449,    10,    11,    12,   195,   173,   217,   165,   137,   217,
     173,  1423,   216,   173,   599,   218,  1750,  1275,   173,   215,
     165,    60,    61,   198,   198,   217,   198,   185,   217,  1287,
     173,   198,   198,   191,  1292,   193,  1336,   217,   166,   197,
     217,   199,   200,    13,  1344,   198,  1669,   198,   217,   198,
     198,   198,   217,   217,    10,   217,    37,   215,   217,    66,
     217,  1075,   198,   221,   220,   217,   198,    43,  1323,   215,
     173,  1102,  1330,   173,   218,  1106,     5,     6,  1329,  1111,
     191,   216,   194,  1301,   217,   124,   671,   217,   127,   128,
      43,  1123,   217,   198,   198,   191,    25,  1111,   137,   198,
     132,    14,    31,  1361,   192,   194,   166,   220,   217,  1123,
      13,  1523,   191,   191,   216,   173,   701,  1148,     8,  1151,
    1152,   173,   199,   218,   173,   173,   165,   173,  1655,   191,
     217,  1389,  1390,   218,   191,  1393,   218,  1151,  1152,    68,
      69,   184,   217,   217,  1176,   218,   185,  1398,   173,   217,
     217,   198,   217,   192,   193,     1,   198,    67,   197,   173,
      43,   200,  1176,   217,   217,   199,   217,   217,   199,   199,
    1425,  1422,   217,   102,   103,   173,   215,   218,   218,   216,
     218,   217,   221,   768,   769,   770,   771,   772,   773,   774,
     775,   218,   218,   217,   216,   216,   781,   782,  1610,   218,
     173,    33,   787,   198,   173,   217,   173,   173,   173,  1241,
      33,   796,   797,   217,   143,   217,   801,   802,   803,   218,
     805,   217,   173,   217,   217,   217,  1444,  1241,    60,    61,
     216,   160,   161,   162,   163,  1486,   181,    60,    61,   218,
     173,    21,    22,   173,   173,   216,    33,   173,   833,  1507,
     199,    70,   216,   199,   183,   218,   217,   217,   217,   217,
     217,   199,   217,  1514,  1295,   217,   177,   217,   185,   198,
     217,   185,    53,   191,   218,   218,   217,   217,   217,   184,
     217,   216,   218,  1538,   218,   185,   215,   218,   184,    33,
     218,  1323,   124,   216,   191,  1329,   128,   218,   218,   218,
     729,   124,   218,  1554,   127,   128,   218,   216,   216,  1323,
    1568,   218,   218,   217,   137,   216,    60,    61,  1349,   218,
     218,   216,   216,    85,     1,    46,   141,    88,  1598,  1515,
     823,  1515,   992,  1515,  1515,  1366,   116,   117,   118,   119,
     120,  1599,  1515,   123,   124,   125,   126,  1378,   128,   129,
     130,   131,   239,     1,  1001,  1259,   136,  1608,   138,   139,
    1486,   193,  1420,  1446,  1398,   197,  1549,   199,   200,   192,
     193,  1449,  1298,  1404,   197,    56,  1550,   200,  1550,  1644,
     124,   461,   393,   215,   128,   461,  1497,  1218,  1422,   221,
     721,  1423,   215,  1425,    -1,    -1,  1652,    -1,   221,    -1,
      -1,    -1,   437,    -1,    -1,  1663,    -1,    -1,    -1,  1423,
    1666,  1425,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
     392,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,   403,    -1,    -1,    -1,    -1,  1702,    -1,    -1,   193,
     412,    -1,  1486,   197,    -1,   199,   200,    33,    -1,    -1,
     422,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1719,    -1,
     432,   215,    -1,    -1,    -1,    21,    22,   221,    -1,    -1,
    1514,   443,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,  1523,    -1,   455,    -1,    -1,    -1,    -1,    -1,    -1,
    1718,    -1,    -1,   465,    -1,    -1,  1538,    -1,   470,  1523,
     472,    -1,    -1,    -1,   492,   493,   494,    -1,    -1,   481,
    1554,    -1,   118,   119,  1538,    -1,    -1,   489,   490,   491,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   504,    -1,    -1,    -1,  1577,    -1,   124,    -1,
      -1,   513,   128,  1584,    -1,  1586,   518,   519,   520,   521,
     522,    -1,  1593,    -1,  1596,    -1,   544,    -1,    -1,   531,
     116,   117,   118,   119,  1608,    -1,  1607,   539,  1610,    -1,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    12,    -1,
     136,    -1,   138,   139,   572,    -1,  1610,    21,    22,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   214,   215,
     582,   197,    -1,   199,   200,    -1,    -1,    -1,  1659,  1661,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   599,    -1,   215,
      -1,  1672,    -1,    -1,    -1,   221,    -1,  1678,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,   203,   204,    -1,
      -1,    -1,  1247,    -1,    -1,   627,    -1,    -1,   214,   215,
      -1,    -1,    -1,    -1,  1705,  1706,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1714,  1715,  1719,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,   675,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,   671,
      -1,  1742,   136,  1744,   138,   139,   678,   679,    -1,   681,
     144,   145,   146,    -1,   686,   703,   150,    -1,    -1,    -1,
     692,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   701,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     712,   713,   714,   715,   716,   717,    -1,   719,    -1,   721,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,   764,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   768,   769,   770,   771,
     772,   773,   774,   775,   776,   777,    -1,    -1,   780,   781,
     782,   783,   784,   785,    -1,   787,   788,    -1,   790,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   811,
     812,   813,   814,   815,   816,    -1,   818,   835,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   833,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,   869,    -1,    -1,
     136,    -1,   138,   139,   876,    -1,    -1,    -1,   144,    -1,
     146,    -1,   884,    -1,   886,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   895,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,   908,   909,   910,   911,
     912,   124,    -1,    -1,    -1,   128,    -1,   935,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   945,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   769,
     770,   771,   772,   773,   774,   775,   948,   949,   214,   215,
      -1,   781,   782,    -1,    -1,    -1,    -1,   787,    -1,    -1,
     962,    -1,    -1,   965,    -1,    -1,   796,   797,    -1,   971,
      -1,   801,   802,   803,    -1,   805,   978,    -1,    -1,    -1,
     193,    -1,    -1,    -1,   197,   987,   199,   200,   990,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1016,  1017,
    1018,    -1,   215,  1021,    -1,  1023,    -1,  1025,   221,  1027,
      -1,  1029,    -1,  1031,    -1,  1033,    -1,  1035,    -1,    -1,
      -1,    -1,  1040,    -1,  1042,    -1,    -1,    -1,    -1,    -1,
    1048,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1060,    -1,  1062,    -1,    13,    21,    22,  1067,
      -1,  1069,    19,  1071,    -1,    -1,  1074,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,  1088,    -1,  1106,    -1,
    1092,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   143,  1179,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1201,
    1202,  1203,    21,    22,    -1,  1207,   173,    60,    61,    -1,
      -1,    -1,    -1,  1215,    -1,    -1,  1218,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,  1239,  1240,    -1,
     214,   215,    -1,    -1,    -1,  1247,    -1,    -1,    -1,    -1,
      -1,  1253,   219,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1269,    -1,  1271,
      -1,   124,    -1,    -1,  1276,   128,    -1,    -1,    -1,    -1,
    1282,    -1,    -1,    -1,  1286,    60,    61,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,   137,   138,
     139,   140,   141,    -1,  1326,   144,   145,   146,   147,   148,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,  1359,    -1,    -1,   197,  1347,   199,   200,    -1,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,   221,    -1,
      -1,    -1,    -1,   192,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,   214,   215,    -1,    -1,  1401,
     219,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
      60,    61,   197,    -1,   199,   200,    -1,    21,    22,  1431,
      -1,  1433,    -1,    -1,    -1,    -1,    -1,  1439,    -1,    -1,
     215,    -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1465,    -1,  1467,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,  1477,  1478,  1479,    -1,    -1,
      -1,    -1,  1484,    -1,   124,    -1,    -1,  1489,   128,    -1,
      -1,  1493,  1494,  1495,  1496,    -1,    -1,  1499,  1500,    -1,
      60,    61,    -1,    -1,  1506,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1515,    -1,  1517,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,  1529,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,   193,    -1,    -1,   150,   197,    -1,   199,
     200,    -1,  1564,    -1,   124,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   215,  1578,  1579,    -1,    -1,
      -1,   221,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1595,    -1,    -1,    -1,    -1,  1600,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,  1613,    -1,  1615,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,   219,   220,    -1,    -1,    -1,
      -1,    -1,  1634,   193,  1636,    -1,    -1,   197,    -1,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1655,  1656,   215,    -1,    -1,    -1,    -1,
      -1,   221,  1664,    -1,    -1,    -1,     1,    -1,  1670,  1671,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,  1697,    31,  1699,  1700,    -1,
      -1,    -1,    -1,    38,    39,    -1,  1708,    42,    -1,    44,
      45,  1713,    -1,    48,    -1,    50,    51,    52,    -1,    54,
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
     198,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,    -1,     5,     6,   215,    -1,   217,
      -1,   219,   220,    13,    -1,    15,    16,    17,    18,    19,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    13,    -1,
      -1,    -1,    -1,   183,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    41,   207,   208,    -1,
      -1,    -1,    -1,    -1,    49,   215,    -1,   217,    -1,   219,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
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
      -1,    -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,
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
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
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
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    38,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,   216,    -1,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,    -1,    -1,    -1,    21,    22,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,   150,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   163,    -1,   207,   208,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    21,
      22,   116,   117,   118,   119,   120,   214,   215,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,   214,
     215,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      21,    22,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,   165,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,   142,    -1,
     144,   145,   146,   185,    -1,    -1,   150,    21,    22,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     214,   215,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,   142,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      21,    22,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    21,    22,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     214,   215,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      21,    22,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    21,    22,    -1,
      -1,    -1,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    21,    22,   116,   117,   118,   119,   120,
     214,   215,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      21,    22,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    21,    22,   116,   117,
     118,   119,   120,   214,   215,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
     214,   215,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,
     198,   199,   200,   201,   202,   203,   204,    19,    -1,    -1,
      -1,   116,   117,   118,   119,   120,   214,   215,   123,    -1,
      -1,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    71,
      72,    73,    19,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    93,    94,    95,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   214,
     215,    -1,    -1,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,   158,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,   173,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,   173,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    35,    -1,
      98,    99,   100,   101,    -1,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,    71,    -1,    73,    -1,    75,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
     158,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   225,   228,
     237,   239,   240,   245,   276,   280,   316,   399,   406,   410,
     417,   463,   468,   473,    19,    20,   173,   268,   269,   270,
     166,   246,   247,   173,   174,   175,   204,   241,   242,   243,
     163,   183,   285,   407,   173,   219,   227,    57,    63,   402,
     402,   402,   143,   173,   302,    34,    63,   107,   136,   208,
     217,   272,   273,   274,   275,   302,   224,     5,     6,     8,
      36,   107,   414,    62,   397,   192,   191,   194,   191,   242,
      22,    57,   203,   214,   244,   402,   403,   405,   403,   397,
     474,   464,   469,   173,   143,   238,   274,   274,   274,   217,
     144,   145,   146,   191,   216,    57,    63,   281,   283,    57,
      63,   408,     5,     6,    57,    63,   415,    57,    63,   398,
      15,    16,   166,   171,   173,   176,   217,   230,   269,   166,
     247,   173,   241,   241,   173,   224,   164,   184,   286,   403,
     224,    57,    63,   226,   173,   173,   173,   173,   177,   236,
     218,   270,   274,   274,   274,   274,   284,   173,   409,   418,
     285,   400,   177,   178,   179,   229,    15,    16,   166,   171,
     173,   230,   266,   267,   244,   404,   165,   185,   373,   224,
     475,   465,   470,   177,   218,    35,    71,    73,    75,    76,
      77,    78,    79,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    93,    94,    95,    98,    99,   100,
     101,   118,   119,   173,   279,   282,   194,   285,   106,   412,
     413,   395,   160,   220,   271,   367,   177,   178,   179,   191,
     218,   192,   373,   285,   285,   285,    21,    22,    38,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   128,   129,   130,
     137,   138,   139,   140,   141,   144,   145,   146,   147,   148,
     149,   150,   193,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   207,   208,   214,   215,    35,    35,   217,
     277,   285,   287,    75,    79,    93,    94,    98,    99,   100,
     101,   422,   401,   173,   419,   286,   396,   270,   269,   220,
     224,   152,   173,   393,   394,   266,    19,    25,    31,    41,
      49,    64,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   156,   219,   302,   421,   423,
     424,   428,   434,   462,   427,   466,   471,   173,   173,   173,
     216,    22,   173,   216,   155,   218,   367,   378,   379,   380,
     194,   278,   290,   285,   173,   286,   194,   411,   285,   416,
     367,   216,   269,   217,    43,   191,   194,   197,   392,   198,
     198,   198,   217,   198,   198,   217,   198,   198,   198,   198,
     198,   198,   217,   302,    33,    60,    61,   124,   128,   193,
     197,   200,   215,   221,   433,   195,   165,   173,   426,   383,
     386,   173,   137,   217,     7,    50,   315,   185,   185,   191,
     218,   462,     1,     5,     6,     9,    10,    11,    13,    15,
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
     357,   358,   359,   366,   421,   476,   491,   502,   506,   519,
     522,   401,   192,   373,   302,   374,   394,   216,    65,   104,
     174,   296,   359,   173,   173,   434,   127,   137,   192,   391,
     435,   440,   442,   359,   444,   438,   173,   429,   446,   448,
     450,   452,   454,   456,   458,   460,   359,   198,   217,    33,
     197,    33,   197,   215,   221,   216,   359,   215,   221,   434,
     165,   426,   191,   286,   173,   191,   224,   381,   431,   462,
     467,   173,   384,   431,   472,   359,   152,   173,   388,   389,
     420,   380,   380,   380,   198,   198,   359,   259,   260,   198,
     304,   423,   476,   217,   302,   198,     5,   102,   103,   198,
     217,   127,   301,   332,   343,   359,   287,   198,   217,    61,
     359,   217,   359,   173,   198,   198,   217,   224,   198,   166,
      58,   359,   217,   287,   198,   217,   198,   198,   217,   198,
     198,   127,   301,   359,   359,   359,   220,   287,   334,   338,
     338,   338,   217,   217,   217,   217,   217,   217,    13,   434,
      13,   434,    13,   359,   501,   517,   198,   359,   198,   234,
      13,   359,   359,   359,   359,   359,    13,    49,   292,   332,
     359,   332,   220,   224,   224,   359,    10,    13,   294,   501,
     518,    37,   334,   340,   173,   217,   224,   224,   224,   224,
     224,    66,   317,   276,   132,   224,    21,    22,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   136,   137,   138,   139,   140,   141,   144,   145,
     146,   147,   148,   149,   150,   192,   193,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   214,   215,   340,
     217,   286,   359,   215,   224,   276,   286,   375,   373,   198,
     218,    43,   224,   391,   301,   359,   462,   462,   432,   462,
     218,   462,   462,   218,   173,   425,   462,   277,   462,   277,
     462,   277,   381,   382,   384,   385,   218,   437,   292,   216,
     216,   359,   194,   431,   286,   194,   431,   286,   218,   217,
      43,   127,   191,   192,   194,   197,   387,   492,   494,   287,
     420,   217,   305,   217,   302,   198,   217,   329,   198,   198,
     198,   513,   332,   301,   332,   191,   108,   109,   110,   111,
     112,   113,   114,   115,   121,   122,   127,   140,   141,   147,
     148,   149,   192,    14,   434,   294,   359,   359,   287,   192,
     322,   324,   359,   326,   194,   166,   359,   515,   332,   498,
     503,   332,   496,   434,   301,   359,   220,   276,   359,   359,
     359,   359,   359,   359,   420,    53,   158,   173,   200,   215,
     217,   359,   477,   480,   484,   500,   505,   420,   217,   480,
     505,   420,   142,   184,   186,   224,   485,   297,   287,   299,
     179,   180,   229,   217,   217,   420,    13,   216,   191,   521,
     521,   152,   157,   198,   302,   348,   287,   257,   420,   286,
     191,   521,   285,   341,    70,   215,   218,   332,   477,   479,
     160,   217,   319,   394,     4,   160,   337,   338,    19,   158,
     173,   421,    19,   158,   173,   421,   133,   134,   135,   288,
     344,   359,   344,   359,   344,   359,   344,   359,   344,   359,
     344,   359,   344,   359,   344,   359,   359,   359,   359,   344,
     359,   344,   359,   359,   359,   359,   173,   344,   359,   359,
     158,   173,   359,   359,   359,   421,   359,   359,   359,   344,
     359,   344,   359,   359,   359,   359,   344,   359,   344,   359,
     344,   359,   359,   344,   359,    22,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   129,   130,   158,
     173,   214,   215,   355,   421,   359,   218,   332,   373,   275,
       8,   367,   372,   434,   173,   301,   359,   224,   199,   199,
     199,   431,   199,   199,   199,   224,   199,   278,   199,   278,
     199,   278,   199,   431,   199,   431,   295,   462,   218,   216,
     462,   462,   359,   173,   173,   462,   359,   434,   434,    20,
     420,   462,    70,   332,   479,   490,   198,   359,   173,   359,
     462,   507,   509,   511,   434,   521,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   287,   199,   431,   218,   218,   265,
     434,   434,   218,   434,   218,   434,   521,   434,   382,   521,
     385,   199,   337,   218,   218,   218,   218,   218,   218,    20,
     338,   217,   137,   387,   215,   359,   218,   142,   191,   224,
     484,   188,   189,   216,   488,   191,   188,   216,   224,   487,
      20,   218,   484,   184,   187,   486,    20,   359,   184,   501,
     295,   295,   359,   420,   420,    20,   217,   420,   218,   217,
     217,   350,   352,   373,   359,    20,   501,   290,   276,   173,
     218,   479,   477,   191,   218,   218,   173,   318,   318,   217,
     127,   137,   173,   192,   197,   335,   336,   277,   198,   217,
     198,   217,   338,   338,   338,   217,   217,   216,    19,   158,
     173,   421,   194,   158,   173,   359,   217,   217,   158,   173,
     359,     1,   217,   216,   218,   216,    57,    63,   370,    67,
     371,   224,   199,   224,   436,   441,   443,   462,   445,   439,
     430,   173,   447,   199,   451,   199,   455,   199,   459,   381,
     461,   384,   199,   431,   218,    43,   387,   199,   199,   332,
      20,   199,   479,   218,   218,   218,   173,   218,   199,   224,
     218,   199,   434,   382,   385,   199,   218,   217,   434,   359,
     199,   199,   199,   199,   218,   199,   199,   218,   199,   337,
     277,   217,   332,   359,   359,   359,   480,   484,   359,   158,
     173,   477,   488,   216,   359,   216,   500,   332,   480,   184,
     187,   190,   489,   216,   332,   199,   199,   194,   232,    20,
      20,   332,   420,    20,   359,   359,   434,   277,    12,    23,
      24,   250,   251,    12,   253,   332,   286,   337,   218,   216,
     215,   191,   216,   218,   336,   173,   173,   217,   173,   173,
     191,   216,   278,   360,   359,   362,   359,   218,   332,   359,
     198,   217,   359,   217,   216,   359,   215,   218,   332,   217,
     216,   356,   218,   332,   224,    47,   371,    46,   106,   368,
     373,   337,   449,   453,   457,   217,   462,   173,   359,   493,
     495,   287,   332,   306,   218,   199,   431,   217,   173,   330,
     199,   199,   199,   514,   294,   199,   224,   323,   325,   327,
     516,   499,   504,   497,   217,   340,   278,   218,   332,   185,
     218,   484,   488,   217,   137,   387,   185,   484,   216,   185,
     298,   300,   233,   181,   332,   332,   185,    20,   332,   218,
     218,   199,   278,   287,   359,   254,   224,   185,   277,   218,
     477,   173,   216,   194,   392,   218,   173,   335,   216,   142,
     287,   333,   434,   218,   462,   218,   218,   218,   364,   359,
     359,   218,   477,   218,   359,   218,   373,    33,   369,   368,
     370,   292,   217,   217,   218,   359,   173,   359,   199,   508,
     510,   512,   217,   218,   217,   359,   359,   359,   217,    70,
     490,   217,   217,   218,   359,   333,   218,   359,   137,   387,
     488,   359,   359,   359,   359,   489,   501,   359,   217,   293,
     231,   218,   218,   359,   332,   185,   351,   199,   287,    26,
     105,   255,   308,   309,   310,   312,   359,   501,   278,   216,
     194,   392,   434,   391,   218,   127,   359,   199,   199,   462,
     218,   218,   216,   218,   376,   369,   388,   389,   390,   218,
     490,   490,   287,   218,   199,   218,   217,   217,   217,   217,
     292,   294,   332,   490,   490,   218,   224,   520,   359,   359,
     218,   520,   520,   292,   177,   185,   185,   520,   218,   359,
     348,   353,   251,   127,   127,   359,   520,   287,   218,   434,
     391,   391,   359,   359,   361,   363,   199,   218,   282,   377,
     217,   477,   481,   482,   483,   483,   359,   359,   490,   490,
     477,   478,   218,   218,   521,   483,   478,    53,   216,   137,
     387,   184,   286,   191,   521,   501,   359,   216,   185,   520,
     348,   359,   286,   391,   359,   359,   224,   365,   224,   282,
     477,   191,   521,   218,   218,   218,   218,   483,   483,   218,
     218,   218,   218,   359,   216,   359,   359,   216,   286,   218,
     520,   520,   359,   216,   359,   224,   224,   373,   287,   218,
     217,   218,   218,   184,   216,   520,   224,   477,   216,   218
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
     359,   359,   359,   359,   360,   361,   359,   359,   359,   359,
     362,   363,   359,   359,   359,   364,   365,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   367,   367,
     367,   368,   368,   368,   369,   369,   370,   370,   370,   371,
     371,   372,   373,   373,   373,   374,   374,   375,   374,   376,
     374,   377,   374,   374,   378,   379,   379,   380,   380,   380,
     380,   380,   381,   381,   382,   382,   383,   383,   383,   384,
     385,   385,   386,   386,   386,   387,   387,   388,   388,   388,
     389,   389,   390,   390,   391,   391,   391,   392,   392,   393,
     393,   393,   393,   393,   394,   394,   394,   394,   394,   395,
     396,   395,   397,   397,   398,   398,   398,   399,   400,   399,
     401,   401,   401,   402,   402,   402,   404,   403,   405,   405,
     406,   407,   406,   408,   408,   408,   409,   410,   410,   411,
     411,   412,   412,   413,   414,   414,   414,   414,   415,   415,
     415,   416,   416,   418,   419,   417,   420,   420,   420,   420,
     420,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   422,   422,
     422,   422,   422,   422,   422,   422,   423,   424,   424,   424,
     425,   425,   426,   426,   427,   427,   427,   427,   429,   430,
     428,   431,   431,   432,   432,   433,   433,   434,   434,   434,
     434,   434,   434,   435,   436,   434,   434,   434,   437,   434,
     434,   434,   434,   434,   434,   434,   434,   434,   434,   434,
     434,   434,   438,   439,   434,   434,   440,   441,   434,   442,
     443,   434,   444,   445,   434,   434,   446,   447,   434,   448,
     449,   434,   434,   450,   451,   434,   452,   453,   434,   434,
     454,   455,   434,   456,   457,   434,   458,   459,   434,   460,
     461,   434,   462,   462,   462,   464,   465,   466,   467,   463,
     469,   470,   471,   472,   468,   474,   475,   473,   476,   476,
     476,   476,   476,   477,   477,   477,   477,   477,   477,   477,
     477,   478,   478,   479,   480,   480,   481,   481,   482,   482,
     483,   483,   484,   484,   485,   485,   486,   486,   487,   487,
     488,   488,   488,   489,   489,   489,   490,   490,   491,   491,
     491,   491,   491,   491,   492,   493,   491,   494,   495,   491,
     496,   497,   491,   498,   499,   491,   500,   500,   500,   501,
     501,   502,   503,   504,   502,   505,   505,   506,   506,   506,
     507,   508,   506,   509,   510,   506,   511,   512,   506,   506,
     513,   514,   506,   506,   515,   516,   506,   517,   517,   518,
     518,   519,   519,   519,   519,   519,   520,   520,   521,   521,
     522,   522,   522,   522,   522,   522,   522,   522,   522
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
       1,     1,     1,     1,     1,     1,     4,     1,     1,     4,
       4,     7,     8,     3,     0,     0,     8,     3,     3,     3,
       0,     0,     8,     3,     4,     0,     0,     9,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     2,
       4,     1,     4,     4,     4,     4,     4,     1,     6,     7,
       6,     6,     7,     7,     6,     7,     6,     6,     0,     4,
       1,     0,     1,     1,     0,     1,     0,     1,     1,     0,
       1,     5,     0,     1,     1,     0,     2,     0,     5,     0,
      10,     0,    10,     6,     3,     3,     4,     1,     1,     3,
       3,     3,     1,     3,     1,     3,     0,     2,     3,     3,
       1,     3,     0,     2,     3,     1,     1,     1,     2,     3,
       3,     5,     1,     1,     1,     1,     1,     0,     1,     1,
       4,     3,     3,     5,     4,     6,     5,     5,     4,     0,
       0,     5,     0,     1,     0,     1,     1,     6,     0,     6,
       0,     3,     5,     0,     1,     1,     0,     5,     3,     4,
       4,     0,     4,     0,     1,     1,     1,     7,     9,     0,
       2,     0,     1,     3,     1,     1,     2,     2,     0,     1,
       1,     0,     3,     0,     0,     7,     1,     4,     3,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     0,     2,     0,     2,     3,     3,     0,     0,
       6,     1,     1,     1,     3,     3,     4,     1,     1,     1,
       1,     2,     3,     0,     0,     6,     4,     5,     0,     9,
       4,     2,     2,     3,     2,     3,     2,     2,     3,     3,
       3,     2,     0,     0,     6,     2,     0,     0,     6,     0,
       0,     6,     0,     0,     6,     1,     0,     0,     6,     0,
       0,     7,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     0,     0,     6,     0,
       0,     6,     1,     3,     3,     0,     0,     0,     0,    10,
       0,     0,     0,     0,    10,     0,     0,     9,     1,     1,
       1,     1,     1,     3,     3,     5,     5,     6,     6,     8,
       8,     0,     1,     1,     1,     3,     3,     5,     1,     2,
       1,     0,     0,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     2,     1,     1,     0,     1,     5,     4,
       6,     7,     5,     7,     0,     0,    10,     0,     0,    10,
       0,     0,    10,     0,     0,     7,     1,     3,     3,     3,
       1,     5,     0,     0,    10,     1,     3,     3,     4,     4,
       0,     0,    11,     0,     0,    11,     0,     0,    10,     5,
       0,     0,     9,     5,     0,     0,    10,     1,     3,     1,
       3,     3,     3,     4,     7,     9,     0,     3,     0,     1,
       9,    11,    12,    11,    10,    10,    10,     9,    10
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

  case 347: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 348: /* expr_list: expr_list ',' expr  */
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
            format::get_writer() << format::substring_between(tokAt(scanner, (yylsp[-3])), tokAt(scanner, (yylsp[-2])))
                      << "capture(" << format::get_substring(tokAt(scanner, (yylsp[-2]))) << ")"
                      << format::substring_between(tokAt(scanner, (yylsp[-2])), tokAt(scanner, (yylsp[-1])));
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

  case 422: /* func_addr_name: "$i" '(' expr ')'  */
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

  case 450: /* expr_not_wrapped: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"!",(yyvsp[0].pExpression)); }
    break;

  case 451: /* expr_not_wrapped: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"~",(yyvsp[0].pExpression)); }
    break;

  case 452: /* expr_not_wrapped: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+",(yyvsp[0].pExpression)); }
    break;

  case 453: /* expr_not_wrapped: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"-",(yyvsp[0].pExpression)); }
    break;

  case 454: /* expr_not_wrapped: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 455: /* expr_not_wrapped: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 456: /* expr_not_wrapped: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 457: /* expr_not_wrapped: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 458: /* expr_not_wrapped: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 459: /* expr_not_wrapped: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 460: /* expr_not_wrapped: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 461: /* expr_not_wrapped: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr_not_wrapped: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr_not_wrapped: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr_not_wrapped: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr_not_wrapped: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr_not_wrapped: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr_not_wrapped: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr_not_wrapped: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_not_wrapped: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_not_wrapped: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_not_wrapped: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_not_wrapped: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr_not_wrapped: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr_not_wrapped: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr_not_wrapped: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 476: /* expr_not_wrapped: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"++", (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr_not_wrapped: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"--", (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr_not_wrapped: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 479: /* expr_not_wrapped: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 480: /* expr_not_wrapped: expr '[' expr ']'  */
                                                      { (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 481: /* expr_not_wrapped: expr '.' '[' expr ']'  */
                                                          { (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 482: /* expr_not_wrapped: expr "?[" expr ']'  */
                                                      { (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 483: /* expr_not_wrapped: expr '.' "?[" expr ']'  */
                                                          { (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 484: /* expr_not_wrapped: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 485: /* expr_not_wrapped: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 486: /* expr_not_wrapped: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 487: /* expr_not_wrapped: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 488: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 489: /* expr: expr_not_wrapped  */
                                {
        // Сheck next token.
        // If it's not ')', then we should try wrap it as multiple line expression.
        if (yychar != ')') {
            format::wrap_par_expr_newline(tokAt(scanner,(yylsp[0])), (yyvsp[0].pExpression)->at);
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 490: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 491: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 492: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 493: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 494: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 495: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 496: /* expr: '(' expr_list optional_comma ')'  */
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

  case 497: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 498: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 499: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 500: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 501: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 502: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 503: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 504: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 505: /* $@31: %empty  */
                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 506: /* expr: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 507: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 508: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 509: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* $@32: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 511: /* $@33: %empty  */
                                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 512: /* expr: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                 {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 513: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 514: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 515: /* $@34: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 516: /* $@35: %empty  */
                                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 517: /* expr: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                     {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-8]), (yylsp[-1])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 518: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 519: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 520: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 521: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 522: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 523: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); (yyval.pExpression)->at = tokAt(scanner, (yylsp[0])); }
    break;

  case 524: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 525: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 526: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); }
    break;

  case 527: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]), (yylsp[0]))); (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])); }
    break;

  case 528: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
    }
    break;

  case 529: /* expr: name_in_namespace "name"  */
                                                  {
        if (format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << "." << format::get_substring(tokAt(scanner,(yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 530: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 531: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 532: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 533: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 534: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 535: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 536: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 537: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 538: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 539: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 540: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 541: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 542: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 543: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 544: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 545: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 546: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 547: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 548: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 549: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
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

  case 550: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 551: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 552: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 553: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 554: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 555: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 556: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 557: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 558: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 559: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 560: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 561: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 565: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 566: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 567: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 568: /* struct_variable_declaration_list: struct_variable_declaration_list $@36 structure_variable_declaration semicolon opt_sem  */
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

  case 569: /* $@37: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 570: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@37 function_declaration_header semicolon opt_sem  */
                                                                  {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-6].b),(yyvsp[-4].b), (yyvsp[-2].pFuncDecl));
            }
    break;

  case 571: /* $@38: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 572: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@38 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 573: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon opt_sem  */
                                                                                               {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-3])), CompilationError::syntax_error);
        delete (yyvsp[-3].faList);
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
    }
    break;

  case 574: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 575: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 576: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 577: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 578: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 579: /* function_argument_list: function_argument_declaration_no_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 580: /* function_argument_list: function_argument_declaration_type "end of expression" function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 581: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 582: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 583: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 584: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 585: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 586: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 587: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 588: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 589: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 590: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 591: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 592: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 593: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 594: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 595: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 596: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 597: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 598: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 599: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 600: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 601: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 602: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 603: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 604: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 605: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 606: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 607: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 608: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 609: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 610: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 611: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 612: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 613: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 614: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 615: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 616: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 617: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 618: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 619: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 620: /* $@39: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 621: /* global_variable_declaration_list: global_variable_declaration_list $@39 optional_field_annotation let_variable_declaration opt_sem  */
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

  case 622: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 623: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 624: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 625: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 626: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 627: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                                   {
        handle_brace(format::Pos::from(tokAt(scanner, (yylsp[-2]))), (yyvsp[-2].ui),
                     format::get_substring(tokRangeAt(scanner, (yylsp[-2]), (yylsp[0]))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 628: /* $@40: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 629: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@40 optional_field_annotation let_variable_declaration  */
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

  case 630: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 631: /* enum_list: enum_list "name" opt_sem  */
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

  case 632: /* enum_list: enum_list "name" '=' expr opt_sem  */
                                                         {
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

  case 633: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 634: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 635: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 636: /* $@41: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 637: /* single_alias: optional_public_or_private_alias "name" $@41 '=' type_declaration  */
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

  case 638: /* alias_list: single_alias semicolon opt_sem  */
                                                   {
        (yyval.positions) = new vector<LineInfo>(1, tokAt(scanner, (yylsp[-2])));
    }
    break;

  case 639: /* alias_list: alias_list single_alias semicolon opt_sem  */
                                                                      {
        (yyvsp[-3].positions)->emplace_back(tokAt(scanner, (yylsp[-2])));
        (yyval.positions) = (yyvsp[-3].positions);
    }
    break;

  case 640: /* alias_declaration: "typedef" open_block alias_list close_block  */
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

  case 641: /* $@42: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 643: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 644: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 645: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 646: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 647: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block enum_list close_block  */
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

  case 648: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block enum_list close_block  */
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

  case 649: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 650: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 651: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 652: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 653: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 654: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 655: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 656: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 657: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 658: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 659: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 660: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 661: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 662: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                                      {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 663: /* $@43: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 664: /* $@44: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 665: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@43 structure_name $@44 optional_struct_variable_declaration_list  */
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

  case 666: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 667: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 668: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 669: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 670: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 671: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 672: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 673: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 674: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 675: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 676: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 677: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 678: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 679: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 680: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 681: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 682: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 683: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 684: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 685: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 686: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 687: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 688: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 689: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 690: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 691: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 692: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 693: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 694: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 695: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 696: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 697: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 698: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 699: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 700: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 701: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 702: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 703: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 704: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 705: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 706: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 707: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 708: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 709: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 710: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 711: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 714: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 715: /* bitfield_alias_bits: bitfield_alias_bits SEMICOLON  */
                                            {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 716: /* bitfield_alias_bits: bitfield_alias_bits "name" SEMICOLON  */
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

  case 717: /* bitfield_alias_bits: bitfield_alias_bits "name" commas  */
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

  case 718: /* $@45: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 719: /* $@46: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 720: /* bitfield_type_declaration: "bitfield" '<' $@45 bitfield_bits '>' $@46  */
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

  case 723: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 724: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 725: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 726: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 727: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 728: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 729: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 730: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 731: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 732: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 733: /* $@47: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@48: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options: "type" '<' $@47 type_declaration '>' $@48  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 736: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 737: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 738: /* $@49: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 739: /* type_declaration_no_options: '$' name_in_namespace '<' $@49 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 740: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 741: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 742: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 743: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 744: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 745: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 746: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 747: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 748: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 749: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 750: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 751: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 752: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 753: /* $@51: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 754: /* type_declaration_no_options: "smart_ptr" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 755: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 756: /* $@52: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 757: /* $@53: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 758: /* type_declaration_no_options: "array" '<' $@52 type_declaration '>' $@53  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 759: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 760: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 761: /* type_declaration_no_options: "table" '<' $@54 table_type_pair '>' $@55  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 762: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 763: /* $@57: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 764: /* type_declaration_no_options: "iterator" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 765: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 766: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 767: /* $@59: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 768: /* type_declaration_no_options: "block" '<' $@58 type_declaration '>' $@59  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 769: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 770: /* $@61: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 771: /* type_declaration_no_options: "block" '<' $@60 optional_function_argument_list optional_function_type '>' $@61  */
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

  case 772: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 773: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 774: /* $@63: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 775: /* type_declaration_no_options: "function" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 776: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 777: /* $@65: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 778: /* type_declaration_no_options: "function" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
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

  case 779: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 780: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 782: /* type_declaration_no_options: "lambda" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 783: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 785: /* type_declaration_no_options: "lambda" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 786: /* $@70: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 787: /* $@71: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 788: /* type_declaration_no_options: "tuple" '<' $@70 tuple_type_list '>' $@71  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 789: /* $@72: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 790: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 791: /* type_declaration_no_options: "variant" '<' $@72 variant_type_list '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 792: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 793: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 794: /* type_declaration: type_declaration '|' '#'  */
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

  case 795: /* $@74: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 796: /* $@75: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 797: /* $@76: %empty  */
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

  case 798: /* $@77: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 799: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@74 "name" $@75 open_block $@76 tuple_alias_type_list $@77 close_block  */
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

  case 800: /* $@78: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 801: /* $@79: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 802: /* $@80: %empty  */
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

  case 803: /* $@81: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 804: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@78 "name" $@79 open_block $@80 variant_alias_type_list $@81 close_block  */
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

  case 805: /* $@82: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 806: /* $@83: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 807: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@82 "name" $@83 open_block bitfield_alias_bits commas close_block  */
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

  case 808: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 809: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 810: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 811: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 812: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 813: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 814: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 815: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 816: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 817: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 818: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner, (yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 819: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                                       {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 820: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                              {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 821: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 822: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 823: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 824: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 825: /* make_struct_dim: make_struct_dim semicolon make_struct_fields  */
                                                               {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 826: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 827: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 828: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 829: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 830: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 831: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 832: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 833: /* optional_block: "where" expr_block  */
                                  { (yyvsp[0].pExpression)->at = tokAt(scanner, (yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 846: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 847: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 848: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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
                    format::get_writer() << before << "fixed_array(" << internal << ")";
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

  case 849: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
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
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 850: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
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
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 851: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 852: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 853: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 854: /* $@84: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 855: /* $@85: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 856: /* make_struct_decl: "struct" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 857: /* $@86: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 858: /* $@87: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 859: /* make_struct_decl: "class" '<' $@86 type_declaration_no_options '>' $@87 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 860: /* $@88: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 861: /* $@89: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 862: /* make_struct_decl: "variant" '<' $@88 variant_type_list '>' $@89 '(' use_initializer make_variant_dim ')'  */
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

  case 863: /* $@90: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 864: /* $@91: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 865: /* make_struct_decl: "default" '<' $@90 type_declaration_no_options '>' $@91 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 866: /* make_tuple: expr  */
                  {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 867: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 868: /* make_tuple: make_tuple ',' expr  */
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

  case 869: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 870: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 871: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 872: /* $@92: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 873: /* $@93: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 874: /* make_tuple_call: "tuple" '<' $@92 tuple_type_list '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 875: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 876: /* make_dim: make_dim semicolon make_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 877: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 878: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
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
                if (type_name.value_or("").find('[') != size_t(-1)) {
                    format::get_writer() << "fixed_array(" << internal << ")";
                } else {
                    format::get_writer() << internal;
                }
            } else {
                format::get_writer() << "fixed_array";
                if (!(yyvsp[-2].pTypeDecl)->isAuto()) {
                    format::get_writer() << "<" << type_name.value().substr(0, type_name.value().find('[')) << ">";
                }
                format::get_writer() << "(" << internal << ")";
            }
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 879: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
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

        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 880: /* $@94: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 881: /* $@95: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 882: /* make_dim_decl: "array" "struct" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 883: /* $@96: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@97: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 885: /* make_dim_decl: "array" "tuple" '<' $@96 tuple_type_list '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 886: /* $@98: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@99: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 888: /* make_dim_decl: "array" "variant" '<' $@98 variant_type_list '>' $@99 '(' make_variant_dim ')'  */
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

  case 889: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 890: /* $@100: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 891: /* $@101: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 892: /* make_dim_decl: "array" '<' $@100 type_declaration_no_options '>' $@101 '(' optional_expr_list ')'  */
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

  case 893: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 894: /* $@102: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 895: /* $@103: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 896: /* make_dim_decl: "fixed_array" '<' $@102 type_declaration_no_options '>' $@103 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 897: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 898: /* make_table: make_table semicolon make_map_tuple  */
                                                      {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 899: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 900: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 901: /* make_table_decl: open_block optional_expr_map_tuple_list close_block  */
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

  case 902: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 903: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 904: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 905: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 906: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 907: /* array_comprehension_where: semicolon "where" expr  */
                                          { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 908: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 909: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 910: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 911: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                           {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 912: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                        {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 913: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 914: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 915: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
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

  case 916: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
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

  case 917: /* array_comprehension: open_block "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block  */
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

  case 918: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block close_block  */
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


