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
  YYSYMBOL_359_30 = 359,                   /* $@30  */
  YYSYMBOL_360_31 = 360,                   /* $@31  */
  YYSYMBOL_361_32 = 361,                   /* $@32  */
  YYSYMBOL_362_33 = 362,                   /* $@33  */
  YYSYMBOL_363_34 = 363,                   /* $@34  */
  YYSYMBOL_364_35 = 364,                   /* $@35  */
  YYSYMBOL_expr = 365,                     /* expr  */
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
  YYSYMBOL_function_argument_declaration = 378, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 379,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 380,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 381,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 382,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 383,             /* variant_type  */
  YYSYMBOL_variant_type_list = 384,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 385,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 386,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 387,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 388,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 389,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 390, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 391, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 392, /* global_variable_declaration_list  */
  YYSYMBOL_393_39 = 393,                   /* $@39  */
  YYSYMBOL_optional_shared = 394,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 395, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 396,               /* global_let  */
  YYSYMBOL_397_40 = 397,                   /* $@40  */
  YYSYMBOL_enum_list = 398,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 399, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 400,             /* single_alias  */
  YYSYMBOL_401_41 = 401,                   /* $@41  */
  YYSYMBOL_alias_list = 402,               /* alias_list  */
  YYSYMBOL_alias_declaration = 403,        /* alias_declaration  */
  YYSYMBOL_404_42 = 404,                   /* $@42  */
  YYSYMBOL_optional_public_or_private_enum = 405, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 406,                /* enum_name  */
  YYSYMBOL_enum_declaration = 407,         /* enum_declaration  */
  YYSYMBOL_optional_structure_parent = 408, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 409,          /* optional_sealed  */
  YYSYMBOL_structure_name = 410,           /* structure_name  */
  YYSYMBOL_class_or_struct = 411,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 412, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 413, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 414,    /* structure_declaration  */
  YYSYMBOL_415_43 = 415,                   /* $@43  */
  YYSYMBOL_416_44 = 416,                   /* $@44  */
  YYSYMBOL_variable_name_with_pos_list = 417, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 418,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 419, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 420, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 421,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 422,            /* bitfield_bits  */
  YYSYMBOL_commas = 423,                   /* commas  */
  YYSYMBOL_bitfield_alias_bits = 424,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 425, /* bitfield_type_declaration  */
  YYSYMBOL_426_45 = 426,                   /* $@45  */
  YYSYMBOL_427_46 = 427,                   /* $@46  */
  YYSYMBOL_c_or_s = 428,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 429,          /* table_type_pair  */
  YYSYMBOL_dim_list = 430,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 431, /* type_declaration_no_options  */
  YYSYMBOL_432_47 = 432,                   /* $@47  */
  YYSYMBOL_433_48 = 433,                   /* $@48  */
  YYSYMBOL_434_49 = 434,                   /* $@49  */
  YYSYMBOL_435_50 = 435,                   /* $@50  */
  YYSYMBOL_436_51 = 436,                   /* $@51  */
  YYSYMBOL_437_52 = 437,                   /* $@52  */
  YYSYMBOL_438_53 = 438,                   /* $@53  */
  YYSYMBOL_439_54 = 439,                   /* $@54  */
  YYSYMBOL_440_55 = 440,                   /* $@55  */
  YYSYMBOL_441_56 = 441,                   /* $@56  */
  YYSYMBOL_442_57 = 442,                   /* $@57  */
  YYSYMBOL_443_58 = 443,                   /* $@58  */
  YYSYMBOL_444_59 = 444,                   /* $@59  */
  YYSYMBOL_445_60 = 445,                   /* $@60  */
  YYSYMBOL_446_61 = 446,                   /* $@61  */
  YYSYMBOL_447_62 = 447,                   /* $@62  */
  YYSYMBOL_448_63 = 448,                   /* $@63  */
  YYSYMBOL_449_64 = 449,                   /* $@64  */
  YYSYMBOL_450_65 = 450,                   /* $@65  */
  YYSYMBOL_451_66 = 451,                   /* $@66  */
  YYSYMBOL_452_67 = 452,                   /* $@67  */
  YYSYMBOL_453_68 = 453,                   /* $@68  */
  YYSYMBOL_454_69 = 454,                   /* $@69  */
  YYSYMBOL_455_70 = 455,                   /* $@70  */
  YYSYMBOL_456_71 = 456,                   /* $@71  */
  YYSYMBOL_457_72 = 457,                   /* $@72  */
  YYSYMBOL_458_73 = 458,                   /* $@73  */
  YYSYMBOL_type_declaration = 459,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 460,  /* tuple_alias_declaration  */
  YYSYMBOL_461_74 = 461,                   /* $@74  */
  YYSYMBOL_462_75 = 462,                   /* $@75  */
  YYSYMBOL_463_76 = 463,                   /* $@76  */
  YYSYMBOL_464_77 = 464,                   /* $@77  */
  YYSYMBOL_variant_alias_declaration = 465, /* variant_alias_declaration  */
  YYSYMBOL_466_78 = 466,                   /* $@78  */
  YYSYMBOL_467_79 = 467,                   /* $@79  */
  YYSYMBOL_468_80 = 468,                   /* $@80  */
  YYSYMBOL_469_81 = 469,                   /* $@81  */
  YYSYMBOL_bitfield_alias_declaration = 470, /* bitfield_alias_declaration  */
  YYSYMBOL_471_82 = 471,                   /* $@82  */
  YYSYMBOL_472_83 = 472,                   /* $@83  */
  YYSYMBOL_make_decl = 473,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 474,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 475,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 476,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 477,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 478,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 479,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 480, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 481,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 482, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 483, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 484, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 485, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 486, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 487,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 488,         /* make_struct_decl  */
  YYSYMBOL_489_84 = 489,                   /* $@84  */
  YYSYMBOL_490_85 = 490,                   /* $@85  */
  YYSYMBOL_491_86 = 491,                   /* $@86  */
  YYSYMBOL_492_87 = 492,                   /* $@87  */
  YYSYMBOL_493_88 = 493,                   /* $@88  */
  YYSYMBOL_494_89 = 494,                   /* $@89  */
  YYSYMBOL_495_90 = 495,                   /* $@90  */
  YYSYMBOL_496_91 = 496,                   /* $@91  */
  YYSYMBOL_make_tuple = 497,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 498,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 499,          /* make_tuple_call  */
  YYSYMBOL_500_92 = 500,                   /* $@92  */
  YYSYMBOL_501_93 = 501,                   /* $@93  */
  YYSYMBOL_make_dim = 502,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 503,            /* make_dim_decl  */
  YYSYMBOL_504_94 = 504,                   /* $@94  */
  YYSYMBOL_505_95 = 505,                   /* $@95  */
  YYSYMBOL_506_96 = 506,                   /* $@96  */
  YYSYMBOL_507_97 = 507,                   /* $@97  */
  YYSYMBOL_508_98 = 508,                   /* $@98  */
  YYSYMBOL_509_99 = 509,                   /* $@99  */
  YYSYMBOL_510_100 = 510,                  /* $@100  */
  YYSYMBOL_511_101 = 511,                  /* $@101  */
  YYSYMBOL_512_102 = 512,                  /* $@102  */
  YYSYMBOL_513_103 = 513,                  /* $@103  */
  YYSYMBOL_make_table = 514,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 515,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 516,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 517, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 518,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 519       /* array_comprehension  */
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
#define YYLAST   15295

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  298
/* YYNRULES -- Number of rules.  */
#define YYNRULES  912
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1752

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
       0,   568,   568,   569,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   590,   593,   596,
     602,   603,   604,   608,   609,   613,   631,   632,   633,   634,
     638,   639,   643,   644,   648,   649,   649,   653,   658,   667,
     682,   698,   703,   711,   711,   752,   782,   786,   787,   788,
     792,   795,   799,   805,   814,   817,   823,   824,   828,   832,
     833,   837,   840,   846,   852,   855,   861,   862,   866,   867,
     868,   881,   882,   886,   887,   887,   893,   894,   895,   896,
     897,   901,   911,   911,   919,   919,   923,   923,   932,   940,
     952,   962,   962,   969,   970,   971,   972,   973,   974,   978,
     983,   991,   992,   993,   997,   998,   999,  1000,  1001,  1002,
    1003,  1004,  1010,  1013,  1019,  1022,  1025,  1031,  1032,  1033,
    1034,  1038,  1051,  1069,  1072,  1080,  1091,  1102,  1113,  1116,
    1123,  1127,  1134,  1135,  1139,  1140,  1141,  1145,  1148,  1155,
    1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,
    1179,  1180,  1181,  1182,  1183,  1184,  1185,  1186,  1187,  1188,
    1189,  1190,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,
    1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,  1208,
    1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,
    1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,  1228,
    1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,
    1239,  1240,  1241,  1242,  1247,  1265,  1266,  1267,  1271,  1277,
    1277,  1294,  1295,  1298,  1299,  1302,  1309,  1333,  1351,  1360,
    1366,  1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,
    1376,  1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,
    1386,  1387,  1391,  1396,  1402,  1408,  1419,  1420,  1424,  1425,
    1429,  1430,  1434,  1438,  1445,  1445,  1445,  1451,  1451,  1451,
    1460,  1494,  1502,  1509,  1516,  1522,  1523,  1534,  1538,  1541,
    1549,  1549,  1549,  1552,  1558,  1561,  1565,  1569,  1576,  1583,
    1589,  1593,  1597,  1600,  1603,  1611,  1614,  1617,  1625,  1628,
    1636,  1639,  1642,  1650,  1662,  1663,  1664,  1668,  1669,  1673,
    1674,  1678,  1683,  1691,  1702,  1708,  1723,  1735,  1738,  1744,
    1744,  1744,  1747,  1747,  1747,  1752,  1752,  1752,  1760,  1760,
    1760,  1766,  1780,  1796,  1814,  1824,  1835,  1850,  1854,  1860,
    1861,  1868,  1879,  1880,  1881,  1885,  1886,  1887,  1888,  1889,
    1893,  1898,  1906,  1907,  1917,  1921,  1931,  1938,  1945,  1945,
    1954,  1955,  1956,  1957,  1958,  1959,  1960,  1964,  1965,  1966,
    1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,  1976,
    1977,  1978,  1979,  1980,  1981,  1982,  1986,  1996,  2005,  2014,
    2019,  2020,  2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,
    2029,  2030,  2031,  2032,  2033,  2034,  2035,  2040,  2047,  2059,
    2064,  2074,  2078,  2085,  2088,  2088,  2088,  2093,  2093,  2093,
    2106,  2110,  2114,  2119,  2126,  2135,  2140,  2147,  2147,  2147,
    2154,  2158,  2168,  2177,  2186,  2190,  2193,  2199,  2200,  2201,
    2202,  2203,  2204,  2205,  2206,  2207,  2208,  2209,  2210,  2211,
    2212,  2213,  2214,  2215,  2216,  2217,  2218,  2219,  2220,  2221,
    2222,  2223,  2224,  2225,  2226,  2227,  2228,  2229,  2230,  2231,
    2232,  2233,  2234,  2240,  2241,  2242,  2243,  2244,  2258,  2259,
    2260,  2261,  2262,  2263,  2264,  2265,  2266,  2267,  2268,  2269,
    2272,  2275,  2276,  2279,  2279,  2279,  2282,  2287,  2291,  2295,
    2295,  2295,  2300,  2303,  2307,  2307,  2307,  2312,  2315,  2316,
    2317,  2318,  2319,  2320,  2321,  2322,  2323,  2325,  2329,  2337,
    2342,  2345,  2351,  2352,  2353,  2354,  2355,  2356,  2357,  2361,
    2365,  2369,  2373,  2377,  2381,  2385,  2389,  2393,  2400,  2401,
    2410,  2414,  2415,  2416,  2420,  2421,  2425,  2426,  2427,  2431,
    2432,  2436,  2447,  2448,  2451,  2454,  2457,  2457,  2476,  2475,
    2491,  2490,  2504,  2513,  2522,  2532,  2533,  2537,  2540,  2549,
    2550,  2554,  2557,  2560,  2576,  2585,  2586,  2590,  2593,  2596,
    2610,  2611,  2615,  2621,  2627,  2630,  2634,  2640,  2649,  2650,
    2651,  2655,  2656,  2660,  2667,  2672,  2681,  2687,  2698,  2701,
    2706,  2711,  2719,  2730,  2733,  2733,  2753,  2754,  2758,  2759,
    2760,  2764,  2771,  2771,  2790,  2793,  2808,  2827,  2828,  2829,
    2834,  2834,  2864,  2867,  2874,  2884,  2884,  2888,  2889,  2890,
    2894,  2904,  2924,  2947,  2948,  2952,  2953,  2957,  2963,  2964,
    2965,  2966,  2970,  2971,  2972,  2976,  2979,  2990,  2995,  2990,
    3015,  3022,  3027,  3036,  3042,  3053,  3054,  3055,  3056,  3057,
    3058,  3059,  3060,  3061,  3062,  3063,  3064,  3065,  3066,  3067,
    3068,  3069,  3070,  3071,  3072,  3073,  3074,  3075,  3076,  3077,
    3078,  3079,  3083,  3084,  3085,  3086,  3087,  3088,  3089,  3090,
    3094,  3105,  3109,  3116,  3128,  3135,  3145,  3146,  3151,  3156,
    3159,  3174,  3187,  3187,  3187,  3200,  3201,  3205,  3209,  3216,
    3220,  3227,  3228,  3229,  3230,  3231,  3246,  3252,  3252,  3252,
    3256,  3261,  3268,  3268,  3275,  3279,  3283,  3288,  3293,  3298,
    3303,  3307,  3311,  3316,  3320,  3324,  3329,  3329,  3329,  3335,
    3342,  3342,  3342,  3347,  3347,  3347,  3353,  3353,  3353,  3358,
    3362,  3362,  3362,  3367,  3367,  3367,  3376,  3380,  3380,  3380,
    3385,  3385,  3385,  3394,  3398,  3398,  3398,  3403,  3403,  3403,
    3412,  3412,  3412,  3418,  3418,  3418,  3427,  3430,  3441,  3457,
    3457,  3462,  3471,  3457,  3500,  3500,  3505,  3515,  3500,  3544,
    3544,  3544,  3587,  3588,  3589,  3590,  3591,  3595,  3602,  3609,
    3615,  3621,  3628,  3635,  3641,  3650,  3653,  3659,  3667,  3672,
    3679,  3684,  3691,  3696,  3702,  3703,  3707,  3708,  3713,  3714,
    3718,  3719,  3723,  3724,  3728,  3729,  3730,  3734,  3735,  3736,
    3740,  3741,  3745,  3774,  3814,  3833,  3853,  3873,  3894,  3894,
    3894,  3902,  3902,  3902,  3909,  3909,  3909,  3920,  3920,  3920,
    3931,  3935,  3941,  3957,  3963,  3969,  3975,  3975,  3975,  3989,
    3994,  4001,  4021,  4049,  4073,  4073,  4073,  4083,  4083,  4083,
    4097,  4097,  4097,  4111,  4120,  4120,  4120,  4140,  4147,  4147,
    4147,  4157,  4162,  4169,  4172,  4178,  4198,  4217,  4225,  4245,
    4270,  4271,  4275,  4276,  4281,  4291,  4294,  4297,  4300,  4308,
    4317,  4329,  4339
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
  "$@29", "expr_call", "expr_not_wrapped", "$@30", "$@31", "$@32", "$@33",
  "$@34", "$@35", "expr", "expr_mtag", "optional_field_annotation",
  "optional_override", "optional_constant",
  "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "opt_sem", "struct_variable_declaration_list", "$@36", "$@37", "$@38",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
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

