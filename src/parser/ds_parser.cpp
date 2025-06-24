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
  YYSYMBOL_SEMICOLON = 165,                /* "end of line"  */
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
  YYSYMBOL_top_level_reader_macro = 224,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 225, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 226,              /* module_name  */
  YYSYMBOL_module_declaration = 227,       /* module_declaration  */
  YYSYMBOL_character_sequence = 228,       /* character_sequence  */
  YYSYMBOL_string_constant = 229,          /* string_constant  */
  YYSYMBOL_format_string = 230,            /* format_string  */
  YYSYMBOL_optional_format_string = 231,   /* optional_format_string  */
  YYSYMBOL_232_1 = 232,                    /* $@1  */
  YYSYMBOL_string_builder_body = 233,      /* string_builder_body  */
  YYSYMBOL_string_builder = 234,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 235, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 236,              /* expr_reader  */
  YYSYMBOL_237_2 = 237,                    /* $@2  */
  YYSYMBOL_options_declaration = 238,      /* options_declaration  */
  YYSYMBOL_require_declaration = 239,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 240,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 241,      /* require_module_name  */
  YYSYMBOL_require_module = 242,           /* require_module  */
  YYSYMBOL_is_public_module = 243,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 244,       /* expect_declaration  */
  YYSYMBOL_expect_list = 245,              /* expect_list  */
  YYSYMBOL_expect_error = 246,             /* expect_error  */
  YYSYMBOL_expression_label = 247,         /* expression_label  */
  YYSYMBOL_expression_goto = 248,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 249,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 250,          /* expression_else  */
  YYSYMBOL_semicolon = 251,                /* semicolon  */
  YYSYMBOL_if_or_static_if = 252,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 253, /* expression_else_one_liner  */
  YYSYMBOL_254_3 = 254,                    /* $@3  */
  YYSYMBOL_expression_if_one_liner = 255,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 256,  /* expression_if_then_else  */
  YYSYMBOL_257_4 = 257,                    /* $@4  */
  YYSYMBOL_expression_for_loop = 258,      /* expression_for_loop  */
  YYSYMBOL_259_5 = 259,                    /* $@5  */
  YYSYMBOL_expression_unsafe = 260,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 261,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 262,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 263,    /* expression_with_alias  */
  YYSYMBOL_264_6 = 264,                    /* $@6  */
  YYSYMBOL_annotation_argument_value = 265, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 266, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 267, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 268,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 269, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 270,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 271, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 272, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 273,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 274,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 275, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 276, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 277,   /* optional_function_type  */
  YYSYMBOL_function_name = 278,            /* function_name  */
  YYSYMBOL_global_function_declaration = 279, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 280, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 281, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 282,     /* function_declaration  */
  YYSYMBOL_283_7 = 283,                    /* $@7  */
  YYSYMBOL_open_block = 284,               /* open_block  */
  YYSYMBOL_close_block = 285,              /* close_block  */
  YYSYMBOL_expression_block = 286,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 287,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 288,           /* expression_any  */
  YYSYMBOL_expressions = 289,              /* expressions  */
  YYSYMBOL_expr_keyword = 290,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 291,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 292, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 293, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 294, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 295,       /* expression_keyword  */
  YYSYMBOL_296_8 = 296,                    /* $@8  */
  YYSYMBOL_297_9 = 297,                    /* $@9  */
  YYSYMBOL_298_10 = 298,                   /* $@10  */
  YYSYMBOL_299_11 = 299,                   /* $@11  */
  YYSYMBOL_expr_pipe = 300,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 301,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 302,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 303,     /* new_type_declaration  */
  YYSYMBOL_304_12 = 304,                   /* $@12  */
  YYSYMBOL_305_13 = 305,                   /* $@13  */
  YYSYMBOL_expr_new = 306,                 /* expr_new  */
  YYSYMBOL_expression_break = 307,         /* expression_break  */
  YYSYMBOL_expression_continue = 308,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 309, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 310,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 311, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 312,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 313,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 314,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 315,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 316,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 317,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 318, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 319,           /* expression_let  */
  YYSYMBOL_expr_cast = 320,                /* expr_cast  */
  YYSYMBOL_321_14 = 321,                   /* $@14  */
  YYSYMBOL_322_15 = 322,                   /* $@15  */
  YYSYMBOL_323_16 = 323,                   /* $@16  */
  YYSYMBOL_324_17 = 324,                   /* $@17  */
  YYSYMBOL_325_18 = 325,                   /* $@18  */
  YYSYMBOL_326_19 = 326,                   /* $@19  */
  YYSYMBOL_expr_type_decl = 327,           /* expr_type_decl  */
  YYSYMBOL_328_20 = 328,                   /* $@20  */
  YYSYMBOL_329_21 = 329,                   /* $@21  */
  YYSYMBOL_expr_type_info = 330,           /* expr_type_info  */
  YYSYMBOL_expr_list = 331,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 332,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 333,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 334,            /* capture_entry  */
  YYSYMBOL_capture_list = 335,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 336,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 337,               /* expr_block  */
  YYSYMBOL_expr_full_block = 338,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 339, /* expr_full_block_assumed_piped  */
  YYSYMBOL_340_22 = 340,                   /* $@22  */
  YYSYMBOL_expr_numeric_const = 341,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 342,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 343,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 344,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 345,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 346,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 347,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 348,           /* func_addr_expr  */
  YYSYMBOL_349_23 = 349,                   /* $@23  */
  YYSYMBOL_350_24 = 350,                   /* $@24  */
  YYSYMBOL_351_25 = 351,                   /* $@25  */
  YYSYMBOL_352_26 = 352,                   /* $@26  */
  YYSYMBOL_expr_field = 353,               /* expr_field  */
  YYSYMBOL_354_27 = 354,                   /* $@27  */
  YYSYMBOL_355_28 = 355,                   /* $@28  */
  YYSYMBOL_expr_call = 356,                /* expr_call  */
  YYSYMBOL_expr = 357,                     /* expr  */
  YYSYMBOL_358_29 = 358,                   /* $@29  */
  YYSYMBOL_359_30 = 359,                   /* $@30  */
  YYSYMBOL_360_31 = 360,                   /* $@31  */
  YYSYMBOL_361_32 = 361,                   /* $@32  */
  YYSYMBOL_362_33 = 362,                   /* $@33  */
  YYSYMBOL_363_34 = 363,                   /* $@34  */
  YYSYMBOL_expr_mtag = 364,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 365, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 366,        /* optional_override  */
  YYSYMBOL_optional_constant = 367,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 368, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 369, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 370, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 371, /* struct_variable_declaration_list  */
  YYSYMBOL_372_35 = 372,                   /* $@35  */
  YYSYMBOL_373_36 = 373,                   /* $@36  */
  YYSYMBOL_374_37 = 374,                   /* $@37  */
  YYSYMBOL_function_argument_declaration_no_type = 375, /* function_argument_declaration_no_type  */
  YYSYMBOL_function_argument_declaration_type = 376, /* function_argument_declaration_type  */
  YYSYMBOL_function_argument_list = 377,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 378,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 379,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 380,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 381,             /* variant_type  */
  YYSYMBOL_variant_type_list = 382,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 383,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 384,             /* copy_or_move  */
  YYSYMBOL_variable_declaration_no_type = 385, /* variable_declaration_no_type  */
  YYSYMBOL_variable_declaration_type = 386, /* variable_declaration_type  */
  YYSYMBOL_variable_declaration = 387,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 388,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 389,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 390, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 391, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 392, /* global_variable_declaration_list  */
  YYSYMBOL_393_38 = 393,                   /* $@38  */
  YYSYMBOL_optional_shared = 394,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 395, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 396,               /* global_let  */
  YYSYMBOL_397_39 = 397,                   /* $@39  */
  YYSYMBOL_enum_list = 398,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 399, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 400,             /* single_alias  */
  YYSYMBOL_401_40 = 401,                   /* $@40  */
  YYSYMBOL_alias_list = 402,               /* alias_list  */
  YYSYMBOL_alias_declaration = 403,        /* alias_declaration  */
  YYSYMBOL_404_41 = 404,                   /* $@41  */
  YYSYMBOL_optional_public_or_private_enum = 405, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 406,                /* enum_name  */
  YYSYMBOL_enum_declaration = 407,         /* enum_declaration  */
  YYSYMBOL_408_42 = 408,                   /* $@42  */
  YYSYMBOL_409_43 = 409,                   /* $@43  */
  YYSYMBOL_410_44 = 410,                   /* $@44  */
  YYSYMBOL_411_45 = 411,                   /* $@45  */
  YYSYMBOL_optional_structure_parent = 412, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 413,          /* optional_sealed  */
  YYSYMBOL_structure_name = 414,           /* structure_name  */
  YYSYMBOL_class_or_struct = 415,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 416, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 417, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 418,    /* structure_declaration  */
  YYSYMBOL_419_46 = 419,                   /* $@46  */
  YYSYMBOL_420_47 = 420,                   /* $@47  */
  YYSYMBOL_variable_name_with_pos_list = 421, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 422,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 423, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 424, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 425,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 426,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 427,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 428, /* bitfield_type_declaration  */
  YYSYMBOL_429_48 = 429,                   /* $@48  */
  YYSYMBOL_430_49 = 430,                   /* $@49  */
  YYSYMBOL_c_or_s = 431,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 432,          /* table_type_pair  */
  YYSYMBOL_dim_list = 433,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 434, /* type_declaration_no_options  */
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
  YYSYMBOL_459_74 = 459,                   /* $@74  */
  YYSYMBOL_460_75 = 460,                   /* $@75  */
  YYSYMBOL_461_76 = 461,                   /* $@76  */
  YYSYMBOL_type_declaration = 462,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 463,  /* tuple_alias_declaration  */
  YYSYMBOL_464_77 = 464,                   /* $@77  */
  YYSYMBOL_465_78 = 465,                   /* $@78  */
  YYSYMBOL_466_79 = 466,                   /* $@79  */
  YYSYMBOL_467_80 = 467,                   /* $@80  */
  YYSYMBOL_variant_alias_declaration = 468, /* variant_alias_declaration  */
  YYSYMBOL_469_81 = 469,                   /* $@81  */
  YYSYMBOL_470_82 = 470,                   /* $@82  */
  YYSYMBOL_471_83 = 471,                   /* $@83  */
  YYSYMBOL_472_84 = 472,                   /* $@84  */
  YYSYMBOL_bitfield_alias_declaration = 473, /* bitfield_alias_declaration  */
  YYSYMBOL_474_85 = 474,                   /* $@85  */
  YYSYMBOL_475_86 = 475,                   /* $@86  */
  YYSYMBOL_476_87 = 476,                   /* $@87  */
  YYSYMBOL_477_88 = 477,                   /* $@88  */
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
  YYSYMBOL_494_89 = 494,                   /* $@89  */
  YYSYMBOL_495_90 = 495,                   /* $@90  */
  YYSYMBOL_496_91 = 496,                   /* $@91  */
  YYSYMBOL_497_92 = 497,                   /* $@92  */
  YYSYMBOL_498_93 = 498,                   /* $@93  */
  YYSYMBOL_499_94 = 499,                   /* $@94  */
  YYSYMBOL_500_95 = 500,                   /* $@95  */
  YYSYMBOL_501_96 = 501,                   /* $@96  */
  YYSYMBOL_make_tuple = 502,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 503,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 504,          /* make_tuple_call  */
  YYSYMBOL_505_97 = 505,                   /* $@97  */
  YYSYMBOL_506_98 = 506,                   /* $@98  */
  YYSYMBOL_make_dim = 507,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 508,            /* make_dim_decl  */
  YYSYMBOL_509_99 = 509,                   /* $@99  */
  YYSYMBOL_510_100 = 510,                  /* $@100  */
  YYSYMBOL_511_101 = 511,                  /* $@101  */
  YYSYMBOL_512_102 = 512,                  /* $@102  */
  YYSYMBOL_513_103 = 513,                  /* $@103  */
  YYSYMBOL_514_104 = 514,                  /* $@104  */
  YYSYMBOL_515_105 = 515,                  /* $@105  */
  YYSYMBOL_516_106 = 516,                  /* $@106  */
  YYSYMBOL_517_107 = 517,                  /* $@107  */
  YYSYMBOL_518_108 = 518,                  /* $@108  */
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
#define YYLAST   14899

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  303
/* YYNRULES -- Number of rules.  */
#define YYNRULES  918
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1725

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
       0,   561,   561,   562,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   583,   589,   590,
     591,   595,   596,   600,   618,   619,   620,   621,   625,   626,
     630,   631,   635,   636,   636,   640,   645,   654,   669,   685,
     690,   698,   698,   737,   767,   771,   772,   773,   777,   780,
     784,   790,   799,   802,   808,   809,   813,   817,   818,   822,
     825,   831,   837,   840,   846,   847,   851,   852,   853,   862,
     863,   867,   868,   872,   873,   873,   879,   880,   881,   882,
     883,   887,   893,   893,   899,   899,   905,   913,   923,   932,
     932,   939,   940,   941,   942,   943,   944,   948,   953,   961,
     962,   963,   967,   968,   969,   970,   971,   972,   973,   974,
     980,   983,   989,   992,   995,  1001,  1002,  1003,  1004,  1008,
    1021,  1039,  1042,  1050,  1061,  1072,  1083,  1086,  1093,  1097,
    1104,  1105,  1109,  1110,  1111,  1115,  1118,  1125,  1129,  1130,
    1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,
    1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,
    1151,  1152,  1153,  1154,  1155,  1156,  1157,  1158,  1159,  1160,
    1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,
    1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,
    1181,  1182,  1183,  1184,  1185,  1186,  1187,  1188,  1189,  1190,
    1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,  1200,
    1201,  1202,  1203,  1204,  1205,  1206,  1207,  1208,  1209,  1210,
    1211,  1212,  1217,  1235,  1236,  1237,  1241,  1247,  1247,  1265,
    1266,  1269,  1270,  1273,  1277,  1288,  1297,  1306,  1312,  1313,
    1314,  1315,  1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1326,  1327,  1328,  1329,  1330,  1331,  1332,  1336,
    1341,  1347,  1353,  1364,  1365,  1369,  1370,  1374,  1375,  1379,
    1383,  1390,  1390,  1390,  1396,  1396,  1396,  1405,  1439,  1442,
    1445,  1448,  1454,  1455,  1466,  1470,  1473,  1481,  1481,  1481,
    1484,  1490,  1493,  1497,  1501,  1508,  1515,  1521,  1525,  1529,
    1532,  1535,  1543,  1546,  1549,  1557,  1560,  1568,  1571,  1574,
    1582,  1588,  1589,  1590,  1594,  1595,  1599,  1600,  1604,  1609,
    1617,  1623,  1629,  1635,  1641,  1650,  1659,  1668,  1680,  1683,
    1689,  1689,  1689,  1692,  1692,  1692,  1697,  1697,  1697,  1705,
    1705,  1705,  1711,  1721,  1732,  1745,  1755,  1766,  1781,  1784,
    1790,  1791,  1798,  1809,  1810,  1811,  1815,  1816,  1817,  1818,
    1819,  1823,  1828,  1836,  1837,  1838,  1842,  1847,  1854,  1861,
    1861,  1870,  1871,  1872,  1873,  1874,  1875,  1876,  1880,  1881,
    1882,  1883,  1884,  1885,  1886,  1887,  1888,  1889,  1890,  1891,
    1892,  1893,  1894,  1895,  1896,  1897,  1898,  1902,  1903,  1904,
    1905,  1910,  1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,
    1919,  1920,  1921,  1922,  1923,  1924,  1925,  1926,  1931,  1938,
    1950,  1955,  1965,  1969,  1976,  1979,  1979,  1979,  1984,  1984,
    1984,  1997,  2001,  2005,  2010,  2017,  2026,  2031,  2038,  2038,
    2038,  2045,  2049,  2058,  2066,  2074,  2078,  2081,  2087,  2088,
    2089,  2090,  2091,  2092,  2093,  2094,  2095,  2096,  2097,  2098,
    2099,  2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,  2108,
    2109,  2110,  2111,  2112,  2113,  2114,  2115,  2116,  2117,  2118,
    2119,  2120,  2121,  2122,  2128,  2129,  2130,  2131,  2132,  2145,
    2154,  2155,  2156,  2157,  2158,  2159,  2160,  2161,  2162,  2163,
    2164,  2165,  2168,  2171,  2172,  2175,  2175,  2175,  2178,  2183,
    2187,  2191,  2191,  2191,  2196,  2199,  2203,  2203,  2203,  2208,
    2211,  2212,  2213,  2214,  2215,  2216,  2217,  2218,  2219,  2221,
    2225,  2226,  2231,  2235,  2236,  2237,  2238,  2239,  2240,  2241,
    2245,  2249,  2253,  2257,  2261,  2265,  2269,  2273,  2277,  2284,
    2285,  2286,  2290,  2291,  2292,  2296,  2297,  2301,  2302,  2303,
    2307,  2308,  2312,  2323,  2326,  2329,  2329,  2348,  2347,  2363,
    2362,  2376,  2385,  2397,  2406,  2416,  2417,  2418,  2419,  2420,
    2424,  2427,  2436,  2437,  2441,  2444,  2447,  2463,  2472,  2473,
    2477,  2480,  2483,  2497,  2498,  2502,  2508,  2514,  2523,  2526,
    2533,  2536,  2542,  2543,  2544,  2548,  2549,  2553,  2560,  2565,
    2574,  2580,  2591,  2594,  2599,  2604,  2612,  2623,  2626,  2629,
    2629,  2649,  2650,  2654,  2655,  2656,  2660,  2663,  2663,  2682,
    2685,  2688,  2703,  2722,  2723,  2724,  2729,  2729,  2755,  2756,
    2760,  2761,  2761,  2765,  2766,  2767,  2771,  2781,  2786,  2781,
    2798,  2803,  2798,  2818,  2819,  2823,  2824,  2828,  2834,  2835,
    2836,  2837,  2841,  2842,  2843,  2847,  2850,  2856,  2861,  2856,
    2881,  2888,  2893,  2902,  2908,  2919,  2920,  2921,  2922,  2923,
    2924,  2925,  2926,  2927,  2928,  2929,  2930,  2931,  2932,  2933,
    2934,  2935,  2936,  2937,  2938,  2939,  2940,  2941,  2942,  2943,
    2944,  2945,  2949,  2950,  2951,  2952,  2953,  2954,  2955,  2956,
    2960,  2971,  2975,  2982,  2994,  3001,  3010,  3015,  3018,  3031,
    3031,  3031,  3044,  3045,  3049,  3053,  3060,  3064,  3071,  3072,
    3073,  3074,  3075,  3090,  3096,  3096,  3096,  3100,  3105,  3112,
    3112,  3119,  3123,  3127,  3132,  3137,  3142,  3147,  3151,  3155,
    3160,  3164,  3168,  3173,  3173,  3173,  3179,  3186,  3186,  3186,
    3191,  3191,  3191,  3197,  3197,  3197,  3202,  3206,  3206,  3206,
    3211,  3211,  3211,  3220,  3224,  3224,  3224,  3229,  3229,  3229,
    3238,  3242,  3242,  3242,  3247,  3247,  3247,  3256,  3256,  3256,
    3262,  3262,  3262,  3271,  3274,  3285,  3301,  3301,  3306,  3311,
    3301,  3336,  3336,  3341,  3347,  3336,  3372,  3372,  3377,  3382,
    3372,  3412,  3413,  3414,  3415,  3416,  3420,  3427,  3434,  3440,
    3446,  3453,  3460,  3466,  3475,  3478,  3484,  3492,  3497,  3504,
    3509,  3516,  3521,  3527,  3528,  3532,  3533,  3538,  3539,  3543,
    3544,  3548,  3549,  3553,  3554,  3555,  3559,  3560,  3561,  3565,
    3566,  3570,  3576,  3583,  3591,  3598,  3606,  3615,  3615,  3615,
    3623,  3623,  3623,  3630,  3630,  3630,  3641,  3641,  3641,  3652,
    3655,  3661,  3675,  3681,  3687,  3693,  3693,  3693,  3707,  3712,
    3719,  3738,  3743,  3750,  3750,  3750,  3760,  3760,  3760,  3774,
    3774,  3774,  3788,  3797,  3797,  3797,  3817,  3824,  3824,  3824,
    3834,  3839,  3846,  3849,  3855,  3874,  3883,  3891,  3911,  3936,
    3937,  3941,  3942,  3947,  3950,  3953,  3956,  3959,  3962
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
  "\"[{\"", "\"{{\"", "\"new scope\"", "\"close scope\"",
  "\"end of line\"", "\"integer constant\"", "\"long integer constant\"",
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
  "character_sequence", "string_constant", "format_string",
  "optional_format_string", "$@1", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@2", "options_declaration",
  "require_declaration", "keyword_or_name", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "semicolon", "if_or_static_if",
  "expression_else_one_liner", "$@3", "expression_if_one_liner",
  "expression_if_then_else", "$@4", "expression_for_loop", "$@5",
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
  "struct_variable_declaration_list", "$@35", "$@36", "$@37",
  "function_argument_declaration_no_type",
  "function_argument_declaration_type", "function_argument_list",
  "tuple_type", "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration_no_type", "variable_declaration_type",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@38", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@39", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@40", "alias_list",
  "alias_declaration", "$@41", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@42", "$@43", "$@44", "$@45",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@46", "$@47", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@48", "$@49", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@50", "$@51", "$@52",
  "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60", "$@61",
  "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70",
  "$@71", "$@72", "$@73", "$@74", "$@75", "$@76", "type_declaration",
  "tuple_alias_declaration", "$@77", "$@78", "$@79", "$@80",
  "variant_alias_declaration", "$@81", "$@82", "$@83", "$@84",
  "bitfield_alias_declaration", "$@85", "$@86", "$@87", "$@88",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "optional_block", "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "$@96",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@97", "$@98",
  "make_dim", "make_dim_decl", "$@99", "$@100", "$@101", "$@102", "$@103",
  "$@104", "$@105", "$@106", "$@107", "$@108", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1519)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-785)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1519,    91, -1519, -1519,    58,   -83,   473,   390, -1519,   -47,
     162,   162,   162, -1519, -1519,    41,   361, -1519, -1519,   455,
   -1519, -1519, -1519, -1519,   318, -1519,    26, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519,   -15, -1519,    -4,
       7,    18, -1519, -1519, -1519, -1519,   473, -1519,    32, -1519,
   -1519, -1519,   162,   162, -1519, -1519,    26, -1519, -1519, -1519,
   -1519, -1519,   134,   169, -1519, -1519, -1519, -1519,   361,   361,
     361,   123, -1519,   884,  -123, -1519, -1519, -1519, -1519,   237,
     803,   620,   826, -1519,   853,    44,    58,   197,   -83,   242,
     200, -1519,   862,   862, -1519,   265,   455,    75,   455,   854,
     305,   334,   393, -1519,   404,   253, -1519, -1519,   143,    58,
     361,   361,   361,   361, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519,   430, -1519, -1519, -1519, -1519, -1519, -1519, -1519,   390,
   -1519, -1519, -1519, -1519, -1519,   861,   104, -1519, -1519, -1519,
   -1519,   555, -1519, -1519, -1519, -1519, -1519, -1519, -1519,   455,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,   478,
   -1519,    73, -1519,   -79,   509,   884, 14726, -1519,   405,   511,
   -1519,   -80, -1519, -1519, -1519,   868, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519,   128, -1519,   472, -1519,   390,   390,   390,
   -1519, -1519, 13655, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,   665,   677,
   -1519,   445,   390,   758, -1519, -1519,   541, -1519,   584,    58,
      58,   172,    95, -1519, -1519, -1519,   104, -1519, 10546, -1519,
   -1519, -1519,   589,   614, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519,   617,   576, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,   786, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519,   646,   625, -1519, -1519,   -85,   642, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519,   390, -1519,   670,
     390, -1519, -1519,   -80,   224, -1519,    58, -1519,   644,   845,
     718, -1519, -1519,   701,   720,   735,   730,   781,   800, -1519,
   -1519, -1519,   787, -1519, -1519, -1519, -1519, -1519,   822, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
     824, -1519, -1519, -1519,   827,   843, -1519, -1519, -1519, -1519,
     844,   856,   796,    41, -1519, -1519, -1519, -1519, -1519,   510,
     837, -1519, -1519, -1519, -1519, -1519, -1519, -1519,   808,   921,
   -1519,   847, -1519,   132,   455,   476,   858, 10546, -1519,  2595,
   -1519,   629,    41, -1519, -1519, -1519,    95,   865, -1519,  9820,
     904,   905, 10546, -1519,   216, -1519, -1519, -1519,  9820, -1519,
   -1519,   907, -1519,   651,   654,   655, -1519, -1519,  9820,   177,
   -1519, -1519, -1519,    19, -1519, -1519, -1519,    52,  6108, -1519,
     869, 10294,   638, 10397,   520, -1519, -1519,  9820, -1519, -1519,
     317,    25,    25,    25, -1519,   837, -1519,   885,   889,  9820,
   -1519, -1519, -1519, -1519, -1519,   741,   -64,   890,    43,  3422,
   -1519, -1519,   390,   524,  6314,   872,  9820,   909,   892,   894,
     874, -1519,   455,   895,   929,  6520,    23,   528,   899, -1519,
     588,   901,   902,  3628,  9820,  9820,   374,   374,   374,   886,
     887,   891,   896,   897,   903, -1519,  2173, 10191,  6728, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519,  6934,   908, -1519,  7142,
    9820,  9820,  9820,  9820,  9820,  5284,  7348, -1519,   881, -1519,
   -1519,   455,   455, -1519,  9820,  1097, -1519, -1519, -1519, -1519,
   -1519, -1519,  1072, -1519, -1519, -1519,   688, -1519,   -49,   455,
   -1519,   455,   455,   455, -1519,   455, -1519, -1519,  1053, -1519,
   -1519, -1519, -1519,   906, -1519, -1519,   364, -1519, -1519, -1519,
   -1519, -1519, -1519,  1864, -1519,   913, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519,   360, -1519,   517, -1519,    48, -1519, -1519,
     914,   912,   934, -1519,  2354, -1519,  1091,   591, -1519, -1519,
   -1519,  4040, 10546, 10546, 10546, 10889, 10546, 10546,   917,   964,
   10546,   445, 10546,   445, 10546,   445, 10649,   965, 11000, -1519,
    9820, -1519, -1519, -1519, -1519,   924, -1519, -1519, 13214,  9820,
   -1519,   510,   455, -1519,   517,   -44, -1519, -1519,   568, -1519,
     837,   517,   947,   568, -1519,   517, 11038,   926,  1101, -1519,
   -1519,   -37, -1519, -1519, -1519, -1519, -1519, 13697,   317, -1519,
     930, -1519, -1519,    41,   618, -1519,   950,   951,   952, -1519,
    9820,  4040, -1519,   962,  1022, 10854,  1141, 10546,  9820,  9820,
   14180,  9820, 13697,   966, -1519, -1519,  9820, -1519, -1519,   963,
     990, 14180,  9820, -1519, -1519,  9820, -1519, -1519,  9820, -1519,
   10546,  4040, -1519, 10854,   167,   167,   939, -1519,   906, -1519,
   -1519, -1519,  9820,  9820,  9820,  9820,  9820,  9820,   317,  2801,
     317,  3008,   317, 13883, -1519,   867, -1519, 13697, -1519,   815,
     317,   976, -1519,   970,   167,   167,    14,   167,   167,   317,
    1151,   949,   975, 14180,   954,   296,   975,   977,   960,   470,
   -1519, -1519, 13697, -1519,   390, -1519, -1519, -1519,  4246, -1519,
   -1519, -1519, -1519, -1519, -1519,   119,    45,   374, -1519,  2021,
   14602,  4452,  4452,  4452,  4452,  4452,  4452,  4452,  4452,  9820,
    9820, -1519, -1519,  9820,  4452,  4452,  9820,  9820,  9820,   994,
    4452,  9820,   456,  9820,  9820,  9820,  9820,  9820,  9820,  4452,
    4452,  9820,  9820,  9820,  4452,  4452,  4452,  9820,  4452,  7554,
    9820,  9820,  9820,  9820,  9820,  9820,  9820,  9820,  9820,  9820,
   14664,  9820, -1519,  7760,   629,  9820, -1519, -1519,   361, -1519,
    1161, -1519,   -80, 10546, -1519,  1006, -1519,  4040, -1519, 10733,
     227,   368,   982,   507, -1519,   491,   559, -1519, -1519,   140,
     680,   642,   692,   642,   737,   642, -1519,   446, -1519,   474,
   -1519, 10546,   967, -1519, -1519, 13249, -1519, -1519, 10546, -1519,
   -1519, 10546, -1519, -1519, -1519,  9820,  1009, -1519,  1010, -1519,
   10546, -1519,  9820, 10546, 10546, -1519,    20, 10546,  5490,  7966,
    1011,  9820, 10546, -1519, -1519, -1519, 10546,   975, -1519,   962,
    9820,  9820,  9820,  9820,  9820,  9820,  9820,  9820,  9820,  9820,
    9820,  9820,  9820,  9820,  9820,  9820,  9820,  9820,   390,  1354,
     968, 14180, 11149, -1519, -1519, 10546, 10546, 11184, 10546, -1519,
   -1519, 11295, 10546,   975, 10546, 10649,   975,   965,    51, -1519,
   10854, -1519,    45, 11333, 11444, 11479, 11590, 11628, 11739,    30,
     374,  3215,  4660,  5696, 13920,   998,    -2,   263,  1000,   261,
      35,  5902,    -2,   821,    46,  9820,  1032,  9820, -1519, -1519,
   10546, -1519, 10546, -1519,  9820,   782,    61, -1519,  9820, -1519,
      62,   317, -1519,  9820, -1519,  9820,  9820,  9820,  1001,   602,
   -1519, -1519,   978,  1007,   706, -1519, -1519,   873,  9820, -1519,
     906,   -55,  4868, -1519,   138,  1008,  1020,  1020, -1519, -1519,
    1014,   338,   445, -1519,   996,  1015, -1519, -1519,  1027,  1016,
   -1519, -1519,   374,   374,   374, -1519, -1519,  1669, -1519,  1669,
   -1519,  1669, -1519,  1669, -1519,  1669, -1519,  1669, -1519,  1669,
   -1519,  1669,   997,   997,   871, -1519,  1669, -1519,  1669,   871,
    1817,  1817,  1024, -1519,  1669,   202,  1026, -1519, 13360,   -24,
     -24,   913, 14180,   997,   997, -1519,  1669, -1519,  1669,  1480,
   10690, 14310, -1519,  1669, -1519,  1669, -1519,  1669, 14273, -1519,
    1669, 14633, 14013, 14403, 14429, 14457,   871,   871,  1044,  1044,
     202,   202,   202,   666,  9820,  1034,  1035,   667,  9820,  1217,
    1036, 13398, -1519,   144,   517, 13790,   257,   864,  1160,   455,
     834, -1519, -1519, 10733, -1519, -1519, -1519, -1519, 10546, -1519,
   -1519, -1519,  1064, -1519,  1040, -1519,  1045, -1519,  1050, -1519,
   10649, -1519,   965,   485,   837, -1519, -1519,   837,   837, 11774,
   -1519,  1211,   -65, 14180,  1085,  1232,  9820,   762,   674,   159,
    1037,  1038,  1087, 11885,   -43, 11923,   774, 10546, 10649,   965,
    1830,  1046, 14180, 14180, 14180, 14180, 14180, 14180, 14180, 14180,
   14180, 14180, 14180, 14180, 14180, 14180, 14180, 14180, 14180, 14180,
   -1519,  1049, 10546, -1519, -1519,  9820,  2009,  2020, -1519,  2260,
   -1519,  2312,  1051,  4449,   551,  1052,   570,    45,   445, -1519,
   -1519, -1519, -1519, -1519,  1054,  9820, -1519,  5076, 13214,     0,
    9820,   602,   674,   263, -1519, -1519,  1047, -1519,  9820,  9820,
   -1519,  1056, -1519,  9820,   674,   753,  1057, -1519, -1519,  9820,
   14180, -1519, -1519,   579,   586, 14050,  9820, -1519,  9820,    67,
   12034, 14180, 14180, -1519,  1059,   266,  9820,  9820, 10546,   445,
     390, -1519, -1519,  9820, -1519,  1387,  2595,    45,   339, -1519,
    1061,   416, 10026, -1519, -1519, -1519,   444,   271,   338,  1094,
    1095,  1060,  1102,  1107, -1519,   467,   642, -1519,  9820, -1519,
    9820, -1519, -1519, -1519,  8172,  9820, -1519,  1076,  1066, -1519,
   -1519,  9820,  1069, -1519, 13509,  9820,  8378,  1070, -1519, 13544,
   -1519,  8584, -1519, -1519, -1519, -1519,   455, -1519, -1519,   734,
   -1519,    55, -1519,    45, -1519, -1519, -1519, -1519,   837, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519,  1071, 10546, -1519,  1108,  9820, -1519, -1519,
     188, -1519,  1073, -1519, -1519, -1519,   608, -1519,  1116,  1077,
   -1519, -1519,  4657,   630,   635, -1519, -1519,  9820,  4865, 14180,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,   797,
     642,  8790,   750,    -2,   263, 14180,   998, -1519, -1519, 14180,
    1000, -1519,   759,    -2,  1074, -1519, -1519, -1519, -1519,   760,
   -1519, -1519, -1519,  1115,   763,   764,  9820,   395,  9820,  9820,
    9820, 12069, 12180,  5073,   642, -1519, 13697, -1519,   455, -1519,
     445, -1519,  1080,  4868,  1126,  1088,   192,   333, -1519, -1519,
    1128, -1519, -1519,   338,  1089,   287, 10546, 12218, 10546, 12329,
   -1519,   342, 12364, -1519,  9820,  2166,  9820, -1519, 12475,  4868,
   -1519,   367,  9820, -1519, -1519, -1519,   384, -1519,  1270,    55,
   -1519, -1519,   864,  1090, -1519, -1519, -1519,  9820,   837, -1519,
   14180,  1092,  1093, -1519, -1519, -1519,  9820,  1135,  1112,  9820,
   -1519, -1519, -1519, -1519,  1096,  1098,  1100,  9820,  9820,  9820,
    1103,  1244,  1104,  1109,  8996, -1519,   287, -1519,   398,  9820,
     263, -1519,  9820,   753, -1519,  9820,  9820,  1117, -1519, -1519,
    9820,  9820,   783,  9820,  9820, 12513, 14180, 14180, -1519, -1519,
   -1519,  1119,   873,  3834, -1519,   642, -1519,   497, -1519,   267,
   10546,   216, -1519,  1114, -1519, -1519,  9202, -1519, -1519,  6931,
   -1519,   777, -1519, -1519, -1519, 10546, 12624, 12659, -1519,   508,
   -1519, 12770, -1519, -1519, -1519,  1270,   317,  1118,  1244,  1244,
   12808,  1120,  1121, 12919,  1123,  1127,  1130,  9820, -1519,  9820,
     871,   871,   871,  9820, -1519, -1519,  1244,  1244, -1519, 12954,
   -1519, -1519, 14143, -1519, 14143, -1519,  1140,   871,  9820, -1519,
    1166,  1140, 14143,  9820, 14180, 14180,   399,   161, -1519, -1519,
    9408,  9614, -1519, -1519, -1519, -1519, -1519, 14180,   390,  1131,
   10546,   216,   221,  4040, -1519,  9820, 14273, -1519, -1519,   785,
   -1519, -1519,  1132, -1519, 14726, -1519, -1519, -1519, -1519, -1519,
     -51,   -51, -1519,  9820,  9820, -1519,  1244,  1244,   674,  1133,
    1137,   975,   -51,   674, -1519,  1292,  1142,  1169,  1173,  1168,
   -1519,  1178,  1149, 14143,  9820,  9820, -1519,   161,  9820,  9820,
   14180, -1519, -1519,   221,  4040,  4040, -1519, 10733,  2166, -1519,
   -1519, -1519, -1519,   455, 14726,   674,   998,  1176, -1519,  1152,
    1154, 13065, 13103,   -51,   -51,   998,  1156, -1519, -1519,  1158,
    1164,  1165,  9820,  1162,  1163,  1201, -1519,  1170, -1519, -1519,
    1175, 14180, 14180, -1519, 14180,  4040, -1519, 10733, -1519, 10733,
   -1519, -1519, -1519,   390,   401,  1172, -1519, -1519, -1519, -1519,
   -1519,  1174,  1180, -1519, -1519, -1519, -1519, 14180, -1519, -1519,
   -1519, -1519, -1519, -1519, 10733, -1519, -1519, -1519, -1519,   674,
   -1519, -1519, -1519,   418, -1519
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   130,     1,   314,     0,     0,     0,   641,   315,     0,
     633,   633,   633,    69,    70,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   621,     6,    11,     5,
       4,    13,    12,    14,   100,   101,    99,   108,   110,    43,
      59,    56,    57,    45,    46,    47,     0,    48,    54,    44,
     230,   229,   633,   633,    22,    21,   621,   635,   634,   806,
     796,   801,     0,   282,    41,   116,   117,   118,     0,     0,
       0,   119,   121,   128,     0,   115,    17,   659,   658,   223,
     643,     0,   662,   622,   623,     0,     0,     0,     0,    49,
       0,    55,     0,     0,    52,     0,     0,   633,     0,    18,
       0,     0,     0,   284,     0,     0,   127,   122,     0,     0,
       0,     0,     0,     0,   131,   225,   224,   227,   222,   645,
     644,     0,   661,   660,   664,   663,   667,   625,   624,   627,
     106,   107,   104,   105,   103,     0,     0,   102,   111,    60,
      58,    54,    51,    50,   636,   638,   232,   231,   640,     0,
     642,    19,    20,    23,   807,   797,   802,   283,    39,    42,
     126,     0,   123,   124,   125,   129,     0,   646,     0,   655,
     617,   549,    24,    25,    29,     0,    95,    96,    93,    94,
      92,    91,    97,     0,    53,     0,   639,     0,     0,     0,
      40,   120,     0,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,     0,     0,
     137,   132,     0,     0,   647,   656,     0,   668,   619,     0,
       0,   551,     0,    26,    27,    28,     0,   109,     0,   808,
     798,   803,   191,   192,   189,   140,   141,   143,   142,   144,
     145,   146,   147,   173,   174,   171,   172,   164,   175,   176,
     165,   162,   163,   190,   184,     0,   188,   177,   178,   179,
     180,   151,   152,   153,   148,   149,   150,   161,     0,   167,
     168,   166,   159,   160,   155,   154,   156,   157,   158,   139,
     138,   183,     0,   169,   170,   549,   135,   259,   228,   702,
     705,   708,   709,   703,   706,   704,   707,     0,   629,   653,
     665,   618,   626,   549,     0,   112,     0,   114,     0,   607,
     605,   628,    98,     0,     0,     0,     0,     0,     0,   675,
     695,   676,   711,   677,   681,   682,   683,   684,   701,   688,
     689,   690,   691,   692,   693,   694,   696,   697,   698,   699,
     766,   680,   687,   700,   773,   780,   678,   685,   679,   686,
       0,     0,     0,     0,   710,   728,   731,   729,   730,   793,
     637,   716,   584,   590,   193,   194,   187,   182,   195,   185,
     181,     0,   133,   313,   575,   576,     0,     0,   226,     0,
     650,   648,     0,   657,   563,   669,     0,     0,   113,     0,
       0,     0,     0,   606,     0,   734,   757,   760,     0,   763,
     753,     0,   719,   767,   774,   781,   787,   790,     0,     0,
     743,   748,   742,     0,   756,   752,   745,     0,     0,   747,
     732,     0,   809,   799,   804,   196,   186,     0,   311,   312,
       0,   549,   549,   549,   134,   136,   261,     0,     0,     0,
      71,    72,    84,   454,   455,     0,     0,     0,     0,   299,
     448,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,   701,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   538,     0,     0,     0,   371,
     373,   372,   374,   375,   376,   377,     0,     0,    35,   267,
       0,     0,     0,     0,     0,   263,     0,   353,   354,   452,
     451,     0,     0,   238,     0,     0,   254,   249,   246,   245,
     247,   248,   233,   281,   260,   240,   532,   239,   449,     0,
     523,    79,    80,    77,   252,    78,   253,   255,   317,   244,
     522,   521,   520,   130,   526,   450,     0,   241,   525,   524,
     496,   456,   497,   378,   457,     0,   453,   811,   815,   812,
     813,   814,   629,     0,   630,     0,   654,   565,   620,   550,
       0,     0,     0,   532,     0,   609,   610,     0,   603,   604,
     602,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   132,     0,   132,     0,   132,     0,     0,     0,   739,
     263,   750,   751,   744,   746,     0,   749,   733,     0,     0,
     795,   794,     0,   717,     0,   282,   722,   723,     0,   585,
     580,     0,     0,     0,   591,     0,     0,     0,   670,   572,
     573,   595,   577,   579,   578,   857,   860,     0,     0,   287,
     291,   290,   296,     0,     0,   339,     0,     0,     0,   893,
       0,     0,   303,   300,     0,   348,     0,     0,   267,     0,
     285,     0,     0,     0,   330,   333,     0,   258,   336,     0,
       0,    63,     0,    86,   897,     0,   866,   875,     0,   863,
       0,     0,   308,   305,   484,   485,   354,   366,   130,   280,
     278,   279,     0,     0,     0,     0,     0,     0,     0,   835,
       0,     0,     0,   873,   900,     0,   271,     0,   274,     0,
       0,     0,   902,   911,   461,   460,   498,   459,   458,     0,
       0,     0,   911,   348,     0,   282,   911,   911,     0,   355,
     256,   257,     0,    82,     0,   369,   236,   530,     0,   243,
     250,   251,   302,   307,   316,     0,   363,     0,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     438,     0,   235,     0,   651,     0,   631,   649,     0,   564,
       0,   666,   549,     0,   608,     0,   612,     0,   616,   378,
       0,     0,     0,   724,   737,     0,     0,   712,   714,     0,
       0,   135,     0,   135,     0,   135,   582,     0,   588,     0,
     713,     0,     0,   741,   726,     0,   718,   810,     0,   586,
     800,     0,   592,   805,   574,     0,     0,   594,     0,   593,
       0,   596,     0,     0,     0,    87,     0,     0,   849,     0,
       0,     0,     0,   883,   886,   889,     0,   911,   304,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   286,     0,    88,    89,     0,     0,     0,     0,    61,
      62,     0,     0,   911,     0,     0,   911,     0,     0,   309,
     306,   355,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   869,   827,   835,     0,   878,     0,
       0,     0,   835,     0,     0,     0,     0,     0,   838,   905,
       0,   262,     0,    38,     0,    36,     0,   904,   912,   268,
       0,     0,   880,   912,   264,     0,     0,     0,     0,   912,
     826,   489,     0,     0,   425,   422,   424,    66,     0,   259,
     130,     0,   263,   441,     0,     0,     0,     0,   329,   328,
       0,     0,   132,   277,     0,     0,   509,   508,     0,     0,
     510,   514,     0,     0,     0,   400,   409,   388,   410,   389,
     412,   391,   411,   390,   413,   392,   403,   382,   404,   383,
     405,   384,   462,   463,   475,   414,   393,   415,   394,   476,
     473,   474,     0,   402,   380,   503,     0,   494,     0,   527,
     528,   529,   381,   464,   465,   416,   395,   417,   396,   480,
     481,   482,   406,   385,   407,   386,   408,   387,   483,   401,
     379,     0,     0,   478,   479,   477,   471,   472,   467,   466,
     468,   469,   470,     0,     0,     0,   431,     0,     0,     0,
       0,     0,   446,     0,     0,     0,     0,   557,   560,     0,
       0,   611,   614,   378,   615,   735,   758,   761,     0,   764,
     754,   720,     0,   768,     0,   775,     0,   782,     0,   788,
       0,   791,     0,     0,   269,   738,   727,   581,   587,     0,
     672,   673,   598,   597,     0,     0,     0,     0,   850,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   349,   388,   389,   391,   390,   392,   382,   383,
     384,   393,   394,   380,   395,   396,   385,   386,   387,   379,
     310,     0,     0,   906,   499,     0,     0,     0,   500,     0,
     531,     0,     0,     0,     0,     0,     0,   363,   132,   533,
     534,   535,   536,   537,     0,     0,   836,     0,   348,   835,
       0,     0,     0,     0,   844,   845,     0,   852,     0,     0,
     842,     0,   881,     0,     0,     0,     0,   840,   882,     0,
     872,   837,   901,     0,     0,    32,     0,   903,     0,     0,
       0,   817,   816,   488,     0,     0,     0,     0,     0,   132,
       0,    64,    65,     0,    81,    73,     0,   363,     0,   442,
       0,     0,     0,   445,   443,   318,     0,     0,     0,     0,
       0,     0,     0,     0,   361,     0,   135,   505,     0,   511,
       0,   399,   397,   398,     0,     0,   492,     0,     0,   515,
     519,     0,     0,   495,     0,     0,     0,     0,   432,     0,
     439,     0,   490,   447,   652,   632,   131,   558,   559,   560,
     561,   552,   566,   363,   613,   736,   759,   762,   725,   765,
     755,   721,   715,   769,   771,   776,   778,   783,   785,   789,
     583,   792,   589,     0,     0,   671,     0,     0,   858,   861,
       0,   288,     0,   293,   294,   292,     0,   342,     0,     0,
     345,   340,     0,     0,     0,   894,   892,   267,     0,    90,
     331,   334,   337,   898,   896,   867,   876,   874,   864,     0,
     135,     0,     0,   835,     0,   870,   828,   851,   843,   871,
     879,   841,     0,   835,     0,   847,   848,   855,   839,     0,
     272,   275,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,    67,     0,    74,     0,   234,
     132,   444,     0,   263,     0,     0,   605,     0,   358,   359,
       0,   357,   356,     0,     0,     0,     0,     0,     0,     0,
     420,     0,     0,   516,     0,   504,     0,   493,     0,   263,
     433,     0,     0,   491,   440,   436,     0,   571,   555,   552,
     553,   554,   557,     0,   772,   779,   786,   263,   270,   674,
     599,     0,     0,    85,   289,   295,     0,     0,     0,     0,
     341,   884,   887,   890,     0,     0,     0,     0,     0,     0,
       0,   849,     0,     0,     0,   237,     0,   539,     0,     0,
       0,   853,     0,     0,   846,     0,     0,   265,    30,    37,
       0,     0,     0,     0,     0,     0,   819,   818,   423,   548,
     426,     0,    66,     0,    83,   135,   418,     0,   319,   605,
       0,     0,   365,     0,   362,   364,     0,   350,   368,     0,
     547,     0,   545,   421,   542,     0,     0,     0,   541,     0,
     434,     0,   437,   556,   567,   555,     0,     0,   849,   849,
       0,     0,     0,     0,     0,     0,     0,   263,   907,   267,
     332,   335,   338,     0,   850,   868,   849,   849,   501,     0,
     367,   540,   909,   854,   909,   856,   909,   273,   263,   276,
      34,   909,   909,     0,   821,   820,     0,     0,   429,    68,
     299,     0,    75,    79,    80,    77,    78,    76,     0,     0,
       0,     0,     0,     0,   360,     0,   351,   506,   512,     0,
     546,   544,     0,   543,     0,   569,   601,   600,   562,   740,
     834,   834,   343,     0,     0,   346,   849,   849,   824,     0,
       0,   911,   834,   824,   502,     0,     0,     0,     0,   911,
      31,     0,     0,   909,     0,     0,   427,     0,     0,     0,
     305,   370,   419,     0,     0,     0,   327,   378,   352,   507,
     513,   517,   435,     0,     0,     0,   831,   911,   833,     0,
       0,     0,     0,   834,   834,   825,     0,   895,   908,     0,
       0,     0,     0,     0,     0,     0,   912,     0,   917,   913,
       0,   823,   822,   430,   306,     0,   325,   378,   323,   378,
     326,   518,   568,     0,     0,   912,   832,   859,   862,   344,
     347,     0,     0,   891,   899,   877,   865,   910,   915,   916,
     918,   266,   914,   321,   378,   324,   322,   570,   829,     0,
     885,   888,   320,     0,   830
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1519, -1519, -1519, -1519, -1519, -1519,   675,  1301, -1519, -1519,
   -1519, -1519, -1519, -1519,  1392, -1519, -1519, -1519,   893,  1350,
   -1519,  1259, -1519, -1519,  1313, -1519, -1519, -1519,   -99,    -1,
   -1519, -1519, -1519,   -98, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519,  1171, -1519, -1519,   -61,   -48, -1519, -1519,
   -1519,   627,   592,  -536,  -570,  -799, -1519, -1519, -1519, -1518,
   -1519, -1519,     8,  -190,  -219,  -383, -1519,   417, -1519,  -605,
   -1519,  -667,  -811,  -281, -1519, -1519, -1519, -1519,  -457,     4,
   -1519, -1519, -1519, -1519, -1519,   -87,   -82,   -77, -1519,   -76,
   -1519, -1519, -1519,  1417, -1519,   426, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,  -169,
     -52,   310,    17,   175,  -903,  -430, -1519,  -528, -1519, -1519,
    -377,   440, -1519, -1519, -1519, -1467, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519,   953, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519,  -148,    -5,  -100,    -3,   141, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519,   619,  -412,  -886, -1519,  -418,
    -880, -1519,  -630,   -95,   -94, -1519,  -580, -1342, -1519,  -351,
   -1519, -1519,  1388, -1519, -1519, -1519,   876,  1058,   121, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,  -616,
    -195, -1519,   988, -1519, -1519, -1519, -1519, -1519, -1519,  -406,
   -1519, -1519,  -368, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
    -228, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519,   991,  -640,  -178,  -726,
    -687, -1519, -1519, -1083,  -916, -1519, -1519, -1519, -1166,   -34,
   -1197, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
     241,  -494, -1519, -1519, -1519,   743, -1519, -1519, -1519, -1519,
   -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519, -1519,
   -1255,  -723, -1519
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   153,    56,    18,   175,   181,  1570,  1383,
    1488,   719,   519,   159,   520,   105,    20,    21,    47,    48,
      49,    94,    22,    41,    42,   521,   522,  1243,  1244,   627,
     524,  1398,  1503,   525,   526,   988,   527,   648,   528,   529,
     530,   531,  1175,   182,   183,    37,    38,    39,   231,    71,
      72,    73,    74,    24,   296,   388,   221,    25,   117,   222,
     118,   166,   297,   148,   697,  1015,   534,   389,   535,   731,
    1569,   721,  1123,   583,   960,  1486,   962,  1487,   537,   538,
     539,   650,   877,  1454,   540,   541,   542,   543,   544,   545,
     546,   547,   440,   548,   755,  1256,   998,   549,   550,   915,
    1467,   916,  1468,   918,  1469,   551,   882,  1460,   552,   732,
    1518,   553,  1264,  1265,  1002,   699,   554,   812,   990,   555,
     664,  1016,   557,   558,   559,   986,   560,  1238,  1577,  1239,
    1637,   561,  1089,  1434,   562,   733,  1416,  1649,  1418,  1650,
    1525,  1691,   564,   383,  1442,  1534,  1299,  1301,  1099,   577,
     822,  1604,  1654,   384,   385,   386,   846,   847,   433,   848,
     849,   434,   977,   639,   640,  1608,   591,   404,   320,   321,
     228,   313,    84,   129,    27,   171,   391,    95,    96,   185,
      97,    28,    53,   121,   168,    29,   308,   575,   572,  1094,
     393,   226,   227,    82,   126,   395,    30,   169,   310,   641,
     565,   307,   366,   367,   839,   432,   368,   599,  1311,  1120,
     832,   430,   369,   592,  1305,   851,   597,  1310,   593,  1306,
     594,  1307,   596,  1309,   600,  1313,   601,  1444,   602,  1315,
     603,  1445,   604,  1317,   605,  1446,   606,  1319,   607,  1321,
     630,    31,   101,   188,   372,   631,    32,   102,   189,   373,
     635,    33,   100,   187,   371,   624,   566,   737,  1666,   738,
     946,  1657,  1658,  1659,   947,   959,  1218,  1212,  1207,  1377,
    1141,   567,   873,  1451,   874,  1452,   927,  1473,   924,  1471,
     948,   722,   568,   925,  1472,   949,   569,  1147,  1544,  1148,
    1545,  1149,  1546,   886,  1464,   922,  1470,   715,   723,   570,
    1626,   969,   571
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   910,   662,   298,   714,   852,   533,   827,   746,   974,
     370,   872,   556,   978,   980,    52,   633,   756,    76,    64,
      75,   628,   995,   232,   952,   138,   692,   629,   634,  1188,
    1203,   841,   876,   843,   587,   845,  1215,  1367,   312,  1184,
    1136,   820,  1114,   365,  1116,   578,  1118,  1186,   656,  1000,
    1195,   940,   611,   940,    90,  1213,  -130,   700,   701,   130,
     131,   161,   867,   621,  1511,   110,  1219,   112,   113,   945,
     381,   945,    75,    75,    75,   229,   533,    34,    35,    62,
     229,  1226,  1228,    40,   420,   613,  1653,  1386,    83,    91,
     867,     2,   939,   114,   950,   145,   954,   150,     3,   104,
     533,  1440,   779,   734,   966,   782,   783,   734,   536,    63,
    1636,   421,   422,   970,    75,    75,    75,    75,  1248,   176,
     177,     4,  1248,     5,   747,     6,    54,   869,   709,   711,
     431,     7,    57,   382,   828,   230,  1693,   170,    58,   438,
     230,     8,  1338,   782,   783,   657,   658,     9,   186,  1223,
     858,  1224,  1140,   653,   868,   869,  1339,   870,   734,   445,
     871,  1441,   932,  1249,  1151,   396,  1655,  1591,   748,   315,
    1683,    10,    55,  1248,    98,   423,   224,    85,   536,   424,
     381,   314,   439,  1202,    62,   229,    50,    86,   759,   760,
     810,   811,   365,    11,    12,   239,   240,   241,  1481,   532,
    1182,    87,   536,  1185,   888,  1001,    51,   365,   533,    88,
     132,   868,   146,    13,    63,   133,   612,   134,   149,    57,
     135,   868,   859,   759,   760,    58,   868,   862,   810,   811,
     317,    36,   147,    14,   929,    92,   365,   868,   365,   146,
     682,   659,   364,   666,   425,   230,    93,   318,   426,   614,
    1187,   427,   868,   868,   420,   398,    13,   683,   868,   147,
     660,   136,  1343,   818,    86,  1250,   428,   615,   319,  1344,
     178,   318,   429,   616,  1555,   179,    14,   180,   533,   996,
     135,   421,   422,  1364,  1359,   771,   772,   110,   111,   112,
     663,   191,   319,   779,   115,    15,   782,   783,   784,   909,
     116,   365,   365,   785,    62,    13,    16,   103,   533,  1627,
     536,  1628,   104,   982,  1563,   390,  1631,  1632,   394,   236,
     771,   772,   928,    77,    78,    14,    79,  1003,   779,  1252,
     781,   782,   783,   784,    63,   890,   997,    13,   785,  1111,
     109,  1610,  1611,   588,  1400,   423,   237,   736,   588,   424,
     890,    50,  1251,   589,    80,  1229,  1253,    14,   589,  1622,
    1623,   160,  1293,   139,   830,   831,   833,   419,   835,   836,
    1102,    51,   840,   141,   842,   609,   844,  1333,  1680,   890,
     536,   810,   811,   441,   443,   817,  1510,   821,   523,   403,
     574,   364,   316,   867,   610,    65,   576,   365,   365,   365,
    1443,   365,   365,  1389,   999,   365,   364,   365,   590,   365,
     536,   365,  1332,   590,   425,    86,   810,   811,   426,  1663,
    1664,   427,   431,   867,    66,    81,  1105,  1108,   875,  1516,
     158,   623,  1266,   976,   857,   364,   428,   364,   144,   104,
     397,   860,   429,  1122,   533,   863,  1209,  1480,   113,  1210,
      50,  1204,  1205,   913,  1247,  1100,    93,  1483,   869,   364,
     654,  1590,  1404,  1222,   403,  1259,   867,  1415,    67,   637,
      51,   677,   365,  1296,  1227,  1260,   976,  1211,   154,  1206,
     536,   536,   536,   536,   536,   536,   536,   536,   869,  1406,
     638,   887,   889,   536,   536,   365,   757,    68,   961,   536,
     364,   364,  1327,  1172,    62,  1134,  1135,   155,   536,   536,
    1196,  1261,  1363,   536,   536,   536,   923,   536,  1150,   926,
     740,   741,   867,   987,  1413,    13,   867,  1373,  1660,    13,
    1262,   869,  1493,   890,    63,  1263,  1634,    50,   749,  1670,
     750,   751,   752,   420,   753,    14,   536,  1176,  1177,    14,
    1179,  1512,   815,    50,  1181,   758,  1183,    51,   890,   945,
    1523,  1476,  1366,   431,  1007,  1011,   156,  1106,    50,    69,
     421,   422,   816,    51,   945,   890,   819,   157,    70,   994,
    1701,  1702,  1271,  1272,  1273,  1530,   826,   869,    51,   890,
    1051,   869,  1201,   517,   696,  1501,   364,   364,   364,   223,
     364,   364,  1532,   167,   364,  1390,   364,  1201,   364,  1201,
     364,    13,    91,    62,  1046,  1090,  1561,   225,  1360,  1718,
      13,   856,   982,  1124,   420,   122,   123,   983,   365,  1047,
    1127,    14,  1402,  1128,   423,  1404,  1724,   626,   424,    13,
      14,    13,  1132,    63,  1093,  1119,    43,    44,    45,  1137,
      13,   421,   422,   110,  1146,   190,   365,   879,  1413,    14,
    1405,    14,   295,   365,   238,   626,   365,   442,   984,  1394,
      14,   364,    13,  1121,  1098,   365,   626,    46,   365,   365,
    1465,   146,   365,  1414,  1323,    13,   431,   365,  1201,  1170,
    1109,   365,    14,   632,   364,   106,   107,   108,   626,  1201,
     293,   147,   431,   425,  1322,    14,  1588,   426,  1320,  1139,
     427,   626,   294,  1589,   309,   423,    13,  1324,   588,   424,
     365,   365,   667,   365,  1602,   428,   684,   365,   589,   365,
     365,   429,  1124,    13,  1124,    13,    14,   162,   163,   164,
     165,   668,   626,   985,    13,   685,   447,   448,   146,   311,
    1356,    13,   989,    14,   431,    14,    13,  1494,  1110,   626,
    1234,   626,   374,  1507,    14,   365,   458,   365,   147,  1358,
     626,    14,   463,    13,   736,  1235,    14,   626,  1380,  1342,
    1122,  1438,   736,   590,   425,  1381,   687,   375,   426,  1529,
     376,   427,   377,    14,    13,    13,   698,   698,   698,   626,
      13,  1300,   573,    13,  1348,   688,   428,  1456,   378,   477,
     478,   622,   429,   574,    14,    14,   880,  1324,  1324,   379,
      14,   626,    75,    14,  1282,  1287,   626,   364,  1104,  1462,
    1505,  1475,   734,   299,  1463,   881,   387,   300,  1112,  1283,
    1288,   380,  1537,   480,   481,  -770,   745,  1248,  -777,  -784,
    -770,   301,   302,  -777,  -784,   364,   303,   304,   305,   306,
     119,   399,   364,   533,   392,   364,   120,   420,  -770,   556,
    1393,  -777,  -784,   745,   364,   431,  1280,   364,   364,  1113,
    1308,   364,  1620,   124,    62,  1240,   364,   431,   400,   125,
     364,  1115,   759,   760,   421,   422,  1241,  1242,  1669,   405,
    -428,   496,   497,   498,  1294,  -428,  1677,   517,   696,   401,
     127,   151,   402,   365,    63,   403,   128,   152,   406,   364,
     364,  1297,   364,  -428,   509,   365,   364,  1298,   364,   364,
    1457,  1593,   431,   407,  1696,  1479,  1117,  1374,  1122,   649,
    1375,   890,  1619,  1376,  1482,  1485,  1635,   408,  1490,  1491,
     890,   890,   365,   365,   890,   890,   515,   431,   423,   233,
     234,  1331,   424,  1629,   364,   536,   364,  1330,  1573,   431,
    1656,  1656,   431,  1341,   890,   745,  1598,   365,  1665,   409,
     431,   435,  1656,  1665,  1651,   142,   143,   769,   770,   771,
     772,  1566,   172,   173,   963,   964,  1571,   779,   410,   781,
     782,   783,   784,   745,   411,  1216,  1209,   785,  1217,   787,
     788,  1644,  1645,   418,  1474,  1694,   517,   696,   759,   760,
     412,  1395,   413,  1656,  1656,   414,  1362,   425,   110,   111,
     112,   426,   431,  1303,   427,    43,    44,    45,   172,   173,
     174,   415,   416,   365,  1372,   233,   234,   235,  1519,   428,
    1379,   956,   957,   958,   417,   429,  1399,  1384,   436,  1385,
     642,   643,   644,  1685,   437,   759,   760,   698,    59,    60,
      61,   805,   806,   807,   808,   809,   444,   585,   586,  1723,
     598,   579,   673,   645,   619,   810,   811,   646,   655,   671,
     674,   676,   675,   678,  1295,   679,  1448,   686,  1302,   689,
     690,   739,  1304,   702,   703,  1421,   718,   743,   704,   744,
     823,  1453,   364,   705,   706,   771,   772,  1431,   420,   754,
     707,    16,  1436,   779,   364,   781,   782,   783,   784,   365,
     813,   682,   716,   785,   825,   837,  1646,   838,   632,   745,
     853,   861,  1592,   865,   866,   421,   422,   878,   883,   884,
     885,   364,   364,   890,   757,   908,   920,   919,   914,   931,
     967,   968,   771,   772,   971,   972,   973,  1042,   979,  1097,
     779,   975,   781,   782,   783,   784,   364,  1502,   981,  1101,
     785,  1107,  1130,  1131,  1144,  1125,  1173,  1686,  1688,  1201,
    1521,  1208,  1478,  1255,  1267,  1236,  1517,   805,   806,   807,
     808,   809,  1018,  1020,  1022,  1024,  1026,  1028,  1030,   423,
     533,   810,   811,   424,  1035,  1037,  1221,  1492,  1290,  1233,
    1043,   365,  1643,   365,  1237,  1269,  1254,  1300,  1713,  1055,
    1057,  1258,  1268,  1270,  1062,  1064,  1066,  1312,  1069,  1314,
     745,  1274,   364,  1275,  1316,   523,   807,   808,   809,  1318,
     698,  1285,  1286,  1291,  1326,  1334,  1335,  1517,   810,   811,
    1336,   533,   533,  1368,  1346,   420,  1347,  1408,  1409,  1354,
    1357,  1361,  1371,  1378,  1423,  1411,  1388,  1410,   425,  1401,
    1412,  1449,   426,  1424,  1328,   427,  1426,  1432,  1447,  1458,
    1484,  1455,   421,   422,  1459,  1437,  1489,  1599,  1506,  1508,
     428,  1513,   533,  1533,  1509,  1515,   429,  1474,  1541,  1538,
    1539,  1542,   536,  1547,  1554,   365,  1548,  1549,  1578,  1613,
    1553,  1556,   698,   698,   698,  1625,  1557,   745,   364,   745,
     365,   745,  1594,   745,  1568,   745,  1609,   745,  1614,   745,
    1616,   745,   563,  1630,  1617,  1672,   745,  1618,   745,  1642,
    1652,  1667,   584,  1674,   745,  1668,   423,  1675,  1673,  1676,
     424,   595,  1678,   536,   536,  1679,   745,  1695,   745,  1641,
    1697,   608,  1698,   745,  1703,   745,  1704,   745,  1708,  1709,
     745,   618,  1705,  1706,  1621,  1710,   137,   420,  1711,  1719,
     636,  1712,  1720,    19,   965,   365,    89,  1504,  1721,  1397,
     184,   140,   647,  1579,   536,  1582,  1246,   322,   759,   760,
    1096,   663,   665,   745,   421,   422,  1583,   670,    26,   672,
     364,  1584,   364,  1257,  1560,   425,  1585,  1586,   681,   426,
    1514,  1329,   427,  1407,  1535,  1605,   693,   694,   695,  1536,
    1439,  1606,  1607,   651,    99,  1671,   652,   428,   814,  1565,
    1370,   713,     0,   429,   953,     0,     0,     0,     0,   717,
       0,     0,   713,   724,   725,   726,   727,   728,     0,   889,
       0,     0,     0,     0,  1717,     0,     0,   742,   423,     0,
       0,     0,   424,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   759,   760,   769,   770,   771,   772,   773,     0,     0,
     776,   777,   778,   779,   364,   781,   782,   783,   784,    13,
       0,     0,     0,   785,     0,   787,   788,     0,     0,   364,
       0,   791,   792,   793,     0,     0,     0,   797,     0,    14,
       0,     0,     0,     0,   829,   626,     0,   425,     0,     0,
       0,   426,     0,  1171,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   428,
       0,     0,   855,     0,     0,   429,     0,     0,     0,     0,
     799,   985,   800,   801,   802,   803,   804,   805,   806,   807,
     808,   809,     0,     0,   364,     0,   769,   770,   771,   772,
     773,   810,   811,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,   665,     0,   785,     0,   787,   788,
       0,   713,   911,     0,   912,     0,     0,     0,     0,   917,
       0,     0,     0,     0,     0,   921,     0,     0,     0,     0,
       0,   985,     0,     0,   930,     0,  1690,     0,     0,     0,
       0,     0,  1692,     0,     0,   933,   934,   935,   936,   937,
     938,     0,   944,     0,   944,     0,     0,     0,     0,   745,
       0,     0,     0,     0,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,     0,  1715,     0,  1716,     0,
     759,   760,     0,     0,   810,   811,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1722,  1017,  1019,  1021,  1023,  1025,  1027,
    1029,  1031,  1032,  1033,     0,     0,  1034,  1036,  1038,  1039,
    1040,  1041,     0,  1044,  1045,     0,  1048,  1049,  1050,  1052,
    1053,  1054,  1056,  1058,  1059,  1060,  1061,  1063,  1065,  1067,
    1068,  1070,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,     0,  1091,     0,     0,     0,  1095,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1103,     0,     0,     0,     0,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,     0,     0,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,  1129,   797,
       0,     0,     0,     0,     0,  1133,     0,     0,     0,     0,
       0,     0,  1143,     0,  1145,     0,     0,     0,   759,   760,
       0,     0,     0,  1152,  1153,  1154,  1155,  1156,  1157,  1158,
    1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,     0,   799,   420,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   -76,     0,     0,     0,     0,     0,
       0,     0,     0,   810,   811,   759,   760,     0,   517,   696,
     421,   422,     0,     0,   724,  1198,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1220,     0,
     713,     0,     0,     0,     0,     0,     0,  1225,     0,     0,
       0,   713,     0,     0,     0,     0,  1152,     0,  1230,  1231,
    1232,     0,     0,   769,   770,   771,   772,   773,     0,     0,
     776,  1245,     0,   779,     0,   781,   782,   783,   784,     0,
       0,     0,     0,   785,   423,   787,   788,   745,   424,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   761,   762,   763,   764,   765,   766,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   783,   784,     0,   745,     0,   745,
     785,   786,   787,   788,   789,   790,     0,     0,   791,   792,
     793,   794,   795,   796,   797,   803,   804,   805,   806,   807,
     808,   809,     0,   425,   745,     0,     0,   426,     0,  1345,
     427,   810,   811,     0,     0,     0,     0,  1284,     0,     0,
    1004,  1289,   420,     0,     0,   428,     0,     0,     0,     0,
       0,   429,     0,   420,     0,     0,   798,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   421,
     422,     0,     0,     0,     0,     0,     0,     0,   810,   811,
     421,   422,     0,   517,   696,     0,     0,     0,     0,     0,
       0,     0,   329,   330,   331,     0,   333,   334,   335,   336,
     337,   479,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,     0,   351,   352,   353,     0,     0,   356,
     357,   358,   359,     0,     0,     0,     0,     0,  1349,     0,
       0,     0,     0,   423,     0,     0,     0,   424,     0,     0,
       0,     0,     0,     0,   423,     0,     0,     0,   424,     0,
       0,     0,     0,  1365,     0,     0,     0,     0,     0,     0,
       0,  1369,   944,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1005,
       0,     0,     0,     0,     0,     0,   708,   759,   760,  1391,
    1392,     0,   323,     0,  1006,     0,  1396,     0,   324,   563,
       0,     0,   425,     0,   325,  1152,   426,     0,  1350,   427,
       0,     0,     0,   425,   326,     0,     0,   426,     0,  1351,
     427,  1417,   327,  1419,   428,     0,     0,     0,  1422,     0,
     429,     0,     0,     0,  1425,   428,     0,   328,  1428,     0,
       0,   429,     0,     0,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,     0,     0,     0,
    1450,     0,   769,   770,   771,   772,   773,     0,     0,   776,
     777,   778,   779,   420,   781,   782,   783,   784,     0,     0,
     713,     0,   785,     0,   787,   788,     0,     0,     0,     0,
     791,   792,   793,     0,     0,     0,    62,     0,     0,     0,
     421,   422,     0,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1495,  1496,  1497,     0,   420,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   799,
       0,   800,   801,   802,   803,   804,   805,   806,   807,   808,
     809,     0,   421,   422,     0,   759,   760,  1526,     0,  1527,
     810,   811,     0,     0,   423,  1531,     0,     0,   424,     0,
       0,     0,   363,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1540,
       0,     0,  1543,     0,     0,     0,     0,     0,     0,     0,
    1550,  1551,  1552,     0,     0,     0,     0,  1559,     0,     0,
       0,     0,  1562,     0,     0,  1564,   423,     0,   713,  1567,
     424,     0,     0,   713,  1572,     0,  1574,  1575,     0,     0,
       0,     0,     0,   425,     0,     0,  1587,   426,     0,  1352,
     427,     0,     0,     0,     0,     0,     0,     0,     0,  1596,
     769,   770,   771,   772,   773,   428,     0,   776,   777,   778,
     779,   429,   781,   782,   783,   784,     0,     0,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,   713,     0,   797,   425,     0,     0,     0,   426,
       0,  1353,   427,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1633,   428,     0,     0,
       0,     0,     0,   429,  1640,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1647,   799,  1648,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,     0,
       0,     0,     0,     0,     0,     0,  1661,  1662,   810,   811,
       0,     0,   824,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1681,  1682,     0,
       0,     0,  1684,     0,     0,     0,   446,  1687,  1689,     0,
     447,   448,     3,     0,   449,   450,   451,     0,   452,     0,
     453,   454,   455,   456,   457,     0,     0,     0,     0,     0,
     458,   459,   460,   461,   462,  1707,   463,     0,     0,     0,
       0,     0,     0,   464,   465,     0,     0,   466,  1714,   467,
     468,     0,     0,   469,     0,     8,   470,   471,     0,   472,
     473,     0,     0,   474,   475,     0,     0,     0,     0,     0,
     476,     0,     0,   477,   478,     0,   329,   330,   331,     0,
     333,   334,   335,   336,   337,   479,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,     0,   351,   352,
     353,     0,     0,   356,   357,   358,   359,   480,   481,   482,
     483,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   489,   490,   491,   492,
     493,     0,   494,     0,   495,   496,   497,   498,     0,   146,
      13,   499,   500,   501,   502,   503,   504,   505,    63,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   509,   147,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   510,   511,   512,     0,    15,
       0,     0,   513,   514,     0,     0,   447,   448,     0,     0,
     515,     0,   516,     0,   517,   518,   453,   454,   455,   456,
     457,     0,     0,     0,     0,     0,   458,     0,   460,     0,
       0,     0,   463,     0,   420,     0,     0,     0,     0,     0,
     465,     0,     0,     0,     0,     0,   468,     0,     0,   469,
       0,     0,   470,     0,   940,     0,   473,     0,     0,     0,
       0,   421,   422,     0,     0,     0,   580,     0,     0,   477,
     478,     0,   329,   330,   331,     0,   333,   334,   335,   336,
     337,   479,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,     0,   351,   352,   353,     0,     0,   356,
     357,   358,   359,   480,   481,   581,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   484,
     485,     0,     0,     0,     0,   423,     0,     0,     0,   424,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   489,   490,   491,   492,   493,     0,   494,   734,
     495,   496,   497,   498,     0,     0,     0,   499,   500,   501,
     502,   503,   504,   505,   735,   582,   507,   508,     0,     0,
       0,     0,     0,     0,   509,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   425,     0,     0,     0,   426,     0,
       0,   941,   511,   512,     0,    15,     0,     0,   513,   514,
       0,     0,     0,   447,   448,     0,   942,     0,   943,     0,
     517,   518,   429,   453,   454,   455,   456,   457,     0,     0,
       0,     0,     0,   458,     0,   460,     0,     0,     0,   463,
       0,   420,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   468,     0,     0,   469,     0,     0,   470,
       0,     0,     0,   473,     0,     0,     0,     0,   421,   422,
       0,     0,     0,   580,     0,     0,   477,   478,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   479,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     480,   481,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,   423,     0,     0,     0,   424,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,     0,   494,   734,   495,   496,   497,
     498,     0,     0,     0,   499,   500,   501,   502,   503,   504,
     505,   735,   582,   507,   508,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   425,     0,     0,     0,   426,     0,     0,   941,   511,
     512,     0,    15,     0,     0,   513,   514,     0,     0,     0,
     447,   448,     0,   942,     0,   951,     0,   517,   518,   429,
     453,   454,   455,   456,   457,     0,     0,     0,     0,     0,
     458,     0,   460,     0,     0,     0,   463,     0,   613,     0,
       0,     0,     0,     0,   465,     0,     0,     0,     0,     0,
     468,     0,     0,   469,     0,     0,   470,     0,     0,     0,
     473,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     580,     0,     0,   477,   478,     0,   329,   330,   331,     0,
     333,   334,   335,   336,   337,   479,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,     0,   351,   352,
     353,     0,     0,   356,   357,   358,   359,   480,   481,   581,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   484,   485,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   489,   490,   491,   492,
     493,     0,   494,     0,   495,   496,   497,   498,     0,     0,
       0,   499,   500,   501,   502,   503,   504,   505,    63,   582,
     507,   508,     0,     0,     0,     0,     0,     0,   509,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   614,     0,     0,   510,   511,   512,     0,    15,
       0,     0,   513,   514,     0,     0,     0,   447,   448,     0,
    1197,     0,   516,     0,   517,   518,   616,   453,   454,   455,
     456,   457,     0,     0,     0,     0,     0,   458,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,   465,     0,     0,     0,     0,     0,   468,     0,     0,
     469,     0,     0,   470,     0,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   580,     0,     0,
     477,   478,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   479,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   480,   481,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,   661,
       0,     0,     0,     0,     0,   486,   487,   488,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   489,   490,   491,   492,   493,     0,   494,
       0,   495,   496,   497,   498,     0,     0,     0,   499,   500,
     501,   502,   503,   504,   505,    63,   582,   507,   508,     0,
       0,     0,     0,     0,     0,   509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,     0,    15,     0,     0,   513,
     514,     0,     0,   447,   448,     0,     0,   515,     0,   516,
       0,   517,   518,   453,   454,   455,   456,   457,     0,     0,
       0,     0,     0,   458,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   468,     0,     0,   469,     0,     0,   470,
       0,     0,     0,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,     0,     0,   477,   478,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   479,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     480,   481,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,   691,     0,     0,     0,     0,
       0,   486,   487,   488,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,     0,   494,     0,   495,   496,   497,
     498,     0,     0,     0,   499,   500,   501,   502,   503,   504,
     505,    63,   582,   507,   508,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   510,   511,
     512,     0,    15,     0,     0,   513,   514,     0,     0,   447,
     448,     0,     0,   515,     0,   516,     0,   517,   518,   453,
     454,   455,   456,   457,     0,     0,     0,     0,     0,   458,
    1580,   460,   461,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,     0,     0,   468,
       0,     0,   469,     0,     0,   470,   471,     0,     0,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
       0,     0,   477,   478,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   479,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   480,   481,   581,  1581,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,   493,
       0,   494,     0,   495,   496,   497,   498,     0,     0,     0,
     499,   500,   501,   502,   503,   504,   505,    63,   582,   507,
     508,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,   511,   512,     0,    15,     0,
       0,   513,   514,     0,     0,   447,   448,     0,     0,   515,
       0,   516,     0,   517,   518,   453,   454,   455,   456,   457,
       0,     0,     0,     0,     0,   458,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   468,     0,     0,   469,     0,
       0,   470,     0,     0,     0,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   580,     0,     0,   477,   478,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     479,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   480,   481,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   486,   487,   488,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   489,   490,   491,   492,   493,     0,   494,     0,   495,
     496,   497,   498,     0,     0,     0,   499,   500,   501,   502,
     503,   504,   505,    63,   582,   507,   508,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   511,   512,     0,    15,     0,     0,   513,   514,     0,
       0,   447,   448,     0,     0,   515,     0,   516,     0,   517,
     518,   453,   454,   455,   456,   457,     0,     0,     0,     0,
       0,   458,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,   465,     0,     0,     0,     0,
       0,   468,     0,     0,   469,     0,     0,   470,     0,     0,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   580,     0,     0,   477,   478,   991,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   479,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   480,   481,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   489,   490,   491,
     492,   493,     0,   494,   734,   495,   496,   497,   498,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,   735,
     582,   507,   508,     0,     0,     0,     0,     0,     0,   509,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,     0,
      15,     0,     0,   513,   514,     0,     0,   447,   448,     0,
       0,   992,     0,   516,   993,   517,   518,   453,   454,   455,
     456,   457,     0,     0,     0,     0,     0,   458,     0,   460,
       0,     0,   420,   463,     0,     0,     0,     0,     0,     0,
       0,   465,     0,     0,     0,     0,     0,   468,     0,     0,
     469,     0,     0,   470,     0,     0,     0,   473,     0,   421,
     422,     0,     0,     0,     0,     0,     0,   580,     0,     0,
     477,   478,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   479,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   480,   481,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     484,   485,     0,   423,     0,     0,     0,   424,     0,     0,
       0,     0,     0,     0,     0,  1012,  1013,  1014,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   489,   490,   491,   492,   493,     0,   494,
       0,   495,   496,   497,   498,     0,     0,     0,   499,   500,
     501,   502,   503,   504,   505,    63,   582,   507,   508,     0,
       0,     0,     0,     0,     0,   509,     0,     0,     0,     0,
       0,     0,   425,     0,     0,     0,   426,     0,  1355,   427,
       0,     0,   510,   511,   512,     0,    15,     0,     0,   513,
     514,     0,     0,     0,   428,   447,   448,   515,     0,   516,
     429,   517,   518,   729,     0,   453,   454,   455,   456,   457,
       0,     0,     0,     0,     0,   458,     0,   460,     0,     0,
     420,   463,     0,     0,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   468,     0,     0,   469,   730,
       0,   470,     0,     0,     0,   473,     0,   421,   422,     0,
       0,     0,     0,     0,     0,   580,     0,     0,   477,   478,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     479,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   480,   481,   581,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   485,
       0,   423,     0,     0,     0,   424,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   489,   490,   491,   492,   493,     0,   494,     0,   495,
     496,   497,   498,     0,     0,     0,   499,   500,   501,   502,
     503,   504,   505,    63,   582,   507,   508,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
     425,     0,     0,     0,   426,     0,  1461,   427,     0,     0,
     510,   511,   512,     0,    15,     0,     0,   513,   514,     0,
       0,     0,   428,   447,   448,   515,   617,   516,   429,   517,
     518,   729,     0,   453,   454,   455,   456,   457,     0,     0,
       0,     0,     0,   458,     0,   460,     0,     0,   420,   463,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   468,     0,     0,   469,   730,     0,   470,
       0,     0,     0,   473,     0,   421,   422,     0,     0,     0,
       0,     0,     0,   580,     0,     0,   477,   478,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   479,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     480,   481,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,     0,   423,
       0,     0,     0,   424,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,     0,   494,   734,   495,   496,   497,
     498,     0,     0,     0,   499,   500,   501,   502,   503,   504,
     505,   735,   582,   507,   508,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,   425,     0,
       0,     0,   426,     0,  1466,   427,     0,     0,   510,   511,
     512,     0,    15,     0,     0,   513,   514,     0,     0,     0,
     428,   447,   448,   515,     0,   516,   429,   517,   518,   729,
       0,   453,   454,   455,   456,   457,     0,     0,     0,     0,
       0,   458,     0,   460,     0,     0,   420,   463,     0,     0,
       0,     0,     0,     0,     0,   465,     0,     0,     0,     0,
       0,   468,     0,     0,   469,   730,     0,   470,     0,     0,
       0,   473,     0,   421,   422,     0,     0,     0,     0,     0,
       0,   580,     0,     0,   477,   478,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   479,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   480,   481,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   484,   485,     0,   423,     0,     0,
       0,   424,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   489,   490,   491,
     492,   493,     0,   494,     0,   495,   496,   497,   498,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,    63,
     582,   507,   508,     0,     0,     0,     0,     0,     0,   509,
       0,     0,     0,     0,     0,     0,   425,     0,     0,     0,
     426,     0,  1500,   427,     0,     0,   510,   511,   512,     0,
      15,     0,     0,   513,   514,     0,     0,     0,   428,   447,
     448,   515,   853,   516,   429,   517,   518,   729,     0,   453,
     454,   455,   456,   457,     0,     0,     0,     0,     0,   458,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,     0,     0,   468,
       0,     0,   469,   730,     0,   470,     0,     0,     0,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
       0,     0,   477,   478,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   479,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   480,   481,   581,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,   493,
       0,   494,     0,   495,   496,   497,   498,     0,     0,     0,
     499,   500,   501,   502,   503,   504,   505,    63,   582,   507,
     508,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,   511,   512,     0,    15,     0,
       0,   513,   514,     0,     0,   447,   448,     0,     0,   515,
       0,   516,     0,   517,   518,   453,   454,   455,   456,   457,
       0,     0,     0,     0,     0,   458,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   468,     0,     0,   469,     0,
       0,   470,     0,     0,     0,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   580,     0,     0,   477,   478,
    1138,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     479,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   480,   481,   581,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   489,   490,   491,   492,   493,     0,   494,   734,   495,
     496,   497,   498,     0,     0,     0,   499,   500,   501,   502,
     503,   504,   505,   735,   582,   507,   508,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   511,   512,     0,    15,     0,     0,   513,   514,     0,
       0,   447,   448,     0,     0,   515,     0,   516,     0,   517,
     518,   453,   454,   455,   456,   457,     0,     0,     0,     0,
       0,   458,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,   465,     0,     0,     0,     0,
       0,   468,     0,     0,   469,     0,     0,   470,     0,     0,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   580,     0,     0,   477,   478,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   479,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   480,   481,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   489,   490,   491,
     492,   493,     0,   494,   734,   495,   496,   497,   498,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,   735,
     582,   507,   508,     0,     0,     0,     0,     0,     0,   509,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,     0,
      15,     0,     0,   513,   514,     0,     0,   447,   448,     0,
       0,   515,     0,   516,  1199,   517,   518,   453,   454,   455,
     456,   457,     0,     0,     0,     0,     0,   458,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,   465,     0,     0,     0,     0,     0,   468,     0,     0,
     469,     0,     0,   470,     0,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   580,     0,     0,
     477,   478,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   479,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   480,   481,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   489,   490,   491,   492,   493,     0,   494,
     734,   495,   496,   497,   498,     0,     0,     0,   499,   500,
     501,   502,   503,   504,   505,   735,   582,   507,   508,     0,
       0,     0,     0,     0,     0,   509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,     0,    15,     0,     0,   513,
     514,     0,     0,   447,   448,     0,     0,   515,     0,   516,
    1214,   517,   518,   453,   454,   455,   456,   457,     0,     0,
       0,     0,     0,   458,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   468,     0,     0,   469,     0,     0,   470,
       0,     0,     0,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,     0,     0,   477,   478,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   479,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     480,   481,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,     0,   494,     0,   495,   496,   497,
     498,     0,     0,     0,   499,   500,   501,   502,   503,   504,
     505,    63,   582,   507,   508,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   510,   511,
     512,     0,    15,     0,     0,   513,   514,     0,     0,   447,
     448,     0,     0,   515,   617,   516,     0,   517,   518,   453,
     454,   455,   456,   457,     0,     0,     0,     0,     0,   458,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,     0,     0,   468,
       0,     0,   469,     0,     0,   470,     0,     0,     0,   473,
       0,     0,     0,     0,     0,   669,     0,     0,     0,   580,
       0,     0,   477,   478,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   479,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   480,   481,   581,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,   493,
       0,   494,     0,   495,   496,   497,   498,     0,     0,     0,
     499,   500,   501,   502,   503,   504,   505,    63,   582,   507,
     508,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,   511,   512,     0,    15,     0,
       0,   513,   514,     0,     0,   447,   448,     0,     0,   515,
       0,   516,     0,   517,   518,   453,   454,   455,   456,   457,
       0,     0,     0,     0,     0,   458,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   468,     0,     0,   469,     0,
       0,   470,     0,     0,     0,   473,     0,     0,   680,     0,
       0,     0,     0,     0,     0,   580,     0,     0,   477,   478,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     479,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   480,   481,   581,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   489,   490,   491,   492,   493,     0,   494,     0,   495,
     496,   497,   498,     0,     0,     0,   499,   500,   501,   502,
     503,   504,   505,    63,   582,   507,   508,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   511,   512,     0,    15,     0,     0,   513,   514,     0,
       0,     0,     0,   447,   448,   515,     0,   516,     0,   517,
     518,   712,     0,   453,   454,   455,   456,   457,     0,     0,
       0,     0,     0,   458,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   468,     0,     0,   469,     0,     0,   470,
       0,     0,     0,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,     0,     0,   477,   478,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   479,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     480,   481,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,     0,   494,     0,   495,   496,   497,
     498,     0,     0,     0,   499,   500,   501,   502,   503,   504,
     505,    63,   582,   507,   508,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   510,   511,
     512,     0,    15,     0,     0,   513,   514,     0,     0,   447,
     448,     0,     0,   515,     0,   516,     0,   517,   518,   453,
     454,   455,   456,   457,     0,     0,     0,     0,     0,   458,
       0,   460,     0,     0,   420,   463,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,     0,     0,   468,
       0,     0,   469,     0,     0,   470,     0,     0,     0,   473,
       0,   421,   422,     0,     0,     0,     0,     0,     0,   580,
       0,     0,   477,   478,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   479,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   480,   481,   581,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,   485,     0,   423,     0,     0,     0,   424,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,   493,
       0,   494,     0,   495,   496,   497,   498,     0,     0,     0,
     499,   500,   501,   502,   503,   504,   505,    63,   582,   507,
     508,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,   425,     0,     0,     0,   426,     0,
    1597,   427,   716,     0,   510,   511,   512,     0,    15,     0,
       0,   513,   514,     0,     0,     0,   428,   447,   448,   515,
       0,   516,   429,   517,   518,   720,     0,   453,   454,   455,
     456,   457,     0,     0,     0,     0,     0,   458,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,   465,     0,     0,     0,     0,     0,   468,     0,     0,
     469,     0,     0,   470,     0,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   580,     0,     0,
     477,   478,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   479,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   480,   481,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   489,   490,   491,   492,   493,     0,   494,
       0,   495,   496,   497,   498,     0,     0,     0,   499,   500,
     501,   502,   503,   504,   505,    63,   582,   507,   508,     0,
       0,     0,     0,     0,     0,   509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,     0,    15,     0,     0,   513,
     514,     0,     0,   447,   448,     0,     0,   515,     0,   516,
       0,   517,   518,   453,   454,   455,   456,   457,     0,     0,
       0,     0,     0,   458,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   468,     0,     0,   469,     0,     0,   470,
       0,     0,     0,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,     0,     0,   477,   478,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   479,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     480,   481,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,     0,   494,   734,   495,   496,   497,
     498,     0,     0,     0,   499,   500,   501,   502,   503,   504,
     505,   735,   582,   507,   508,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   510,   511,
     512,     0,    15,     0,     0,   513,   514,     0,     0,   447,
     448,     0,     0,   515,     0,   516,     0,   517,   518,   453,
     454,   455,   456,   457,     0,     0,  1071,     0,     0,   458,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,     0,     0,   468,
       0,     0,   469,     0,     0,   470,     0,     0,     0,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
       0,     0,   477,   478,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   479,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   480,   481,   581,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,   493,
       0,   494,     0,   495,   496,   497,   498,     0,     0,     0,
     499,   500,   501,   502,   503,   504,   505,    63,   582,   507,
     508,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,   511,   512,     0,    15,     0,
       0,   513,   514,     0,     0,   447,   448,     0,     0,   515,
       0,   516,     0,   517,   518,   453,   454,   455,   456,   457,
       0,     0,     0,     0,     0,   458,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   468,     0,     0,   469,     0,
       0,   470,     0,     0,     0,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   580,     0,     0,   477,   478,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     479,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   480,   481,   581,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   489,   490,   491,   492,   493,     0,   494,     0,   495,
     496,   497,   498,     0,     0,     0,   499,   500,   501,   502,
     503,   504,   505,    63,   582,   507,   508,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   511,   512,     0,    15,     0,     0,   513,   514,     0,
       0,   447,   448,     0,     0,   515,     0,   516,  1092,   517,
     518,   453,   454,   455,   456,   457,     0,     0,     0,     0,
       0,   458,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,   465,     0,     0,     0,     0,
       0,   468,     0,     0,   469,     0,     0,   470,     0,     0,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   580,     0,     0,   477,   478,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   479,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   480,   481,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   489,   490,   491,
     492,   493,     0,   494,     0,   495,   496,   497,   498,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,    63,
     582,   507,   508,     0,     0,     0,     0,     0,     0,   509,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1142,     0,   510,   511,   512,     0,
      15,     0,     0,   513,   514,     0,     0,   447,   448,     0,
       0,   515,     0,   516,     0,   517,   518,   453,   454,   455,
     456,   457,     0,     0,     0,     0,     0,   458,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,   465,     0,     0,     0,     0,     0,   468,     0,     0,
     469,     0,     0,   470,     0,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   580,     0,     0,
     477,   478,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   479,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   480,   481,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   489,   490,   491,   492,   493,     0,   494,
       0,   495,   496,   497,   498,     0,     0,     0,   499,   500,
     501,   502,   503,   504,   505,    63,   582,   507,   508,     0,
       0,     0,     0,     0,     0,   509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,     0,    15,     0,     0,   513,
     514,     0,     0,   447,   448,     0,     0,   515,     0,   516,
    1420,   517,   518,   453,   454,   455,   456,   457,     0,     0,
       0,     0,     0,   458,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   468,     0,     0,   469,     0,     0,   470,
       0,     0,     0,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,     0,     0,   477,   478,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   479,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     480,   481,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,     0,   494,     0,   495,   496,   497,
     498,     0,     0,     0,   499,   500,   501,   502,   503,   504,
     505,    63,   582,   507,   508,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   510,   511,
     512,     0,    15,     0,     0,   513,   514,     0,     0,   447,
     448,     0,     0,  1429,     0,   516,  1430,   517,   518,   453,
     454,   455,   456,   457,     0,     0,     0,     0,     0,   458,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,     0,     0,   468,
       0,     0,   469,     0,     0,   470,     0,     0,     0,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
       0,     0,   477,   478,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   479,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   480,   481,   581,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,   493,
       0,   494,     0,   495,   496,   497,   498,     0,     0,     0,
     499,   500,   501,   502,   503,   504,   505,    63,   582,   507,
     508,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,   511,   512,     0,    15,     0,
       0,   513,   514,     0,     0,   447,   448,     0,     0,   515,
       0,   516,  1435,   517,   518,   453,   454,   455,   456,   457,
       0,     0,     0,     0,     0,   458,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   468,     0,     0,   469,     0,
       0,   470,     0,     0,     0,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   580,     0,     0,   477,   478,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     479,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   480,   481,   581,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   489,   490,   491,   492,   493,     0,   494,     0,   495,
     496,   497,   498,     0,     0,     0,   499,   500,   501,   502,
     503,   504,   505,    63,   582,   507,   508,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   511,   512,     0,    15,     0,     0,   513,   514,     0,
       0,   447,   448,     0,     0,   515,     0,   516,  1477,   517,
     518,   453,   454,   455,   456,   457,     0,     0,     0,     0,
       0,   458,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,   465,     0,     0,     0,     0,
       0,   468,     0,     0,   469,     0,     0,   470,     0,     0,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   580,     0,     0,   477,   478,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   479,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   480,   481,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   489,   490,   491,
     492,   493,     0,   494,     0,   495,   496,   497,   498,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,    63,
     582,   507,   508,     0,     0,     0,     0,     0,     0,   509,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,     0,
      15,     0,     0,   513,   514,     0,     0,   447,   448,     0,
       0,   515,     0,   516,  1558,   517,   518,   453,   454,   455,
     456,   457,     0,     0,     0,     0,     0,   458,     0,   460,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,   465,     0,     0,     0,     0,     0,   468,     0,     0,
     469,     0,     0,   470,     0,     0,     0,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   580,     0,     0,
     477,   478,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   479,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   480,   481,   581,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     484,   485,     0,     0,     0,     0,     0,     0,     0,  1595,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   489,   490,   491,   492,   493,     0,   494,
       0,   495,   496,   497,   498,     0,     0,     0,   499,   500,
     501,   502,   503,   504,   505,    63,   582,   507,   508,     0,
       0,     0,     0,     0,     0,   509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   511,   512,     0,    15,     0,     0,   513,
     514,     0,     0,   447,   448,     0,     0,   515,     0,   516,
       0,   517,   518,   453,   454,   455,   456,   457,     0,     0,
       0,     0,     0,   458,     0,   460,     0,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,   468,     0,     0,   469,     0,     0,   470,
       0,     0,     0,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,     0,     0,   477,   478,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   479,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     480,   481,   581,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,     0,     0,
       0,     0,     0,     0,     0,  1638,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,     0,   494,     0,   495,   496,   497,
     498,     0,     0,     0,   499,   500,   501,   502,   503,   504,
     505,    63,   582,   507,   508,     0,     0,     0,     0,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   510,   511,
     512,     0,    15,     0,     0,   513,   514,     0,     0,   447,
     448,     0,     0,   515,     0,   516,     0,   517,   518,   453,
     454,   455,   456,   457,     0,     0,     0,     0,     0,   458,
       0,   460,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,     0,     0,   468,
       0,     0,   469,     0,     0,   470,     0,     0,     0,   473,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
       0,     0,   477,   478,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   479,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   480,   481,   581,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   484,   485,     0,     0,     0,     0,     0,     0,
       0,  1639,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,   493,
       0,   494,     0,   495,   496,   497,   498,     0,     0,     0,
     499,   500,   501,   502,   503,   504,   505,    63,   582,   507,
     508,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,   511,   512,     0,    15,     0,
       0,   513,   514,     0,     0,   447,   448,     0,     0,   515,
       0,   516,     0,   517,   518,   453,   454,   455,   456,   457,
       0,     0,     0,     0,     0,   458,     0,   460,     0,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,   465,
       0,     0,     0,     0,     0,   468,     0,     0,   469,     0,
       0,   470,     0,     0,     0,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   580,     0,     0,   477,   478,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     479,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   480,   481,   581,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   484,   485,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   489,   490,   491,   492,   493,     0,   494,     0,   495,
     496,   497,   498,     0,     0,     0,   499,   500,   501,   502,
     503,   504,   505,    63,   582,   507,   508,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   511,   512,     0,    15,     0,     0,   513,   514,     0,
       0,   447,   448,     0,     0,   515,     0,   516,     0,   517,
     518,   453,   454,   455,   456,   457,     0,     0,     0,     0,
       0,   458,     0,   460,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,   465,     0,     0,     0,     0,
       0,   468,     0,     0,   469,     0,     0,   470,     0,     0,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   580,     0,     0,   477,   478,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   479,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   480,   481,
     581,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   484,   485,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   489,   490,   491,
     492,   493,     0,   494,     0,   495,   496,   497,   498,     0,
       0,     0,   499,   500,   501,   502,   503,   504,   505,    63,
     582,   507,   508,     0,   710,     0,     0,     0,     0,   509,
     323,     0,     0,     0,     0,     0,   324,     0,     0,     0,
       0,     0,   325,     0,     0,     0,   510,   511,   512,     0,
      15,     0,   326,   513,   514,     0,     0,     0,     0,     0,
     327,  1403,     0,   516,     0,   517,   518,     0,     0,     0,
       0,     0,     0,     0,     0,   328,     0,     0,     0,     0,
       0,     0,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   323,     0,     0,     0,     0,     0,   324,
       0,     0,     0,     0,     0,   325,     0,     0,     0,     0,
       0,     0,     0,     0,    62,   326,     0,     0,     0,     0,
       0,     0,     0,   327,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   328,     0,
       0,     0,     0,     0,    63,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,   323,     0,     0,     0,
       0,     0,   324,     0,     0,     0,     0,     0,   325,     0,
       0,     0,     0,     0,     0,     0,     0,    62,   326,     0,
       0,     0,     0,     0,     0,     0,   327,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   328,     0,     0,     0,     0,     0,    63,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,   620,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,    13,     0,     0,   323,     0,     0,     0,     0,
     625,   324,     0,     0,     0,     0,     0,   325,     0,     0,
       0,     0,    14,     0,     0,     0,     0,   326,   626,     0,
       0,     0,     0,     0,     0,   327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     328,     0,     0,     0,     0,     0,   363,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,     0,
       0,     0,     0,     0,   324,     0,     0,     0,     0,     0,
     325,     0,     0,     0,     0,     0,     0,     0,     0,    62,
     326,     0,     0,     0,     0,     0,     0,     0,   327,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,   759,   760,   328,     0,     0,     0,     0,     0,    63,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,     0,   759,   760,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   362,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,   625,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,     0,   793,     0,     0,     0,
       0,   891,   892,   893,   894,   895,   896,   897,   898,   769,
     770,   771,   772,   773,   899,   900,   776,   777,   778,   779,
     901,   781,   782,   783,   784,     0,     0,     0,   363,   785,
     786,   787,   788,   902,   903,   759,   760,   791,   792,   793,
     904,   905,   906,   797,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,     0,     0,     0,    13,     0,
       0,     0,     0,     0,   810,   811,     0,     0,     0,     0,
     759,   760,     0,     0,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,   907,   799,     0,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,     0,   517,   696,     0,     0,     0,     0,     0,     0,
       0,     0,   891,   892,   893,   894,   895,   896,   897,   898,
     769,   770,   771,   772,   773,   899,   900,   776,   777,   778,
     779,   901,   781,   782,   783,   784,  -378,     0,     0,     0,
     785,   786,   787,   788,   902,   903,     0,     0,   791,   792,
     793,   904,   905,   906,   797,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,   759,   760,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
       0,     0,     0,     0,     0,     0,   907,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   759,
     760,     0,     0,     0,     0,     0,     0,     0,   810,   811,
       0,     0,     0,   517,   696,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,   834,     0,     0,
       0,     0,     0,     0,     0,     0,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,   792,   793,     0,     0,     0,
     797,     0,     0,     0,   769,   770,   771,   772,   773,     0,
       0,   776,   777,   778,   779,     0,   781,   782,   783,   784,
     759,   760,     0,     0,   785,     0,   787,   788,     0,     0,
       0,     0,   791,   792,   793,     0,     0,     0,   797,     0,
       0,     0,     0,   799,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   759,   760,     0,     0,     0,
       0,     0,     0,     0,   810,   811,     0,     0,   850,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   799,     0,   800,   801,   802,   803,   804,   805,   806,
     807,   808,   809,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   810,   811,     0,     0,   864,     0,     0,     0,
       0,     0,     0,     0,     0,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,     0,     0,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,   759,   760,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,     0,   797,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   759,   760,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,  1174,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,   811,
       0,     0,  1178,     0,     0,     0,     0,     0,     0,     0,
       0,   769,   770,   771,   772,   773,     0,     0,   776,   777,
     778,   779,     0,   781,   782,   783,   784,     0,     0,     0,
       0,   785,     0,   787,   788,     0,     0,     0,     0,   791,
     792,   793,     0,     0,     0,   797,     0,     0,     0,   769,
     770,   771,   772,   773,     0,     0,   776,   777,   778,   779,
       0,   781,   782,   783,   784,   759,   760,     0,     0,   785,
       0,   787,   788,     0,     0,     0,     0,   791,   792,   793,
       0,     0,     0,   797,     0,     0,     0,     0,   799,     0,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     759,   760,     0,     0,     0,     0,     0,     0,     0,   810,
     811,     0,     0,  1180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   799,     0,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,  1189,     0,     0,     0,     0,     0,     0,     0,     0,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,     0,     0,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,     0,   797,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,   759,   760,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   759,
     760,     0,     0,     0,     0,     0,     0,     0,   810,   811,
       0,     0,  1190,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,  1191,     0,     0,
       0,     0,     0,     0,     0,     0,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,   792,   793,     0,     0,     0,
     797,     0,     0,     0,   769,   770,   771,   772,   773,     0,
       0,   776,   777,   778,   779,     0,   781,   782,   783,   784,
     759,   760,     0,     0,   785,     0,   787,   788,     0,     0,
       0,     0,   791,   792,   793,     0,     0,     0,   797,     0,
       0,     0,     0,   799,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   759,   760,     0,     0,     0,
       0,     0,     0,     0,   810,   811,     0,     0,  1192,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   799,     0,   800,   801,   802,   803,   804,   805,   806,
     807,   808,   809,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   810,   811,     0,     0,  1193,     0,     0,     0,
       0,     0,     0,     0,     0,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,     0,     0,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,   759,   760,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,     0,   797,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   759,   760,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,  1194,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,   811,
       0,     0,  1325,     0,     0,     0,     0,     0,     0,     0,
       0,   769,   770,   771,   772,   773,     0,     0,   776,   777,
     778,   779,     0,   781,   782,   783,   784,     0,     0,     0,
       0,   785,     0,   787,   788,     0,     0,     0,     0,   791,
     792,   793,     0,     0,     0,   797,     0,     0,     0,   769,
     770,   771,   772,   773,     0,     0,   776,   777,   778,   779,
       0,   781,   782,   783,   784,   759,   760,     0,     0,   785,
       0,   787,   788,     0,     0,     0,     0,   791,   792,   793,
       0,     0,     0,   797,     0,     0,     0,     0,   799,     0,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     759,   760,     0,     0,     0,     0,     0,     0,     0,   810,
     811,     0,     0,  1337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   799,     0,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,     0,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,     0,     0,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,     0,   797,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,   759,   760,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   759,
     760,     0,     0,     0,     0,     0,     0,     0,   810,   811,
       0,     0,  1387,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,  1498,     0,     0,
       0,     0,     0,     0,     0,     0,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,   792,   793,     0,     0,     0,
     797,     0,     0,     0,   769,   770,   771,   772,   773,     0,
       0,   776,   777,   778,   779,     0,   781,   782,   783,   784,
     759,   760,     0,     0,   785,     0,   787,   788,     0,     0,
       0,     0,   791,   792,   793,     0,     0,     0,   797,     0,
       0,     0,     0,   799,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   759,   760,     0,     0,     0,
       0,     0,     0,     0,   810,   811,     0,     0,  1499,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   799,     0,   800,   801,   802,   803,   804,   805,   806,
     807,   808,   809,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   810,   811,     0,     0,  1520,     0,     0,     0,
       0,     0,     0,     0,     0,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,     0,     0,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,   759,   760,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,     0,   797,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   759,   760,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,  1522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,   811,
       0,     0,  1524,     0,     0,     0,     0,     0,     0,     0,
       0,   769,   770,   771,   772,   773,     0,     0,   776,   777,
     778,   779,     0,   781,   782,   783,   784,     0,     0,     0,
       0,   785,     0,   787,   788,     0,     0,     0,     0,   791,
     792,   793,     0,     0,     0,   797,     0,     0,     0,   769,
     770,   771,   772,   773,     0,     0,   776,   777,   778,   779,
       0,   781,   782,   783,   784,   759,   760,     0,     0,   785,
       0,   787,   788,     0,     0,     0,     0,   791,   792,   793,
       0,     0,     0,   797,     0,     0,     0,     0,   799,     0,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     759,   760,     0,     0,     0,     0,     0,     0,     0,   810,
     811,     0,     0,  1528,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   799,     0,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,  1576,     0,     0,     0,     0,     0,     0,     0,     0,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,     0,     0,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,     0,   797,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,   759,   760,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   759,
     760,     0,     0,     0,     0,     0,     0,     0,   810,   811,
       0,     0,  1600,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,  1601,     0,     0,
       0,     0,     0,     0,     0,     0,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,   792,   793,     0,     0,     0,
     797,     0,     0,     0,   769,   770,   771,   772,   773,     0,
       0,   776,   777,   778,   779,     0,   781,   782,   783,   784,
     759,   760,     0,     0,   785,     0,   787,   788,     0,     0,
       0,     0,   791,   792,   793,     0,     0,     0,   797,     0,
       0,     0,     0,   799,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   759,   760,     0,     0,     0,
       0,     0,     0,     0,   810,   811,     0,     0,  1603,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   799,     0,   800,   801,   802,   803,   804,   805,   806,
     807,   808,   809,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   810,   811,     0,     0,  1612,     0,     0,     0,
       0,     0,     0,     0,     0,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,     0,     0,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,   759,   760,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,     0,   797,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   759,   760,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,  1615,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,   811,
       0,     0,  1624,     0,     0,     0,     0,     0,     0,     0,
       0,   769,   770,   771,   772,   773,     0,     0,   776,   777,
     778,   779,     0,   781,   782,   783,   784,     0,     0,     0,
       0,   785,     0,   787,   788,     0,     0,     0,     0,   791,
     792,   793,     0,     0,     0,   797,     0,     0,     0,   769,
     770,   771,   772,   773,     0,     0,   776,   777,   778,   779,
       0,   781,   782,   783,   784,   759,   760,     0,     0,   785,
       0,   787,   788,     0,     0,     0,     0,   791,   792,   793,
       0,     0,     0,   797,     0,     0,     0,     0,   799,     0,
     800,   801,   802,   803,   804,   805,   806,   807,   808,   809,
     759,   760,     0,     0,     0,     0,     0,     0,     0,   810,
     811,     0,     0,  1699,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   799,     0,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,  1700,     0,     0,     0,     0,     0,     0,     0,     0,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,     0,     0,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,     0,   797,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,   759,   760,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   759,
     760,     0,     0,     0,     0,     0,     0,     0,   810,   811,
     854,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   810,   811,  1126,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,   792,   793,     0,     0,     0,
     797,     0,     0,     0,   769,   770,   771,   772,   773,     0,
       0,   776,   777,   778,   779,     0,   781,   782,   783,   784,
     759,   760,     0,     0,   785,     0,   787,   788,     0,     0,
       0,     0,   791,   792,   793,     0,     0,     0,   797,     0,
       0,     0,     0,   799,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   759,   760,     0,     0,     0,
       0,     0,     0,     0,   810,   811,  1276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   799,     0,   800,   801,   802,   803,   804,   805,   806,
     807,   808,   809,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   810,   811,  1292,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,     0,     0,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   791,   792,   793,     0,     0,     0,   797,
     769,   770,   771,   772,   773,     0,     0,   776,   777,   778,
     779,     0,   781,   782,   783,   784,   242,   243,     0,     0,
     785,     0,   787,   788,     0,     0,     0,     0,   791,   792,
     793,     0,     0,   244,   797,     0,     0,     0,     0,     0,
       0,     0,   799,     0,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,     0,     0,     0,     0,   759,   760,
       0,     0,     0,   810,   811,  1427,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   799,     0,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,   811,
    1433,     0,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,     0,   263,   264,   265,     0,     0,     0,     0,
       0,     0,   266,   267,   268,   269,   270,     0,     0,   271,
     272,   273,   274,   275,   276,   277,     0,     0,     0,     0,
       0,   759,   760,   769,   770,   771,   772,   773,     0,     0,
     776,   777,   778,   779,     0,   781,   782,   783,   784,     0,
       0,     0,     0,   785,     0,   787,   788,     0,     0,     0,
       0,   791,   792,   793,     0,     0,     0,   797,   278,     0,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
      50,     0,   289,   290,     0,     0,     0,     0,     0,   291,
     292,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     799,     0,   800,   801,   802,   803,   804,   805,   806,   807,
     808,   809,     0,     0,   759,   760,   769,   770,   771,   772,
     773,   810,   811,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,   792,   793,     0,     0,     0,
     797,   759,   760,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,   799,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,     0,     0,     0,     0,   769,
     770,   771,   772,   773,   810,   811,   776,   777,   778,   779,
       0,   781,   782,   783,   784,     0,     0,     0,     0,   785,
       0,   787,   788,     0,     0,   955,     0,   791,   792,   793,
       0,     0,     0,   797,   759,   760,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,  1200,     0,   791,   792,   793,     0,     0,     0,
     797,   759,   760,     0,     0,     0,   799,     0,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   799,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,     0,     0,     0,     0,   769,
     770,   771,   772,   773,   810,   811,   776,   777,   778,   779,
       0,   781,   782,   783,   784,     0,     0,     0,     0,   785,
       0,   787,   788,     0,     0,     0,     0,   791,   792,   793,
       0,     0,     0,   797,   759,   760,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,   792,   793,     0,     0,     0,
     797,   759,   760,     0,     0,     0,   799,  1281,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   799,  1382,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,     0,     0,     0,     0,   769,
     770,   771,   772,   773,   810,   811,   776,   777,   778,   779,
       0,   781,   782,   783,   784,     0,     0,     0,     0,   785,
       0,   787,   788,     0,     0,     0,     0,   791,   792,   793,
       0,     0,     0,   797,   759,   760,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
       0,     0,     0,     0,   791,   792,   793,     0,  1625,     0,
     797,   759,   760,     0,     0,     0,   799,     0,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   799,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,     0,     0,     0,     0,   769,
     770,   771,   772,   773,   810,   811,   776,   777,   778,   779,
       0,   781,   782,   783,   784,     0,     0,     0,     0,   785,
       0,   787,   788,     0,     0,     0,     0,   791,   792,   793,
       0,     0,     0,  -785,   759,   760,   769,   770,   771,   772,
     773,     0,     0,   776,   777,   778,   779,     0,   781,   782,
     783,   784,     0,     0,     0,     0,   785,     0,   787,   788,
     759,   760,     0,     0,   791,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   799,     0,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   759,   760,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,     0,     0,     0,     0,   769,
     770,   771,   772,   773,   810,   811,   776,   777,   778,   779,
       0,   781,   782,   783,   784,     0,     0,     0,     0,   785,
       0,   787,   788,     0,     0,   769,   770,   771,   772,   773,
       0,     0,   776,   777,   778,   779,     0,   781,   782,   783,
     784,     0,     0,     0,     0,   785,     0,   787,   788,     0,
       0,     0,     0,   769,   770,   771,   772,   773,     0,     0,
     776,   777,   778,   779,     0,   781,   782,   783,   784,     0,
       0,     0,     0,   785,     0,   787,   788,     0,     0,   801,
     802,   803,   804,   805,   806,   807,   808,   809,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   810,   811,     0,
       0,  1008,     0,     0,     0,     0,   802,   803,   804,   805,
     806,   807,   808,   809,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   810,   811,     0,     0,     0,     0,     0,
       0,     0,  1277,     0,     0,   803,   804,   805,   806,   807,
     808,   809,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   810,   811,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   479,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   329,   330,   331,     0,   333,   334,
     335,   336,   337,   479,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,     0,   351,   352,   353,     0,
       0,   356,   357,   358,   359,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   479,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
    1009,   192,   356,   357,   358,   359,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1010,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1278,     0,  1083,  1084,     0,     0,   193,     0,   194,
       0,   195,   196,   197,   198,   199,  1279,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,     0,   211,
     212,   213,  1085,     0,   214,   215,   216,   217,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1086,     0,     0,
       0,     0,     0,     0,   218,   219,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1087,  1088,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   220
};