#define YYPACT_NINF (-1527)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-778)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1527,    99, -1527, -1527,   116,   -88,   362,   -89, -1527,    19,
     312,   312,   312, -1527, -1527,   112,   309, -1527, -1527, -1527,
     384, -1527, -1527, -1527,   109, -1527,   101, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527,   -17, -1527,   -11,
      -5,    71, -1527, -1527, -1527, -1527,   362, -1527,    22, -1527,
   -1527, -1527,   312,   312, -1527, -1527,   101, -1527, -1527, -1527,
   -1527, -1527,   102,   449, -1527, -1527, -1527, -1527,   309,   309,
     309,    80, -1527,   587,   -34, -1527, -1527, -1527, -1527,   438,
     530,   996,   620, -1527,   666,   122,   116,   210,   -88,   192,
     432, -1527,   877,   877, -1527,   471,   384,   127,   384,   881,
     549,   642,   649, -1527,   654,   602, -1527, -1527,   -22,   116,
     309,   309,   309,   309, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527,   676, -1527, -1527, -1527, -1527, -1527, -1527, -1527,   -89,
   -1527, -1527, -1527, -1527, -1527,   497,   150, -1527, -1527, -1527,
   -1527,   782, -1527, -1527, -1527,   696, -1527, -1527, -1527,   384,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,   697,
   -1527,   -19, -1527,   417,   738,   587, 15122, -1527,   165,   784,
   -1527,   -60, -1527, -1527, -1527,   882, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527,   235, -1527,   737, -1527, -1527,   696,   -89,
     -89,   -89, -1527, -1527, 13963, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
     871,   915, -1527,   745,   -89,  1138, -1527, -1527,   769, -1527,
     670,   116,   116,   -14,   126, -1527, -1527, -1527,   150, -1527,
   10592, -1527, -1527, -1527, -1527,   798,   816, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527,   855,   739, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527,   952, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527,   883,   780, -1527, -1527,   263,   868,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
     -89,   716,   869,   -89, -1527,   -60,   233, -1527,   116, -1527,
     841,  1024,   743, -1527, -1527,   873,   886,   888,   863,   889,
     890, -1527, -1527, -1527,   874, -1527, -1527, -1527, -1527, -1527,
     892, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527,   898, -1527, -1527, -1527,   899,   902, -1527, -1527,
   -1527, -1527,   903,   905,   887,   112, -1527, -1527, -1527, -1527,
   -1527,   275,   912,   -85, -1527, -1527, -1527, -1527, -1527, -1527,
     908,   977, -1527,   901, -1527,   317, -1527,   276, 10592, -1527,
    2707, -1527,   274, -1527,   112, -1527, -1527, -1527,   126,   900,
   -1527,  9726,   947,   951, 10592, -1527,   146, -1527, -1527, -1527,
    9726, -1527, -1527,   953, -1527,   231,   575,   613, -1527, -1527,
    9726,   334, -1527, -1527, -1527,    25, -1527, -1527, -1527,    30,
    5808, -1527,   910, 10340, -1527,   962,   182, 10443,   618, -1527,
   -1527,  9726, -1527, -1527,   345, -1527,     1,   912, -1527,   931,
     933,  9726, -1527, -1527,   916, -1527, -1527,    51,   -77,   936,
      41,  3534, -1527, -1527,   -89,   650,  6014,   918,  9726,   964,
     943,   945,   927, -1527,   384,   950,   979,  6220,   154,   701,
     955, -1527,   713,   956,   958,  3740,  9726,  9726,   288,   288,
     288,   932,   935,   946,   957,   959,   961, -1527,  2326, 10237,
    6428, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,  6634,
     966, -1527,  1147, -1527,  9726,  9726,  9726,  9726,  9726,  5396,
    9726, -1527,   948, -1527, -1527,   384,   384,  9726,  1157, -1527,
   -1527, -1527, -1527, -1527, -1527,  6842,  1135, -1527, -1527, -1527,
     792, -1527,  -101,   384, -1527,   384,   384,   384, -1527,   384,
   -1527, -1527,  1104, -1527, -1527, -1527, -1527,   967, -1527, -1527,
     169, -1527, -1527, -1527, -1527, -1527, -1527, -1527, 10045, -1527,
     968, -1527, -1527, -1527, -1527, -1527, -1527,   716,  9726, -1527,
   -1527,    95,   696, -1527,   970,   981,   983, -1527, 11159, -1527,
    1140,   965, -1527, -1527, -1527,  4152, 10592, 10592, 10592, 11197,
   10592, 10592,   972,  1011, 10592,   745, 10592,   745, 10592,   745,
   10695,  1015, 11308, -1527,  9726, -1527, -1527, -1527, -1527,   975,
   -1527, -1527, 13519,  9726, -1527,   275, -1527,  1001, -1527, -1527,
     161, -1527, -1527,   646, -1527,   912,   670,  1002,   646, -1527,
     670, 11343,   990,  1155, -1527,   385, -1527, -1527, -1527, 14005,
     345,   991, -1527,   998, -1527, -1527,   112,   714, -1527,  1018,
    1020,  1022, -1527,  9726,  4152, -1527,  1030,  1090,    52, 11013,
    1209, 10592,  9726,  9726, 14491,  9726, 14005,  1032, -1527, -1527,
    9726, -1527, -1527,  1034,  1060, 14491,  9726, -1527, -1527,  9726,
   -1527, -1527,  9726, -1527, 10592,  4152, -1527, 10900,   494,   494,
    1009, -1527,   967, -1527, -1527, -1527,  9726,  9726,  9726,  9726,
    9726,  9726,   345,  2913,   345,  3120,   345, 14191, -1527,   723,
   -1527, 14005, -1527,   864,  1017,   494,   494,   142,   494,   494,
     -71,  1227,  1025,  1052, 14491,  1052,   491, -1527, -1527, 14005,
   -1527,   345,   670, -1527,  1053,   -89, -1527, -1527, -1527,  4358,
   -1527, -1527, -1527, -1527, -1527, -1527,   -26,    49,   288, -1527,
    2074, 14930,  4564,  4564,  4564,  4564,  4564,  4564,  4564,  4564,
    9726,  9726, -1527, -1527,  9726,  4564,  4564,  9726,  9726,  9726,
    1072,  4564,  9726,   329,  9726,  9726,  9726,  9726,  9726,  9726,
    4564,  4564,  9726,  9726,  9726,  4564,  4564,  4564,  9726,  4564,
    7048,  9726,  9726,  9726,  9726,  9726,  9726,  9726,  9726,  9726,
    9726, 15060,  9726, -1527,  7254, -1527, 14231,   309, -1527,  1239,
   -1527,   -60, -1527, 10592, -1527,  1077, -1527,  4152, -1527, 10779,
     606,   671,  1054,   539, -1527,   744,   795, -1527, -1527,   429,
     796,   868,   804,   868,   805,   868, -1527,   418, -1527,   456,
   -1527, 10592,  1036, -1527, -1527, 13557, 10592, -1527, -1527, 10592,
   -1527, -1527, -1527,  9726,  1079, -1527,  1082, -1527, 10592, -1527,
    4152, 10592, 10592, -1527,    28,   345, 10592,  5602,  7460,  1083,
    9726, 10592, -1527, -1527, -1527, 10592,  1052, -1527,  1030,  9726,
    9726,  9726,  9726,  9726,  9726,  9726,  9726,  9726,  9726,  9726,
    9726,  9726,  9726,  9726,  9726,  9726,  9726,   -89,  1004,  1041,
   14491, 11454, -1527, -1527, 10592, 10592, 11492, 10592, -1527, -1527,
   11603, 10592,  1052, 10592, 10695,  1052,  1015,   341, -1527, 10900,
   -1527,    49, 11638, 11749, 11787, 11898, 11933, 12044,    32,   288,
    1043,   188,  3327,  4772,  7666, 14324,  1071,     8,   195,  1074,
     434,    35,  7872,     8,   678,    39,  9726,  1084, -1527,  9726,
   -1527, 10592, -1527, 10592, -1527,  9726,   858,   345,   345,    40,
     -43, -1527,  9726, -1527,  1051,  1055,  1056,   641, -1527, -1527,
     696,  9726,    53, -1527,  9726, -1527, -1527,   967,   134,  4980,
   -1527,   367,  1071,  1058,  1093,  1093, -1527, -1527,  1057,   270,
     745, -1527,  1081,  1063, -1527, -1527,  1085,  1065, -1527, -1527,
     288,   288,   288, -1527, -1527, 11048, -1527, 11048, -1527, 11048,
   -1527, 11048, -1527, 11048, -1527, 11048, -1527, 11048, -1527, 11048,
     765,   765, 10436, -1527, 11048, -1527, 11048, 10436, 14861, 14861,
    1067, -1527, 11048,   291,  1070, -1527, 13668,   350,   350,   968,
   14491,   765,   765, -1527, 11048, -1527, 11048,  2100, 10736, 10092,
   -1527, 11048, -1527, 11048, -1527, 11048, 14584, -1527, 11048, 15029,
   14361, 14714, 14740, 14768, 10436, 10436,   232,   232,   291,   291,
     291,   512,  9726,  1075,  1080,   567,  9726,  1288,  1087, 13703,
   -1527,   393, -1527,   436,   907,  1229,   384,   705, -1527, -1527,
   10779, -1527, -1527, -1527, -1527, 10592, -1527, -1527, -1527,  1121,
   -1527,  1099, -1527,  1101, -1527,  1102, -1527, 10695, -1527,  1015,
     514,   912, -1527, -1527,   912,   912, 12082, -1527,  1259,     5,
   -1527, 10900,   817,  1012,  9726,    55,   823,   588,   435,  1088,
    1089,  1132, 12193,   522, 12228,   832, 10592, 10695,  1015,  1049,
    1094,   111, 14491, 14491, 14491, 14491, 14491, 14491, 14491, 14491,
   14491, 14491, 14491, 14491, 14491, 14491, 14491, 14491, 14491, -1527,
    1097, 10592, -1527, -1527,  9726,  1078,  1133, -1527,  1225, -1527,
    1248,  1100,  1477,   521,  1103,   572,    49,   745, -1527, -1527,
   -1527, -1527, -1527,  1105,  9726, -1527,  9726,  9726,  9726,  5188,
      37,  9726,   755,   588,   195, -1527, -1527,  1095, -1527,  9726,
   -1527,  1107,  9726, -1527,  9726,   588,   770,  1108, -1527, -1527,
    9726, 14491, -1527, -1527,   582,   585, 14454,    57,    65,  9726,
     345,    66, -1527,  9726,  9726, 10592,   745,   924,  2450,  9726,
   -1527,  2707,    49,   278, -1527,  1109,   477,  9932, -1527, -1527,
   -1527,   523,   445,   270,  1156,  1161,  1111,  1163,  1164, -1527,
     573,   868, -1527,  9726, -1527,  9726, -1527, -1527, -1527,  8078,
    9726, -1527,  1122,  1123, -1527, -1527,  9726,  1124, -1527, 13814,
    9726,  8284,  1129, -1527, 13852, -1527,  8490, -1527, -1527,   384,
   -1527, -1527,   808, -1527,    82,   696,    49, -1527, -1527, -1527,
   -1527,   912, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527,  1130, 10592, -1527,  1165,
    9726, -1527, -1527,   207,  9726, -1527,  1134, -1527, -1527, -1527,
     625, -1527,  1139,  1166, -1527, -1527,  1576,   661,   688, -1527,
   -1527,  9726,  1666, 14098, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527,   113,   868,  8696,   577, 12339, 14491, 14491,
       8,   195, 14491,  1143,   382,  1071, -1527, -1527, 14491, -1527,
    1074,   787,     8,  1145, -1527, -1527, -1527, -1527,   790, -1527,
   -1527, -1527,  1169,  9726,  9726,   791,    72,  9726, 12377, 12488,
    1680,   868,   -89, -1527, -1527,  9726, -1527, -1527,   384,   801,
   -1527,   745, -1527,  1137,  4980,  1178,  1146,   356,   447, -1527,
   -1527,  1190, -1527, -1527,   270,  1148,   412, 10592, 12523, 10592,
   12634, -1527,   448, 12672, -1527,  9726, 14621,  9726, -1527, 12783,
    4980, -1527,   451,  9726, -1527, -1527, -1527,   460,   696,  1333,
      82, -1527, -1527,   907, -1527,  1150, -1527, -1527, -1527,  9726,
     912, -1527, 14491,  1151,  1153, -1527,   470, -1527, -1527,  9726,
    1200,  9726,  1175, -1527, -1527, -1527, -1527,  1158,  1160,  1162,
   -1527,  9726,  9726,  9726,  1167,  1310,  1170,  1172,  8902, -1527,
     412, -1527,   493,  9726,   441,   195, -1527,  9726,  9726,  9726,
    9726,   770, -1527,  9726,  9726,  1173, -1527, -1527,   501,   525,
    9726,  9726,   802, -1527, -1527, -1527,  1182, -1527, 14005,  3946,
   -1527,  9726,   868, -1527,   581, -1527,   725, 10592,   146, -1527,
    1174, -1527, -1527,  9108, -1527, -1527,  1871, -1527,   834, -1527,
   -1527, -1527, 10592, 12818, 12929, -1527,   584, -1527, 12967, -1527,
   -1527, -1527, -1527,  1333,   345,  1176,  1310,  1310,   -89, 13078,
    1192, 13113,  1179,  1181,  1183,  1184,  9726, -1527,  9726, 10436,
   10436, 10436,  9726, -1527, -1527,  1310,  1310, -1527, 13224, -1527,
   -1527, 14098,  9726,  9726, -1527, 13262, 14491, 14491, 14098, -1527,
     384, 10436,  9726, -1527,  1216,  1210,  1214, 14098,   535,  9726,
     387, -1527,   924,  9314,  9520, -1527, -1527, -1527, -1527, -1527,
   14491,   384,   -89,  1185, 10592,   146,   621,  9726, -1527,  9726,
   14584, -1527, -1527,   835, -1527, -1527,  1186, -1527, 15122, -1527,
   -1527, -1527,   147,   147, -1527, -1527,  9726, -1527,  9726,  1310,
    1310,   588,  1187,  1188,  1052,   147,   588, -1527,  1349,  1191,
   14491, 14491,   454,  1224,   670,  1218, -1527,  9726,  9726,  1194,
    1226, 14098, -1527,   387, -1527,  9726,  9726, 14491,   670, -1527,
   -1527,   621,  9726,  9726, 14098, 14621, -1527, -1527, -1527, -1527,
     384, 15122,   588,  1071,  1222, -1527,  1196,  1197, 13373, 13408,
     147,   147,  1071,  1203, -1527, -1527,  1205,  1208,  1211,  9726,
    1212,  9726,  9726,  1215,   670, -1527,  1217,   384, 14098, -1527,
    9726,  1220, -1527, 14491, -1527,  9726, 14098, 14098, -1527, -1527,
     696,   -89,   537,  1233, -1527, -1527, -1527, -1527, -1527,  1219,
    1221, -1527, -1527, -1527, -1527, 14491, -1527, 14491, 14491, -1527,
   -1527, -1527,  1243,  1228, 14098, -1527, 14098, -1527, -1527, -1527,
   -1527, -1527,   588, -1527, -1527, -1527, -1527,  1236, -1527,   560,
   -1527, -1527
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   132,     1,   317,     0,     0,     0,   635,   318,     0,
     627,   627,   627,    17,    18,     0,     0,    16,    15,     3,
       0,    10,     9,     8,     0,     7,   616,     6,    11,     5,
       4,    13,    12,    14,   102,   103,   101,   110,   112,    45,
      61,    58,    59,    47,    48,    49,     0,    50,    56,    46,
     232,   231,   627,   627,    24,    23,   616,   629,   628,   799,
     789,   794,     0,   285,    43,   118,   119,   120,     0,     0,
       0,   121,   123,   130,     0,   117,    19,   649,   648,   225,
     637,     0,   652,   617,   618,     0,     0,     0,     0,    51,
       0,    57,     0,     0,    54,     0,     0,   627,     0,    20,
       0,     0,     0,   287,     0,     0,   129,   124,     0,     0,
       0,     0,     0,     0,   133,   227,   226,   229,   224,   639,
     638,     0,   651,   650,   654,   653,   657,   620,   619,   622,
     108,   109,   106,   107,   105,     0,     0,   104,   113,    62,
      60,    56,    53,    52,   630,   562,   234,   233,   634,     0,
     636,    21,    22,    25,   800,   790,   795,   286,    41,    44,
     128,     0,   125,   126,   127,   131,     0,   640,     0,   645,
     613,   548,    26,    27,    31,     0,    97,    98,    95,    96,
      94,    93,    99,     0,    55,     0,   563,   632,   562,     0,
       0,     0,    42,   122,     0,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
       0,     0,   139,   134,     0,     0,   624,   646,     0,   658,
     614,     0,     0,   550,     0,    28,    29,    30,     0,   111,
       0,   633,   708,   791,   796,   193,   194,   191,   142,   143,
     145,   144,   146,   147,   148,   149,   175,   176,   173,   174,
     166,   177,   178,   167,   164,   165,   192,   186,     0,   190,
     179,   180,   181,   182,   153,   154,   155,   150,   151,   152,
     163,     0,   169,   170,   168,   161,   162,   157,   156,   158,
     159,   160,   141,   140,   185,     0,   171,   172,   548,   137,
     262,   230,   692,   695,   698,   699,   693,   696,   694,   697,
       0,     0,   643,   655,   621,   548,     0,   114,     0,   116,
       0,   603,   601,   623,   100,     0,     0,     0,     0,     0,
       0,   665,   685,   666,   701,   667,   671,   672,   673,   674,
     691,   678,   679,   680,   681,   682,   683,   684,   686,   687,
     688,   689,   759,   670,   677,   690,   766,   773,   668,   675,
     669,   676,     0,     0,     0,     0,   700,   721,   724,   722,
     723,   786,   631,   706,   581,   587,   195,   196,   189,   184,
     197,   187,   183,     0,   135,   316,   575,     0,     0,   228,
       0,   624,   562,   641,     0,   647,   564,   659,     0,     0,
     115,     0,     0,     0,     0,   602,     0,   727,   750,   753,
       0,   756,   746,     0,   712,   760,   767,   774,   780,   783,
       0,     0,   736,   741,   735,     0,   749,   745,   738,     0,
       0,   740,   725,     0,   709,   706,     0,   792,   797,   198,
     188,     0,   314,   315,     0,   136,   548,   138,   264,     0,
       0,     0,    71,    72,    84,   453,   454,     0,     0,     0,
       0,   302,   447,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,   691,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,   240,   370,   372,   371,   373,   374,   375,   376,     0,
       0,    37,   231,   241,     0,     0,     0,     0,     0,   266,
       0,   352,   353,   451,   450,     0,     0,     0,     0,   257,
     252,   249,   248,   250,   251,   270,   235,   284,   263,   243,
     530,   242,   448,     0,   521,    79,    80,    77,   255,    78,
     256,   258,   320,   247,   520,   519,   518,   132,   524,   449,
       0,   244,   523,   522,   494,   455,   495,   531,   377,   456,
       0,   452,   802,   806,   803,   804,   805,     0,     0,   625,
     644,   566,   562,   549,     0,     0,     0,   530,     0,   605,
     606,     0,   599,   600,   598,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   134,     0,   134,     0,   134,
       0,     0,     0,   732,   266,   743,   744,   737,   739,     0,
     742,   726,     0,     0,   788,   787,   710,   711,   707,   801,
     285,   715,   716,     0,   582,   577,     0,     0,     0,   588,
       0,     0,     0,   660,   573,   592,   576,   848,   851,     0,
       0,     0,   290,   294,   293,   299,     0,     0,   338,     0,
       0,     0,   884,     0,     0,   306,   303,     0,   531,   377,
       0,     0,   270,     0,   288,     0,     0,     0,   329,   332,
       0,   261,   335,     0,     0,    65,     0,    88,   888,     0,
     857,   866,     0,   854,     0,     0,   311,   308,   483,   484,
     353,   365,   132,   283,   281,   282,     0,     0,     0,     0,
       0,     0,     0,   826,     0,     0,     0,   864,   891,     0,
     274,     0,   277,     0,     0,   460,   459,   496,   458,   457,
       0,     0,     0,   902,     0,   902,   354,   259,   260,     0,
      82,     0,     0,   893,   902,     0,   368,   238,   528,     0,
     246,   253,   254,   305,   310,   319,     0,   362,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   485,   486,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,     0,   237,     0,   642,   562,     0,   565,     0,
     656,   548,   615,     0,   604,     0,   608,     0,   612,   377,
       0,     0,     0,   717,   730,     0,     0,   702,   704,     0,
       0,   137,     0,   137,     0,   137,   579,     0,   585,     0,
     703,     0,     0,   734,   719,     0,     0,   583,   793,     0,
     589,   798,   574,     0,     0,   591,     0,   590,     0,   593,
       0,     0,     0,    89,     0,     0,     0,   840,     0,     0,
       0,     0,   874,   877,   880,     0,   902,   307,   304,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     289,     0,    90,    91,     0,     0,     0,     0,    63,    64,
       0,     0,   902,     0,     0,   902,     0,     0,   312,   309,
     354,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   285,     0,     0,     0,   860,   818,   826,     0,   869,
       0,     0,     0,   826,     0,     0,     0,     0,   829,     0,
     896,     0,   265,     0,    40,     0,    38,     0,     0,     0,
       0,   871,   903,   267,     0,     0,     0,   424,   421,   423,
     562,     0,     0,   895,   903,   271,   262,   132,     0,   266,
     440,     0,   817,     0,     0,     0,   328,   327,     0,     0,
     134,   280,     0,     0,   507,   506,     0,     0,   508,   512,
       0,     0,     0,   399,   408,   387,   409,   388,   411,   390,
     410,   389,   412,   391,   402,   381,   403,   382,   404,   383,
     461,   462,   474,   413,   392,   414,   393,   475,   472,   473,
       0,   401,   379,   501,     0,   492,     0,   525,   526,   527,
     380,   463,   464,   415,   394,   416,   395,   479,   480,   481,
     405,   384,   406,   385,   407,   386,   482,   400,   378,     0,
       0,   477,   478,   476,   470,   471,   466,   465,   467,   468,
     469,     0,     0,     0,   430,     0,     0,     0,     0,     0,
     445,     0,   626,     0,   556,   559,     0,     0,   607,   610,
     377,   611,   728,   751,   754,     0,   757,   747,   713,     0,
     761,     0,   768,     0,   775,     0,   781,     0,   784,     0,
       0,   272,   731,   720,   578,   584,     0,   662,   663,   594,
     597,   596,     0,     0,     0,     0,     0,   841,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   531,   387,   388,   390,   389,   391,   381,   382,   383,
     392,   393,   379,   394,   395,   384,   385,   386,   378,   313,
       0,     0,   897,   497,     0,     0,     0,   498,     0,   529,
       0,     0,     0,     0,     0,     0,   362,   134,   532,   533,
     534,   535,   536,     0,     0,   827,     0,     0,     0,     0,
     826,     0,     0,     0,     0,   835,   836,     0,   843,     0,
     833,     0,     0,   872,     0,     0,     0,     0,   831,   873,
       0,   863,   828,   892,     0,     0,    34,     0,     0,     0,
       0,     0,   487,     0,     0,     0,   134,    68,    73,     0,
     894,     0,   362,     0,   441,     0,     0,     0,   444,   442,
     321,     0,     0,     0,     0,     0,     0,     0,     0,   360,
       0,   137,   503,     0,   509,     0,   398,   396,   397,     0,
       0,   490,     0,     0,   513,   517,     0,     0,   493,     0,
       0,     0,     0,   431,     0,   438,     0,   488,   446,   133,
     557,   558,   559,   560,   551,   562,   362,   609,   729,   752,
     755,   718,   758,   748,   714,   705,   762,   764,   769,   771,
     776,   778,   782,   580,   785,   586,     0,     0,   661,     0,
       0,   849,   852,     0,     0,   291,     0,   296,   297,   295,
       0,   341,     0,     0,   344,   339,     0,     0,     0,   885,
     883,   270,     0,     0,   330,   333,   336,   889,   887,   858,
     867,   865,   855,     0,   137,     0,     0,     0,   808,   807,
     826,     0,   861,     0,     0,   819,   842,   834,   862,   832,
     870,     0,   826,     0,   838,   839,   846,   830,     0,   275,
     278,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   137,     0,    66,    67,     0,    81,    74,     0,     0,
     236,   134,   443,     0,   266,     0,     0,   601,     0,   357,
     358,     0,   356,   355,     0,     0,     0,     0,     0,     0,
       0,   419,     0,     0,   514,     0,   502,     0,   491,     0,
     266,   432,     0,     0,   489,   439,   435,     0,   562,   554,
     551,   552,   553,   556,   567,     0,   765,   772,   779,   266,
     273,   664,   595,     0,     0,    85,     0,   292,   298,     0,
       0,     0,     0,   340,   875,   878,   881,     0,     0,     0,
      92,     0,     0,     0,     0,   840,     0,     0,     0,   239,
       0,   538,     0,     0,     0,     0,   844,     0,     0,     0,
       0,     0,   837,     0,     0,   268,    32,    39,     0,     0,
       0,     0,     0,   422,   547,   425,     0,    69,     0,     0,
      83,     0,   137,   417,     0,   322,   601,     0,     0,   364,
       0,   361,   363,     0,   349,   367,     0,   546,     0,   544,
     420,   541,     0,     0,     0,   540,     0,   433,     0,   436,
     572,   555,   568,   554,     0,     0,   840,   840,     0,     0,
       0,     0,     0,     0,     0,     0,   266,   898,   270,   331,
     334,   337,     0,   841,   859,   840,   840,   499,     0,   366,
     539,   900,     0,     0,   845,     0,   810,   809,   900,   847,
     900,   276,   266,   279,    36,     0,     0,   900,     0,     0,
       0,   428,    68,   302,     0,    75,    79,    80,    77,    78,
      76,   900,     0,     0,     0,     0,     0,     0,   359,     0,
     350,   504,   510,     0,   545,   543,     0,   542,     0,   570,
     561,   733,   825,   825,    87,   342,     0,   345,     0,   840,
     840,   815,     0,     0,   902,   825,   815,   500,     0,     0,
     812,   811,     0,     0,     0,   902,    33,     0,     0,     0,
       0,   900,   426,     0,    70,     0,     0,   308,     0,   369,
     418,     0,     0,     0,     0,   351,   505,   511,   515,   434,
       0,     0,     0,   822,   902,   824,     0,     0,     0,     0,
     825,   825,   816,     0,   886,   899,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   903,     0,   900,   900,   904,
       0,     0,   429,   309,   911,     0,     0,     0,   326,   516,
     562,     0,     0,   903,   823,   850,   853,   343,   346,     0,
       0,   882,   890,   868,   856,   901,   909,   814,   813,   910,
     912,   269,     0,     0,   900,   908,     0,   325,   324,   569,
     571,   820,     0,   876,   879,   907,   905,     0,   323,     0,
     906,   821
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1527, -1527,    -1, -1527, -1527, -1527, -1527,   709,  1357, -1527,
   -1527, -1527, -1527, -1527, -1527,  1433, -1527, -1527, -1527,   954,
    1407, -1527,  1313, -1527, -1527,  1367, -1527, -1527, -1527,  -136,
   -1527, -1527, -1527,   -52, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527,  1223, -1527, -1527,   -49,   -55, -1527,
   -1527, -1527,   590,   643,  -532,  -569,  -821, -1527, -1527, -1527,
   -1468, -1527, -1527,    -4,  -228,  -217,  -377, -1527,   473, -1527,
    -605, -1527,  -661,    92,  1415, -1527, -1527, -1527, -1527,  -414,
       0, -1527, -1527, -1527, -1527, -1527,   -47,   -45,   -44, -1527,
     -42, -1527, -1527, -1527,  1469, -1527,   476, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
     694,    -8,  1040,    59,   221,  -900,  -439, -1527,  -528, -1527,
   -1527,  -384,  1669, -1527, -1527, -1527, -1526, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527,    96, -1527, -1527, -1527,
   -1527, -1527, -1527,  1048, -1527,  -148,    42,   -68,    34,   189,
   -1527,  -187, -1527, -1527, -1527, -1527,  1038, -1527,  -419,  -882,
   -1527,  -424,  -881, -1527,  -637,   -59,  -586, -1364, -1527,  -364,
   -1527, -1527,  1430, -1527, -1527, -1527,  1096,  1066,    45, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527,  -623,   198, -1527,  1031, -1527,
   -1527,  1061, -1527, -1527, -1527, -1527,  -405, -1527, -1527,  -394,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527,  -219, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527,  1033,  -648,  -145,  -847,  -687, -1527, -1527, -1092,  -918,
   -1527, -1527, -1527, -1163,     2, -1388, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527,   280,  -496, -1527, -1527, -1527,
     783, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527, -1527,
   -1527, -1527, -1527, -1527, -1527, -1187,  -718, -1527
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   632,    18,   153,    56,    19,   175,   181,  1584,
    1382,  1496,   723,   523,   159,   524,   105,    21,    22,    47,
      48,    49,    94,    23,    41,    42,   525,   526,  1395,  1396,
     527,  1398,  1509,   528,   529,   981,   530,   650,   651,   531,
     532,   533,   534,  1174,   182,   183,    37,    38,    39,   233,
      71,    72,    73,    74,    24,   299,   389,   223,    25,   117,
     224,   118,   166,   535,   148,   701,  1013,   538,   390,   539,
     732,  1583,   742,  1120,   587,   961,  1494,   963,  1495,   541,
     542,   543,   653,   876,  1457,   544,   545,   546,   547,   548,
     549,   550,   551,   444,   552,   756,  1251,   996,   553,   554,
     914,  1471,   915,  1472,   917,  1473,   555,   881,  1463,   556,
     733,  1525,   557,  1259,  1260,  1000,   703,   558,   813,   987,
     559,   667,  1014,   561,   562,   563,   979,   564,  1235,  1590,
    1236,  1653,   565,  1087,  1435,   566,   567,  1417,  1666,  1419,
    1667,  1532,  1709,   734,   569,   385,  1443,  1542,  1292,  1294,
    1096,   187,   581,   821,  1618,  1671,   386,   387,   846,   847,
     437,   848,   849,   438,  1198,   644,   595,   406,   322,   323,
     230,   315,    84,   129,    27,   171,   311,    95,    96,   185,
      97,    28,    53,   121,   168,    29,   395,   228,   229,    82,
     126,   397,    30,   169,   313,   645,   570,   310,   368,   369,
     839,   436,   373,   370,   603,  1304,  1117,   832,   432,   371,
     596,  1298,   851,   601,  1303,   597,  1299,   598,  1300,   600,
    1302,   604,  1306,   605,  1446,   606,  1308,   607,  1447,   608,
    1310,   609,  1448,   610,  1312,   611,  1314,   635,    31,   101,
     190,   374,   636,    32,   102,   191,   375,   640,    33,   100,
     189,   571,  1673,  1683,   993,   947,  1674,  1675,  1676,   948,
     960,  1219,  1213,  1208,  1376,  1140,   572,   871,  1453,   872,
    1454,   926,  1477,   923,  1475,   949,   743,   573,   924,  1476,
     950,   574,  1146,  1553,  1147,  1554,  1148,  1555,   885,  1467,
     921,  1474,   719,   744,   575,  1639,   973,   576
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,   241,   314,    52,   718,   827,   560,   301,   870,   852,
     591,   909,   747,   537,   638,    64,    75,   974,   633,    76,
    1111,   372,  1113,   234,  1115,   757,   985,   874,   953,  1204,
    1139,  1187,   634,   639,   582,  1216,   841,   138,   843,   625,
     845,  1366,  1183,  1518,    90,  1185,   659,   665,  1134,   819,
     704,   705,  1194,   998,   161,  1214,   449,   450,   615,  1220,
    1229,   939,  -347,   617,  1652,   946,    62,   946,    75,    75,
      75,   696,   748,  1239,    50,  1324,   460,  1383,    40,    91,
     434,   642,   465,   393,   537,  1384,  1387,  1564,   435,   938,
     939,   951,  1501,   955,    51,   145,    63,   150,    98,     2,
     231,   992,   643,  -132,   713,   715,     3,   969,   537,   642,
      75,    75,    75,    75,    77,    78,   749,    79,   982,   479,
     480,  -348,   110,   111,   112,   170,   320,  1702,  1441,     4,
     643,     5,   865,     6,   994,    34,    35,   130,   131,     7,
     656,  1245,   149,   660,   661,    80,   968,   321,   188,     8,
    1670,    13,  1605,   482,   483,     9,   383,   113,  1622,  1623,
     232,   231,   536,    83,   226,   176,   177,   398,  1150,   447,
     931,    14,    86,    13,  1230,    85,   316,  1635,  1636,    10,
      86,   828,   114,   317,    57,   242,   243,   244,  1442,    87,
      58,   995,    54,    14,    62,   940,   160,   867,  1486,   193,
     433,    11,    12,  1711,  1181,   579,   318,  1184,   629,   999,
    1243,   498,   499,   500,    50,  -347,    81,  -347,   537,   866,
     300,   232,   616,   866,    63,    92,   866,   618,   857,   992,
     866,   866,   319,   860,   512,  -347,    93,  -347,    55,   662,
     366,  1680,  1681,  -347,   866,   619,   866,   670,   866,   652,
     887,   620,  1135,   760,   761,    62,   866,   866,   663,   146,
      13,   687,    88,   866,    13,  1337,   519,  1338,  -347,   400,
    -347,   783,   784,   592,  -348,   103,  -348,   908,   320,   147,
      14,   928,  1361,   593,    14,    63,  1353,   537,   132,    36,
    1326,   146,   940,   133,  -348,   134,  -348,   109,   135,   321,
     927,   758,  -348,    15,   104,   940,   391,  1243,   422,   396,
     817,   147,   760,   761,    16,   865,   178,    50,   537,  1001,
    1243,   179,  1574,   180,   442,  1197,   135,  -348,    50,  -348,
    1478,   104,   521,   700,    13,   423,   424,    51,   594,   136,
     992,  1246,  1401,    65,  1227,  1228,   146,  1231,    51,   815,
     772,   773,  1244,   820,    14,   856,   811,   812,   780,   225,
     782,   783,   784,   785,  1672,   421,   147,   443,   786,    57,
      50,   686,    66,   628,   422,    58,   139,   830,   831,   833,
     867,   835,   836,  1205,  1206,   840,   446,   842,   366,   844,
      51,  1643,   997,  1644,   580,   822,  1445,  1254,   889,   425,
    1649,   423,   424,   426,   366,   865,    93,  1255,   858,   772,
     773,  1207,   861,  1099,  1658,  1197,    67,   780,   383,   782,
     783,   784,   785,   231,    86,  -763,   238,   786,  1105,  1097,
    -763,  1261,   873,   366,   808,   809,   810,   366,   367,   186,
    1416,    13,  1485,  1256,  1119,    68,   811,   812,  -763,   399,
     537,    50,    62,   239,  1491,  1242,  1130,   366,   657,   912,
     300,    14,  1257,  1223,  1701,   425,   578,  1258,   427,   426,
     867,    51,   428,   681,   300,   429,   780,  1132,  1133,   783,
     784,   384,    63,   232,   300,   300,   300,  1044,  1240,   992,
     430,  1149,  1320,   537,   445,   115,   431,   642,   366,   366,
    1195,   116,  1045,  1171,   962,   811,   812,   521,   700,   865,
    1732,  1733,   865,  1360,   983,   760,   761,    69,   643,  1488,
    1175,  1176,   980,  1178,   737,   738,    70,  1180,  1372,  1182,
      62,  1677,   613,  1480,   427,    43,    44,    45,   428,   975,
    1186,   429,   750,  1687,   751,   752,   753,  1747,   754,    13,
    1517,   614,   946,   405,  1523,  1365,   430,   668,  1247,   759,
      63,   110,   431,   112,   811,   812,    46,   946,   865,    14,
    1506,  1266,  1267,  1268,   867,    50,   866,   867,  1572,   868,
     818,   865,   869,    13,   889,  1248,   367,   119,  1719,  1720,
     826,  1691,   104,   120,    13,    51,   366,   366,   366,    13,
     366,   366,   367,    14,   366,   141,   366,  1386,   366,   631,
     366,  1288,   772,   773,    14,   668,   668,  1116,  1354,    14,
     780,    13,  1210,   783,   784,   785,   889,   113,  1108,  1092,
     786,   367,  1121,   867,    62,   367,  1405,  1124,  1414,   889,
    1125,    14,   889,   975,   144,   300,   867,   631,   976,  1129,
    1211,   889,  1289,  1327,   422,  1118,   878,  1136,   106,   107,
     108,   889,  1145,  1407,    63,  1519,  1530,  1391,  1202,  1537,
    1277,   366,   300,  1095,   172,   173,   174,   124,  1539,    13,
    1468,   423,   424,   125,   889,  1278,    13,    13,  1548,   977,
    1169,  1602,   889,  1403,   366,  1315,   367,   367,  1313,    14,
     162,   163,   164,   165,    13,   631,    14,    14,   811,   812,
     668,  1570,   631,  1316,  1405,  1317,   889,   300,   959,  1585,
    1350,  1332,   154,   127,    14,  1282,   889,  1489,  1202,   128,
     631,   110,   111,   112,   433,   300,   978,    13,   422,  1406,
    1283,   986,  1121,  1586,  1121,   425,   940,    13,   592,   426,
      13,  1202,  1336,  1650,   300,  1741,  1514,    14,   593,   668,
     668,  1243,  1483,   631,  1414,   423,   424,    14,   889,  -770,
      14,  1352,  1202,   631,  -770,  1202,   631,  1342,  1751,   158,
    1119,  1379,  1536,    13,  1380,   668,   760,   761,   668,  1415,
      13,   637,  -770,  1237,   367,   367,   367,  1603,   367,   367,
    1616,   433,   367,    14,   367,  1102,   367,  -777,   367,   631,
      14,    13,  -777,   594,   427,   155,   631,    75,   428,  1317,
    1317,   429,   156,   366,  1459,  1479,    13,   157,  1101,   425,
    -777,    14,  1512,   426,   146,  -427,   430,   631,  1109,    91,
    -427,  1390,   431,    13,  1545,   668,    14,  1573,   671,   167,
     422,   366,   631,    13,   147,  1439,   366,   560,  -427,   366,
    1465,   186,  1217,    14,   537,  1218,   433,   672,   366,   367,
    1103,   366,   366,    14,   192,  1293,   366,   423,   424,   631,
     146,   366,   110,   772,   773,   366,  1301,  1466,    13,   392,
     227,   780,   367,   782,   783,   784,   785,  1633,   427,   688,
     147,   786,   428,   300,  1296,   429,   296,   957,    14,   958,
     668,   691,   879,  1363,   366,   366,  1686,   366,   689,  1604,
     430,   366,   405,   366,   366,  1460,   431,  1696,  1364,   240,
     692,   880,  1607,  1119,   403,   300,  1392,   404,   151,   433,
     405,   425,   312,  1106,   152,   426,  1203,  1393,  1394,  1212,
     297,  1632,  1203,  1212,  1373,   379,  1714,  1374,  1005,  1009,
    1375,   366,   298,   366,  1290,   806,   807,   808,   809,   810,
    1291,   376,  1490,   668,   380,  1493,  1500,  1645,   889,   811,
     812,   889,   889,  1682,  1049,  1151,  1511,  1589,  1682,   377,
     433,   433,   889,   889,  1107,  1110,   382,  1580,   422,   433,
     433,   122,   123,  1112,  1114,  1692,   300,   300,   300,  1088,
     427,   521,   700,  1400,   428,  1601,  1321,   429,   433,  1662,
    1663,   367,  1325,  1526,  1712,   423,   424,   433,   378,   433,
     433,  1335,   430,  1612,  1668,   235,   236,   422,   431,   668,
     668,   172,   173,   964,   965,   422,   142,   143,   668,   367,
      43,    44,    45,  1224,   367,  1225,   381,   367,   401,   235,
     236,   237,   388,   394,   423,   424,   367,   402,  1151,   367,
     367,   407,   423,   424,   367,  1705,    59,    60,    61,   367,
     410,   439,   422,   367,   408,   668,   409,   411,   412,   425,
     414,   413,   592,   426,  1749,  1295,   415,   416,  1450,  1297,
     417,   418,   593,   419,   420,   366,  1455,   433,  1444,   423,
     424,   422,   367,   367,   440,   367,   583,   366,   441,   367,
     589,   367,   367,  1606,   590,   623,   602,   626,   425,   647,
      13,   648,   426,   -86,   658,   675,   425,   677,   423,   424,
     426,   678,  1333,   679,   680,   683,   366,   366,   682,   706,
      14,  1697,   707,   690,   693,   666,   694,   594,   427,   367,
     724,   367,   428,   708,   722,   429,   422,   740,   736,    13,
     755,   366,   745,   425,   709,  1507,   710,   426,   711,   823,
     430,   720,    16,   825,   838,   814,   431,   686,   637,    14,
     837,   853,   628,   423,   424,   631,   859,   427,   864,  1524,
    1528,   428,   425,  1170,   429,   427,   426,   863,   875,   428,
    1661,  1322,   429,   302,   735,   877,   882,   303,   883,   430,
     884,   889,   758,   907,   913,   431,   919,   430,   918,   930,
     668,   304,   305,   431,   967,   366,   306,   307,   308,   309,
     970,   971,   427,   972,   984,  1040,   428,  1094,  1339,   429,
    1098,  1540,  1127,  1104,  1122,  1128,  1143,   425,   422,  1172,
    1196,   426,  1202,  1524,   430,  1209,  1250,  1275,  1222,  1232,
     431,   427,  1233,  1234,  1253,   428,  1249,  1344,   429,  1262,
    1263,   422,  1265,  1264,  1269,   423,   424,  1270,  1438,  1285,
     668,  1592,  1280,   430,  1305,   668,  1293,  1281,  1307,   431,
    1309,  1311,  1319,   367,  1286,  1330,  1328,  1329,   423,   424,
     668,  1367,  1340,  1613,  1341,   367,   668,   366,  1348,   300,
    1424,  1351,  1355,  1369,  1377,   668,   427,  1402,  1411,  1409,
     428,  1624,  1345,   429,  1410,   668,  1412,  1413,  1451,  1462,
    1425,  1427,  1470,  1151,   367,   367,  1433,  1449,   430,   425,
    1497,  1515,  1458,   426,   431,  1513,  1461,   886,   888,  1203,
    1487,  1492,  1516,  1520,  1522,   668,  1541,  1478,  1546,   367,
    1547,  1203,   425,  1550,  1552,  1556,   426,   668,  1557,  1558,
    1563,  1591,   668,   922,  1562,  1659,   925,  1565,   300,  1566,
    1582,  1626,  1608,  1646,  1621,  1647,  1628,  1510,  1629,  1648,
    1630,  1631,  1689,  1660,  1669,  1684,  1685,  1690,  1693,  1695,
    1699,  1700,   300,  1713,  1715,  1716,  1694,   366,   427,   366,
     668,  1721,   428,  1722,  1346,   429,  1723,  1745,  1726,  1724,
    1704,  1729,   966,   367,    20,  1731,  1735,  1743,   568,  1744,
     430,   427,   137,   991,  1746,   428,   431,  1347,   429,   588,
    1742,   668,  1750,    89,   184,   140,  1654,  1595,   599,  1241,
    1093,   324,  1596,   430,  1597,  1598,  1730,  1599,   612,   431,
      26,  1252,  1569,  1521,  1408,  1619,   300,  1544,   622,   668,
     668,  1440,  1543,   668,   646,  1620,    99,   577,   654,   641,
     655,  1688,  1370,  1579,  1740,     0,   627,     0,   954,   649,
     668,     0,     0,     0,   300,     0,     0,     0,  1091,   669,
     422,     0,     0,     0,   674,   367,   676,   366,     0,     0,
       0,     0,     0,  1739,     0,   685,   668,     0,   702,   702,
     702,     0,   366,   697,   698,   699,     0,   423,   424,     0,
       0,     0,     0,     0,   300,   668,     0,     0,   717,     0,
       0,     0,     0,     0,     0,     0,     0,   721,     0,     0,
       0,     0,   725,   726,   727,   728,   729,     0,     0,     0,
    1638,  1138,     0,     0,     0,   739,     0,  1638,     0,  1638,
     746,     0,     0,   717,     0,     0,  1638,     0,     0,     0,
     978,     0,     0,     0,     0,     0,     0,   668,   300,     0,
    1638,   425,     0,     0,   366,   426,     0,     0,   746,   422,
       0,     0,     0,     0,     0,   367,     0,   367,     0,     0,
       0,     0,     0,     0,     0,     0,   816,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   423,   424,   735,     0,
       0,     0,     0,   829,     0,     0,   735,     0,     0,     0,
    1638,     0,   668,   978,     0,     0,     0,     0,   668,     0,
       0,     0,     0,  1708,     0,     0,     0,     0,     0,  1710,
     427,   855,     0,     0,   428,     0,  1349,   429,   668,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   668,
       0,     0,   430,     0,     0,     0,  1638,  1638,   431,   422,
     425,     0,     0,     0,   426,  1737,  1738,   300,     0,   746,
       0,     0,   669,   422,     0,   367,     0,     0,     0,     0,
     717,   910,     0,   911,     0,     0,   423,   424,   916,     0,
     367,     0,     0,  1638,   920,  1748,     0,   746,     0,     0,
     423,   424,     0,   929,     0,     0,     0,     0,     0,     0,
       0,   668,     0,     0,   932,   933,   934,   935,   936,   937,
       0,   945,     0,   945,     0,     0,     0,     0,     0,   427,
       0,     0,     0,   428,     0,  1464,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     425,   430,     0,     0,   426,     0,     0,   431,   702,     0,
       0,     0,   367,     0,   425,   540,     0,     0,   426,     0,
    1015,  1017,  1019,  1021,  1023,  1025,  1027,  1029,  1030,  1031,
       0,     0,  1032,  1034,  1036,  1037,  1038,  1039,  1323,  1042,
    1043,     0,  1046,  1047,  1048,  1050,  1051,  1052,  1054,  1056,
    1057,  1058,  1059,  1061,  1063,  1065,  1066,  1068,  1070,  1071,
    1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,   427,
    1089,     0,     0,   428,     0,  1469,   429,     0,     0,   746,
       0,     0,     0,   427,     0,  1100,   540,   428,     0,  1505,
     429,   430,     0,     0,     0,     0,     0,   431,  1356,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
     540,   431,     0,     0,   422,     0,     0,     0,  1371,     0,
       0,  1126,     0,     0,  1378,     0,     0,     0,  1131,     0,
       0,     0,     0,  1385,     0,     0,  1142,     0,  1144,     0,
       0,   423,   424,  1399,     0,     0,     0,     0,  1152,  1153,
    1154,  1155,  1156,  1157,  1158,  1159,  1160,  1161,  1162,  1163,
    1164,  1165,  1166,  1167,  1168,     0,     0,     0,     0,     0,
       0,     0,     0,  1422,     0,     0,     0,     0,     0,   746,
       0,     0,     0,     0,     0,  1432,     0,     0,     0,   702,
    1437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     725,   622,     0,     0,     0,   425,     0,     0,     0,   426,
       0,     0,     0,     0,  1221,     0,     0,   717,     0,     0,
     540,     0,     0,  1226,     0,     0,     0,     0,  1456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1238,
       0,     0,   717,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1482,
     702,   702,   702,     0,     0,   746,     0,   746,     0,   746,
       0,   746,     0,   746,   427,   746,     0,   746,   428,   746,
    1611,   429,     0,     0,   746,     0,   746,  1498,  1499,   540,
       0,  1502,   746,     0,     0,     0,   430,     0,     0,     0,
       0,     0,   431,  1002,   746,     0,   746,     0,     0,     0,
       0,   746,     0,   746,     0,   746,     0,     0,   746,     0,
     540,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   760,   761,     0,     0,     0,     0,     0,     0,     0,
    1279,     0,     0,     0,  1284,     0,     0,     0,     0,     0,
     746,     0,     0,     0,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,   746,   358,   359,   360,   361,     0,   540,   540,   540,
     540,   540,   540,   540,   540,     0,     0,     0,     0,     0,
     540,   540,     0,     0,     0,  1588,   540,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   540,     0,     0,     0,
     540,   540,   540,     0,   540,     0,   770,   771,   772,   773,
     774,     0,  1343,   777,   778,   779,   780,     0,   782,   783,
     784,   785,  1003,     0,     0,     0,   786,     0,   788,   789,
       0,     0,   540,     0,  1357,  1358,  1359,  1004,     0,  1362,
       0,     0,     0,     0,     0,     0,  1634,  1368,     0,     0,
     945,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1388,  1389,     0,     0,   540,     0,   666,     0,   568,
       0,     0,     0,     0,     0,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,     0,     0,     0,     0,     0,
       0,  1418,     0,  1420,   811,   812,     0,     0,  1423,     0,
       0,     0,     0,     0,  1426,     0,     0,     0,  1429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   712,
       0,     0,     0,     0,     0,   325,     0,     0,     0,   888,
       0,   326,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   328,  1452,     0,
       0,     0,     0,     0,     0,   329,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   717,
     330,     0,     0,   746,     0,     0,     0,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
       0,     0,  1016,  1018,  1020,  1022,  1024,  1026,  1028,     0,
       0,     0,     0,  1508,  1033,  1035,     0,     0,     0,     0,
    1041,     0,     0,     0,     0,     0,     0,     0,     0,  1053,
    1055,     0,  1397,     0,  1060,  1062,  1064,     0,  1067,    62,
       0,   760,   761,  1533,     0,  1534,     0,     0,     0,     0,
       0,  1538,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,  1549,     0,  1551,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1559,
    1560,  1561,     0,     0,     0,     0,  1568,     0,     0,     0,
       0,  1571,     0,     0,     0,  1575,  1576,  1577,  1578,     0,
       0,   717,  1581,     0,     0,   365,     0,     0,  1587,     0,
       0,     0,     0,     0,     0,     0,     0,  1600,     0,   717,
       0,     0,     0,     0,     0,     0,   770,   771,   772,   773,
     774,  1610,     0,   777,   778,   779,   780,     0,   782,   783,
     784,   785,     0,     0,     0,     0,   786,     0,   788,   789,
       0,     0,     0,     0,   792,   793,   794,     0,     0,     0,
     798,     0,     0,     0,     0,     0,   717,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1640,  1641,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1651,     0,     0,
       0,     0,  1657,   800,     0,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,  1664,   540,  1665,     0,     0,
       0,     0,     0,     0,   811,   812,     0,     0,     0,     0,
       0,     0,     0,     0,  1678,     0,  1679,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   717,  1698,     0,     0,     0,
       0,     0,     0,     0,  1703,     0,     0,     0,   448,     0,
    1706,  1707,   449,   450,     3,     0,   451,   452,   453,     0,
     454,     0,   455,   456,   457,   458,   459,     0,     0,     0,
       0,     0,   460,   461,   462,   463,   464,  1725,   465,  1727,
    1728,     0,     0,     0,     0,   466,   467,     0,  1734,   468,
       0,   469,   470,  1736,     0,   471,     0,     8,   472,   473,
       0,   474,   475,     0,     0,   476,   477,     0,     0,     0,
       0,     0,   478,     0,     0,   479,   480,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   481,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   482,
     483,   484,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   486,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     488,   489,   490,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   491,   492,
     493,   494,   495,     0,   496,     0,   497,   498,   499,   500,
      50,   146,   501,   502,   503,   504,   505,   506,   507,   508,
      63,   509,   510,   511,     0,     0,     0,     0,     0,     0,
     512,   147,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   514,   515,   516,
       0,    15,     0,     0,   517,   518,     0,     0,   449,   450,
       0,     0,   519,     0,   520,     0,   521,   522,   455,   456,
     457,   458,   459,     0,     0,     0,     0,     0,   460,     0,
     462,     0,     0,     0,   465,     0,   422,     0,     0,     0,
       0,     0,   467,     0,     0,     0,     0,     0,   470,     0,
       0,   471,     0,     0,   472,     0,   939,     0,   475,     0,
       0,     0,     0,   423,   424,     0,     0,     0,   584,     0,
       0,   479,   480,     0,   331,   332,   333,     0,   335,   336,
     337,   338,   339,   481,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,     0,   353,   354,   355,     0,
       0,   358,   359,   360,   361,   482,   483,   585,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,   487,     0,     0,     0,     0,   425,     0,     0,
       0,   426,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   491,   492,   493,   494,   495,     0,
     496,   940,   497,   498,   499,   500,    50,     0,     0,   502,
     503,   504,   505,   506,   507,   508,   941,   586,   510,   511,
       0,     0,     0,     0,     0,     0,   512,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
     428,     0,     0,   942,   515,   516,     0,    15,     0,     0,
     517,   518,     0,     0,     0,   449,   450,     0,   943,     0,
     944,     0,   521,   522,   431,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,   422,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
     423,   424,     0,     0,     0,   584,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   585,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,   425,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,   940,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,   941,   586,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,   428,     0,     0,
     942,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,     0,   449,   450,     0,   943,     0,   952,     0,   521,
     522,   431,   455,   456,   457,   458,   459,     0,     0,     0,
       0,     0,   460,     0,   462,     0,     0,     0,   465,     0,
     617,     0,     0,     0,     0,     0,   467,     0,     0,     0,
       0,     0,   470,     0,     0,   471,     0,     0,   472,     0,
       0,     0,   475,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   584,     0,     0,   479,   480,     0,   331,   332,
     333,     0,   335,   336,   337,   338,   339,   481,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,     0,
     353,   354,   355,     0,     0,   358,   359,   360,   361,   482,
     483,   585,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   486,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   491,   492,
     493,   494,   495,     0,   496,     0,   497,   498,   499,   500,
      50,     0,     0,   502,   503,   504,   505,   506,   507,   508,
      63,   586,   510,   511,     0,     0,     0,     0,     0,     0,
     512,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   618,     0,     0,   514,   515,   516,
       0,    15,     0,     0,   517,   518,     0,     0,     0,   449,
     450,     0,  1199,     0,   520,     0,   521,   522,   620,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   584,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   484,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,   664,     0,     0,     0,     0,     0,   488,   489,   490,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   586,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,     0,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   584,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   484,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,   695,     0,     0,
       0,     0,     0,   488,   489,   490,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   586,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,     0,   521,
     522,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,  1593,   462,   463,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,   473,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   584,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     585,  1594,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     586,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,     0,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   484,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,   489,   490,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   586,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
       0,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,     0,     0,   479,   480,   988,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   585,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,   940,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,   941,   586,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   989,     0,   520,   990,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   584,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   484,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1010,  1011,  1012,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   586,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,     0,     0,   449,   450,   519,
       0,   520,     0,   521,   522,   730,     0,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,   731,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   585,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   586,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,     0,     0,   449,   450,   519,   621,   520,
       0,   521,   522,   730,     0,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,   731,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   584,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   585,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,   940,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,   941,   586,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,     0,     0,   449,   450,   519,     0,   520,     0,   521,
     522,   730,     0,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,   731,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   585,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   586,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,     0,
       0,   449,   450,   519,   853,   520,     0,   521,   522,   730,
       0,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,   731,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   584,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     585,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     586,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,     0,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,     0,     0,
     479,   480,  1137,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   585,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
     940,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,   941,   586,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
       0,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   585,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   586,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,   621,   520,     0,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,   673,     0,     0,     0,   584,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   585,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   586,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,     0,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,   684,     0,
       0,     0,     0,     0,     0,   584,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   585,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   586,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,     0,     0,   449,   450,   519,     0,   520,     0,   521,
     522,   716,     0,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   585,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   586,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,     0,   520,     0,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   584,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   585,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   586,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   720,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,     0,     0,   449,   450,   519,
       0,   520,     0,   521,   522,   741,     0,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   585,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   586,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
       0,   521,   522,   455,   456,   457,   458,   459,     0,     0,
    1069,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   585,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   586,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,     0,   520,     0,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   584,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   585,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   586,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,  1090,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   584,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   585,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   586,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1141,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,     0,   521,
     522,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   584,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     585,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     586,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,  1200,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   585,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   586,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
    1215,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   585,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   586,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,     0,   520,  1421,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   584,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   585,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   586,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,  1430,
       0,   520,  1431,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   584,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   585,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   586,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,  1436,   521,
     522,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   584,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     585,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     586,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,  1481,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   585,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   586,   510,   511,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,   449,   450,     0,     0,   519,     0,   520,
    1567,   521,   522,   455,   456,   457,   458,   459,     0,     0,
       0,     0,     0,   460,     0,   462,     0,     0,     0,   465,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,   470,     0,     0,   471,     0,     0,   472,
       0,     0,     0,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   584,     0,     0,   479,   480,     0,   331,
     332,   333,     0,   335,   336,   337,   338,   339,   481,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
       0,   353,   354,   355,     0,     0,   358,   359,   360,   361,
     482,   483,   585,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,  1609,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   491,
     492,   493,   494,   495,     0,   496,     0,   497,   498,   499,
     500,    50,     0,     0,   502,   503,   504,   505,   506,   507,
     508,    63,   586,   510,   511,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,     0,    15,     0,     0,   517,   518,     0,     0,   449,
     450,     0,     0,   519,     0,   520,     0,   521,   522,   455,
     456,   457,   458,   459,     0,     0,     0,     0,     0,   460,
       0,   462,     0,     0,     0,   465,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,   470,
       0,     0,   471,     0,     0,   472,     0,     0,     0,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   584,
       0,     0,   479,   480,     0,   331,   332,   333,     0,   335,
     336,   337,   338,   339,   481,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,     0,   353,   354,   355,
       0,     0,   358,   359,   360,   361,   482,   483,   585,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,  1655,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   491,   492,   493,   494,   495,
       0,   496,     0,   497,   498,   499,   500,    50,     0,     0,
     502,   503,   504,   505,   506,   507,   508,    63,   586,   510,
     511,     0,     0,     0,     0,     0,     0,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,     0,    15,     0,
       0,   517,   518,     0,     0,   449,   450,     0,     0,   519,
       0,   520,     0,   521,   522,   455,   456,   457,   458,   459,
       0,     0,     0,     0,     0,   460,     0,   462,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,   470,     0,     0,   471,     0,
       0,   472,     0,     0,     0,   475,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   584,     0,     0,   479,   480,
       0,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,   482,   483,   585,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,  1656,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   491,   492,   493,   494,   495,     0,   496,     0,   497,
     498,   499,   500,    50,     0,     0,   502,   503,   504,   505,
     506,   507,   508,    63,   586,   510,   511,     0,     0,     0,
       0,     0,     0,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,     0,    15,     0,     0,   517,   518,     0,
       0,   449,   450,     0,     0,   519,     0,   520,     0,   521,
     522,   455,   456,   457,   458,   459,     0,     0,     0,     0,
       0,   460,     0,   462,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,     0,
       0,   470,     0,     0,   471,     0,     0,   472,     0,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   584,     0,     0,   479,   480,     0,   331,   332,   333,
       0,   335,   336,   337,   338,   339,   481,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,   353,
     354,   355,     0,     0,   358,   359,   360,   361,   482,   483,
     585,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   491,   492,   493,
     494,   495,     0,   496,     0,   497,   498,   499,   500,    50,
       0,     0,   502,   503,   504,   505,   506,   507,   508,    63,
     586,   510,   511,     0,     0,     0,     0,     0,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,     0,
      15,     0,     0,   517,   518,     0,     0,   449,   450,     0,
       0,   519,     0,   520,     0,   521,   522,   455,   456,   457,
     458,   459,     0,     0,     0,     0,     0,   460,     0,   462,
       0,     0,     0,   465,     0,     0,     0,     0,     0,     0,
       0,   467,     0,     0,     0,     0,     0,   470,     0,     0,
     471,     0,     0,   472,     0,     0,     0,   475,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   584,     0,     0,
     479,   480,     0,   331,   332,   333,     0,   335,   336,   337,
     338,   339,   481,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,     0,   353,   354,   355,     0,     0,
     358,   359,   360,   361,   482,   483,   585,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,   -76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   760,   761,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   491,   492,   493,   494,   495,     0,   496,
       0,   497,   498,   499,   500,    50,     0,     0,   502,   503,
     504,   505,   506,   507,   508,    63,   586,   510,   511,     0,
       0,     0,     0,   760,   761,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,     0,    15,     0,     0,   517,
     518,     0,     0,     0,     0,     0,     0,  1404,     0,   520,
       0,   521,   522,   762,   763,   764,   765,   766,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,     0,     0,     0,
       0,   786,   787,   788,   789,   790,   791,     0,     0,   792,
     793,   794,   795,   796,   797,   798,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   770,   771,
     772,   773,   774,     0,     0,   777,   778,   779,   780,     0,
     782,   783,   784,   785,     0,     0,     0,     0,   786,     0,
     788,   789,     0,     0,     0,     0,   792,   799,   800,     0,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   810,
     714,     0,     0,     0,     0,     0,   325,     0,     0,   811,
     812,     0,   326,     0,   521,   700,     0,     0,   327,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   328,     0,
       0,     0,     0,     0,     0,     0,   329,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,     0,     0,     0,
       0,   330,     0,     0,     0,     0,   811,   812,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   325,
       0,     0,     0,     0,     0,   326,     0,     0,     0,     0,
       0,   327,     0,     0,     0,     0,     0,     0,     0,     0,
      62,   328,     0,     0,     0,     0,     0,     0,     0,   329,
       0,     0,     0,   364,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   330,     0,     0,     0,     0,     0,
      63,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   365,   760,   761,     0,
       0,     0,   325,     0,     0,     0,     0,     0,   326,     0,
       0,     0,     0,     0,   327,     0,     0,     0,     0,     0,
       0,     0,     0,    62,   328,     0,     0,     0,     0,     0,
       0,     0,   329,     0,     0,     0,   364,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,     0,     0,
       0,     0,     0,    63,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,     0,     0,     0,
       0,     0,   770,   771,   772,   773,     0,     0,     0,   365,
       0,   624,   780,     0,   782,   783,   784,   785,     0,     0,
       0,     0,   786,     0,   788,   789,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   364,
       0,     0,     0,     0,     0,     0,     0,     0,    13,     0,
       0,   325,     0,     0,     0,     0,   630,   326,     0,     0,
       0,     0,     0,   327,     0,     0,     0,     0,    14,     0,
       0,     0,     0,   328,   631,     0,   806,   807,   808,   809,
     810,   329,     0,     0,     0,     0,     0,     0,     0,     0,
     811,   812,     0,     0,     0,     0,   330,     0,     0,     0,
       0,     0,   365,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   325,     0,     0,     0,     0,     0,
     326,     0,     0,     0,     0,     0,   327,     0,     0,     0,
       0,     0,     0,     0,     0,    62,   328,     0,     0,     0,
       0,     0,     0,     0,   329,     0,     0,     0,   364,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,   330,
       0,     0,     0,     0,     0,    63,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,     0,
     760,   761,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   364,   770,   771,   772,   773,   774,     0,     0,   777,
     778,   779,   780,     0,   782,   783,   784,   785,   630,     0,
       0,     0,   786,     0,   788,   789,     0,     0,     0,     0,
     792,     0,   794,     0,     0,     0,     0,   890,   891,   892,
     893,   894,   895,   896,   897,   770,   771,   772,   773,   774,
     898,   899,   777,   778,   779,   780,   900,   782,   783,   784,
     785,     0,     0,     0,   365,   786,   787,   788,   789,   901,
     902,   760,   761,   792,   793,   794,   903,   904,   905,   798,
       0,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     810,     0,     0,     0,    13,     0,     0,     0,     0,     0,
     811,   812,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,   906,   800,     0,   801,   802,   803,   804,   805,   806,
     807,   808,   809,   810,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   811,   812,     0,     0,     0,   521,   700,
       0,     0,     0,     0,     0,     0,     0,     0,   890,   891,
     892,   893,   894,   895,   896,   897,   770,   771,   772,   773,
     774,   898,   899,   777,   778,   779,   780,   900,   782,   783,
     784,   785,  -377,     0,   760,   761,   786,   787,   788,   789,
     901,   902,     0,     0,   792,   793,   794,   903,   904,   905,
     798,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   760,
     761,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   906,   800,     0,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   811,   812,     0,     0,     0,   521,
     700,   890,   891,   892,   893,   894,   895,   896,   897,   770,
     771,   772,   773,   774,   898,   899,   777,   778,   779,   780,
     900,   782,   783,   784,   785,     0,     0,     0,     0,   786,
     787,   788,   789,   901,   902,     0,     0,   792,   793,   794,
     903,   904,   905,   798,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
     760,   761,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,     0,
       0,     0,     0,     0,     0,   906,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   760,   761,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,     0,   521,   700,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,     0,   521,   700,     0,
       0,     0,     0,     0,     0,   770,   771,   772,   773,   774,
       0,     0,   777,   778,   779,   780,     0,   782,   783,   784,
     785,     0,     0,     0,     0,   786,     0,   788,   789,     0,
       0,     0,     0,   792,   793,   794,     0,     0,     0,   798,
       0,     0,     0,   770,   771,   772,   773,   774,     0,     0,
     777,   778,   779,   780,     0,   782,   783,   784,   785,   760,
     761,     0,     0,   786,     0,   788,   789,     0,     0,     0,
       0,   792,   793,   794,     0,     0,     0,   798,     0,     0,
       0,     0,   800,     0,   801,   802,   803,   804,   805,   806,
     807,   808,   809,   810,   760,   761,     0,     0,     0,     0,
       0,     0,     0,   811,   812,     0,     0,   824,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     800,     0,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   811,   812,     0,     0,   834,     0,     0,     0,     0,
       0,     0,     0,     0,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
       0,     0,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,   760,   761,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,   760,   761,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,   850,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,   862,     0,     0,     0,     0,     0,     0,     0,     0,
     770,   771,   772,   773,   774,     0,     0,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,     0,     0,   792,   793,
     794,     0,     0,     0,   798,     0,     0,     0,   770,   771,
     772,   773,   774,     0,     0,   777,   778,   779,   780,     0,
     782,   783,   784,   785,   760,   761,     0,     0,   786,     0,
     788,   789,     0,     0,     0,     0,   792,   793,   794,     0,
       0,     0,   798,     0,     0,     0,     0,   800,     0,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   760,
     761,     0,     0,     0,     0,     0,     0,     0,   811,   812,
       0,     0,  1173,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   800,     0,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   811,   812,     0,     0,
    1177,     0,     0,     0,     0,     0,     0,     0,     0,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,     0,     0,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
     760,   761,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   760,   761,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,  1179,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,  1188,     0,     0,     0,
       0,     0,     0,     0,     0,   770,   771,   772,   773,   774,
       0,     0,   777,   778,   779,   780,     0,   782,   783,   784,
     785,     0,     0,     0,     0,   786,     0,   788,   789,     0,
       0,     0,     0,   792,   793,   794,     0,     0,     0,   798,
       0,     0,     0,   770,   771,   772,   773,   774,     0,     0,
     777,   778,   779,   780,     0,   782,   783,   784,   785,   760,
     761,     0,     0,   786,     0,   788,   789,     0,     0,     0,
       0,   792,   793,   794,     0,     0,     0,   798,     0,     0,
       0,     0,   800,     0,   801,   802,   803,   804,   805,   806,
     807,   808,   809,   810,   760,   761,     0,     0,     0,     0,
       0,     0,     0,   811,   812,     0,     0,  1189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     800,     0,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   811,   812,     0,     0,  1190,     0,     0,     0,     0,
       0,     0,     0,     0,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
       0,     0,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,   760,   761,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,   760,   761,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,  1191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,  1192,     0,     0,     0,     0,     0,     0,     0,     0,
     770,   771,   772,   773,   774,     0,     0,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,     0,     0,   792,   793,
     794,     0,     0,     0,   798,     0,     0,     0,   770,   771,
     772,   773,   774,     0,     0,   777,   778,   779,   780,     0,
     782,   783,   784,   785,   760,   761,     0,     0,   786,     0,
     788,   789,     0,     0,     0,     0,   792,   793,   794,     0,
       0,     0,   798,     0,     0,     0,     0,   800,     0,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   760,
     761,     0,     0,     0,     0,     0,     0,     0,   811,   812,
       0,     0,  1193,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   800,     0,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   811,   812,     0,     0,
    1318,     0,     0,     0,     0,     0,     0,     0,     0,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,     0,     0,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
     760,   761,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   760,   761,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,  1331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,  1334,     0,     0,     0,
       0,     0,     0,     0,     0,   770,   771,   772,   773,   774,
       0,     0,   777,   778,   779,   780,     0,   782,   783,   784,
     785,     0,     0,     0,     0,   786,     0,   788,   789,     0,
       0,     0,     0,   792,   793,   794,     0,     0,     0,   798,
       0,     0,     0,   770,   771,   772,   773,   774,     0,     0,
     777,   778,   779,   780,     0,   782,   783,   784,   785,   760,
     761,     0,     0,   786,     0,   788,   789,     0,     0,     0,
       0,   792,   793,   794,     0,     0,     0,   798,     0,     0,
       0,     0,   800,     0,   801,   802,   803,   804,   805,   806,
     807,   808,   809,   810,   760,   761,     0,     0,     0,     0,
       0,     0,     0,   811,   812,     0,     0,  1484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     800,     0,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   811,   812,     0,     0,  1503,     0,     0,     0,     0,
       0,     0,     0,     0,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
       0,     0,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,   760,   761,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,   760,   761,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,  1504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,  1527,     0,     0,     0,     0,     0,     0,     0,     0,
     770,   771,   772,   773,   774,     0,     0,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,     0,     0,   792,   793,
     794,     0,     0,     0,   798,     0,     0,     0,   770,   771,
     772,   773,   774,     0,     0,   777,   778,   779,   780,     0,
     782,   783,   784,   785,   760,   761,     0,     0,   786,     0,
     788,   789,     0,     0,     0,     0,   792,   793,   794,     0,
       0,     0,   798,     0,     0,     0,     0,   800,     0,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   760,
     761,     0,     0,     0,     0,     0,     0,     0,   811,   812,
       0,     0,  1529,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   800,     0,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   811,   812,     0,     0,
    1531,     0,     0,     0,     0,     0,     0,     0,     0,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,     0,     0,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
     760,   761,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   760,   761,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,  1535,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,  1614,     0,     0,     0,
       0,     0,     0,     0,     0,   770,   771,   772,   773,   774,
       0,     0,   777,   778,   779,   780,     0,   782,   783,   784,
     785,     0,     0,     0,     0,   786,     0,   788,   789,     0,
       0,     0,     0,   792,   793,   794,     0,     0,     0,   798,
       0,     0,     0,   770,   771,   772,   773,   774,     0,     0,
     777,   778,   779,   780,     0,   782,   783,   784,   785,   760,
     761,     0,     0,   786,     0,   788,   789,     0,     0,     0,
       0,   792,   793,   794,     0,     0,     0,   798,     0,     0,
       0,     0,   800,     0,   801,   802,   803,   804,   805,   806,
     807,   808,   809,   810,   760,   761,     0,     0,     0,     0,
       0,     0,     0,   811,   812,     0,     0,  1615,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     800,     0,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   811,   812,     0,     0,  1617,     0,     0,     0,     0,
       0,     0,     0,     0,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
       0,     0,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,   760,   761,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,   760,   761,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,  1625,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,  1627,     0,     0,     0,     0,     0,     0,     0,     0,
     770,   771,   772,   773,   774,     0,     0,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,     0,     0,   792,   793,
     794,     0,     0,     0,   798,     0,     0,     0,   770,   771,
     772,   773,   774,     0,     0,   777,   778,   779,   780,     0,
     782,   783,   784,   785,   760,   761,     0,     0,   786,     0,
     788,   789,     0,     0,     0,     0,   792,   793,   794,     0,
       0,     0,   798,     0,     0,     0,     0,   800,     0,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   760,
     761,     0,     0,     0,     0,     0,     0,     0,   811,   812,
       0,     0,  1637,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   800,     0,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   811,   812,     0,     0,
    1642,     0,     0,     0,     0,     0,     0,     0,     0,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,     0,     0,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
     760,   761,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,   760,   761,
       0,     0,     0,     0,     0,     0,     0,   811,   812,     0,
       0,  1717,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   811,   812,     0,     0,  1718,     0,     0,     0,
       0,     0,     0,     0,     0,   770,   771,   772,   773,   774,
       0,     0,   777,   778,   779,   780,     0,   782,   783,   784,
     785,     0,     0,     0,     0,   786,     0,   788,   789,     0,
       0,     0,     0,   792,   793,   794,     0,     0,     0,   798,
       0,     0,     0,   770,   771,   772,   773,   774,     0,     0,
     777,   778,   779,   780,     0,   782,   783,   784,   785,   760,
     761,     0,     0,   786,     0,   788,   789,     0,     0,     0,
       0,   792,   793,   794,     0,     0,     0,   798,     0,     0,
       0,     0,   800,     0,   801,   802,   803,   804,   805,   806,
     807,   808,   809,   810,   760,   761,     0,     0,     0,     0,
       0,     0,     0,   811,   812,   854,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     800,     0,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   811,   812,  1123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
       0,     0,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,   770,
     771,   772,   773,   774,     0,     0,   777,   778,   779,   780,
       0,   782,   783,   784,   785,   760,   761,     0,     0,   786,
       0,   788,   789,     0,     0,     0,     0,   792,   793,   794,
       0,     0,     0,   798,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,   760,   761,     0,     0,     0,     0,     0,
       0,     0,   811,   812,  1271,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   800,     0,   801,   802,
     803,   804,   805,   806,   807,   808,   809,   810,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   811,   812,  1287,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     770,   771,   772,   773,   774,     0,     0,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,     0,     0,   792,   793,
     794,     0,     0,     0,   798,     0,     0,     0,   770,   771,
     772,   773,   774,     0,     0,   777,   778,   779,   780,     0,
     782,   783,   784,   785,   245,   246,     0,     0,   786,     0,
     788,   789,     0,     0,     0,     0,   792,   793,   794,     0,
       0,   247,   798,     0,     0,     0,     0,   800,     0,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,     0,
       0,     0,     0,     0,     0,     0,   760,   761,   811,   812,
    1428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   800,     0,   801,   802,   803,
     804,   805,   806,   807,   808,   809,   810,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   811,   812,  1434,     0,
       0,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,     0,
       0,   266,   267,   268,     0,     0,     0,     0,     0,     0,
     269,   270,   271,   272,   273,     0,     0,   274,   275,   276,
     277,   278,   279,   280,     0,     0,     0,     0,     0,   760,
     761,   770,   771,   772,   773,   774,     0,     0,   777,   778,
     779,   780,     0,   782,   783,   784,   785,     0,     0,     0,
       0,   786,     0,   788,   789,     0,     0,     0,     0,   792,
     793,   794,     0,     0,     0,   798,   281,     0,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,    50,     0,
     292,   293,     0,     0,     0,     0,     0,   294,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   800,     0,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   810,
       0,     0,   760,   761,   770,   771,   772,   773,   774,   811,
     812,   777,   778,   779,   780,     0,   782,   783,   784,   785,
       0,     0,     0,     0,   786,     0,   788,   789,     0,     0,
       0,     0,   792,   793,   794,     0,     0,     0,   798,     0,
       0,     0,   760,   761,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,   800,     0,   801,   802,   803,   804,   805,   806,   807,
     808,   809,   810,     0,     0,     0,     0,   770,   771,   772,
     773,   774,   811,   812,   777,   778,   779,   780,     0,   782,
     783,   784,   785,     0,     0,     0,     0,   786,     0,   788,
     789,     0,     0,   956,     0,   792,   793,   794,     0,     0,
       0,   798,     0,     0,     0,   760,   761,   770,   771,   772,
     773,   774,     0,     0,   777,   778,   779,   780,     0,   782,
     783,   784,   785,     0,     0,     0,     0,   786,     0,   788,
     789,     0,     0,     0,     0,   792,   793,   794,     0,     0,
       0,   798,   760,   761,   800,     0,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   810,   186,     0,     0,     0,
       0,     0,     0,     0,     0,   811,   812,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   800,     0,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   810,     0,     0,     0,     0,
     770,   771,   772,   773,   774,   811,   812,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,  1201,     0,   792,   793,
     794,     0,     0,     0,   798,   760,   761,   770,   771,   772,
     773,   774,     0,     0,   777,   778,   779,   780,     0,   782,
     783,   784,   785,     0,     0,     0,     0,   786,     0,   788,
     789,     0,     0,     0,     0,   792,   793,   794,     0,     0,
       0,   798,   760,   761,     0,     0,     0,   800,     0,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   811,   812,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   800,  1276,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   810,     0,     0,     0,     0,
     770,   771,   772,   773,   774,   811,   812,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,     0,     0,   792,   793,
     794,     0,     0,     0,   798,   760,   761,   770,   771,   772,
     773,   774,     0,     0,   777,   778,   779,   780,     0,   782,
     783,   784,   785,     0,     0,     0,     0,   786,     0,   788,
     789,     0,     0,     0,     0,   792,   793,   794,     0,     0,
       0,   798,   760,   761,     0,     0,     0,   800,  1381,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   811,   812,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   800,     0,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   810,     0,     0,     0,     0,
     770,   771,   772,   773,   774,   811,   812,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,     0,     0,   792,   793,
     794,     0,     0,     0,  -778,   760,   761,   770,   771,   772,
     773,   774,     0,     0,   777,   778,   779,   780,     0,   782,
     783,   784,   785,     0,     0,     0,     0,   786,     0,   788,
     789,   760,   761,     0,     0,   792,   793,   794,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   800,     0,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   760,
     761,     0,     0,     0,     0,     0,     0,     0,   811,   812,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   800,     0,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   810,     0,     0,     0,     0,
     770,   771,   772,   773,   774,   811,   812,   777,   778,   779,
     780,     0,   782,   783,   784,   785,     0,     0,     0,     0,
     786,     0,   788,   789,     0,     0,   770,   771,   772,   773,
     774,     0,     0,   777,   778,   779,   780,     0,   782,   783,
     784,   785,     0,     0,     0,     0,   786,     0,   788,   789,
       0,     0,   760,   761,   770,   771,   772,   773,   774,     0,
       0,   777,   778,   779,   780,     0,   782,   783,   784,   785,
       0,     0,     0,     0,   786,     0,   788,   789,     0,     0,
     802,   803,   804,   805,   806,   807,   808,   809,   810,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   811,   812,
       0,     0,     0,     0,     0,     0,     0,   803,   804,   805,
     806,   807,   808,   809,   810,     0,     0,     0,     0,  1006,
       0,     0,     0,     0,   811,   812,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   804,   805,   806,   807,
     808,   809,   810,     0,     0,     0,     0,   770,   771,   772,
     773,   774,   811,   812,   777,     0,     0,   780,     0,   782,
     783,   784,   785,     0,     0,     0,     0,   786,     0,   788,
     789,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,     0,   358,   359,
     360,   361,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   804,
     805,   806,   807,   808,   809,   810,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   811,   812,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1007,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     331,   332,   333,  1008,   335,   336,   337,   338,   339,   481,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,     0,   353,   354,   355,     0,     0,   358,   359,   360,
     361,   331,   332,   333,     0,   335,   336,   337,   338,   339,
     481,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   353,   354,   355,     0,   194,   358,   359,
     360,   361,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1273,     0,  1081,
    1082,     0,     0,   195,     0,   196,     0,   197,   198,   199,
     200,   201,  1274,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,     0,   213,   214,   215,  1083,     0,
     216,   217,   218,   219,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1084,     0,     0,     0,     0,     0,     0,
     220,   221,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1085,  1086,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   222
};

static const yytype_int16 yycheck[] =
{
       1,   188,   230,     7,   500,   591,   390,   224,   645,   614,
     404,   672,   540,   390,   438,    15,    16,   735,   437,    20,
     841,   240,   843,   171,   845,   557,   744,   650,   715,   947,
     877,   931,   437,   438,   398,   953,   605,    86,   607,   433,
     609,  1204,   924,  1407,    22,   926,     5,   461,    20,   581,
     489,   490,    20,     4,   109,    20,     5,     6,    33,    20,
      20,    53,    10,    33,  1590,   713,   143,   715,    68,    69,
      70,   485,   173,    20,   163,    20,    25,    20,   166,    57,
     165,   152,    31,   311,   461,    20,    20,  1475,   173,   712,
      53,   714,    20,   716,   183,    96,   173,    98,    53,     0,
     160,   749,   173,     8,   498,   499,     7,   730,   485,   152,
     110,   111,   112,   113,     5,     6,   217,     8,   741,    68,
      69,    10,   144,   145,   146,   129,   152,  1653,    46,    30,
     173,    32,   127,    34,   160,    19,    20,    15,    16,    40,
     217,   988,    97,   102,   103,    36,   217,   173,   149,    50,
    1618,   165,  1516,   102,   103,    56,   155,   191,  1546,  1547,
     220,   160,   390,    62,   168,    15,    16,   315,   886,   388,
     702,   185,   191,   165,   217,   192,   231,  1565,  1566,    80,
     191,   595,   216,   232,    57,   189,   190,   191,   106,   194,
      63,   217,   173,   185,   143,   158,   218,   192,  1361,   218,
     195,   102,   103,  1671,   922,   392,   220,   925,   436,   160,
     173,   160,   161,   162,   163,   163,   107,   165,   595,   191,
     224,   220,   197,   191,   173,   203,   191,   197,   633,   877,
     191,   191,   233,   638,   183,   183,   214,   185,   219,   198,
     240,  1629,  1630,   191,   191,   215,   191,   464,   191,   198,
     664,   221,   875,    21,    22,   143,   191,   191,   217,   164,
     165,   478,   191,   191,   165,  1147,   215,  1148,   216,   318,
     218,   129,   130,   127,   163,   173,   165,   671,   152,   184,
     185,   695,  1200,   137,   185,   173,  1186,   664,   166,   173,
    1137,   164,   158,   171,   183,   173,   185,   217,   176,   173,
     694,   132,   191,   204,   143,   158,   310,   173,    33,   313,
     215,   184,    21,    22,   215,   127,   166,   163,   695,   758,
     173,   171,  1485,   173,     7,   137,   176,   216,   163,   218,
     217,   143,   219,   220,   165,    60,    61,   183,   192,   217,
     988,   989,  1242,    34,   967,   968,   164,   970,   183,   577,
     118,   119,   218,   581,   185,   194,   214,   215,   126,   194,
     128,   129,   130,   131,   217,   365,   184,    50,   136,    57,
     163,   217,    63,   191,    33,    63,   166,   596,   597,   598,
     192,   600,   601,   188,   189,   604,   387,   606,   388,   608,
     183,  1578,   756,  1580,   394,   582,  1296,   127,   191,   124,
    1587,    60,    61,   128,   404,   127,   214,   137,   636,   118,
     119,   216,   640,   827,  1601,   137,   107,   126,   155,   128,
     129,   130,   131,   160,   191,   194,   191,   136,   833,   823,
     199,  1000,   649,   433,   202,   203,   204,   437,   240,   165,
    1261,   165,  1360,   173,   849,   136,   214,   215,   217,   216,
     827,   163,   143,   218,  1372,   987,   870,   457,   458,   676,
     464,   185,   192,   959,  1651,   124,   192,   197,   193,   128,
     192,   183,   197,   474,   478,   200,   126,   871,   872,   129,
     130,   218,   173,   220,   488,   489,   490,   158,   984,  1137,
     215,   885,  1129,   870,   218,    57,   221,   152,   498,   499,
     939,    63,   173,   908,   721,   214,   215,   219,   220,   127,
    1697,  1698,   127,  1200,   742,    21,    22,   208,   173,   137,
     914,   915,   739,   917,   525,   526,   217,   921,  1215,   923,
     143,  1623,   198,  1354,   193,   173,   174,   175,   197,   152,
     199,   200,   543,  1635,   545,   546,   547,  1734,   549,   165,
     194,   217,  1200,   197,   142,  1203,   215,   461,   191,   560,
     173,   144,   221,   146,   214,   215,   204,  1215,   127,   185,
    1391,  1010,  1011,  1012,   192,   163,   191,   192,   137,   194,
     581,   127,   197,   165,   191,   218,   388,    57,  1680,  1681,
     591,   137,   143,    63,   165,   183,   596,   597,   598,   165,
     600,   601,   404,   185,   604,   173,   606,  1230,   608,   191,
     610,   218,   118,   119,   185,   519,   520,   199,  1187,   185,
     126,   165,   188,   129,   130,   131,   191,   191,   199,   816,
     136,   433,   851,   192,   143,   437,   191,   856,   191,   191,
     859,   185,   191,   152,   173,   649,   192,   191,   157,   868,
     216,   191,   216,   218,    33,   199,   656,   876,    68,    69,
      70,   191,   881,   218,   173,   218,   218,  1236,   191,   218,
     158,   671,   676,   821,   177,   178,   179,    57,   218,   165,
    1341,    60,    61,    63,   191,   173,   165,   165,   218,   198,
     907,  1512,   191,   216,   694,  1119,   498,   499,  1117,   185,
     110,   111,   112,   113,   165,   191,   185,   185,   214,   215,
     614,   218,   191,   199,   191,  1120,   191,   721,   719,   218,
     199,   199,   173,    57,   185,   158,   191,  1364,   191,    63,
     191,   144,   145,   146,   195,   739,   736,   165,    33,   216,
     173,   745,   961,   218,   963,   124,   158,   165,   127,   128,
     165,   191,  1146,   218,   758,   218,  1404,   185,   137,   663,
     664,   173,   185,   191,   191,    60,    61,   185,   191,   194,
     185,   199,   191,   191,   199,   191,   191,  1171,   218,   177,
    1185,   199,  1430,   165,   199,   689,    21,    22,   692,   216,
     165,   173,   217,   980,   596,   597,   598,   216,   600,   601,
     216,   195,   604,   185,   606,   199,   608,   194,   610,   191,
     185,   165,   199,   192,   193,   173,   191,   817,   197,  1224,
    1225,   200,   173,   823,   199,  1353,   165,   173,   829,   124,
     217,   185,  1401,   128,   164,   194,   215,   191,   839,    57,
     199,  1235,   221,   165,  1449,   749,   185,  1484,   198,   173,
      33,   851,   191,   165,   184,    47,   856,  1241,   217,   859,
     199,   165,   184,   185,  1241,   187,   195,   217,   868,   671,
     199,   871,   872,   185,   177,    67,   876,    60,    61,   191,
     164,   881,   144,   118,   119,   885,  1105,   199,   165,   173,
     106,   126,   694,   128,   129,   130,   131,  1558,   193,   198,
     184,   136,   197,   907,   199,   200,    35,   184,   185,   186,
     814,   198,   198,   158,   914,   915,  1634,   917,   217,   194,
     215,   921,   197,   923,   924,  1330,   221,  1645,   173,   192,
     217,   217,  1518,  1338,   191,   939,    12,   194,    57,   195,
     197,   124,   173,   199,    63,   128,   947,    23,    24,   950,
      35,  1556,   953,   954,   184,   216,  1674,   187,   760,   761,
     190,   961,   217,   963,    57,   200,   201,   202,   203,   204,
      63,   173,   185,   877,    22,   185,   185,  1582,   191,   214,
     215,   191,   191,  1631,   786,   889,   185,   185,  1636,   173,
     195,   195,   191,   191,   199,   199,   216,  1493,    33,   195,
     195,     5,     6,   199,   199,  1642,  1010,  1011,  1012,   811,
     193,   219,   220,  1241,   197,  1511,   199,   200,   195,  1605,
    1606,   823,   199,  1417,  1672,    60,    61,   195,   173,   195,
     195,   199,   215,   199,   199,   177,   178,    33,   221,   943,
     944,   177,   178,   179,   180,    33,    92,    93,   952,   851,
     173,   174,   175,   961,   856,   963,   173,   859,   217,   177,
     178,   179,   194,   194,    60,    61,   868,    43,   972,   871,
     872,   198,    60,    61,   876,  1661,    10,    11,    12,   881,
     217,   173,    33,   885,   198,   989,   198,   198,   198,   124,
     198,   217,   127,   128,  1742,  1096,   198,   198,  1317,  1100,
     198,   198,   137,   198,   217,  1105,  1323,   195,  1295,    60,
      61,    33,   914,   915,   137,   917,   216,  1117,   217,   921,
     173,   923,   924,  1517,   173,   215,   173,   165,   124,   198,
     165,   198,   128,   217,   198,   217,   124,   173,    60,    61,
     128,   198,  1143,   198,   217,   166,  1146,  1147,   198,   217,
     185,  1647,   217,   198,   198,   461,   198,   192,   193,   961,
      13,   963,   197,   217,   198,   200,    33,    10,   220,   165,
      66,  1171,    37,   124,   217,  1392,   217,   128,   217,   198,
     215,   198,   215,    43,   173,   217,   221,   217,   173,   185,
     218,   216,   191,    60,    61,   191,   194,   193,    43,  1416,
    1419,   197,   124,   199,   200,   193,   128,   217,   217,   197,
    1604,   199,   200,    75,   520,   217,   198,    79,   198,   215,
     198,   191,   132,    14,   192,   221,   166,   215,   194,   220,
    1134,    93,    94,   221,   217,  1235,    98,    99,   100,   101,
      13,   216,   193,   191,   191,   173,   197,     8,   199,   200,
     173,  1438,   173,   199,   218,   173,   173,   124,    33,   218,
     217,   128,   191,  1480,   215,   191,   173,  1069,   184,   218,
     221,   193,   217,   217,   217,   197,   218,   199,   200,   198,
     217,    33,   217,   198,   217,    60,    61,   217,  1289,     1,
    1194,  1508,   217,   215,   173,  1199,    67,   217,   199,   221,
     199,   199,    43,  1105,   217,   173,   218,   218,    60,    61,
    1214,   216,   218,  1532,   217,  1117,  1220,  1317,   218,  1323,
     198,   218,   217,   216,   216,  1229,   193,   218,   217,   173,
     197,  1548,   199,   200,   173,  1239,   173,   173,   173,   173,
     217,   217,  1343,  1247,  1146,  1147,   217,   217,   215,   124,
     181,   173,   218,   128,   221,   218,   217,   663,   664,  1360,
     217,   216,   216,   173,   216,  1269,    33,   217,   217,  1171,
     217,  1372,   124,   173,   199,   217,   128,  1281,   218,   217,
      70,   199,  1286,   689,   217,  1602,   692,   217,  1392,   217,
     217,   199,   218,   177,   218,   185,   217,  1398,   217,   185,
     217,   217,    53,   218,   218,   218,   218,   216,   184,   191,
     216,   185,  1416,   191,   218,   218,  1644,  1417,   193,  1419,
    1324,   218,   197,   218,   199,   200,   218,   184,   216,   218,
    1658,   216,   723,  1235,     1,   218,   216,   218,   390,   218,
     215,   193,    85,   749,   216,   197,   221,   199,   200,   401,
     217,  1355,   216,    46,   141,    88,  1592,  1509,   410,   986,
     817,   238,  1509,   215,  1509,  1509,  1694,  1509,   420,   221,
       1,   995,  1480,  1414,  1253,  1543,  1480,  1443,   430,  1383,
    1384,  1292,  1440,  1387,   446,  1544,    56,   391,   457,   441,
     457,  1636,  1212,  1491,  1711,    -1,   435,    -1,   715,   451,
    1404,    -1,    -1,    -1,  1508,    -1,    -1,    -1,   814,   461,
      33,    -1,    -1,    -1,   466,  1317,   468,  1517,    -1,    -1,
      -1,    -1,    -1,  1710,    -1,   477,  1430,    -1,   488,   489,
     490,    -1,  1532,   485,   486,   487,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,  1548,  1449,    -1,    -1,   500,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   509,    -1,    -1,
      -1,    -1,   514,   515,   516,   517,   518,    -1,    -1,    -1,
    1571,   877,    -1,    -1,    -1,   527,    -1,  1578,    -1,  1580,
     540,    -1,    -1,   535,    -1,    -1,  1587,    -1,    -1,    -1,
    1590,    -1,    -1,    -1,    -1,    -1,    -1,  1501,  1602,    -1,
    1601,   124,    -1,    -1,  1604,   128,    -1,    -1,   568,    33,
      -1,    -1,    -1,    -1,    -1,  1417,    -1,  1419,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   578,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,   944,    -1,
      -1,    -1,    -1,   595,    -1,    -1,   952,    -1,    -1,    -1,
    1651,    -1,  1556,  1653,    -1,    -1,    -1,    -1,  1562,    -1,
      -1,    -1,    -1,  1664,    -1,    -1,    -1,    -1,    -1,  1670,
     193,   623,    -1,    -1,   197,    -1,   199,   200,  1582,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1593,
      -1,    -1,   215,    -1,    -1,    -1,  1697,  1698,   221,    33,
     124,    -1,    -1,    -1,   128,  1706,  1707,  1711,    -1,   669,
      -1,    -1,   664,    33,    -1,  1517,    -1,    -1,    -1,    -1,
     672,   673,    -1,   675,    -1,    -1,    60,    61,   680,    -1,
    1532,    -1,    -1,  1734,   686,  1736,    -1,   697,    -1,    -1,
      60,    61,    -1,   695,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1655,    -1,    -1,   706,   707,   708,   709,   710,   711,
      -1,   713,    -1,   715,    -1,    -1,    -1,    -1,    -1,   193,
      -1,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   215,    -1,    -1,   128,    -1,    -1,   221,   758,    -1,
      -1,    -1,  1604,    -1,   124,   390,    -1,    -1,   128,    -1,
     762,   763,   764,   765,   766,   767,   768,   769,   770,   771,
      -1,    -1,   774,   775,   776,   777,   778,   779,  1134,   781,
     782,    -1,   784,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   193,
     812,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,   829,
      -1,    -1,    -1,   193,    -1,   827,   461,   197,    -1,   199,
     200,   215,    -1,    -1,    -1,    -1,    -1,   221,  1194,    -1,
      -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
     485,   221,    -1,    -1,    33,    -1,    -1,    -1,  1214,    -1,
      -1,   863,    -1,    -1,  1220,    -1,    -1,    -1,   870,    -1,
      -1,    -1,    -1,  1229,    -1,    -1,   878,    -1,   880,    -1,
      -1,    60,    61,  1239,    -1,    -1,    -1,    -1,   890,   891,
     892,   893,   894,   895,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1269,    -1,    -1,    -1,    -1,    -1,   929,
      -1,    -1,    -1,    -1,    -1,  1281,    -1,    -1,    -1,   939,
    1286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     942,   943,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,   956,    -1,    -1,   959,    -1,    -1,
     595,    -1,    -1,   965,    -1,    -1,    -1,    -1,  1324,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   981,
      -1,    -1,   984,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1355,
    1010,  1011,  1012,    -1,    -1,  1015,    -1,  1017,    -1,  1019,
      -1,  1021,    -1,  1023,   193,  1025,    -1,  1027,   197,  1029,
     199,   200,    -1,    -1,  1034,    -1,  1036,  1383,  1384,   664,
      -1,  1387,  1042,    -1,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,   221,    19,  1054,    -1,  1056,    -1,    -1,    -1,
      -1,  1061,    -1,  1063,    -1,  1065,    -1,    -1,  1068,    -1,
     695,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1082,    -1,    -1,    -1,  1086,    -1,    -1,    -1,    -1,    -1,
    1100,    -1,    -1,    -1,    -1,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,  1131,    98,    99,   100,   101,    -1,   762,   763,   764,
     765,   766,   767,   768,   769,    -1,    -1,    -1,    -1,    -1,
     775,   776,    -1,    -1,    -1,  1501,   781,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   790,   791,    -1,    -1,    -1,
     795,   796,   797,    -1,   799,    -1,   116,   117,   118,   119,
     120,    -1,  1174,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   158,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,   827,    -1,  1196,  1197,  1198,   173,    -1,  1201,
      -1,    -1,    -1,    -1,    -1,    -1,  1562,  1209,    -1,    -1,
    1212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1233,  1234,    -1,    -1,   870,    -1,  1593,    -1,  1241,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,  1263,    -1,  1265,   214,   215,    -1,    -1,  1270,    -1,
      -1,    -1,    -1,    -1,  1276,    -1,    -1,    -1,  1280,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,  1655,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,  1320,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1341,
      64,    -1,    -1,  1353,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,   763,   764,   765,   766,   767,   768,   769,    -1,
      -1,    -1,    -1,  1395,   775,   776,    -1,    -1,    -1,    -1,
     781,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   790,
     791,    -1,    12,    -1,   795,   796,   797,    -1,   799,   143,
      -1,    21,    22,  1425,    -1,  1427,    -1,    -1,    -1,    -1,
      -1,  1433,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1459,    -1,  1461,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1471,
    1472,  1473,    -1,    -1,    -1,    -1,  1478,    -1,    -1,    -1,
      -1,  1483,    -1,    -1,    -1,  1487,  1488,  1489,  1490,    -1,
      -1,  1493,  1494,    -1,    -1,   219,    -1,    -1,  1500,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1509,    -1,  1511,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,  1523,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,  1558,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1572,  1573,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1589,    -1,    -1,
      -1,    -1,  1594,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,  1607,  1241,  1609,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1626,    -1,  1628,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1647,  1648,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1656,    -1,    -1,    -1,     1,    -1,
    1662,  1663,     5,     6,     7,    -1,     9,    10,    11,    -1,
      13,    -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,  1689,    31,  1691,
    1692,    -1,    -1,    -1,    -1,    38,    39,    -1,  1700,    42,
      -1,    44,    45,  1705,    -1,    48,    -1,    50,    51,    52,
      -1,    54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,
     197,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,    -1,     5,     6,    -1,   215,    -1,
     217,    -1,   219,   220,   221,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,    -1,     5,     6,    -1,   215,    -1,   217,    -1,   219,
     220,   221,    15,    16,    17,    18,    19,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   197,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,     5,
       6,    -1,   215,    -1,   217,    -1,   219,   220,   221,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
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
      -1,    25,    26,    27,    28,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    52,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,    -1,    -1,     5,     6,   215,
      -1,   217,    -1,   219,   220,    13,    -1,    15,    16,    17,
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
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,    -1,     5,     6,   215,   216,   217,
      -1,   219,   220,    13,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
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
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,    -1,    -1,     5,     6,   215,    -1,   217,    -1,   219,
     220,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
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
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,
      -1,     5,     6,   215,   216,   217,    -1,   219,   220,    13,
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
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,    -1,   219,   220,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,   216,   217,    -1,   219,   220,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
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
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    58,    -1,
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
      -1,    -1,    -1,     5,     6,   215,    -1,   217,    -1,   219,
     220,    13,    -1,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,   198,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,    -1,    -1,     5,     6,   215,
      -1,   217,    -1,   219,   220,    13,    -1,    15,    16,    17,
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
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
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
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     118,   119,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    21,    22,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,   137,   138,   139,   140,   141,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    -1,   144,   192,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   214,
     215,    -1,    25,    -1,   219,   220,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,   214,   215,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
     173,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   219,    21,    22,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,   173,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,    -1,    -1,    -1,   219,
      -1,   221,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   173,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    41,   191,    -1,   200,   201,   202,   203,
     204,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,   219,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,   156,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    64,
      -1,    -1,    -1,    -1,    -1,   173,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   173,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,    -1,   146,    -1,    -1,    -1,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,    -1,   219,   136,   137,   138,   139,   140,
     141,    21,    22,   144,   145,   146,   147,   148,   149,   150,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,   219,   220,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    21,    22,   136,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   192,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,   219,
     220,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,   219,   220,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,   219,   220,    -1,
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
      -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    38,   150,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,   214,   215,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   214,   215,   216,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
      -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,   140,   141,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    21,
      22,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   163,    -1,
     207,   208,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    21,    22,   116,   117,   118,   119,   120,   214,
     215,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   214,   215,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,   142,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    21,    22,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    21,    22,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,   214,   215,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,   142,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    21,    22,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    21,    22,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,   214,   215,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    21,    22,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    21,    22,    -1,    -1,    -1,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,   214,   215,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    21,    22,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    21,    22,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,   214,   215,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    21,    22,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   214,   215,   123,    -1,    -1,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,   173,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    35,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,   129,
     130,    -1,    -1,    71,    -1,    73,    -1,    75,    76,    77,
      78,    79,   173,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,   158,    -1,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   225,   228,
     237,   239,   240,   245,   276,   280,   316,   396,   403,   407,
     414,   460,   465,   470,    19,    20,   173,   268,   269,   270,
     166,   246,   247,   173,   174,   175,   204,   241,   242,   243,
     163,   183,   285,   404,   173,   219,   227,    57,    63,   399,
     399,   399,   143,   173,   302,    34,    63,   107,   136,   208,
     217,   272,   273,   274,   275,   302,   224,     5,     6,     8,
      36,   107,   411,    62,   394,   192,   191,   194,   191,   242,
      22,    57,   203,   214,   244,   399,   400,   402,   400,   394,
     471,   461,   466,   173,   143,   238,   274,   274,   274,   217,
     144,   145,   146,   191,   216,    57,    63,   281,   283,    57,
      63,   405,     5,     6,    57,    63,   412,    57,    63,   395,
      15,    16,   166,   171,   173,   176,   217,   230,   269,   166,
     247,   173,   241,   241,   173,   224,   164,   184,   286,   400,
     224,    57,    63,   226,   173,   173,   173,   173,   177,   236,
     218,   270,   274,   274,   274,   274,   284,   173,   406,   415,
     285,   397,   177,   178,   179,   229,    15,    16,   166,   171,
     173,   230,   266,   267,   244,   401,   165,   373,   224,   472,
     462,   467,   177,   218,    35,    71,    73,    75,    76,    77,
      78,    79,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    93,    94,    95,    98,    99,   100,   101,
     118,   119,   173,   279,   282,   194,   285,   106,   409,   410,
     392,   160,   220,   271,   367,   177,   178,   179,   191,   218,
     192,   373,   285,   285,   285,    21,    22,    38,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   128,   129,   130,   137,
     138,   139,   140,   141,   144,   145,   146,   147,   148,   149,
     150,   193,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   207,   208,   214,   215,    35,    35,   217,   277,
     285,   287,    75,    79,    93,    94,    98,    99,   100,   101,
     419,   398,   173,   416,   286,   393,   270,   269,   220,   224,
     152,   173,   390,   391,   266,    19,    25,    31,    41,    49,
      64,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   156,   219,   302,   418,   420,   421,
     425,   431,   459,   424,   463,   468,   173,   173,   173,   216,
      22,   173,   216,   155,   218,   367,   378,   379,   194,   278,
     290,   285,   173,   286,   194,   408,   285,   413,   367,   216,
     269,   217,    43,   191,   194,   197,   389,   198,   198,   198,
     217,   198,   198,   217,   198,   198,   198,   198,   198,   198,
     217,   302,    33,    60,    61,   124,   128,   193,   197,   200,
     215,   221,   430,   195,   165,   173,   423,   382,   385,   173,
     137,   217,     7,    50,   315,   218,   224,   459,     1,     5,
       6,     9,    10,    11,    13,    15,    16,    17,    18,    19,
      25,    26,    27,    28,    29,    31,    38,    39,    42,    44,
      45,    48,    51,    52,    54,    55,    58,    59,    65,    68,
      69,    80,   102,   103,   104,   105,   118,   119,   133,   134,
     135,   151,   152,   153,   154,   155,   157,   159,   160,   161,
     162,   165,   166,   167,   168,   169,   170,   171,   172,   174,
     175,   176,   183,   185,   200,   201,   202,   207,   208,   215,
     217,   219,   220,   235,   237,   248,   249,   252,   255,   256,
     258,   261,   262,   263,   264,   285,   286,   288,   289,   291,
     296,   301,   302,   303,   307,   308,   309,   310,   311,   312,
     313,   314,   316,   320,   321,   328,   331,   334,   339,   342,
     343,   345,   346,   347,   349,   354,   357,   358,   365,   366,
     418,   473,   488,   499,   503,   516,   519,   398,   192,   373,
     302,   374,   391,   216,    65,   104,   174,   296,   365,   173,
     173,   431,   127,   137,   192,   388,   432,   437,   439,   365,
     441,   435,   173,   426,   443,   445,   447,   449,   451,   453,
     455,   457,   365,   198,   217,    33,   197,    33,   197,   215,
     221,   216,   365,   215,   221,   431,   165,   423,   191,   286,
     173,   191,   224,   380,   428,   459,   464,   173,   383,   428,
     469,   365,   152,   173,   387,   417,   378,   198,   198,   365,
     259,   260,   198,   304,   420,   473,   217,   302,   198,     5,
     102,   103,   198,   217,   127,   301,   332,   343,   358,   365,
     287,   198,   217,    61,   365,   217,   365,   173,   198,   198,
     217,   224,   198,   166,    58,   365,   217,   287,   198,   217,
     198,   198,   217,   198,   198,   127,   301,   365,   365,   365,
     220,   287,   334,   338,   338,   338,   217,   217,   217,   217,
     217,   217,    13,   431,    13,   431,    13,   365,   498,   514,
     198,   365,   198,   234,    13,   365,   365,   365,   365,   365,
      13,    49,   292,   332,   365,   332,   220,   224,   224,   365,
      10,    13,   294,   498,   515,    37,   334,   340,   173,   217,
     224,   224,   224,   224,   224,    66,   317,   276,   132,   224,
      21,    22,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   136,   137,   138,   139,
     140,   141,   144,   145,   146,   147,   148,   149,   150,   192,
     193,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   214,   215,   340,   217,   286,   365,   215,   224,   276,
     286,   375,   373,   198,   218,    43,   224,   388,   301,   365,
     459,   459,   429,   459,   218,   459,   459,   218,   173,   422,
     459,   277,   459,   277,   459,   277,   380,   381,   383,   384,
     218,   434,   292,   216,   216,   365,   194,   428,   286,   194,
     428,   286,   218,   217,    43,   127,   191,   192,   194,   197,
     386,   489,   491,   287,   417,   217,   305,   217,   302,   198,
     217,   329,   198,   198,   198,   510,   332,   301,   332,   191,
     108,   109,   110,   111,   112,   113,   114,   115,   121,   122,
     127,   140,   141,   147,   148,   149,   192,    14,   431,   294,
     365,   365,   287,   192,   322,   324,   365,   326,   194,   166,
     365,   512,   332,   495,   500,   332,   493,   431,   301,   365,
     220,   276,   365,   365,   365,   365,   365,   365,   417,    53,
     158,   173,   200,   215,   217,   365,   474,   477,   481,   497,
     502,   417,   217,   477,   502,   417,   142,   184,   186,   224,
     482,   297,   287,   299,   179,   180,   229,   217,   217,   417,
      13,   216,   191,   518,   518,   152,   157,   198,   302,   348,
     287,   257,   417,   286,   191,   518,   285,   341,    70,   215,
     218,   332,   474,   476,   160,   217,   319,   391,     4,   160,
     337,   338,    19,   158,   173,   418,    19,   158,   173,   418,
     133,   134,   135,   288,   344,   365,   344,   365,   344,   365,
     344,   365,   344,   365,   344,   365,   344,   365,   344,   365,
     365,   365,   365,   344,   365,   344,   365,   365,   365,   365,
     173,   344,   365,   365,   158,   173,   365,   365,   365,   418,
     365,   365,   365,   344,   365,   344,   365,   365,   365,   365,
     344,   365,   344,   365,   344,   365,   365,   344,   365,    22,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   129,   130,   158,   173,   214,   215,   355,   418,   365,
     218,   332,   373,   275,     8,   367,   372,   431,   173,   301,
     365,   224,   199,   199,   199,   428,   199,   199,   199,   224,
     199,   278,   199,   278,   199,   278,   199,   428,   199,   428,
     295,   459,   218,   216,   459,   459,   365,   173,   173,   459,
     301,   365,   431,   431,    20,   417,   459,    70,   332,   476,
     487,   198,   365,   173,   365,   459,   504,   506,   508,   431,
     518,   358,   365,   365,   365,   365,   365,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   287,
     199,   428,   218,   218,   265,   431,   431,   218,   431,   218,
     431,   518,   431,   381,   518,   384,   199,   337,   218,   218,
     218,   218,   218,   218,    20,   338,   217,   137,   386,   215,
     218,   142,   191,   224,   481,   188,   189,   216,   485,   191,
     188,   216,   224,   484,    20,   218,   481,   184,   187,   483,
      20,   365,   184,   498,   295,   295,   365,   417,   417,    20,
     217,   417,   218,   217,   217,   350,   352,   373,   365,    20,
     498,   290,   276,   173,   218,   476,   474,   191,   218,   218,
     173,   318,   318,   217,   127,   137,   173,   192,   197,   335,
     336,   277,   198,   217,   198,   217,   338,   338,   338,   217,
     217,   216,    19,   158,   173,   418,   194,   158,   173,   365,
     217,   217,   158,   173,   365,     1,   217,   216,   218,   216,
      57,    63,   370,    67,   371,   224,   199,   224,   433,   438,
     440,   459,   442,   436,   427,   173,   444,   199,   448,   199,
     452,   199,   456,   380,   458,   383,   199,   428,   218,    43,
     386,   199,   199,   332,    20,   199,   476,   218,   218,   218,
     173,   218,   199,   224,   218,   199,   431,   381,   384,   199,
     218,   217,   431,   365,   199,   199,   199,   199,   218,   199,
     199,   218,   199,   337,   277,   217,   332,   365,   365,   365,
     477,   481,   365,   158,   173,   474,   485,   216,   365,   216,
     497,   332,   477,   184,   187,   190,   486,   216,   332,   199,
     199,   194,   232,    20,    20,   332,   417,    20,   365,   365,
     431,   277,    12,    23,    24,   250,   251,    12,   253,   332,
     286,   337,   218,   216,   215,   191,   216,   218,   336,   173,
     173,   217,   173,   173,   191,   216,   278,   359,   365,   361,
     365,   218,   332,   365,   198,   217,   365,   217,   216,   365,
     215,   218,   332,   217,   216,   356,   218,   332,   224,    47,
     371,    46,   106,   368,   373,   337,   446,   450,   454,   217,
     459,   173,   365,   490,   492,   287,   332,   306,   218,   199,
     428,   217,   173,   330,   199,   199,   199,   511,   294,   199,
     224,   323,   325,   327,   513,   496,   501,   494,   217,   340,
     278,   218,   332,   185,   218,   481,   485,   217,   137,   386,
     185,   481,   216,   185,   298,   300,   233,   181,   332,   332,
     185,    20,   332,   218,   218,   199,   278,   287,   365,   254,
     224,   185,   277,   218,   474,   173,   216,   194,   389,   218,
     173,   335,   216,   142,   287,   333,   431,   218,   459,   218,
     218,   218,   363,   365,   365,   218,   474,   218,   365,   218,
     373,    33,   369,   368,   370,   292,   217,   217,   218,   365,
     173,   365,   199,   505,   507,   509,   217,   218,   217,   365,
     365,   365,   217,    70,   487,   217,   217,   218,   365,   333,
     218,   365,   137,   386,   485,   365,   365,   365,   365,   486,
     498,   365,   217,   293,   231,   218,   218,   365,   332,   185,
     351,   199,   287,    26,   105,   255,   308,   309,   310,   312,
     365,   498,   278,   216,   194,   389,   431,   388,   218,   127,
     365,   199,   199,   459,   218,   218,   216,   218,   376,   369,
     387,   218,   487,   487,   287,   218,   199,   218,   217,   217,
     217,   217,   292,   294,   332,   487,   487,   218,   224,   517,
     365,   365,   218,   517,   517,   292,   177,   185,   185,   517,
     218,   365,   348,   353,   251,   127,   127,   365,   517,   287,
     218,   431,   388,   388,   365,   365,   360,   362,   199,   218,
     282,   377,   217,   474,   478,   479,   480,   480,   365,   365,
     487,   487,   474,   475,   218,   218,   518,   480,   475,    53,
     216,   137,   386,   184,   286,   191,   518,   498,   365,   216,
     185,   517,   348,   365,   286,   388,   365,   365,   224,   364,
     224,   282,   474,   191,   518,   218,   218,   218,   218,   480,
     480,   218,   218,   218,   218,   365,   216,   365,   365,   216,
     286,   218,   517,   517,   365,   216,   365,   224,   224,   373,
     287,   218,   217,   218,   218,   184,   216,   517,   224,   474,
     216,   218
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
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   359,   360,   358,   358,   358,   358,   361,
     362,   358,   358,   358,   363,   364,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   365,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   367,   367,
     367,   368,   368,   368,   369,   369,   370,   370,   370,   371,
     371,   372,   373,   373,   374,   374,   375,   374,   376,   374,
     377,   374,   374,   378,   378,   379,   379,   380,   380,   381,
     381,   382,   382,   382,   383,   384,   384,   385,   385,   385,
     386,   386,   387,   387,   387,   387,   387,   387,   388,   388,
     388,   389,   389,   390,   390,   390,   390,   390,   391,   391,
     391,   391,   391,   392,   393,   392,   394,   394,   395,   395,
     395,   396,   397,   396,   398,   398,   398,   399,   399,   399,
     401,   400,   402,   402,   403,   404,   403,   405,   405,   405,
     406,   407,   407,   408,   408,   409,   409,   410,   411,   411,
     411,   411,   412,   412,   412,   413,   413,   415,   416,   414,
     417,   417,   417,   417,   417,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   419,   419,   419,   419,   419,   419,   419,   419,
     420,   421,   421,   421,   422,   422,   423,   423,   424,   424,
     424,   424,   426,   427,   425,   428,   428,   429,   429,   430,
     430,   431,   431,   431,   431,   431,   431,   432,   433,   431,
     431,   431,   434,   431,   431,   431,   431,   431,   431,   431,
     431,   431,   431,   431,   431,   431,   435,   436,   431,   431,
     437,   438,   431,   439,   440,   431,   441,   442,   431,   431,
     443,   444,   431,   445,   446,   431,   431,   447,   448,   431,
     449,   450,   431,   431,   451,   452,   431,   453,   454,   431,
     455,   456,   431,   457,   458,   431,   459,   459,   459,   461,
     462,   463,   464,   460,   466,   467,   468,   469,   465,   471,
     472,   470,   473,   473,   473,   473,   473,   474,   474,   474,
     474,   474,   474,   474,   474,   475,   475,   476,   477,   477,
     478,   478,   479,   479,   480,   480,   481,   481,   482,   482,
     483,   483,   484,   484,   485,   485,   485,   486,   486,   486,
     487,   487,   488,   488,   488,   488,   488,   488,   489,   490,
     488,   491,   492,   488,   493,   494,   488,   495,   496,   488,
     497,   497,   497,   498,   498,   499,   500,   501,   499,   502,
     502,   503,   503,   503,   504,   505,   503,   506,   507,   503,
     508,   509,   503,   503,   510,   511,   503,   503,   512,   513,
     503,   514,   514,   515,   515,   516,   516,   516,   516,   516,
     517,   517,   518,   518,   519,   519,   519,   519,   519,   519,
     519,   519,   519
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
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     4,     4,     5,
       4,     5,     3,     4,     1,     1,     2,     4,     4,     7,
       8,     3,     5,     0,     0,     8,     3,     3,     3,     0,
       0,     8,     3,     4,     0,     0,     9,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     2,     4,
       1,     1,     4,     4,     4,     4,     4,     1,     6,     7,
       6,     6,     7,     7,     6,     7,     6,     6,     0,     4,
       1,     0,     1,     1,     0,     1,     0,     1,     1,     0,
       1,     5,     0,     1,     0,     2,     0,     5,     0,    10,
       0,    10,     6,     3,     4,     1,     3,     1,     3,     1,
       3,     0,     2,     3,     3,     1,     3,     0,     2,     3,
       1,     1,     1,     2,     3,     5,     3,     3,     1,     1,
       1,     0,     1,     1,     4,     3,     3,     5,     4,     6,
       5,     5,     4,     0,     0,     5,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     3,     5,     0,     1,     1,
       0,     5,     3,     4,     4,     0,     4,     0,     1,     1,
       1,     7,     9,     0,     2,     0,     1,     3,     1,     1,
       2,     2,     0,     1,     1,     0,     3,     0,     0,     7,
       1,     4,     3,     3,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     1,     3,     0,     2,     0,     2,
       3,     3,     0,     0,     6,     1,     1,     1,     3,     3,
       4,     1,     1,     1,     1,     2,     3,     0,     0,     6,
       4,     5,     0,     9,     4,     2,     2,     3,     2,     3,
       2,     2,     3,     3,     3,     2,     0,     0,     6,     2,
       0,     0,     6,     0,     0,     6,     0,     0,     6,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       0,     0,     6,     0,     0,     6,     1,     3,     3,     0,
       0,     0,     0,    10,     0,     0,     0,     0,    10,     0,
       0,     9,     1,     1,     1,     1,     1,     3,     3,     5,
       5,     6,     6,     8,     8,     0,     1,     1,     1,     3,
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

  case 347: /* expr_list: expr_not_wrapped  */
                                  {
        // We use `expr_not_wrapped` because expr_list already wrapped in some braces
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 348: /* expr_list: expr_list ',' expr_not_wrapped  */
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

  case 447: /* expr_not_wrapped: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 448: /* expr_not_wrapped: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 449: /* expr_not_wrapped: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 450: /* expr_not_wrapped: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 451: /* expr_not_wrapped: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 452: /* expr_not_wrapped: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 453: /* expr_not_wrapped: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 454: /* expr_not_wrapped: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 455: /* expr_not_wrapped: expr_field  */
                                              { (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 456: /* expr_not_wrapped: expr_mtag  */
                                              { (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 457: /* expr_not_wrapped: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"!",(yyvsp[0].pExpression)); }
    break;

  case 458: /* expr_not_wrapped: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"~",(yyvsp[0].pExpression)); }
    break;

  case 459: /* expr_not_wrapped: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+",(yyvsp[0].pExpression)); }
    break;

  case 460: /* expr_not_wrapped: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"-",(yyvsp[0].pExpression)); }
    break;

  case 461: /* expr_not_wrapped: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 462: /* expr_not_wrapped: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr_not_wrapped: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr_not_wrapped: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr_not_wrapped: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr_not_wrapped: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr_not_wrapped: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr_not_wrapped: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr_not_wrapped: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr_not_wrapped: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr_not_wrapped: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr_not_wrapped: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr_not_wrapped: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr_not_wrapped: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr_not_wrapped: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr_not_wrapped: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr_not_wrapped: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr_not_wrapped: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr_not_wrapped: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr_not_wrapped: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr_not_wrapped: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr_not_wrapped: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 483: /* expr_not_wrapped: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"++", (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr_not_wrapped: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"--", (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr_not_wrapped: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 486: /* expr_not_wrapped: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokRangeAt(scanner,(yylsp[-1]), (yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 487: /* expr_not_wrapped: '(' expr_list optional_comma ')'  */
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

  case 488: /* expr_not_wrapped: expr '[' expr ']'  */
                                                      { (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 489: /* expr_not_wrapped: expr '.' '[' expr ']'  */
                                                          { (yyval.pExpression) = new ExprAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 490: /* expr_not_wrapped: expr "?[" expr ']'  */
                                                      { (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 491: /* expr_not_wrapped: expr '.' "?[" expr ']'  */
                                                          { (yyval.pExpression) = new ExprSafeAt(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 492: /* expr_not_wrapped: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 493: /* expr_not_wrapped: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 494: /* expr_not_wrapped: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 495: /* expr_not_wrapped: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 496: /* expr_not_wrapped: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 497: /* expr_not_wrapped: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 498: /* expr_not_wrapped: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 499: /* expr_not_wrapped: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 500: /* expr_not_wrapped: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 501: /* expr_not_wrapped: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 502: /* expr_not_wrapped: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokRangeAt(scanner,(yylsp[-4]), (yylsp[0])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 503: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 504: /* $@31: %empty  */
                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 505: /* expr_not_wrapped: expr "is" "type" '<' $@30 type_declaration_no_options '>' $@31  */
                                                                                                                                                            {
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 506: /* expr_not_wrapped: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 507: /* expr_not_wrapped: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 508: /* expr_not_wrapped: expr "as" "name"  */
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

  case 511: /* expr_not_wrapped: expr "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                 {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 512: /* expr_not_wrapped: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 513: /* expr_not_wrapped: expr '?' "as" "name"  */
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

  case 516: /* expr_not_wrapped: expr '?' "as" "type" '<' $@34 type_declaration '>' $@35  */
                                                                                                                                                     {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-8]), (yylsp[-1])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 517: /* expr_not_wrapped: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokRangeAt(scanner,(yylsp[-3]), (yylsp[0])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 518: /* expr_not_wrapped: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 519: /* expr_not_wrapped: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 520: /* expr_not_wrapped: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 521: /* expr_not_wrapped: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 522: /* expr_not_wrapped: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); (yyval.pExpression)->at = tokAt(scanner, (yylsp[0])); }
    break;

  case 523: /* expr_not_wrapped: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr_not_wrapped: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 525: /* expr_not_wrapped: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]))); }
    break;

  case 526: /* expr_not_wrapped: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-2]), (yylsp[0]))); (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])); }
    break;

  case 527: /* expr_not_wrapped: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
    }
    break;

  case 528: /* expr_not_wrapped: name_in_namespace "name"  */
                                                  {
        if (format::prepare_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-1]))))) {
            format::get_writer() << "." << format::get_substring(tokAt(scanner,(yylsp[0])));
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        }

        (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokRangeAt(scanner,(yylsp[-1]),(yylsp[0])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 529: /* expr_not_wrapped: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 530: /* expr_not_wrapped: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 531: /* expr: expr_not_wrapped  */
                                {
        format::wrap_par_expr_newline(tokAt(scanner,(yylsp[0])), (yyvsp[0].pExpression)->at);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
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

  case 564: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 565: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 566: /* $@36: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 567: /* struct_variable_declaration_list: struct_variable_declaration_list $@36 structure_variable_declaration semicolon opt_sem  */
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

  case 568: /* $@37: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 569: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@37 function_declaration_header semicolon opt_sem  */
                                                                  {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-2].pFuncDecl),tak);
                }
                (yyvsp[-2].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-6].b),(yyvsp[-4].b), (yyvsp[-2].pFuncDecl));
            }
    break;

  case 570: /* $@38: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 571: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@38 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure->isTemplate;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 572: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon opt_sem  */
                                                                                               {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-3])), CompilationError::syntax_error);
        delete (yyvsp[-3].faList);
        (yyval.pVarDeclList) = (yyvsp[-5].pVarDeclList);
    }
    break;

  case 573: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 574: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 575: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 576: /* function_argument_list: function_argument_list semicolon function_argument_declaration  */
                                                                                       { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 577: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 578: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 579: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 580: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 581: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 582: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 583: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 584: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 585: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 586: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 587: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 588: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 589: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 590: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 591: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 592: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 593: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 594: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 595: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 596: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 597: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 598: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 599: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 600: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 601: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 602: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 603: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 604: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 605: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 606: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 607: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 608: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 609: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 610: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 611: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 612: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 613: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 614: /* $@39: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 615: /* global_variable_declaration_list: global_variable_declaration_list $@39 optional_field_annotation let_variable_declaration opt_sem  */
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

  case 616: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 617: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 618: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 619: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 620: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 621: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                                   {
        handle_brace(format::Pos::from(tokAt(scanner, (yylsp[-2]))), (yyvsp[-2].ui),
                     format::get_substring(tokRangeAt(scanner, (yylsp[-2]), (yylsp[0]))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[0]))));
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 622: /* $@40: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 623: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@40 optional_field_annotation let_variable_declaration  */
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

  case 624: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 625: /* enum_list: enum_list "name" opt_sem  */
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

  case 626: /* enum_list: enum_list "name" '=' expr opt_sem  */
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

  case 627: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 628: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 629: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 630: /* $@41: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 631: /* single_alias: optional_public_or_private_alias "name" $@41 '=' type_declaration  */
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

  case 632: /* alias_list: single_alias semicolon opt_sem  */
                                                   {
        (yyval.positions) = new vector<LineInfo>(1, tokAt(scanner, (yylsp[-2])));
    }
    break;

  case 633: /* alias_list: alias_list single_alias semicolon opt_sem  */
                                                                      {
        (yyvsp[-3].positions)->emplace_back(tokAt(scanner, (yylsp[-2])));
        (yyval.positions) = (yyvsp[-3].positions);
    }
    break;

  case 634: /* alias_declaration: "typedef" open_block alias_list close_block  */
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

  case 635: /* $@42: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 637: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 638: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 639: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 640: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 641: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block enum_list close_block  */
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

  case 642: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block enum_list close_block  */
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

  case 643: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 644: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 645: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 646: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 647: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 648: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 649: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 650: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 651: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 652: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 653: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 654: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 655: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 656: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                                      {
        const auto prev_loc = format::Pos::from(tokAt(scanner,(yylsp[-2])));
        handle_brace(prev_loc, (yyvsp[-2].ui),
                     format::get_substring(prev_loc, format::Pos::from_last(tokAt(scanner,(yylsp[-1])))),
                     yyextra->das_tab_size,
                     format::Pos::from_last(tokAt(scanner,(yylsp[-1]))));
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 657: /* $@43: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 658: /* $@44: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 659: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@43 structure_name $@44 optional_struct_variable_declaration_list  */
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

  case 660: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 661: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 662: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 663: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 664: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 665: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 666: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 667: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 668: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 669: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 670: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 671: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 672: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 673: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 674: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 675: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 676: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 677: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 678: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 679: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 680: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 681: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 682: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 683: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 684: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 685: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 686: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 687: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 688: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 689: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 690: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 691: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 692: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 693: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 694: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 695: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 696: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 697: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 698: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 699: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 700: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 701: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 702: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 703: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 704: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 705: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 708: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 709: /* bitfield_alias_bits: bitfield_alias_bits SEMICOLON  */
                                            {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 710: /* bitfield_alias_bits: bitfield_alias_bits "name" SEMICOLON  */
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

  case 711: /* bitfield_alias_bits: bitfield_alias_bits "name" commas  */
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

  case 712: /* $@45: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 713: /* $@46: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 714: /* bitfield_type_declaration: "bitfield" '<' $@45 bitfield_bits '>' $@46  */
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

  case 717: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 718: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 719: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 720: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 721: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 722: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 723: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 724: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 725: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 726: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 727: /* $@47: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 728: /* $@48: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 729: /* type_declaration_no_options: "type" '<' $@47 type_declaration '>' $@48  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 730: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 731: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 732: /* $@49: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 733: /* type_declaration_no_options: '$' name_in_namespace '<' $@49 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 734: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 735: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 736: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 737: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 738: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 739: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 740: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 741: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 742: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 743: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 744: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 745: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 746: /* $@50: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 747: /* $@51: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 748: /* type_declaration_no_options: "smart_ptr" '<' $@50 type_declaration '>' $@51  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 749: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 750: /* $@52: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 751: /* $@53: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 752: /* type_declaration_no_options: "array" '<' $@52 type_declaration '>' $@53  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 753: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 754: /* $@55: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 755: /* type_declaration_no_options: "table" '<' $@54 table_type_pair '>' $@55  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 756: /* $@56: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 757: /* $@57: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 758: /* type_declaration_no_options: "iterator" '<' $@56 type_declaration '>' $@57  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 759: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 760: /* $@58: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 761: /* $@59: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 762: /* type_declaration_no_options: "block" '<' $@58 type_declaration '>' $@59  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 763: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 764: /* $@61: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 765: /* type_declaration_no_options: "block" '<' $@60 optional_function_argument_list optional_function_type '>' $@61  */
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

  case 766: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 767: /* $@62: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 768: /* $@63: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 769: /* type_declaration_no_options: "function" '<' $@62 type_declaration '>' $@63  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 770: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 771: /* $@65: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 772: /* type_declaration_no_options: "function" '<' $@64 optional_function_argument_list optional_function_type '>' $@65  */
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

  case 773: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 774: /* $@66: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 775: /* $@67: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 776: /* type_declaration_no_options: "lambda" '<' $@66 type_declaration '>' $@67  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 777: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 778: /* $@69: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 779: /* type_declaration_no_options: "lambda" '<' $@68 optional_function_argument_list optional_function_type '>' $@69  */
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

  case 780: /* $@70: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 781: /* $@71: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 782: /* type_declaration_no_options: "tuple" '<' $@70 tuple_type_list '>' $@71  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 783: /* $@72: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 784: /* $@73: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 785: /* type_declaration_no_options: "variant" '<' $@72 variant_type_list '>' $@73  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 786: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 787: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 788: /* type_declaration: type_declaration '|' '#'  */
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

  case 789: /* $@74: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 790: /* $@75: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 791: /* $@76: %empty  */
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

  case 792: /* $@77: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 793: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@74 "name" $@75 open_block $@76 tuple_alias_type_list $@77 close_block  */
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

  case 794: /* $@78: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 795: /* $@79: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 796: /* $@80: %empty  */
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

  case 797: /* $@81: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 798: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@78 "name" $@79 open_block $@80 variant_alias_type_list $@81 close_block  */
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

  case 799: /* $@82: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 800: /* $@83: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 801: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@82 "name" $@83 open_block bitfield_alias_bits commas close_block  */
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

  case 802: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 803: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 804: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 805: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 806: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 807: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 808: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 809: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 810: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 811: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 812: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner, (yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 813: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                                       {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]),(yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 814: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                              {
        auto mfd = make_smart<MakeFieldDecl>(tokRangeAt(scanner,(yylsp[-5]), (yylsp[0])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 815: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 816: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 817: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 818: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 819: /* make_struct_dim: make_struct_dim semicolon make_struct_fields  */
                                                               {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 820: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 821: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 822: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 823: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 824: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 825: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 826: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 827: /* optional_block: "where" expr_block  */
                                  { (yyvsp[0].pExpression)->at = tokAt(scanner, (yylsp[0])); (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 840: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 841: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 842: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 843: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
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

  case 844: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
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

  case 845: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
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

  case 846: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 847: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 848: /* $@84: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 849: /* $@85: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 850: /* make_struct_decl: "struct" '<' $@84 type_declaration_no_options '>' $@85 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 851: /* $@86: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 852: /* $@87: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 853: /* make_struct_decl: "class" '<' $@86 type_declaration_no_options '>' $@87 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 854: /* $@88: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 855: /* $@89: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 856: /* make_struct_decl: "variant" '<' $@88 variant_type_list '>' $@89 '(' use_initializer make_variant_dim ')'  */
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

  case 857: /* $@90: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 858: /* $@91: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 859: /* make_struct_decl: "default" '<' $@90 type_declaration_no_options '>' $@91 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 860: /* make_tuple: expr  */
                  {
        (yyvsp[0].pExpression)->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 861: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]), (yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 862: /* make_tuple: make_tuple ',' expr  */
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

  case 863: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 864: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 865: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 866: /* $@92: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 867: /* $@93: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 868: /* make_tuple_call: "tuple" '<' $@92 tuple_type_list '>' $@93 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 869: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 870: /* make_dim: make_dim semicolon make_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 871: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 872: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
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
                    format::get_writer() << "[" << internal << "]";
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

  case 873: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
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

  case 874: /* $@94: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 875: /* $@95: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 876: /* make_dim_decl: "array" "struct" '<' $@94 type_declaration_no_options '>' $@95 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 877: /* $@96: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 878: /* $@97: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 879: /* make_dim_decl: "array" "tuple" '<' $@96 tuple_type_list '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 880: /* $@98: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 881: /* $@99: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 882: /* make_dim_decl: "array" "variant" '<' $@98 variant_type_list '>' $@99 '(' make_variant_dim ')'  */
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

  case 883: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 884: /* $@100: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 885: /* $@101: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 886: /* make_dim_decl: "array" '<' $@100 type_declaration_no_options '>' $@101 '(' optional_expr_list ')'  */
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

  case 887: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 888: /* $@102: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 889: /* $@103: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 890: /* make_dim_decl: "fixed_array" '<' $@102 type_declaration_no_options '>' $@103 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 891: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 892: /* make_table: make_table semicolon make_map_tuple  */
                                                      {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 893: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 894: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 895: /* make_table_decl: open_block optional_expr_map_tuple_list close_block  */
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

  case 896: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 897: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 898: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 899: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 900: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 901: /* array_comprehension_where: semicolon "where" expr  */
                                          { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 902: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 903: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 904: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }

        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 905: /* array_comprehension: '[' "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                           {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 906: /* array_comprehension: '[' "iterator" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                        {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 907: /* array_comprehension: "begin of code block" "for" '(' variable_name_with_pos_list "in" expr_list ')' "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                                     {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-9])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 908: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        if (format::is_replace_braces() && format::prepare_rule(format::Pos::from(tokAt(scanner,(yylsp[-6]))))) {
            format::get_writer() << "(" << format::get_substring(format::Pos::from(tokAt(scanner,(yylsp[-6]))),
                                                                 format::Pos::from_last(tokAt(scanner,(yylsp[-4])))) << ")";
            format::finish_rule(format::Pos::from_last(tokAt(scanner,(yylsp[-4]))));
        }
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 909: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
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

  case 910: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
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

  case 911: /* array_comprehension: open_block "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block  */
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

  case 912: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where close_block close_block  */
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