static const yytype_int16 yycheck[] =
{
       1,   668,   459,   222,   498,   610,   389,   587,   536,   732,
     238,   641,   389,   736,   737,     7,   434,   553,    19,    15,
      16,   433,   748,   171,   711,    86,   483,   433,   434,   932,
     946,   601,   648,   603,   402,   605,   952,  1203,   228,   925,
      20,   577,   841,   238,   843,   396,   845,   927,     5,     4,
      20,    53,    33,    53,    22,    20,     8,   487,   488,    15,
      16,   109,   127,   431,  1406,   144,    20,   146,   191,   709,
     155,   711,    68,    69,    70,   160,   459,    19,    20,   143,
     160,    20,    20,   166,    33,    33,  1604,    20,    62,    57,
     127,     0,   708,   216,   710,    96,   712,    98,     7,   143,
     483,    46,   126,   158,   720,   129,   130,   158,   389,   173,
    1577,    60,    61,   729,   110,   111,   112,   113,   173,    15,
      16,    30,   173,    32,   173,    34,   173,   192,   496,   497,
     195,    40,    57,   218,   591,   220,  1654,   129,    63,     7,
     220,    50,   185,   129,   130,   102,   103,    56,   149,   960,
     194,   962,   878,   217,   191,   192,   199,   194,   158,   387,
     197,   106,   698,   218,   887,   313,   217,  1509,   217,   230,
    1637,    80,   219,   173,    53,   124,   168,   192,   459,   128,
     155,   229,    50,   185,   143,   160,   163,   191,    21,    22,
     214,   215,   387,   102,   103,   187,   188,   189,  1364,   389,
     923,   194,   483,   926,   661,   160,   183,   402,   591,   191,
     166,   191,   164,   165,   173,   171,   197,   173,    97,    57,
     176,   191,   628,    21,    22,    63,   191,   633,   214,   215,
     231,   173,   184,   185,   691,   203,   431,   191,   433,   164,
     217,   198,   238,   462,   193,   220,   214,   152,   197,   197,
     199,   200,   191,   191,    33,   316,   165,   476,   191,   184,
     217,   217,  1148,   215,   191,   991,   215,   215,   173,  1149,
     166,   152,   221,   221,  1471,   171,   185,   173,   661,   160,
     176,    60,    61,  1199,  1187,   118,   119,   144,   145,   146,
     459,   218,   173,   126,    57,   204,   129,   130,   131,   667,
      63,   496,   497,   136,   143,   165,   215,   173,   691,  1564,
     591,  1566,   143,   152,  1480,   307,  1571,  1572,   310,   191,
     118,   119,   690,     5,     6,   185,     8,   757,   126,   191,
     128,   129,   130,   131,   173,   191,   217,   165,   136,   199,
     217,  1538,  1539,   127,  1247,   124,   218,   516,   127,   128,
     191,   163,   992,   137,    36,   971,   218,   185,   137,  1556,
    1557,   218,   218,   166,   592,   593,   594,   363,   596,   597,
     827,   183,   600,   173,   602,   198,   604,   218,  1633,   191,
     661,   214,   215,   384,   385,   575,   194,   577,   389,   197,
     391,   387,   220,   127,   217,    34,   392,   592,   593,   594,
    1303,   596,   597,   137,   755,   600,   402,   602,   192,   604,
     691,   606,  1138,   192,   193,   191,   214,   215,   197,  1616,
    1617,   200,   195,   127,    63,   107,   199,   833,   647,   142,
     177,   432,  1002,   137,   624,   431,   215,   433,   173,   143,
     216,   631,   221,   849,   827,   635,   185,  1363,   191,   188,
     163,   188,   189,   672,   990,   823,   214,  1373,   192,   455,
     456,   194,   191,   957,   197,   127,   127,  1266,   107,   152,
     183,   472,   667,   216,   968,   137,   137,   216,   173,   216,
     761,   762,   763,   764,   765,   766,   767,   768,   192,   218,
     173,   660,   661,   774,   775,   690,   132,   136,   717,   780,
     496,   497,  1132,   909,   143,   873,   874,   173,   789,   790,
     940,   173,  1199,   794,   795,   796,   685,   798,   886,   688,
     521,   522,   127,   742,   191,   165,   127,  1214,  1611,   165,
     192,   192,   137,   191,   173,   197,   137,   163,   539,  1622,
     541,   542,   543,    33,   545,   185,   827,   915,   916,   185,
     918,   218,   192,   163,   922,   556,   924,   183,   191,  1199,
     218,  1360,  1202,   195,   759,   760,   173,   199,   163,   208,
      60,    61,   573,   183,  1214,   191,   577,   173,   217,   748,
    1663,  1664,  1012,  1013,  1014,   218,   587,   192,   183,   191,
     785,   192,   191,   219,   220,  1394,   592,   593,   594,   194,
     596,   597,   218,   173,   600,  1235,   602,   191,   604,   191,
     606,   165,    57,   143,   158,   810,   218,   106,  1188,   218,
     165,   622,   152,   851,    33,     5,     6,   157,   823,   173,
     858,   185,   216,   861,   124,   191,   218,   191,   128,   165,
     185,   165,   870,   173,   813,   199,   173,   174,   175,   877,
     165,    60,    61,   144,   882,   177,   851,   653,   191,   185,
     216,   185,   217,   858,   192,   191,   861,   191,   198,  1239,
     185,   667,   165,   199,   822,   870,   191,   204,   873,   874,
    1347,   164,   877,   216,   199,   165,   195,   882,   191,   908,
     199,   886,   185,   173,   690,    68,    69,    70,   191,   191,
      35,   184,   195,   193,  1122,   185,  1505,   197,  1120,   878,
     200,   191,    35,   216,   173,   124,   165,  1123,   127,   128,
     915,   916,   198,   918,   216,   215,   198,   922,   137,   924,
     925,   221,   960,   165,   962,   165,   185,   110,   111,   112,
     113,   217,   191,   739,   165,   217,     5,     6,   164,   165,
     199,   165,   744,   185,   195,   185,   165,  1387,   199,   191,
     158,   191,   173,  1403,   185,   960,    25,   962,   184,   199,
     191,   185,    31,   165,   943,   173,   185,   191,   199,  1147,
    1186,    47,   951,   192,   193,   199,   198,   173,   197,  1429,
     173,   200,   216,   185,   165,   165,   486,   487,   488,   191,
     165,    67,   173,   165,  1172,   217,   215,   199,    22,    68,
      69,   173,   221,   814,   185,   185,   198,  1223,  1224,   173,
     185,   191,   818,   185,   158,   158,   191,   823,   829,   199,
    1400,  1359,   158,    75,   199,   217,   194,    79,   839,   173,
     173,   216,  1447,   102,   103,   194,   536,   173,   194,   194,
     199,    93,    94,   199,   199,   851,    98,    99,   100,   101,
      57,   217,   858,  1246,   194,   861,    63,    33,   217,  1246,
    1238,   217,   217,   563,   870,   195,  1071,   873,   874,   199,
    1108,   877,  1549,    57,   143,    12,   882,   195,    43,    63,
     886,   199,    21,    22,    60,    61,    23,    24,  1621,   198,
     194,   160,   161,   162,  1094,   199,  1629,   219,   220,   191,
      57,    57,   194,  1108,   173,   197,    63,    63,   198,   915,
     916,    57,   918,   217,   183,  1120,   922,    63,   924,   925,
    1336,  1511,   195,   198,  1657,   185,   199,   184,  1344,   198,
     187,   191,  1547,   190,   185,   185,  1576,   217,   185,   185,
     191,   191,  1147,  1148,   191,   191,   215,   195,   124,   177,
     178,   199,   128,  1568,   960,  1246,   962,  1136,   185,   195,
    1610,  1611,   195,   199,   191,   665,   199,  1172,  1618,   198,
     195,   173,  1622,  1623,   199,    92,    93,   116,   117,   118,
     119,  1485,   177,   178,   179,   180,  1490,   126,   198,   128,
     129,   130,   131,   693,   217,   184,   185,   136,   187,   138,
     139,  1591,  1592,   217,   217,  1655,   219,   220,    21,    22,
     198,  1240,   198,  1663,  1664,   198,  1195,   193,   144,   145,
     146,   197,   195,   199,   200,   173,   174,   175,   177,   178,
     179,   198,   198,  1238,  1213,   177,   178,   179,  1416,   215,
    1219,   184,   185,   186,   198,   221,  1246,  1226,   137,  1228,
     441,   442,   443,  1643,   217,    21,    22,   757,    10,    11,
      12,   200,   201,   202,   203,   204,   218,   173,   173,  1719,
     173,   216,   173,   198,   215,   214,   215,   198,   198,   217,
     198,   217,   198,   198,  1095,   166,  1324,   198,  1099,   198,
     198,   220,  1103,   217,   217,  1274,   198,    10,   217,    37,
     198,  1330,  1108,   217,   217,   118,   119,  1286,    33,    66,
     217,   215,  1291,   126,  1120,   128,   129,   130,   131,  1324,
     217,   217,   198,   136,    43,   218,  1593,   173,   173,   829,
     216,   194,  1510,   217,    43,    60,    61,   217,   198,   198,
     198,  1147,  1148,   191,   132,    14,   166,   194,   192,   220,
     184,   191,   118,   119,    13,   216,   191,   173,   191,     8,
     126,   217,   128,   129,   130,   131,  1172,  1396,   218,   173,
     136,   199,   173,   173,   173,   218,   218,  1644,  1645,   191,
    1418,   191,  1361,   173,   198,   217,  1415,   200,   201,   202,
     203,   204,   762,   763,   764,   765,   766,   767,   768,   124,
    1593,   214,   215,   128,   774,   775,   184,  1386,     1,   218,
     780,  1416,  1590,  1418,   217,   198,   218,    67,  1685,   789,
     790,   217,   217,   217,   794,   795,   796,   173,   798,   199,
     930,   217,  1238,   217,   199,  1246,   202,   203,   204,   199,
     940,   217,   217,   217,    43,   218,   218,  1476,   214,   215,
     173,  1644,  1645,   216,   218,    33,   217,   173,   173,   218,
     218,   217,   216,   216,   198,   173,   217,   217,   193,   218,
     173,   173,   197,   217,   199,   200,   217,   217,   217,   173,
     216,   218,    60,    61,   217,  1296,   181,  1525,   218,   173,
     215,   173,  1685,    33,   216,   216,   221,   217,   173,   217,
     217,   199,  1593,   217,    70,  1510,   218,   217,   199,   199,
     217,   217,  1012,  1013,  1014,   185,   217,  1017,  1324,  1019,
    1525,  1021,   218,  1023,   217,  1025,   218,  1027,   217,  1029,
     217,  1031,   389,   177,   217,    53,  1036,   217,  1038,   218,
     218,   218,   399,   184,  1044,   218,   124,   184,   216,   191,
     128,   408,   184,  1644,  1645,   216,  1056,   191,  1058,  1588,
     218,   418,   218,  1063,   218,  1065,   218,  1067,   216,   216,
    1070,   428,   218,   218,  1553,   184,    85,    33,   218,   217,
     437,   216,   218,     1,   719,  1590,    46,  1398,   218,    12,
     141,    88,   449,  1502,  1685,  1503,   989,   236,    21,    22,
     818,  1580,   459,  1103,    60,    61,  1503,   464,     1,   466,
    1416,  1503,  1418,   997,  1476,   193,  1503,  1503,   475,   197,
    1413,   199,   200,  1258,  1439,  1535,   483,   484,   485,  1442,
    1299,  1536,  1536,   455,    56,  1623,   455,   215,   572,  1483,
    1209,   498,    -1,   221,   711,    -1,    -1,    -1,    -1,   506,
      -1,    -1,   509,   510,   511,   512,   513,   514,    -1,  1638,
      -1,    -1,    -1,    -1,  1693,    -1,    -1,   524,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,  1510,   128,   129,   130,   131,   165,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,  1525,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,   185,
      -1,    -1,    -1,    -1,   591,   191,    -1,   193,    -1,    -1,
      -1,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,
      -1,    -1,   619,    -1,    -1,   221,    -1,    -1,    -1,    -1,
     193,  1577,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,  1590,    -1,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,   661,    -1,   136,    -1,   138,   139,
      -1,   668,   669,    -1,   671,    -1,    -1,    -1,    -1,   676,
      -1,    -1,    -1,    -1,    -1,   682,    -1,    -1,    -1,    -1,
      -1,  1637,    -1,    -1,   691,    -1,  1647,    -1,    -1,    -1,
      -1,    -1,  1653,    -1,    -1,   702,   703,   704,   705,   706,
     707,    -1,   709,    -1,   711,    -1,    -1,    -1,    -1,  1359,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,  1687,    -1,  1689,    -1,
      21,    22,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1714,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   770,    -1,    -1,   773,   774,   775,   776,
     777,   778,    -1,   780,   781,    -1,   783,   784,   785,   786,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,   806,
     807,   808,   809,    -1,   811,    -1,    -1,    -1,   815,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     827,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,   865,   150,
      -1,    -1,    -1,    -1,    -1,   872,    -1,    -1,    -1,    -1,
      -1,    -1,   879,    -1,   881,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,   890,   891,   892,   893,   894,   895,   896,
     897,   898,   899,   900,   901,   902,   903,   904,   905,   906,
     907,    -1,   193,    33,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    21,    22,    -1,   219,   220,
      60,    61,    -1,    -1,   941,   942,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   955,    -1,
     957,    -1,    -1,    -1,    -1,    -1,    -1,   964,    -1,    -1,
      -1,   968,    -1,    -1,    -1,    -1,   973,    -1,   975,   976,
     977,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   988,    -1,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,   124,   138,   139,  1647,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,    -1,  1687,    -1,  1689,
     136,   137,   138,   139,   140,   141,    -1,    -1,   144,   145,
     146,   147,   148,   149,   150,   198,   199,   200,   201,   202,
     203,   204,    -1,   193,  1714,    -1,    -1,   197,    -1,   199,
     200,   214,   215,    -1,    -1,    -1,    -1,  1084,    -1,    -1,
      19,  1088,    33,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,   221,    -1,    33,    -1,    -1,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      60,    61,    -1,   219,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,  1175,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,  1200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1208,  1209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    21,    22,  1236,
    1237,    -1,    19,    -1,   173,    -1,  1243,    -1,    25,  1246,
      -1,    -1,   193,    -1,    31,  1252,   197,    -1,   199,   200,
      -1,    -1,    -1,   193,    41,    -1,    -1,   197,    -1,   199,
     200,  1268,    49,  1270,   215,    -1,    -1,    -1,  1275,    -1,
     221,    -1,    -1,    -1,  1281,   215,    -1,    64,  1285,    -1,
      -1,   221,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
    1327,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    33,   128,   129,   130,   131,    -1,    -1,
    1347,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1388,  1389,  1390,    -1,    33,   173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    60,    61,    -1,    21,    22,  1424,    -1,  1426,
     214,   215,    -1,    -1,   124,  1432,    -1,    -1,   128,    -1,
      -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1456,
      -1,    -1,  1459,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1467,  1468,  1469,    -1,    -1,    -1,    -1,  1474,    -1,    -1,
      -1,    -1,  1479,    -1,    -1,  1482,   124,    -1,  1485,  1486,
     128,    -1,    -1,  1490,  1491,    -1,  1493,  1494,    -1,    -1,
      -1,    -1,    -1,   193,    -1,    -1,  1503,   197,    -1,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1516,
     116,   117,   118,   119,   120,   215,    -1,   123,   124,   125,
     126,   221,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,  1549,    -1,   150,   193,    -1,    -1,    -1,   197,
      -1,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1573,   215,    -1,    -1,
      -1,    -1,    -1,   221,  1581,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1593,   193,  1595,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1613,  1614,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1634,  1635,    -1,
      -1,    -1,  1639,    -1,    -1,    -1,     1,  1644,  1645,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,  1672,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    42,  1685,    44,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,   164,
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
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
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
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
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
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
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
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
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
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
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
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
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
     154,   155,    -1,   157,   158,   159,   160,   161,   162,    -1,
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
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
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
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
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
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
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
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
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
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
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
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
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
     154,   155,    -1,   157,   158,   159,   160,   161,   162,    -1,
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
     158,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
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
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
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
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
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
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
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
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,
     199,   200,   198,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,    -1,   215,     5,     6,   215,
      -1,   217,   221,   219,   220,    13,    -1,    15,    16,    17,
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
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
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
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,
       6,    -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,
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
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
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
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
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
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,   200,   201,   202,    -1,
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
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
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
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
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
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
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
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
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
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
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
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,   157,
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
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
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
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
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
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
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
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
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    13,    -1,    -1,    -1,    -1,   183,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    41,   207,   208,    -1,    -1,    -1,    -1,    -1,
      49,   215,    -1,   217,    -1,   219,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,   173,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     219,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   173,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   219,    -1,   221,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    19,    -1,    -1,    -1,    -1,
     173,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    -1,    -1,    41,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,   219,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    64,    -1,    -1,    -1,    -1,    -1,   173,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,   173,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,    -1,   146,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,   219,   136,
     137,   138,   139,   140,   141,    21,    22,   144,   145,   146,
     147,   148,   149,   150,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,   219,   220,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,    -1,
     136,   137,   138,   139,   140,   141,    -1,    -1,   144,   145,
     146,   147,   148,   149,   150,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,   192,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,   219,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    21,    22,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    38,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    21,    22,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     163,    -1,   207,   208,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    21,    22,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,   142,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,   142,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
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
      -1,    -1,    -1,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,   185,    -1,
     150,    21,    22,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,   150,    21,    22,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      21,    22,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,   116,
     117,   118,   119,   120,   214,   215,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
      -1,   138,   139,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    71,    72,    73,    -1,    75,    76,    77,
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
      94,    95,   158,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   227,   236,
     238,   239,   244,   251,   275,   279,   315,   396,   403,   407,
     418,   463,   468,   473,    19,    20,   173,   267,   268,   269,
     166,   245,   246,   173,   174,   175,   204,   240,   241,   242,
     163,   183,   284,   404,   173,   219,   226,    57,    63,   399,
     399,   399,   143,   173,   301,    34,    63,   107,   136,   208,
     217,   271,   272,   273,   274,   301,   251,     5,     6,     8,
      36,   107,   415,    62,   394,   192,   191,   194,   191,   241,
      22,    57,   203,   214,   243,   399,   400,   402,   400,   394,
     474,   464,   469,   173,   143,   237,   273,   273,   273,   217,
     144,   145,   146,   191,   216,    57,    63,   280,   282,    57,
      63,   405,     5,     6,    57,    63,   416,    57,    63,   395,
      15,    16,   166,   171,   173,   176,   217,   229,   268,   166,
     246,   173,   240,   240,   173,   251,   164,   184,   285,   400,
     251,    57,    63,   225,   173,   173,   173,   173,   177,   235,
     218,   269,   273,   273,   273,   273,   283,   173,   406,   419,
     284,   397,   177,   178,   179,   228,    15,    16,   166,   171,
     173,   229,   265,   266,   243,   401,   251,   475,   465,   470,
     177,   218,    35,    71,    73,    75,    76,    77,    78,    79,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    93,    94,    95,    98,    99,   100,   101,   118,   119,
     173,   278,   281,   194,   284,   106,   413,   414,   392,   160,
     220,   270,   365,   177,   178,   179,   191,   218,   192,   284,
     284,   284,    21,    22,    38,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   128,   129,   130,   137,   138,   139,   140,
     141,   144,   145,   146,   147,   148,   149,   150,   193,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   207,
     208,   214,   215,    35,    35,   217,   276,   284,   286,    75,
      79,    93,    94,    98,    99,   100,   101,   423,   408,   173,
     420,   165,   285,   393,   269,   268,   220,   251,   152,   173,
     390,   391,   265,    19,    25,    31,    41,    49,    64,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   156,   219,   301,   422,   424,   425,   428,   434,
     462,   476,   466,   471,   173,   173,   173,   216,    22,   173,
     216,   155,   218,   365,   375,   376,   377,   194,   277,   289,
     284,   398,   194,   412,   284,   417,   365,   216,   268,   217,
      43,   191,   194,   197,   389,   198,   198,   198,   217,   198,
     198,   217,   198,   198,   198,   198,   198,   198,   217,   301,
      33,    60,    61,   124,   128,   193,   197,   200,   215,   221,
     433,   195,   427,   380,   383,   173,   137,   217,     7,    50,
     314,   251,   191,   251,   218,   462,     1,     5,     6,     9,
      10,    11,    13,    15,    16,    17,    18,    19,    25,    26,
      27,    28,    29,    31,    38,    39,    42,    44,    45,    48,
      51,    52,    54,    55,    58,    59,    65,    68,    69,    80,
     102,   103,   104,   105,   118,   119,   133,   134,   135,   151,
     152,   153,   154,   155,   157,   159,   160,   161,   162,   166,
     167,   168,   169,   170,   171,   172,   174,   175,   176,   183,
     200,   201,   202,   207,   208,   215,   217,   219,   220,   234,
     236,   247,   248,   251,   252,   255,   256,   258,   260,   261,
     262,   263,   285,   287,   288,   290,   295,   300,   301,   302,
     306,   307,   308,   309,   310,   311,   312,   313,   315,   319,
     320,   327,   330,   333,   338,   341,   342,   344,   345,   346,
     348,   353,   356,   357,   364,   422,   478,   493,   504,   508,
     521,   524,   410,   173,   251,   409,   301,   371,   391,   216,
      65,   104,   174,   295,   357,   173,   173,   434,   127,   137,
     192,   388,   435,   440,   442,   357,   444,   438,   173,   429,
     446,   448,   450,   452,   454,   456,   458,   460,   357,   198,
     217,    33,   197,    33,   197,   215,   221,   216,   357,   215,
     221,   434,   173,   251,   477,   173,   191,   251,   378,   431,
     462,   467,   173,   381,   431,   472,   357,   152,   173,   385,
     386,   421,   377,   377,   377,   198,   198,   357,   259,   198,
     303,   424,   478,   217,   301,   198,     5,   102,   103,   198,
     217,   127,   300,   331,   342,   357,   286,   198,   217,    61,
     357,   217,   357,   173,   198,   198,   217,   251,   198,   166,
      58,   357,   217,   286,   198,   217,   198,   198,   217,   198,
     198,   127,   300,   357,   357,   357,   220,   286,   333,   337,
     337,   337,   217,   217,   217,   217,   217,   217,    13,   434,
      13,   434,    13,   357,   503,   519,   198,   357,   198,   233,
      13,   293,   503,   520,   357,   357,   357,   357,   357,    13,
      49,   291,   331,   357,   158,   173,   331,   479,   481,   220,
     251,   251,   357,    10,    37,   333,   339,   173,   217,   251,
     251,   251,   251,   251,    66,   316,   275,   132,   251,    21,
      22,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   136,   137,   138,   139,   140,
     141,   144,   145,   146,   147,   148,   149,   150,   192,   193,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     214,   215,   339,   217,   398,   192,   251,   285,   215,   251,
     275,   285,   372,   198,   218,    43,   251,   388,   300,   357,
     462,   462,   432,   462,   218,   462,   462,   218,   173,   426,
     462,   276,   462,   276,   462,   276,   378,   379,   381,   382,
     218,   437,   291,   216,   216,   357,   251,   285,   194,   431,
     285,   194,   431,   285,   218,   217,    43,   127,   191,   192,
     194,   197,   384,   494,   496,   286,   421,   304,   217,   301,
     198,   217,   328,   198,   198,   198,   515,   331,   300,   331,
     191,   108,   109,   110,   111,   112,   113,   114,   115,   121,
     122,   127,   140,   141,   147,   148,   149,   192,    14,   434,
     293,   357,   357,   286,   192,   321,   323,   357,   325,   194,
     166,   357,   517,   331,   500,   505,   331,   498,   434,   300,
     357,   220,   275,   357,   357,   357,   357,   357,   357,   421,
      53,   200,   215,   217,   357,   479,   482,   486,   502,   507,
     421,   217,   482,   507,   421,   142,   184,   185,   186,   487,
     296,   286,   298,   179,   180,   228,   421,   184,   191,   523,
     421,    13,   216,   191,   523,   217,   137,   384,   523,   191,
     523,   218,   152,   157,   198,   301,   347,   286,   257,   284,
     340,    70,   215,   218,   331,   481,   160,   217,   318,   391,
       4,   160,   336,   337,    19,   158,   173,   422,    19,   158,
     173,   422,   133,   134,   135,   287,   343,   357,   343,   357,
     343,   357,   343,   357,   343,   357,   343,   357,   343,   357,
     343,   357,   357,   357,   357,   343,   357,   343,   357,   357,
     357,   357,   173,   343,   357,   357,   158,   173,   357,   357,
     357,   422,   357,   357,   357,   343,   357,   343,   357,   357,
     357,   357,   343,   357,   343,   357,   343,   357,   357,   343,
     357,    22,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   129,   130,   158,   173,   214,   215,   354,
     422,   357,   218,   331,   411,   357,   274,     8,   365,   370,
     434,   173,   300,   357,   251,   199,   199,   199,   431,   199,
     199,   199,   251,   199,   277,   199,   277,   199,   277,   199,
     431,   199,   431,   294,   462,   218,   216,   462,   462,   357,
     173,   173,   462,   357,   434,   434,    20,   462,    70,   331,
     481,   492,   198,   357,   173,   357,   462,   509,   511,   513,
     434,   523,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     286,   199,   431,   218,   218,   264,   434,   434,   218,   434,
     218,   434,   523,   434,   379,   523,   382,   199,   336,   218,
     218,   218,   218,   218,   218,    20,   337,   215,   357,   218,
     142,   191,   185,   486,   188,   189,   216,   490,   191,   185,
     188,   216,   489,    20,   218,   486,   184,   187,   488,    20,
     357,   184,   503,   294,   294,   357,    20,   503,    20,   421,
     357,   357,   357,   218,   158,   173,   217,   217,   349,   351,
      12,    23,    24,   249,   250,   357,   289,   275,   173,   218,
     481,   479,   191,   218,   218,   173,   317,   317,   217,   127,
     137,   173,   192,   197,   334,   335,   276,   198,   217,   198,
     217,   337,   337,   337,   217,   217,   216,    19,   158,   173,
     422,   194,   158,   173,   357,   217,   217,   158,   173,   357,
       1,   217,   216,   218,   285,   251,   216,    57,    63,   368,
      67,   369,   251,   199,   251,   436,   441,   443,   462,   445,
     439,   430,   173,   447,   199,   451,   199,   455,   199,   459,
     378,   461,   381,   199,   431,   218,    43,   384,   199,   199,
     331,   199,   481,   218,   218,   218,   173,   218,   185,   199,
     218,   199,   434,   379,   382,   199,   218,   217,   434,   357,
     199,   199,   199,   199,   218,   199,   199,   218,   199,   336,
     276,   217,   331,   482,   486,   357,   479,   490,   216,   357,
     502,   216,   331,   482,   184,   187,   190,   491,   216,   331,
     199,   199,   194,   231,   331,   331,    20,   218,   217,   137,
     384,   357,   357,   434,   276,   286,   357,    12,   253,   285,
     336,   218,   216,   215,   191,   216,   218,   335,   173,   173,
     217,   173,   173,   191,   216,   277,   358,   357,   360,   357,
     218,   331,   357,   198,   217,   357,   217,   216,   357,   215,
     218,   331,   217,   216,   355,   218,   331,   251,    47,   369,
      46,   106,   366,   336,   449,   453,   457,   217,   462,   173,
     357,   495,   497,   286,   305,   218,   199,   431,   173,   217,
     329,   199,   199,   199,   516,   293,   199,   322,   324,   326,
     518,   501,   506,   499,   217,   339,   277,   218,   331,   185,
     486,   490,   185,   486,   216,   185,   297,   299,   232,   181,
     185,   185,   331,   137,   384,   357,   357,   357,   218,   218,
     199,   277,   286,   254,   251,   276,   218,   479,   173,   216,
     194,   389,   218,   173,   334,   216,   142,   286,   332,   434,
     218,   462,   218,   218,   218,   362,   357,   357,   218,   479,
     218,   357,   218,    33,   367,   366,   368,   291,   217,   217,
     357,   173,   199,   357,   510,   512,   514,   217,   218,   217,
     357,   357,   357,   217,    70,   492,   217,   217,   218,   357,
     332,   218,   357,   490,   357,   491,   503,   357,   217,   292,
     230,   503,   357,   185,   357,   357,   218,   350,   199,   250,
      26,   105,   255,   307,   308,   309,   311,   357,   277,   216,
     194,   389,   434,   388,   218,   127,   357,   199,   199,   462,
     218,   218,   216,   218,   373,   367,   385,   386,   387,   218,
     492,   492,   218,   199,   217,   218,   217,   217,   217,   291,
     293,   331,   492,   492,   218,   185,   522,   522,   522,   291,
     177,   522,   522,   357,   137,   384,   347,   352,   127,   127,
     357,   286,   218,   434,   388,   388,   300,   357,   357,   359,
     361,   199,   218,   281,   374,   217,   479,   483,   484,   485,
     485,   357,   357,   492,   492,   479,   480,   218,   218,   523,
     485,   480,    53,   216,   184,   184,   191,   523,   184,   216,
     522,   357,   357,   347,   357,   388,   300,   357,   300,   357,
     251,   363,   251,   281,   479,   191,   523,   218,   218,   218,
     218,   485,   485,   218,   218,   218,   218,   357,   216,   216,
     184,   218,   216,   300,   357,   251,   251,   286,   218,   217,
     218,   218,   251,   479,   218
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   222,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   224,   225,   225,
     225,   226,   226,   227,   228,   228,   228,   228,   229,   229,
     230,   230,   231,   232,   231,   233,   233,   233,   234,   235,
     235,   237,   236,   238,   239,   240,   240,   240,   241,   241,
     241,   241,   242,   242,   243,   243,   244,   245,   245,   246,
     246,   247,   248,   248,   249,   249,   250,   250,   250,   251,
     251,   252,   252,   253,   254,   253,   255,   255,   255,   255,
     255,   256,   257,   256,   259,   258,   260,   261,   262,   264,
     263,   265,   265,   265,   265,   265,   265,   266,   266,   267,
     267,   267,   268,   268,   268,   268,   268,   268,   268,   268,
     269,   269,   270,   270,   270,   271,   271,   271,   271,   272,
     272,   273,   273,   273,   273,   273,   273,   273,   274,   274,
     275,   275,   276,   276,   276,   277,   277,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   279,   280,   280,   280,   281,   283,   282,   284,
     284,   285,   285,   286,   286,   287,   287,   287,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   289,
     289,   289,   290,   291,   291,   292,   292,   293,   293,   294,
     294,   296,   297,   295,   298,   299,   295,   300,   300,   300,
     300,   300,   301,   301,   301,   302,   302,   304,   305,   303,
     303,   306,   306,   306,   306,   306,   306,   307,   308,   309,
     309,   309,   310,   310,   310,   311,   311,   312,   312,   312,
     313,   314,   314,   314,   315,   315,   316,   316,   317,   317,
     318,   318,   318,   318,   318,   318,   318,   318,   319,   319,
     321,   322,   320,   323,   324,   320,   325,   326,   320,   328,
     329,   327,   330,   330,   330,   330,   330,   330,   331,   331,
     332,   332,   332,   333,   333,   333,   334,   334,   334,   334,
     334,   335,   335,   336,   336,   336,   337,   337,   338,   340,
     339,   341,   341,   341,   341,   341,   341,   341,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   343,   343,   343,
     343,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   345,   345,
     346,   346,   347,   347,   348,   349,   350,   348,   351,   352,
     348,   353,   353,   353,   353,   353,   353,   353,   354,   355,
     353,   356,   356,   356,   356,   356,   356,   356,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   358,   359,   357,   357,   357,
     357,   360,   361,   357,   357,   357,   362,   363,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   364,   364,   364,   364,   364,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   365,
     365,   365,   366,   366,   366,   367,   367,   368,   368,   368,
     369,   369,   370,   371,   371,   372,   371,   373,   371,   374,
     371,   371,   375,   376,   376,   377,   377,   377,   377,   377,
     378,   378,   379,   379,   380,   380,   380,   381,   382,   382,
     383,   383,   383,   384,   384,   385,   385,   385,   386,   386,
     387,   387,   388,   388,   388,   389,   389,   390,   390,   390,
     390,   390,   391,   391,   391,   391,   391,   392,   392,   393,
     392,   394,   394,   395,   395,   395,   396,   397,   396,   398,
     398,   398,   398,   399,   399,   399,   401,   400,   402,   402,
     403,   404,   403,   405,   405,   405,   406,   408,   409,   407,
     410,   411,   407,   412,   412,   413,   413,   414,   415,   415,
     415,   415,   416,   416,   416,   417,   417,   419,   420,   418,
     421,   421,   421,   421,   421,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   423,   423,   423,   423,   423,   423,   423,   423,
     424,   425,   425,   425,   426,   426,   427,   427,   427,   429,
     430,   428,   431,   431,   432,   432,   433,   433,   434,   434,
     434,   434,   434,   434,   435,   436,   434,   434,   434,   437,
     434,   434,   434,   434,   434,   434,   434,   434,   434,   434,
     434,   434,   434,   438,   439,   434,   434,   440,   441,   434,
     442,   443,   434,   444,   445,   434,   434,   446,   447,   434,
     448,   449,   434,   434,   450,   451,   434,   452,   453,   434,
     434,   454,   455,   434,   456,   457,   434,   458,   459,   434,
     460,   461,   434,   462,   462,   462,   464,   465,   466,   467,
     463,   469,   470,   471,   472,   468,   474,   475,   476,   477,
     473,   478,   478,   478,   478,   478,   479,   479,   479,   479,
     479,   479,   479,   479,   480,   480,   481,   482,   482,   483,
     483,   484,   484,   485,   485,   486,   486,   487,   487,   488,
     488,   489,   489,   490,   490,   490,   491,   491,   491,   492,
     492,   493,   493,   493,   493,   493,   493,   494,   495,   493,
     496,   497,   493,   498,   499,   493,   500,   501,   493,   502,
     502,   502,   503,   503,   504,   505,   506,   504,   507,   507,
     508,   508,   508,   509,   510,   508,   511,   512,   508,   513,
     514,   508,   508,   515,   516,   508,   508,   517,   518,   508,
     519,   519,   520,   520,   521,   521,   521,   521,   521,   522,
     522,   523,   523,   524,   524,   524,   524,   524,   524
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     4,     1,     1,     2,     2,     3,     2,
       0,     2,     0,     0,     3,     0,     2,     5,     3,     1,
       2,     0,     4,     2,     2,     1,     1,     1,     1,     2,
       3,     3,     2,     4,     0,     1,     2,     1,     3,     1,
       3,     3,     3,     2,     1,     1,     0,     2,     4,     1,
       1,     1,     1,     0,     0,     3,     1,     1,     1,     1,
       1,     4,     0,     6,     0,     6,     2,     3,     3,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     1,     5,
       1,     3,     2,     3,     2,     1,     1,     1,     1,     1,
       4,     1,     2,     3,     3,     3,     3,     2,     1,     3,
       0,     3,     0,     2,     3,     0,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     2,     2,     3,     4,     3,     2,     2,
       2,     2,     2,     3,     3,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     1,     3,     0,     4,     1,
       1,     1,     1,     3,     7,     2,     2,     6,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     0,
       2,     2,     3,     0,     2,     0,     4,     0,     2,     1,
       3,     0,     0,     7,     0,     0,     7,     3,     2,     2,
       2,     1,     1,     3,     2,     2,     3,     0,     0,     5,
       1,     2,     5,     5,     5,     6,     2,     1,     1,     1,
       2,     3,     2,     2,     3,     2,     3,     2,     2,     3,
       4,     1,     1,     0,     1,     1,     1,     0,     1,     3,
       9,     8,     8,     7,     8,     7,     7,     6,     3,     3,
       0,     0,     7,     0,     0,     7,     0,     0,     7,     0,
       0,     6,     5,     8,    10,     5,     8,    10,     1,     3,
       1,     2,     3,     1,     1,     2,     2,     2,     2,     2,
       4,     1,     3,     0,     4,     4,     1,     6,     6,     0,
       7,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     6,     8,
       5,     6,     1,     4,     3,     0,     0,     8,     0,     0,
       9,     3,     4,     5,     6,     8,     5,     6,     0,     0,
       5,     3,     4,     4,     5,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     4,     3,
       4,     5,     4,     5,     3,     4,     1,     1,     2,     4,
       4,     7,     8,     3,     5,     0,     0,     8,     3,     3,
       3,     0,     0,     8,     3,     4,     0,     0,     9,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       2,     4,     1,     4,     4,     4,     4,     4,     1,     6,
       7,     6,     6,     7,     7,     6,     7,     6,     6,     0,
       4,     1,     0,     1,     1,     0,     1,     0,     1,     1,
       0,     1,     5,     0,     2,     0,     4,     0,     9,     0,
      10,     5,     3,     3,     4,     1,     1,     3,     3,     3,
       1,     3,     1,     3,     0,     2,     3,     3,     1,     3,
       0,     2,     3,     1,     1,     1,     2,     3,     3,     5,
       1,     1,     1,     1,     1,     0,     1,     1,     4,     3,
       3,     5,     4,     6,     5,     5,     4,     0,     2,     0,
       4,     0,     1,     0,     1,     1,     6,     0,     6,     0,
       2,     3,     5,     0,     1,     1,     0,     5,     2,     3,
       4,     0,     4,     0,     1,     1,     1,     0,     0,     9,
       0,     0,    11,     0,     2,     0,     1,     3,     1,     1,
       2,     2,     0,     1,     1,     0,     3,     0,     0,     7,
       1,     4,     3,     3,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     1,     3,     0,     2,     3,     0,
       0,     6,     1,     1,     1,     3,     3,     4,     1,     1,
       1,     1,     2,     3,     0,     0,     6,     4,     5,     0,
       9,     4,     2,     2,     3,     2,     3,     2,     2,     3,
       3,     3,     2,     0,     0,     6,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     6,     1,     0,     0,     6,
       0,     0,     7,     1,     0,     0,     6,     0,     0,     7,
       1,     0,     0,     6,     0,     0,     7,     0,     0,     6,
       0,     0,     6,     1,     3,     3,     0,     0,     0,     0,
      10,     0,     0,     0,     0,    10,     0,     0,     0,     0,
      10,     1,     1,     1,     1,     1,     3,     3,     5,     5,
       6,     6,     8,     8,     0,     1,     2,     1,     3,     3,
       5,     1,     2,     1,     0,     0,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     2,     1,     1,     0,
       1,     5,     4,     6,     7,     5,     7,     0,     0,    10,
       0,     0,    10,     0,     0,    10,     0,     0,     7,     1,
       3,     3,     3,     1,     5,     0,     0,    10,     1,     3,
       3,     4,     4,     0,     0,    11,     0,     0,    11,     0,
       0,    10,     5,     0,     0,     9,     5,     0,     0,    10,
       1,     3,     1,     3,     3,     3,     4,     7,     9,     0,
       3,     0,     1,     9,    10,    10,    10,     9,    10
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

  case 17: /* top_level_reader_macro: expr_reader semicolon  */
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

  case 30: /* format_string: %empty  */
        { (yyval.s) = new string(); }
    break;

  case 31: /* format_string: format_string STRING_CHARACTER  */
                                                 { (yyval.s) = (yyvsp[-1].s); (yyvsp[-1].s)->push_back((yyvsp[0].ch)); }
    break;

  case 32: /* optional_format_string: %empty  */
        { (yyval.s) = new string(""); }
    break;

  case 33: /* $@1: %empty  */
            { das_strfmt(scanner); }
    break;

  case 34: /* optional_format_string: ':' $@1 format_string  */
                                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 35: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 36: /* string_builder_body: string_builder_body character_sequence  */
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

  case 37: /* string_builder_body: string_builder_body "{" expr optional_format_string "}"  */
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

  case 38: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 39: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 40: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 41: /* $@2: %empty  */
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

  case 42: /* expr_reader: '%' name_in_namespace $@2 reader_character_sequence  */
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

  case 43: /* options_declaration: "options" annotation_argument_list  */
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

  case 45: /* keyword_or_name: "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 46: /* keyword_or_name: "keyword"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 47: /* keyword_or_name: "type function"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 48: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 49: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 50: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 51: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 52: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 53: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 54: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 55: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 59: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 60: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 61: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 62: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 63: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 64: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 65: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 66: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 67: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 68: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),
            (yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 69: /* semicolon: "end of line"  */
                       {}
    break;

  case 70: /* semicolon: "end of expression"  */
          {}
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

  case 81: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),
            (yyvsp[-1].pExpression),(yyvsp[0].pExpression));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 82: /* $@4: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 83: /* expression_if_then_else: expression_if_one_liner "if" $@4 expr expression_else_one_liner semicolon  */
                                                                                                                                                         {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),(yyvsp[-2].pExpression),ast_wrapInBlock((yyvsp[-5].pExpression)),(yyvsp[-1].pExpression) ? ast_wrapInBlock((yyvsp[-1].pExpression)) : nullptr);
    }
    break;

  case 84: /* $@5: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 85: /* expression_for_loop: "for" $@5 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 86: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 87: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = (yyvsp[-1].pExpression);
        pWhile->body = (yyvsp[0].pExpression);
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 88: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = (yyvsp[-1].pExpression);
        pWith->body = (yyvsp[0].pExpression);
        (yyval.pExpression) = pWith;
    }
    break;

  case 89: /* $@6: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 90: /* expression_with_alias: "assume" "name" '=' $@6 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 91: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 92: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 93: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 94: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 95: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 96: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 97: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 98: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 99: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 100: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 101: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 102: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 103: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 104: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 105: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 106: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 107: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 108: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 109: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 110: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 111: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 112: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 113: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 114: /* metadata_argument_list: metadata_argument_list semicolon  */
                                               {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 115: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 116: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 117: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 118: /* annotation_declaration_name: "template"  */
                                    { (yyval.s) = new string("template"); }
    break;

  case 119: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 120: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 121: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 122: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 123: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 124: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 125: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 126: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 127: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 128: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 129: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 130: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 131: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 132: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 133: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 134: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 135: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 136: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 137: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 138: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 139: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 140: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 141: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 142: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 143: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 144: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 145: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 146: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 147: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 148: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 149: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 150: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 151: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 152: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 153: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 154: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 155: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 156: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 157: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 158: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 159: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 160: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 161: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 162: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 163: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 164: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 165: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 166: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 167: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 168: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 169: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 170: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 171: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 172: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 173: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 174: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 175: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 176: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 177: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 178: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 179: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 180: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 181: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 182: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 183: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 184: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 185: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 186: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 187: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 188: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 189: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 190: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 191: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 192: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 193: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 194: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 195: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 196: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 197: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 198: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 199: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 200: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 201: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 202: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 203: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 204: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 205: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 206: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 207: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 208: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 209: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 210: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 211: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 212: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 213: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 214: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 215: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 216: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 217: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 218: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 219: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 220: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 221: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 222: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 223: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 224: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 225: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 226: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 227: /* $@7: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 228: /* function_declaration: optional_public_or_private_function $@7 function_declaration_header expression_block  */
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

  case 233: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 234: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 235: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 236: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 237: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 238: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 239: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 240: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 243: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 244: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 245: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 246: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 247: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 249: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 250: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 251: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 254: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 255: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 257: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 258: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 259: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 260: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 261: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 262: /* expr_keyword: "keyword" expr expression_block  */
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

  case 263: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 264: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 265: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 266: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 267: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 268: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 269: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 270: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 271: /* $@8: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 272: /* $@9: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 273: /* expression_keyword: "keyword" '<' $@8 type_declaration_no_options_list '>' $@9 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 274: /* $@10: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 275: /* $@11: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 276: /* expression_keyword: "type function" '<' $@10 type_declaration_no_options_list '>' $@11 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 277: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 278: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 279: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 280: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 281: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 282: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 283: /* name_in_namespace: "name" "::" "name"  */
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

  case 284: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 285: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 286: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 287: /* $@12: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 288: /* $@13: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 289: /* new_type_declaration: '<' $@12 type_declaration '>' $@13  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 290: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 291: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 292: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 293: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 294: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 295: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 296: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 297: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 298: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 299: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 300: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 301: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 302: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 303: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 304: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 305: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 306: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 307: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 308: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 309: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 310: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 311: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 312: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 313: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 314: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 315: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 316: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 317: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 318: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 319: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 320: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 321: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 322: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 323: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 324: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 325: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 327: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 328: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 329: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 330: /* $@14: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 331: /* $@15: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 332: /* expr_cast: "cast" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 333: /* $@16: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 334: /* $@17: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 335: /* expr_cast: "upcast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 336: /* $@18: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 337: /* $@19: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 338: /* expr_cast: "reinterpret" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 339: /* $@20: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 340: /* $@21: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 341: /* expr_type_decl: "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 342: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
                                                                         {
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

  case 343: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
                                                                                                {
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

  case 344: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
                                                                                                                        {
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

  case 345: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 346: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 347: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 348: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 349: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 350: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 351: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 352: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 353: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 354: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 355: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 356: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 357: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 358: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 359: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 360: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 361: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 362: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 363: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 364: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 365: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 366: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 367: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 368: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 369: /* $@22: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 370: /* expr_full_block_assumed_piped: block_or_lambda $@22 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 371: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 372: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 373: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 374: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 375: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 376: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 377: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 378: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 379: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 380: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 381: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 382: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 383: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 384: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 385: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 386: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 387: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 388: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 389: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 390: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 391: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 392: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 393: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 394: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 398: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 399: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 400: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 401: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 409: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 410: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 411: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 412: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 413: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 414: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 415: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 416: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 419: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 420: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 421: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 422: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 423: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 424: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 425: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 426: /* $@24: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 427: /* func_addr_expr: '@' '@' '<' $@23 type_declaration_no_options '>' $@24 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 428: /* $@25: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 429: /* $@26: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 430: /* func_addr_expr: '@' '@' '<' $@25 optional_function_argument_list optional_function_type '>' $@26 func_addr_name  */
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

  case 431: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 432: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 433: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 434: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 435: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 436: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 437: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 438: /* $@27: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 439: /* $@28: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 440: /* expr_field: expr '.' $@27 error $@28  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 441: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 442: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
                                                          {
            auto dd = new ExprMakeStruct(tokAt(scanner,(yylsp[-3])));
            dd->at = tokAt(scanner,(yylsp[-3]));
            dd->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            dd->useInitializer = false;
            dd->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = dd;
    }
    break;

  case 443: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 444: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 445: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 446: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 447: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 448: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 449: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 450: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 451: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 452: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 453: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 454: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 455: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 456: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 457: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 458: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 459: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 460: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 461: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 462: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 463: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 484: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 487: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 488: /* expr: '(' expr_list optional_comma ')'  */
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

  case 489: /* expr: '(' make_struct_single ')'  */
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

  case 490: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 491: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 492: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 493: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 494: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 495: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 496: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 497: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 498: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
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
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 504: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 505: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 506: /* $@30: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 507: /* expr: expr "is" "type" '<' $@29 type_declaration_no_options '>' $@30  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 508: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 509: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 510: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* $@31: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 512: /* $@32: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 513: /* expr: expr "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 514: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 515: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 516: /* $@33: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 517: /* $@34: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 518: /* expr: expr '?' "as" "type" '<' $@33 type_declaration '>' $@34  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 519: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 520: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 521: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 522: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 523: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 525: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 526: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 527: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 528: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 529: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 530: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 531: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 532: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 533: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 534: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 535: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 536: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 537: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 538: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 539: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 540: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 541: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 542: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 543: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 544: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 545: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 546: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 547: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 548: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 549: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 550: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 551: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 552: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 553: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 554: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 555: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 556: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 557: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 558: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 559: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 560: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 561: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 562: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 563: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 564: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 565: /* $@35: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 566: /* struct_variable_declaration_list: struct_variable_declaration_list $@35 structure_variable_declaration semicolon  */
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

  case 567: /* $@36: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 568: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@36 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 569: /* $@37: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 570: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@37 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 571: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 572: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 573: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 574: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 575: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 576: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 577: /* function_argument_list: function_argument_declaration_no_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 578: /* function_argument_list: function_argument_declaration_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 579: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 580: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 581: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 582: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 583: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 584: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 585: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 586: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 587: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 588: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 589: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 590: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 591: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 592: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 593: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 594: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 595: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 596: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 597: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 598: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 599: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 600: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 601: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 602: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 603: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 604: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 605: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 606: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 607: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 608: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 609: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 610: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 611: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 612: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 613: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 614: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 615: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 616: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 617: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 618: /* global_variable_declaration_list: global_variable_declaration_list "end of line"  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 619: /* $@38: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 620: /* global_variable_declaration_list: global_variable_declaration_list $@38 optional_field_annotation let_variable_declaration  */
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

  case 621: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 622: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 623: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 624: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 625: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 626: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                      {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 627: /* $@39: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 628: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@39 optional_field_annotation let_variable_declaration  */
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

  case 629: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 630: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 631: /* enum_list: enum_list "name" semicolon  */
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

  case 632: /* enum_list: enum_list "name" '=' expr semicolon  */
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

  case 636: /* $@40: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 637: /* single_alias: optional_public_or_private_alias "name" $@40 '=' type_declaration  */
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
        delete (yyvsp[-3].s);
    }
    break;

  case 641: /* $@41: %empty  */
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

  case 647: /* $@42: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 648: /* $@43: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 649: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@42 enum_list $@43 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 650: /* $@44: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 651: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 652: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 653: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 654: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 655: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 656: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 657: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 658: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 659: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 660: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 661: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 662: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 663: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 664: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 665: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 666: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 667: /* $@46: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 668: /* $@47: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 669: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@46 structure_name $@47 optional_struct_variable_declaration_list  */
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

  case 670: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 671: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 672: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 673: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 674: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 675: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 676: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 677: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 678: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 679: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 680: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 681: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 682: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 683: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 684: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 685: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 686: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 687: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 688: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 689: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 690: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 691: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 692: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 693: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 694: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 695: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 696: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 697: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 698: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 699: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 700: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 701: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 702: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 703: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 704: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 705: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 706: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 707: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 708: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 709: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 710: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 711: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 712: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 713: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 714: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 715: /* bitfield_bits: bitfield_bits semicolon "name"  */
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

  case 717: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 718: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
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

  case 719: /* $@48: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 720: /* $@49: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 721: /* bitfield_type_declaration: "bitfield" '<' $@48 bitfield_bits '>' $@49  */
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

  case 724: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 725: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 726: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 727: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 728: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 729: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 730: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 731: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 732: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 733: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 734: /* $@50: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 735: /* $@51: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 736: /* type_declaration_no_options: "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 737: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 738: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 739: /* $@52: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 740: /* type_declaration_no_options: '$' name_in_namespace '<' $@52 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 741: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 742: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 743: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 744: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 745: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 746: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 747: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 748: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 749: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 750: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 751: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 752: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 753: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 754: /* $@54: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 755: /* type_declaration_no_options: "smart_ptr" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 756: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 757: /* $@55: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 758: /* $@56: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 759: /* type_declaration_no_options: "array" '<' $@55 type_declaration '>' $@56  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 760: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 761: /* $@58: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 762: /* type_declaration_no_options: "table" '<' $@57 table_type_pair '>' $@58  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 763: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 764: /* $@60: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 765: /* type_declaration_no_options: "iterator" '<' $@59 type_declaration '>' $@60  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 766: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 767: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 768: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 769: /* type_declaration_no_options: "block" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 770: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 771: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 772: /* type_declaration_no_options: "block" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 773: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 774: /* $@65: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 775: /* $@66: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 776: /* type_declaration_no_options: "function" '<' $@65 type_declaration '>' $@66  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 777: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 778: /* $@68: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 779: /* type_declaration_no_options: "function" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 780: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 781: /* $@69: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 782: /* $@70: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 783: /* type_declaration_no_options: "lambda" '<' $@69 type_declaration '>' $@70  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 784: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 785: /* $@72: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 786: /* type_declaration_no_options: "lambda" '<' $@71 optional_function_argument_list optional_function_type '>' $@72  */
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

  case 787: /* $@73: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 788: /* $@74: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 789: /* type_declaration_no_options: "tuple" '<' $@73 tuple_type_list '>' $@74  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 790: /* $@75: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 791: /* $@76: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 792: /* type_declaration_no_options: "variant" '<' $@75 variant_type_list '>' $@76  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 793: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 794: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 795: /* type_declaration: type_declaration '|' '#'  */
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

  case 796: /* $@77: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 797: /* $@78: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 798: /* $@79: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 799: /* $@80: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 800: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@77 "name" $@78 open_block $@79 tuple_alias_type_list $@80 close_block  */
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

  case 801: /* $@81: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 802: /* $@82: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 803: /* $@83: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 804: /* $@84: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 805: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@81 "name" $@82 open_block $@83 variant_alias_type_list $@84 close_block  */
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

  case 806: /* $@85: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 807: /* $@86: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 808: /* $@87: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 809: /* $@88: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 810: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@85 "name" $@86 open_block $@87 bitfield_alias_bits $@88 close_block  */
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
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 817: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 818: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 819: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 820: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 821: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 822: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 823: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
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

  case 828: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
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
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 849: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 850: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 851: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 852: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 853: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
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
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 855: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
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
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 857: /* $@89: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 858: /* $@90: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 859: /* make_struct_decl: "struct" '<' $@89 type_declaration_no_options '>' $@90 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 860: /* $@91: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 861: /* $@92: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 862: /* make_struct_decl: "class" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 863: /* $@93: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 864: /* $@94: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 865: /* make_struct_decl: "variant" '<' $@93 variant_type_list '>' $@94 '(' use_initializer make_variant_dim ')'  */
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

  case 866: /* $@95: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 867: /* $@96: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 868: /* make_struct_decl: "default" '<' $@95 type_declaration_no_options '>' $@96 use_initializer  */
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
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 870: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 871: /* make_tuple: make_tuple ',' expr  */
                                      {
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back((yyvsp[-2].pExpression));
        }
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 872: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
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

  case 875: /* $@97: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 876: /* $@98: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 877: /* make_tuple_call: "tuple" '<' $@97 tuple_type_list '>' $@98 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 879: /* make_dim: make_dim "end of expression" make_tuple  */
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
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 882: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 883: /* $@99: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 884: /* $@100: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 885: /* make_dim_decl: "array" "struct" '<' $@99 type_declaration_no_options '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 886: /* $@101: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@102: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 888: /* make_dim_decl: "array" "tuple" '<' $@101 tuple_type_list '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 889: /* $@103: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 890: /* $@104: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 891: /* make_dim_decl: "array" "variant" '<' $@103 variant_type_list '>' $@104 '(' make_variant_dim ')'  */
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

  case 893: /* $@105: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 894: /* $@106: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 895: /* make_dim_decl: "array" '<' $@105 type_declaration_no_options '>' $@106 '(' optional_expr_list ')'  */
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

  case 897: /* $@107: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 898: /* $@108: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 899: /* make_dim_decl: "fixed_array" '<' $@107 type_declaration_no_options '>' $@108 '(' expr_list optional_comma ')'  */
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

  case 901: /* make_table: make_table "end of expression" make_map_tuple  */
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

  case 904: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
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

  case 910: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 911: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 912: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 913: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 914: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 915: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 916: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 917: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 918: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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
