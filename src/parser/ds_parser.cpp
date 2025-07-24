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
#define YYLAST   15006

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  303
/* YYNRULES -- Number of rules.  */
#define YYNRULES  929
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1736

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
    1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,
    1221,  1222,  1223,  1228,  1246,  1247,  1248,  1252,  1258,  1258,
    1276,  1277,  1280,  1281,  1284,  1288,  1299,  1308,  1317,  1323,
    1324,  1325,  1326,  1327,  1328,  1329,  1330,  1331,  1332,  1333,
    1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,  1342,  1343,
    1347,  1352,  1358,  1364,  1375,  1376,  1380,  1381,  1385,  1386,
    1390,  1394,  1401,  1401,  1401,  1407,  1407,  1407,  1416,  1450,
    1453,  1456,  1459,  1465,  1466,  1477,  1481,  1484,  1492,  1492,
    1492,  1495,  1501,  1504,  1508,  1512,  1519,  1526,  1532,  1536,
    1540,  1543,  1546,  1554,  1557,  1560,  1568,  1571,  1579,  1582,
    1585,  1593,  1599,  1600,  1601,  1605,  1606,  1610,  1611,  1615,
    1620,  1628,  1634,  1640,  1646,  1652,  1661,  1670,  1679,  1691,
    1694,  1700,  1700,  1700,  1703,  1703,  1703,  1708,  1708,  1708,
    1716,  1716,  1716,  1722,  1732,  1743,  1756,  1766,  1777,  1792,
    1795,  1801,  1802,  1809,  1820,  1821,  1822,  1826,  1827,  1828,
    1829,  1830,  1834,  1839,  1847,  1848,  1849,  1853,  1858,  1865,
    1872,  1872,  1881,  1882,  1883,  1884,  1885,  1886,  1887,  1891,
    1892,  1893,  1894,  1895,  1896,  1897,  1898,  1899,  1900,  1901,
    1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,  1913,  1914,
    1915,  1916,  1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,
    1929,  1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1942,
    1949,  1961,  1966,  1976,  1980,  1987,  1990,  1990,  1990,  1995,
    1995,  1995,  2008,  2012,  2016,  2021,  2028,  2037,  2042,  2049,
    2049,  2049,  2056,  2060,  2069,  2077,  2085,  2089,  2092,  2098,
    2099,  2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,  2108,
    2109,  2110,  2111,  2112,  2113,  2114,  2115,  2116,  2117,  2118,
    2119,  2120,  2121,  2122,  2123,  2124,  2125,  2126,  2127,  2128,
    2129,  2130,  2131,  2132,  2133,  2139,  2140,  2141,  2142,  2143,
    2156,  2165,  2166,  2167,  2168,  2169,  2170,  2171,  2172,  2173,
    2174,  2175,  2176,  2179,  2182,  2183,  2186,  2186,  2186,  2189,
    2194,  2198,  2202,  2202,  2202,  2207,  2210,  2214,  2214,  2214,
    2219,  2222,  2223,  2224,  2225,  2226,  2227,  2228,  2229,  2230,
    2232,  2236,  2237,  2242,  2246,  2247,  2248,  2249,  2250,  2251,
    2252,  2256,  2260,  2264,  2268,  2272,  2276,  2280,  2284,  2288,
    2295,  2296,  2297,  2301,  2302,  2303,  2307,  2308,  2312,  2313,
    2314,  2318,  2319,  2323,  2334,  2337,  2340,  2340,  2359,  2358,
    2374,  2373,  2387,  2396,  2408,  2417,  2427,  2428,  2429,  2430,
    2431,  2435,  2438,  2447,  2448,  2452,  2455,  2458,  2474,  2483,
    2484,  2488,  2491,  2494,  2508,  2509,  2513,  2519,  2525,  2534,
    2537,  2544,  2547,  2553,  2554,  2555,  2559,  2560,  2564,  2571,
    2576,  2585,  2591,  2602,  2605,  2610,  2615,  2623,  2634,  2637,
    2640,  2640,  2660,  2661,  2665,  2666,  2667,  2671,  2674,  2674,
    2693,  2696,  2699,  2714,  2733,  2734,  2735,  2740,  2740,  2766,
    2767,  2771,  2772,  2772,  2776,  2777,  2778,  2782,  2792,  2797,
    2792,  2809,  2814,  2809,  2829,  2830,  2834,  2835,  2839,  2845,
    2846,  2847,  2848,  2852,  2853,  2854,  2858,  2861,  2867,  2872,
    2867,  2892,  2899,  2904,  2913,  2919,  2930,  2931,  2932,  2933,
    2934,  2935,  2936,  2937,  2938,  2939,  2940,  2941,  2942,  2943,
    2944,  2945,  2946,  2947,  2948,  2949,  2950,  2951,  2952,  2953,
    2954,  2955,  2956,  2960,  2961,  2962,  2963,  2964,  2965,  2966,
    2967,  2971,  2982,  2986,  2993,  3005,  3012,  3021,  3026,  3029,
    3042,  3042,  3042,  3055,  3056,  3060,  3064,  3071,  3075,  3082,
    3083,  3084,  3085,  3086,  3101,  3107,  3107,  3107,  3111,  3116,
    3123,  3123,  3130,  3134,  3138,  3143,  3148,  3153,  3158,  3162,
    3166,  3171,  3175,  3179,  3184,  3184,  3184,  3190,  3197,  3197,
    3197,  3202,  3202,  3202,  3208,  3208,  3208,  3213,  3217,  3217,
    3217,  3222,  3222,  3222,  3231,  3235,  3235,  3235,  3240,  3240,
    3240,  3249,  3253,  3253,  3253,  3258,  3258,  3258,  3267,  3267,
    3267,  3273,  3273,  3273,  3282,  3285,  3296,  3312,  3312,  3317,
    3322,  3312,  3347,  3347,  3352,  3358,  3347,  3383,  3383,  3388,
    3393,  3383,  3423,  3424,  3425,  3426,  3427,  3431,  3438,  3445,
    3451,  3457,  3464,  3471,  3477,  3486,  3489,  3495,  3503,  3508,
    3515,  3520,  3527,  3532,  3538,  3539,  3543,  3544,  3549,  3550,
    3554,  3555,  3559,  3560,  3564,  3565,  3566,  3570,  3571,  3572,
    3576,  3577,  3581,  3587,  3594,  3602,  3609,  3617,  3626,  3626,
    3626,  3634,  3634,  3634,  3641,  3641,  3641,  3652,  3652,  3652,
    3663,  3666,  3672,  3686,  3692,  3698,  3704,  3704,  3704,  3718,
    3723,  3730,  3749,  3754,  3761,  3761,  3761,  3771,  3771,  3771,
    3785,  3785,  3785,  3799,  3808,  3808,  3808,  3828,  3835,  3835,
    3835,  3845,  3850,  3857,  3860,  3866,  3885,  3894,  3902,  3922,
    3947,  3948,  3952,  3953,  3958,  3961,  3964,  3967,  3970,  3973
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

#define YYPACT_NINF (-1373)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-796)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1373,   125, -1373, -1373,    45,   -80,   190,   260, -1373,   -79,
     117,   117,   117, -1373, -1373,   226,    43, -1373, -1373,   470,
   -1373, -1373, -1373, -1373,    75, -1373,    36, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373,   -66, -1373,    13,
      90,    58, -1373, -1373, -1373, -1373,   190, -1373,    18, -1373,
   -1373, -1373,   117,   117, -1373, -1373,    36, -1373, -1373, -1373,
   -1373, -1373,    -3,   138, -1373, -1373, -1373, -1373,    43,    43,
      43,    77, -1373,   855,    59, -1373, -1373, -1373, -1373,   463,
     510,   469,   696, -1373,   740,    35,    45,   202,   -80,   203,
     252, -1373,   829,   829, -1373,   288,   470,    81,   470,   742,
     311,   319,   356, -1373,   387,   388, -1373, -1373,   -75,    45,
      43,    43,    43,    43, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373,   395, -1373, -1373, -1373, -1373, -1373, -1373, -1373,   260,
   -1373, -1373, -1373, -1373, -1373,   831,   112, -1373, -1373, -1373,
   -1373,   515, -1373, -1373, -1373, -1373, -1373, -1373, -1373,   470,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,   517,
   -1373,   124, -1373,   504,   501,   855, 14833, -1373,   136,   605,
   -1373,   -24, -1373, -1373, -1373,   841, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373,   201, -1373,   529, -1373,   260,   260,   260,
   -1373, -1373, 13710, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,   693,   707,
   -1373,   548,   260,   937, -1373, -1373,   602, -1373,   559,    45,
      45,   -44,   253, -1373, -1373, -1373,   112, -1373, 10490, -1373,
   -1373, -1373,   623,   681, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373,   705,   609, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,   805, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373,   706,   668, -1373, -1373,   -73,   715, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373,   260, -1373,   724,
     260, -1373, -1373,   -24,   137, -1373,    45, -1373,   647,   852,
     621, -1373, -1373,   734,   746,   747,   725,   750,   752, -1373,
   -1373, -1373,   745, -1373, -1373, -1373, -1373, -1373,   761, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
     763, -1373, -1373, -1373,   772,   775, -1373, -1373, -1373, -1373,
     787,   796,   759,   226, -1373, -1373, -1373, -1373, -1373,   601,
     800, -1373, -1373, -1373, -1373, -1373, -1373, -1373,   838,   843,
   -1373,   797, -1373,    85,   470,   333,   795, 10490, -1373,  2606,
   -1373,   562,   226, -1373, -1373, -1373,   253,   807, -1373,  9624,
     851,   875, 10490, -1373,   143, -1373, -1373, -1373,  9624, -1373,
   -1373,   879, -1373,   502,   505,   513, -1373, -1373,  9624,   153,
   -1373, -1373, -1373,     2, -1373, -1373, -1373,     5,  5912, -1373,
     844, 10238,   579, 10341,   518, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,  9624, -1373,
   -1373,   491,   -53,   -53,   -53, -1373,   800, -1373,   863,   864,
    9624, -1373, -1373, -1373, -1373, -1373,   648,   -94,   871,    31,
    3226, -1373, -1373,   260,   522,  6118,   836,  9624,   897,   874,
     878,   860, -1373,   470,   880,   913,  6324,    83,   551,   887,
   -1373,   564,   888,   889,  3432,  9624,  9624,   158,   158,   158,
     872,   873,   881,   882,   883,   885, -1373,  2184, 10135,  6532,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373,  6738,   890, -1373,
    6946,  9624,  9624,  9624,  9624,  9624,  5088,  7152, -1373,   877,
   -1373, -1373,   470,   470, -1373,  9624,  1082, -1373, -1373, -1373,
   -1373, -1373, -1373,  1069, -1373, -1373, -1373,   439, -1373,   -68,
     470, -1373,   470,   470,   470, -1373,   470, -1373, -1373,  1041,
   -1373, -1373, -1373, -1373,   893, -1373, -1373,   191, -1373, -1373,
   -1373, -1373, -1373, -1373,  9943, -1373,   894, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373,   350, -1373,   524, -1373,    29, -1373,
   -1373,   895,   912,   916, -1373, 10833, -1373,  1072,  1394, -1373,
   -1373, -1373,  3844, 10490, 10490, 10490, 10944, 10490, 10490,   898,
     944, 10490,   548, 10490,   548, 10490,   548, 10593,   945, 10982,
   -1373,  9624, -1373, -1373, -1373, -1373,   904, -1373, -1373, 13193,
    9624, -1373,   601,   470, -1373,   524,   -65, -1373, -1373,   413,
   -1373,   800,   524,   927,   413, -1373,   524, 11093,   907,  1083,
   -1373, -1373,   110, -1373, -1373, -1373, -1373, -1373, 13752,   491,
   -1373,   908, -1373, -1373,   226,   565, -1373,   933,   934,   939,
   -1373,  9624,  3844, -1373,   943,  1006, 10798,  1125, 10490,  9624,
    9624, 14235,  9624, 13752,   956, -1373, -1373,  9624, -1373, -1373,
     951,   983, 14235,  9624, -1373, -1373,  9624, -1373, -1373,  9624,
   -1373, 10490,  3844, -1373, 10798,   654,   654,   930, -1373,   893,
   -1373, -1373, -1373,  9624,  9624,  9624,  9624,  9624,  9624,   491,
    1940,   491,  2812,   491, 13938, -1373,   842, -1373, 13752, -1373,
     737,   491,   968, -1373,   962,   654,   654,   -17,   654,   654,
     491,  1142,   940,   967, 14235,   942,   134,   967,   969,   946,
     229, -1373, -1373, 13752, -1373,   260, -1373, -1373, -1373,  4050,
   -1373, -1373, -1373, -1373, -1373, -1373,   103,    55,   158, -1373,
   14607, 14641,  4256,  4256,  4256,  4256,  4256,  4256,  4256,  4256,
    9624,  9624, -1373, -1373,  9624,  4256,  4256,  9624,  9624,  9624,
     988,  4256,  9624,    99,  9624,  9624,  9624,  9624,  9624,  9624,
    4256,  4256,  9624,  9624,  9624,  4256,  4256,  4256,  9624,  4256,
    7358,  9624,  9624,  9624,  9624,  9624,  9624,  9624,  9624,  9624,
    9624, 14771,  9624, -1373,  7564,   562,  9624, -1373, -1373,    43,
   -1373,  1162, -1373,   -24, 10490, -1373,   998, -1373,  3844, -1373,
   10677,    87,   162,   974,   475, -1373,   538,   571, -1373, -1373,
     133,   635,   715,   663,   715,   682,   715, -1373,   194, -1373,
     239, -1373, 10490,   957, -1373, -1373, 13304, -1373, -1373, 10490,
   -1373, -1373, 10490, -1373, -1373, -1373,  9624,  1003, -1373,  1004,
   -1373, 10490, -1373,  9624, 10490, 10490, -1373,    21, 10490,  5294,
    7770,  1005,  9624, 10490, -1373, -1373, -1373, 10490,   967, -1373,
     943,  9624,  9624,  9624,  9624,  9624,  9624,  9624,  9624,  9624,
    9624,  9624,  9624,  9624,  9624,  9624,  9624,  9624,  9624,   260,
     728,   963, 14235, 11128, -1373, -1373, 10490, 10490, 11239, 10490,
   -1373, -1373, 11277, 10490,   967, 10490, 10593,   967,   945,   850,
   -1373, 10798, -1373,    55, 11388, 11423, 11534, 11572, 11683, 11718,
      28,   158,  3019,  4464,  5500, 13975,   991,    10,   212,   992,
     300,    32,  5706,    10,   500,    33,  9624,  1000,  9624, -1373,
   -1373, 10490, -1373, 10490, -1373,  9624,   664,    40, -1373,  9624,
   -1373,    42,   491, -1373,  9624, -1373,  9624,  9624,  9624,   971,
     401, -1373, -1373,   973,   977,   519, -1373, -1373,   783,  9624,
   -1373,   893,   -27,  4672, -1373,   251,   979,  1012,  1012, -1373,
   -1373,   981,   -19,   548, -1373,   993,   982, -1373, -1373,  1002,
     984, -1373, -1373,   158,   158,   158, -1373, -1373,  2365, -1373,
    2365, -1373,  2365, -1373,  2365, -1373,  2365, -1373,  2365, -1373,
    2365, -1373,  2365,  1508,  1508,   965, -1373,  2365, -1373,  2365,
     965, 14455, 14455,   985, -1373,  2365,   219,   986, -1373, 13342,
     -30,   -30,   894, 14235,  1508,  1508, -1373,  2365, -1373,  2365,
   10634,  2177,  9990, -1373,  2365, -1373,  2365, -1373,  2365, 14328,
   -1373,  2365, 14740, 14068, 14365, 14421,  1174,   965,   965,   853,
     853,   219,   219,   219,   587,  9624,   987,   989,   613,  9624,
    1185,   994, 13453, -1373,   268,   524, 13845,   373,   756,  1138,
     470,  1086, -1373, -1373, 10677, -1373, -1373, -1373, -1373, 10490,
   -1373, -1373, -1373,  1035, -1373,  1010, -1373,  1014, -1373,  1016,
   -1373, 10593, -1373,   945,   342,   800, -1373, -1373,   800,   800,
   11829, -1373,  1173,   -39, 14235,  1381,  1854,  9624,   691,   619,
     277,   999,  1007,  1045, 11867,   222, 11978,   701, 10490, 10593,
     945,  2271,  1008, 14235, 14235, 14235, 14235, 14235, 14235, 14235,
   14235, 14235, 14235, 14235, 14235, 14235, 14235, 14235, 14235, 14235,
   14235, -1373,  1011, 10490, -1373, -1373,  9624,  2323,  4253, -1373,
    4461, -1373,  4669,  1009,  4877,   371,  1013,   378,    55,   548,
   -1373, -1373, -1373, -1373, -1373,  1018,  9624, -1373,  4880, 13193,
     -21,  9624,   401,   619,   212, -1373, -1373,  1026, -1373,  9624,
    9624, -1373,  1027, -1373,  9624,   619,   633,  1032, -1373, -1373,
    9624, 14235, -1373, -1373,   429,   431, 14105,  9624, -1373,  9624,
      56, 12013, 14235, 14235, -1373,  1038,   168,  9624,  9624, 10490,
     548,   260, -1373, -1373,  9624, -1373,  1680,  2606,    55,   179,
   -1373,  1040,   411,  9830, -1373, -1373, -1373,   433,   305,   -19,
    1047,  1049,  1044,  1050,  1051, -1373,   455,   715, -1373,  9624,
   -1373,  9624, -1373, -1373, -1373,  7976,  9624, -1373,  1031,  1046,
   -1373, -1373,  9624,  1052, -1373, 13488,  9624,  8182,  1055, -1373,
   13599, -1373,  8388, -1373, -1373, -1373, -1373,   470, -1373, -1373,
     679, -1373,   116, -1373,    55, -1373, -1373, -1373, -1373,   800,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373,  1056, 10490, -1373,  1057,  9624, -1373,
   -1373,   317, -1373,  1059, -1373, -1373, -1373,   438, -1373,  1089,
    1058, -1373, -1373,  6735,   453,   466, -1373, -1373,  9624,  9827,
   14235, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
     703,   715,  8594,   641,    10,   212, 14235,   991, -1373, -1373,
   14235,   992, -1373,   644,    10,  1048, -1373, -1373, -1373, -1373,
     652, -1373, -1373, -1373,  1093,   659,   660,  9624,   197,  9624,
    9624,  9624, 12124, 12162, 10323,   715, -1373, 13752, -1373,   470,
   -1373,   548, -1373,  1060,  4672,  1107,  1065,   697,   353, -1373,
   -1373,  1109, -1373, -1373,   -19,  1068,   183, 10490, 12273, 10490,
   12308, -1373,   363, 12419, -1373,  9624,  1121,  9624, -1373, 12457,
    4672, -1373,   366,  9624, -1373, -1373, -1373,   367, -1373,  1254,
     116, -1373, -1373,   756,  1071, -1373, -1373, -1373,  9624,   800,
   -1373, 14235,  1078,  1079, -1373, -1373, -1373,  9624,  1116,  1110,
    9624, -1373, -1373, -1373, -1373,  1112,  1113,  1115,  9624,  9624,
    9624,  1117,  1260,  1120,  1123,  8800, -1373,   183, -1373,   415,
    9624,   212, -1373,  9624,   633, -1373,  9624,  9624,  1126, -1373,
   -1373,  9624,  9624,   670,  9624,  9624, 12568, 14235, 14235, -1373,
   -1373, -1373,  1134,   783,  3638, -1373,   715, -1373,   456, -1373,
     744, 10490,   143, -1373,  1127, -1373, -1373,  9006, -1373, -1373,
   10336, -1373,   708, -1373, -1373, -1373, 10490, 12603, 12714, -1373,
     465, -1373, 12752, -1373, -1373, -1373,  1254,   491,  1128,  1260,
    1260, 12863,  1139,  1130, 12898,  1131,  1132,  1133,  9624, -1373,
    9624,   965,   965,   965,  9624, -1373, -1373,  1260,  1260, -1373,
   13009, -1373, -1373, 14198, -1373, 14198, -1373,  1159,   965,  9624,
   -1373,  1176,  1159, 14198,  9624, 14235, 14235,   206,   289, -1373,
   -1373,  9212,  9418, -1373, -1373, -1373, -1373, -1373, 14235,   260,
    1136, 10490,   143,   936,  3844, -1373,  9624, 14328, -1373, -1373,
     713, -1373, -1373,  1137, -1373, 14833, -1373, -1373, -1373, -1373,
   -1373,    52,    52, -1373,  9624,  9624, -1373,  1260,  1260,   619,
    1140,  1141,   967,    52,   619, -1373,  1298,  1144,  1172,  1178,
    1166, -1373,  1179,  1148, 14198,  9624,  9624, -1373,   289,  9624,
    9624, 14235, -1373, -1373,   936,  3844,  3844, -1373, 10677,  1121,
   -1373, -1373, -1373, -1373,   470, 14833,   619,   991,  1175, -1373,
    1147,  1149, 13047, 13158,    52,    52,   991,  1151, -1373, -1373,
    1152,  1161,  1164,  9624,  1167,  1168,  1201, -1373,  1169, -1373,
   -1373,  1170, 14235, 14235, -1373, 14235,  3844, -1373, 10677, -1373,
   10677, -1373, -1373, -1373,   260,   445,  1177, -1373, -1373, -1373,
   -1373, -1373,  1180,  1181, -1373, -1373, -1373, -1373, 14235, -1373,
   -1373, -1373, -1373, -1373, -1373, 10677, -1373, -1373, -1373, -1373,
     619, -1373, -1373, -1373,   451, -1373
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   130,     1,   325,     0,     0,     0,   652,   326,     0,
     644,   644,   644,    69,    70,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   632,     6,    11,     5,
       4,    13,    12,    14,   100,   101,    99,   108,   110,    43,
      59,    56,    57,    45,    46,    47,     0,    48,    54,    44,
     241,   240,   644,   644,    22,    21,   632,   646,   645,   817,
     807,   812,     0,   293,    41,   116,   117,   118,     0,     0,
       0,   119,   121,   128,     0,   115,    17,   670,   669,   234,
     654,     0,   673,   633,   634,     0,     0,     0,     0,    49,
       0,    55,     0,     0,    52,     0,     0,   644,     0,    18,
       0,     0,     0,   295,     0,     0,   127,   122,     0,     0,
       0,     0,     0,     0,   131,   236,   235,   238,   233,   656,
     655,     0,   672,   671,   675,   674,   678,   636,   635,   638,
     106,   107,   104,   105,   103,     0,     0,   102,   111,    60,
      58,    54,    51,    50,   647,   649,   243,   242,   651,     0,
     653,    19,    20,    23,   818,   808,   813,   294,    39,    42,
     126,     0,   123,   124,   125,   129,     0,   657,     0,   666,
     628,   560,    24,    25,    29,     0,    95,    96,    93,    94,
      92,    91,    97,     0,    53,     0,   650,     0,     0,     0,
      40,   120,     0,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,     0,     0,
     137,   132,     0,     0,   658,   667,     0,   679,   630,     0,
       0,   562,     0,    26,    27,    28,     0,   109,     0,   819,
     809,   814,   202,   203,   200,   140,   141,   143,   142,   144,
     145,   146,   147,   173,   174,   171,   172,   164,   175,   176,
     165,   162,   163,   201,   184,     0,   199,   177,   178,   179,
     180,   151,   152,   153,   148,   149,   150,   161,     0,   167,
     168,   166,   159,   160,   155,   154,   156,   157,   158,   139,
     138,   183,     0,   169,   170,   560,   135,   270,   239,   713,
     716,   719,   720,   714,   717,   715,   718,     0,   640,   664,
     676,   629,   637,   560,     0,   112,     0,   114,     0,   618,
     616,   639,    98,     0,     0,     0,     0,     0,     0,   686,
     706,   687,   722,   688,   692,   693,   694,   695,   712,   699,
     700,   701,   702,   703,   704,   705,   707,   708,   709,   710,
     777,   691,   698,   711,   784,   791,   689,   696,   690,   697,
       0,     0,     0,     0,   721,   739,   742,   740,   741,   804,
     648,   727,   595,   601,   204,   205,   198,   182,   206,   185,
     181,     0,   133,   324,   586,   587,     0,     0,   237,     0,
     661,   659,     0,   668,   574,   680,     0,     0,   113,     0,
       0,     0,     0,   617,     0,   745,   768,   771,     0,   774,
     764,     0,   730,   778,   785,   792,   798,   801,     0,     0,
     754,   759,   753,     0,   767,   763,   756,     0,     0,   758,
     743,     0,   820,   810,   815,   207,   187,   188,   190,   189,
     191,   192,   193,   194,   186,   195,   196,   197,     0,   322,
     323,     0,   560,   560,   560,   134,   136,   272,     0,     0,
       0,    71,    72,    84,   465,   466,     0,     0,     0,     0,
     310,   459,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,     0,     0,     0,     0,     0,     0,
     712,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   549,     0,     0,     0,
     382,   384,   383,   385,   386,   387,   388,     0,     0,    35,
     278,     0,     0,     0,     0,     0,   274,     0,   364,   365,
     463,   462,     0,     0,   249,     0,     0,   265,   260,   257,
     256,   258,   259,   244,   292,   271,   251,   543,   250,   460,
       0,   534,    79,    80,    77,   263,    78,   264,   266,   328,
     255,   533,   532,   531,   130,   537,   461,     0,   252,   536,
     535,   507,   467,   508,   389,   468,     0,   464,   822,   826,
     823,   824,   825,   640,     0,   641,     0,   665,   576,   631,
     561,     0,     0,     0,   543,     0,   620,   621,     0,   614,
     615,   613,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   132,     0,   132,     0,   132,     0,     0,     0,
     750,   274,   761,   762,   755,   757,     0,   760,   744,     0,
       0,   806,   805,     0,   728,     0,   293,   733,   734,     0,
     596,   591,     0,     0,     0,   602,     0,     0,     0,   681,
     583,   584,   606,   588,   590,   589,   868,   871,     0,     0,
     298,   302,   301,   307,     0,     0,   350,     0,     0,     0,
     904,     0,     0,   314,   311,     0,   359,     0,     0,   278,
       0,   296,     0,     0,     0,   341,   344,     0,   269,   347,
       0,     0,    63,     0,    86,   908,     0,   877,   886,     0,
     874,     0,     0,   319,   316,   495,   496,   365,   377,   130,
     291,   289,   290,     0,     0,     0,     0,     0,     0,     0,
     846,     0,     0,     0,   884,   911,     0,   282,     0,   285,
       0,     0,     0,   913,   922,   472,   471,   509,   470,   469,
       0,     0,     0,   922,   359,     0,   293,   922,   922,     0,
     366,   267,   268,     0,    82,     0,   380,   247,   541,     0,
     254,   261,   262,   313,   318,   327,     0,   374,     0,   253,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   449,     0,   246,     0,   662,     0,   642,   660,     0,
     575,     0,   677,   560,     0,   619,     0,   623,     0,   627,
     389,     0,     0,     0,   735,   748,     0,     0,   723,   725,
       0,     0,   135,     0,   135,     0,   135,   593,     0,   599,
       0,   724,     0,     0,   752,   737,     0,   729,   821,     0,
     597,   811,     0,   603,   816,   585,     0,     0,   605,     0,
     604,     0,   607,     0,     0,     0,    87,     0,     0,   860,
       0,     0,     0,     0,   894,   897,   900,     0,   922,   315,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,     0,    88,    89,     0,     0,     0,     0,
      61,    62,     0,     0,   922,     0,     0,   922,     0,     0,
     320,   317,   366,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   880,   838,   846,     0,   889,
       0,     0,     0,   846,     0,     0,     0,     0,     0,   849,
     916,     0,   273,     0,    38,     0,    36,     0,   915,   923,
     279,     0,     0,   891,   923,   275,     0,     0,     0,     0,
     923,   837,   500,     0,     0,   436,   433,   435,    66,     0,
     270,   130,     0,   274,   452,     0,     0,     0,     0,   340,
     339,     0,     0,   132,   288,     0,     0,   520,   519,     0,
       0,   521,   525,     0,     0,     0,   411,   420,   399,   421,
     400,   423,   402,   422,   401,   424,   403,   414,   393,   415,
     394,   416,   395,   473,   474,   486,   425,   404,   426,   405,
     487,   484,   485,     0,   413,   391,   514,     0,   505,     0,
     538,   539,   540,   392,   475,   476,   427,   406,   428,   407,
     491,   492,   493,   417,   396,   418,   397,   419,   398,   494,
     412,   390,     0,     0,   489,   490,   488,   482,   483,   478,
     477,   479,   480,   481,     0,     0,     0,   442,     0,     0,
       0,     0,     0,   457,     0,     0,     0,     0,   568,   571,
       0,     0,   622,   625,   389,   626,   746,   769,   772,     0,
     775,   765,   731,     0,   779,     0,   786,     0,   793,     0,
     799,     0,   802,     0,     0,   280,   749,   738,   592,   598,
       0,   683,   684,   609,   608,     0,     0,     0,     0,   861,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   360,   399,   400,   402,   401,   403,   393,
     394,   395,   404,   405,   391,   406,   407,   396,   397,   398,
     390,   321,     0,     0,   917,   510,     0,     0,     0,   511,
       0,   542,     0,     0,     0,     0,     0,     0,   374,   132,
     544,   545,   546,   547,   548,     0,     0,   847,     0,   359,
     846,     0,     0,     0,     0,   855,   856,     0,   863,     0,
       0,   853,     0,   892,     0,     0,     0,     0,   851,   893,
       0,   883,   848,   912,     0,     0,    32,     0,   914,     0,
       0,     0,   828,   827,   499,     0,     0,     0,     0,     0,
     132,     0,    64,    65,     0,    81,    73,     0,   374,     0,
     453,     0,     0,     0,   456,   454,   329,     0,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   135,   516,     0,
     522,     0,   410,   408,   409,     0,     0,   503,     0,     0,
     526,   530,     0,     0,   506,     0,     0,     0,     0,   443,
       0,   450,     0,   501,   458,   663,   643,   131,   569,   570,
     571,   572,   563,   577,   374,   624,   747,   770,   773,   736,
     776,   766,   732,   726,   780,   782,   787,   789,   794,   796,
     800,   594,   803,   600,     0,     0,   682,     0,     0,   869,
     872,     0,   299,     0,   304,   305,   303,     0,   353,     0,
       0,   356,   351,     0,     0,     0,   905,   903,   278,     0,
      90,   342,   345,   348,   909,   907,   878,   887,   885,   875,
       0,   135,     0,     0,   846,     0,   881,   839,   862,   854,
     882,   890,   852,     0,   846,     0,   858,   859,   866,   850,
       0,   283,   286,    33,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   135,    67,     0,    74,     0,
     245,   132,   455,     0,   274,     0,     0,   616,     0,   369,
     370,     0,   368,   367,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,   527,     0,   515,     0,   504,     0,
     274,   444,     0,     0,   502,   451,   447,     0,   582,   566,
     563,   564,   565,   568,     0,   783,   790,   797,   274,   281,
     685,   610,     0,     0,    85,   300,   306,     0,     0,     0,
       0,   352,   895,   898,   901,     0,     0,     0,     0,     0,
       0,     0,   860,     0,     0,     0,   248,     0,   550,     0,
       0,     0,   864,     0,     0,   857,     0,     0,   276,    30,
      37,     0,     0,     0,     0,     0,     0,   830,   829,   434,
     559,   437,     0,    66,     0,    83,   135,   429,     0,   330,
     616,     0,     0,   376,     0,   373,   375,     0,   361,   379,
       0,   558,     0,   556,   432,   553,     0,     0,     0,   552,
       0,   445,     0,   448,   567,   578,   566,     0,     0,   860,
     860,     0,     0,     0,     0,     0,     0,     0,   274,   918,
     278,   343,   346,   349,     0,   861,   879,   860,   860,   512,
       0,   378,   551,   920,   865,   920,   867,   920,   284,   274,
     287,    34,   920,   920,     0,   832,   831,     0,     0,   440,
      68,   310,     0,    75,    79,    80,    77,    78,    76,     0,
       0,     0,     0,     0,     0,   371,     0,   362,   517,   523,
       0,   557,   555,     0,   554,     0,   580,   612,   611,   573,
     751,   845,   845,   354,     0,     0,   357,   860,   860,   835,
       0,     0,   922,   845,   835,   513,     0,     0,     0,     0,
     922,    31,     0,     0,   920,     0,     0,   438,     0,     0,
       0,   316,   381,   430,     0,     0,     0,   338,   389,   363,
     518,   524,   528,   446,     0,     0,     0,   842,   922,   844,
       0,     0,     0,     0,   845,   845,   836,     0,   906,   919,
       0,     0,     0,     0,     0,     0,     0,   923,     0,   928,
     924,     0,   834,   833,   441,   317,     0,   336,   389,   334,
     389,   337,   529,   579,     0,     0,   923,   843,   870,   873,
     355,   358,     0,     0,   902,   910,   888,   876,   921,   926,
     927,   929,   277,   925,   332,   389,   335,   333,   581,   840,
       0,   896,   899,   331,     0,   841
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1373, -1373, -1373, -1373, -1373, -1373,   661,  1305, -1373, -1373,
   -1373, -1373, -1373, -1373,  1391, -1373, -1373, -1373,   778,  1347,
   -1373,  1255, -1373, -1373,  1307, -1373, -1373, -1373,  -116,    -1,
   -1373, -1373, -1373,  -112, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373,  1171, -1373, -1373,   -58,   -52, -1373, -1373,
   -1373,   435,   574,  -541,  -558,  -810, -1373, -1373, -1373, -1372,
   -1373, -1373,     1,  -206,  -219,  -385, -1373,   400, -1373,  -611,
   -1373,  -678,  -276,  -328, -1373, -1373, -1373, -1373,  -463,     4,
   -1373, -1373, -1373, -1373, -1373,  -110,  -109,  -104, -1373,  -103,
   -1373, -1373, -1373,  1411, -1373,   407, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,  -437,
     -71,   468,    -7,   149,  -919,  -432, -1373,  -542, -1373, -1373,
    -377,  1560, -1373, -1373, -1373, -1321, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373,   953, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373,  -155,   -29,  -126,   -28,   114, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373,   592,  -416,  -906, -1373,  -419,
    -904, -1373,  -641,  -121,  -118, -1373,  -596, -1349, -1373,  -369,
   -1373, -1373,  1376, -1373, -1373, -1373,   854,  1029,    66, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,  -630,
    -195, -1373,   970, -1373, -1373, -1373, -1373, -1373, -1373,  -330,
   -1373, -1373,  -347, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
    -114, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373,   972,  -600,  -200,  -738,
    -697, -1373, -1373, -1260,  -918, -1373, -1373, -1373, -1188,   -59,
   -1028, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
     220,  -503, -1373, -1373, -1373,   721, -1373, -1373, -1373, -1373,
   -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373, -1373,
   -1286,  -734, -1373
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   153,    56,    18,   175,   181,  1581,  1394,
    1499,   730,   530,   159,   531,   105,    20,    21,    47,    48,
      49,    94,    22,    41,    42,   532,   533,  1254,  1255,   638,
     535,  1409,  1514,   536,   537,   999,   538,   659,   539,   540,
     541,   542,  1186,   182,   183,    37,    38,    39,   231,    71,
      72,    73,    74,    24,   296,   388,   221,    25,   117,   222,
     118,   166,   297,   148,   708,  1026,   545,   389,   546,   742,
    1580,   732,  1134,   594,   971,  1497,   973,  1498,   548,   549,
     550,   661,   888,  1465,   551,   552,   553,   554,   555,   556,
     557,   558,   451,   559,   766,  1267,  1009,   560,   561,   926,
    1478,   927,  1479,   929,  1480,   562,   893,  1471,   563,   743,
    1529,   564,  1275,  1276,  1013,   710,   565,   823,  1001,   566,
     675,  1027,   568,   569,   570,   997,   571,  1249,  1588,  1250,
    1648,   572,  1100,  1445,   573,   744,  1427,  1660,  1429,  1661,
    1536,  1702,   575,   383,  1453,  1545,  1310,  1312,  1110,   588,
     833,  1615,  1665,   384,   385,   386,   857,   858,   433,   859,
     860,   434,   988,   650,   651,  1619,   602,   404,   320,   321,
     228,   313,    84,   129,    27,   171,   391,    95,    96,   185,
      97,    28,    53,   121,   168,    29,   308,   586,   583,  1105,
     393,   226,   227,    82,   126,   395,    30,   169,   310,   652,
     576,   307,   366,   367,   850,   432,   368,   610,  1322,  1131,
     843,   430,   369,   603,  1316,   862,   608,  1321,   604,  1317,
     605,  1318,   607,  1320,   611,  1324,   612,  1455,   613,  1326,
     614,  1456,   615,  1328,   616,  1457,   617,  1330,   618,  1332,
     641,    31,   101,   188,   372,   642,    32,   102,   189,   373,
     646,    33,   100,   187,   371,   635,   577,   748,  1677,   749,
     957,  1668,  1669,  1670,   958,   970,  1229,  1223,  1218,  1388,
    1152,   578,   884,  1462,   885,  1463,   938,  1484,   935,  1482,
     959,   733,   579,   936,  1483,   960,   580,  1158,  1555,  1159,
    1556,  1160,  1557,   897,  1475,   933,  1481,   726,   734,   581,
    1637,   980,   582
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   921,   838,   298,   544,   757,   725,   673,    52,   985,
     863,   883,   567,   989,   991,   644,   232,   639,    76,    64,
      75,  1006,   312,   767,  1199,   963,  1378,   589,   138,   887,
    1195,   703,   951,   674,  1197,   622,   667,  -130,   624,  1214,
      90,  1147,  1125,   365,  1127,  1226,  1129,   831,  1206,    62,
     130,   131,  1224,  1230,   852,   598,   854,   161,   856,  1011,
    1237,   547,  1239,   951,    34,    35,   711,   712,  1522,   110,
     111,   112,    75,    75,    75,    91,  1397,    65,   104,    63,
      77,    78,   381,    79,   632,   544,    40,   229,   878,   950,
     747,   961,   449,   965,    54,   145,   790,   150,    83,   793,
     794,   977,   381,   640,   645,   758,    66,   229,  1270,   544,
     981,    80,   793,   794,    75,    75,    75,    75,  1271,    98,
     956,    13,   956,   664,   370,     2,    85,   176,   177,   869,
     170,   745,     3,   668,   669,   450,   229,   745,    57,   839,
      55,    14,   547,   160,    58,   382,  1259,   230,   186,   759,
      67,  1151,  1259,   880,  1272,     4,   431,     5,   396,     6,
     720,   722,  1451,   149,  1162,     7,   547,   230,   943,   224,
     103,  1602,   315,  1273,    57,     8,   316,   314,  1274,    68,
      58,     9,    81,   543,   821,   822,    62,  1492,   239,   240,
     241,  1260,   365,   146,    13,  1213,   230,   821,   822,   623,
    1193,   132,   625,  1196,    86,    10,   133,   365,   134,   899,
     745,   135,   879,   147,    14,  1012,    63,   544,    36,   879,
     626,    92,  1452,   879,   879,  1259,   627,    11,    12,   670,
     317,   879,    93,   879,   898,   900,   365,   878,   365,   940,
     770,   771,   364,  1664,   829,   146,    50,   879,   671,    88,
     113,    69,   136,  1354,   677,   318,  1355,  1057,   398,   934,
      70,   878,   937,  1007,  1261,   147,    51,  1647,   694,  1666,
     599,   987,  1058,   456,   547,   114,   319,   104,   178,  1370,
     600,   104,   431,   179,    87,   180,  1116,   544,   135,  1638,
      13,  1639,  1375,  1704,   109,   878,  1642,  1643,    13,    50,
     693,   879,   880,  1574,   881,  1400,   878,   882,   390,   870,
      14,   394,   365,   365,   873,    86,   987,   544,    14,    51,
    1008,    50,  1005,   768,   878,  1527,   880,  1694,    86,    15,
     223,   920,  1122,   878,  1504,   601,  1014,   782,   783,  1411,
      16,    51,   191,  1645,   547,   790,    50,   792,   793,   794,
     795,   620,  1240,   397,   939,   796,    13,   431,  1691,    13,
     880,  1117,  1671,    43,    44,    45,    51,   419,   139,    62,
     621,   880,    62,  1681,   547,  1113,    14,   528,   707,    14,
     828,   993,   832,   452,   454,   637,   994,  1104,   534,   880,
     585,   364,   236,  1130,    46,  1454,   587,  1010,   880,    63,
    1215,  1216,    63,  1262,    13,   318,   364,  1349,   365,   365,
     365,  1343,   365,   365,  1712,  1713,   365,    93,   365,   237,
     365,  1350,   365,    50,    14,   141,   319,   995,  1217,   868,
     637,   634,    62,   821,   822,   364,   871,   364,  1132,   886,
     874,   993,  1263,    51,   547,   547,   547,   547,   547,   547,
     547,   547,  1150,   544,  1566,  1277,  1491,   547,   547,   901,
    1258,   144,    63,   547,   924,  1233,  1494,  1426,   901,  1264,
     364,   665,   547,   547,   122,   123,  1238,   547,   547,   547,
      50,   547,   688,   365,   154,  1220,  1304,  1111,  1221,   841,
     842,   844,   155,   846,   847,  1344,  1415,   851,    13,   853,
      51,   855,  1338,   106,   107,   108,   365,    13,   901,   972,
     547,   364,   364,  1374,  1119,    13,  1222,   747,    14,  1207,
     115,  1621,  1622,  1417,   453,   747,   116,    14,  1384,   156,
    1133,   751,   752,   637,   998,    14,    13,  1145,  1146,  1633,
    1634,  1334,   826,    13,  1424,   162,   163,   164,   165,   760,
    1161,   761,   762,   763,   901,   764,    14,   901,   901,  1245,
     157,  1487,   637,    14,   113,   158,   769,   119,   167,   637,
    1367,  1523,    91,   120,  1246,  1018,  1022,  1369,    13,  1187,
    1188,  1534,  1190,   827,  1541,  1543,  1192,   830,  1194,  1307,
    1183,  1282,  1283,  1284,    13,  1512,    13,   837,    14,  1674,
    1675,  1062,  1212,    13,   637,  1401,   901,   364,   364,   364,
     956,   364,   364,  1377,    14,   364,    14,   364,    13,   364,
     637,   364,   637,    14,  1415,   956,  1101,  1413,  1391,   637,
    1392,    13,   867,  1572,   420,    13,  1212,  1467,    14,   365,
      13,  1371,  1212,   648,   637,   110,  1424,  1212,   110,  1416,
     112,    14,  1473,   458,   459,    14,  1212,   637,   528,   707,
      14,   421,   422,  1729,   649,  1474,   637,   365,   890,  1735,
     431,  1425,  1600,   469,   365,   770,   771,   365,  1109,   474,
    1476,  1613,   364,    13,  1227,  1220,   365,  1228,   146,   365,
     365,   643,  1405,   365,   190,  1234,  -781,  1235,   365,  -788,
    1181,  -781,   365,    14,  -788,   364,  1599,  -795,   147,   637,
    1341,   225,  -795,  -439,  1333,  1331,   488,   489,  -439,  -781,
     678,   238,  -788,   146,   311,   423,  1449,    13,   293,   424,
    -795,   365,   365,   431,   365,   584,  -439,  1120,   365,   679,
     365,   365,   294,   147,    13,  1293,  1311,    14,  1135,   695,
     491,   492,   633,   124,   996,  1138,  1000,  1505,  1139,   125,
    1294,   420,   698,   891,    14,   295,   431,  1143,   696,  1373,
    1121,  1298,   782,   783,  1148,   309,   365,   745,   365,  1157,
     790,   699,   892,   793,   794,   795,  1299,  1383,   421,   422,
     796,    62,  1259,  1390,   425,  1251,   374,   127,   426,   151,
    1395,   427,  1396,   128,  1335,   152,  1252,  1253,   507,   508,
     509,  1353,   401,  1308,  1518,   402,   428,  1385,   403,  1309,
    1386,    63,   429,  1387,   585,   377,  1490,   378,  1486,  1493,
     431,   520,   901,    75,  1124,   901,  1359,  1496,   364,  1115,
    1540,   233,   234,   901,  1501,  1502,   660,  1548,  1432,  1123,
     901,   901,   423,  1516,   375,  1584,   424,  1135,   431,  1135,
    1442,   901,  1126,   526,   399,  1447,   364,  1133,   821,   822,
     142,   143,   544,   364,   770,   771,   364,   431,   376,   379,
     567,  1128,  1631,   420,   380,   364,   431,  1291,   364,   364,
    1342,  1521,   364,    13,   403,   400,   431,   364,  1680,  1305,
    1352,   364,  1404,   431,  1335,  1335,  1688,  1609,   431,   387,
     421,   422,  1662,    14,   172,   173,   974,   975,   392,   637,
    1485,   425,   528,   707,   365,   426,  1604,  1182,   427,   547,
     364,   364,   405,   364,  1707,  1489,   365,   364,  1601,   364,
     364,   403,   408,   428,   406,   407,  1646,  1630,   409,   429,
     410,   436,   437,   438,   439,   440,   441,   442,   443,   412,
    1503,   413,   411,   365,   365,   709,   709,   709,  1640,   420,
     414,   782,   783,   415,   423,   364,   418,   364,   424,   790,
     444,   792,   793,   794,   795,   416,   770,   771,   365,   796,
     445,   446,   447,  1577,   417,   431,   421,   422,  1582,   110,
     111,   112,    43,    44,    45,  1319,  1655,  1656,   172,   173,
     174,   435,   299,   455,   448,   756,   300,  1468,   233,   234,
     235,  1667,  1667,   590,   596,  1133,   967,   968,   969,  1676,
     301,   302,  1406,  1667,  1676,   303,   304,   305,   306,    59,
      60,    61,   756,   425,   653,   654,   655,   426,   597,  1198,
     427,  1410,   609,   682,   365,   818,   819,   820,  1696,   630,
     423,   656,   657,   599,   424,   428,  1705,   821,   822,   666,
     684,   429,   685,   600,  1667,  1667,   686,   687,   689,   690,
    1530,   780,   781,   782,   783,   697,   700,   701,   729,   713,
     714,   790,   754,   792,   793,   794,   795,   750,   715,   716,
     717,   796,   718,   798,   799,  1306,   755,   765,    16,  1313,
     834,   824,   693,  1315,   727,   836,   848,   849,   643,   420,
     864,   872,  1464,   364,   876,   889,   877,  1632,   601,   425,
    1734,   894,   895,   426,   901,   364,   427,   896,   768,   919,
     365,  1657,   770,   771,   756,   930,   421,   422,   925,   931,
     942,   428,   978,   979,   674,   982,   983,   429,   984,   986,
     990,  1053,   364,   364,   992,   816,   817,   818,   819,   820,
    1108,  1112,   756,  1118,  1603,  1136,  1141,  1142,  1155,   821,
     822,  1184,  1212,  1219,  1232,  1266,  1301,   364,  1513,  1244,
    1247,  1278,  1697,  1699,  1248,   770,   771,  1265,  1269,  1279,
    1280,  1281,  1285,  1286,  1296,  1311,  1297,  1528,  1323,  1325,
     423,  1302,   900,  1327,   424,  1329,  1337,  1345,  1347,   544,
    1419,  1459,  1420,  1422,  1423,  1346,  1357,  1365,  1358,  1434,
    1460,  1368,   365,  1724,   365,  1372,   709,   780,   781,   782,
     783,   784,  1379,  1382,   787,   788,   789,   790,  1389,   792,
     793,   794,   795,   364,  1654,  1399,   534,   796,  1412,   798,
     799,  1421,  1469,  1435,  1495,   802,   803,   804,  1528,  1437,
     544,   544,  1443,  1458,  1500,  1470,   547,  1466,  1517,   425,
    1519,  1520,  1524,   426,  1526,  1314,   427,  1544,  1485,  1552,
     780,   781,   782,   783,   784,  1549,  1550,   787,   788,   789,
     790,   428,   792,   793,   794,   795,  1448,   429,   756,  1553,
     796,   544,   798,   799,   810,  1532,   811,   812,   813,   814,
     815,   816,   817,   818,   819,   820,   365,   547,   547,  1558,
    1565,  1559,  1560,  1589,  1564,   821,   822,  1567,  1624,   364,
    1568,   365,   574,  1579,  1636,  1605,  1620,  1625,  1627,  1628,
    1629,  1683,   595,  1641,  1653,  1663,  1685,  1687,  1678,  1679,
    1684,   606,  1686,  1689,  1690,  1708,  1706,  1709,   547,  1714,
    1715,   619,   814,   815,   816,   817,   818,   819,   820,  1716,
    1652,   629,  1717,  1719,  1720,  1721,  1723,  1722,   821,   822,
     137,   976,    19,    89,  1730,   140,   184,  1590,  1731,  1732,
    1257,   647,  1593,  1107,  1594,  1595,   365,   322,  1515,   756,
    1596,  1597,    26,   658,   420,  1268,  1571,  1525,  1418,   709,
    1616,  1546,  1610,   676,  1450,  1547,  1617,   420,   681,  1618,
     683,   364,    99,   364,  1682,  1576,   662,   825,   663,   692,
    1381,   421,   422,   964,     0,     0,     0,   704,   705,   706,
       0,     0,     0,     0,   421,   422,     0,     0,     0,     0,
       0,     0,   724,     0,     0,     0,     0,     0,     0,     0,
     728,     0,     0,   724,   735,   736,   737,   738,   739,     0,
       0,     0,     0,     0,     0,  1728,     0,     0,   753,     0,
       0,   709,   709,   709,     0,     0,   756,     0,   756,     0,
     756,     0,   756,     0,   756,   423,   756,     0,   756,   424,
     756,     0,     0,     0,     0,   756,     0,   756,   423,     0,
       0,   599,   424,   756,     0,   364,     0,     0,     0,   770,
     771,   600,     0,     0,     0,   756,     0,   756,     0,     0,
     364,     0,   756,     0,   756,     0,   756,     0,     0,   756,
       0,     0,     0,     0,     0,   840,     0,     0,     0,    13,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   425,     0,     0,     0,   426,    14,
    1339,   427,   756,   866,     0,     0,   601,   425,     0,     0,
       0,   426,   996,     0,   427,     0,   428,     0,     0,     0,
       0,     0,   429,     0,     0,   364,     0,     0,     0,   428,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   676,   782,   783,     0,     0,
       0,     0,   724,   922,   790,   923,   792,   793,   794,   795,
     928,     0,     0,     0,   796,     0,   932,     0,     0,     0,
       0,     0,   996,     0,     0,   941,     0,  1701,     0,     0,
       0,     0,     0,  1703,     0,     0,   944,   945,   946,   947,
     948,   949,     0,   955,     0,   955,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1408,     0,     0,     0,     0,  1726,     0,  1727,
       0,   770,   771,     0,     0,     0,     0,     0,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,  1733,  1028,  1030,  1032,  1034,  1036,
    1038,  1040,  1042,  1043,  1044,     0,     0,  1045,  1047,  1049,
    1050,  1051,  1052,     0,  1055,  1056,     0,  1059,  1060,  1061,
    1063,  1064,  1065,  1067,  1069,  1070,  1071,  1072,  1074,  1076,
    1078,  1079,  1081,  1083,  1084,  1085,  1086,  1087,  1088,  1089,
    1090,  1091,  1092,  1093,     0,  1102,     0,     0,     0,  1106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1114,     0,     0,     0,     0,   780,   781,   782,   783,
     784,     0,     0,   787,   788,   789,   790,     0,   792,   793,
     794,   795,     0,     0,     0,     0,   796,     0,   798,   799,
       0,     0,     0,     0,   802,   803,   804,     0,     0,  1140,
     808,     0,     0,     0,     0,     0,  1144,     0,   756,     0,
       0,     0,     0,  1154,     0,  1156,     0,     0,     0,     0,
       0,     0,     0,     0,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,
    1179,  1180,     0,   810,     0,   811,   812,   813,   814,   815,
     816,   817,   818,   819,   820,     0,     0,   420,     0,     0,
       0,     0,     0,     0,   821,   822,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   735,  1209,     0,     0,     0,
       0,     0,     0,     0,   421,   422,     0,     0,     0,  1231,
       0,   724,     0,     0,     0,     0,     0,     0,  1236,     0,
       0,     0,   724,     0,     0,     0,     0,  1163,     0,  1241,
    1242,  1243,     0,     0,     0,   458,   459,     0,     0,     0,
       0,     0,  1256,     0,     0,   464,   465,   466,   467,   468,
       0,     0,     0,     0,     0,   469,     0,   471,     0,     0,
       0,   474,     0,   420,     0,     0,     0,     0,   423,   476,
       0,     0,   424,     0,     0,   479,     0,     0,   480,     0,
       0,   481,     0,   951,     0,   484,     0,     0,     0,     0,
     421,   422,     0,     0,     0,   591,     0,     0,   488,   489,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     490,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   491,   492,   592,     0,     0,   425,  1295,     0,
       0,   426,  1300,  1340,   427,     0,     0,     0,   495,   496,
       0,     0,     0,     0,   423,     0,     0,     0,   424,   428,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   500,   501,   502,   503,   504,     0,   505,   745,   506,
     507,   508,   509,     0,     0,     0,   510,   511,   512,   513,
     514,   515,   516,   746,   593,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,   756,     0,     0,     0,
       0,     0,     0,   425,     0,     0,     0,   426,     0,  1360,
     952,   522,   523,     0,    15,     0,     0,   524,   525,     0,
       0,     0,     0,     0,     0,   953,     0,   954,     0,   528,
     529,   429,     0,     0,  1376,     0,   756,     0,   756,     0,
       0,     0,  1380,   955,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   756,     0,     0,     0,   719,   770,   771,
    1402,  1403,     0,   323,     0,     0,     0,  1407,     0,   324,
     574,     0,     0,     0,     0,   325,  1163,     0,     0,     0,
       0,     0,     0,     0,     0,   326,     0,     0,     0,     0,
       0,     0,  1428,   327,  1430,     0,     0,     0,     0,  1433,
       0,     0,     0,     0,     0,  1436,     0,     0,   328,  1439,
       0,     0,     0,     0,     0,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,     0,     0,
       0,  1461,     0,   780,   781,   782,   783,   784,     0,     0,
     787,   788,   789,   790,   420,   792,   793,   794,   795,     0,
       0,   724,     0,   796,     0,   798,   799,     0,     0,     0,
       0,   802,     0,   804,     0,     0,     0,    62,     0,     0,
       0,   421,   422,  1029,  1031,  1033,  1035,  1037,  1039,  1041,
     362,     0,     0,     0,     0,  1046,  1048,     0,     0,     0,
       0,  1054,  1506,  1507,  1508,     0,   420,    63,     0,     0,
    1066,  1068,     0,     0,     0,  1073,  1075,  1077,     0,  1080,
       0,     0,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,     0,   421,   422,     0,   770,   771,  1537,     0,
    1538,   821,   822,     0,     0,   423,  1542,     0,     0,   424,
       0,     0,     0,   363,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1551,     0,     0,  1554,     0,     0,     0,     0,     0,     0,
       0,  1561,  1562,  1563,     0,     0,     0,     0,  1570,     0,
       0,     0,     0,  1573,     0,     0,  1575,   423,     0,   724,
    1578,   424,     0,     0,   724,  1583,     0,  1585,  1586,     0,
       0,     0,     0,     0,   425,     0,     0,  1598,   426,     0,
    1356,   427,     0,     0,     0,     0,     0,     0,     0,     0,
    1607,   780,   781,   782,   783,   784,   428,     0,   787,   788,
     789,   790,   429,   792,   793,   794,   795,     0,     0,     0,
       0,   796,     0,   798,   799,     0,     0,     0,     0,   802,
     803,   804,     0,   724,     0,   808,   425,     0,     0,     0,
     426,     0,  1361,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1644,   428,     0,
       0,     0,     0,     0,   429,  1651,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1658,   810,  1659,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
       0,     0,     0,     0,     0,     0,     0,  1672,  1673,   821,
     822,     0,     0,     0,   528,   707,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1692,  1693,
       0,     0,     0,  1695,     0,     0,     0,   457,  1698,  1700,
       0,   458,   459,     3,     0,   460,   461,   462,     0,   463,
       0,   464,   465,   466,   467,   468,     0,     0,     0,     0,
       0,   469,   470,   471,   472,   473,  1718,   474,     0,     0,
       0,     0,     0,     0,   475,   476,     0,     0,   477,  1725,
     478,   479,     0,     0,   480,     0,     8,   481,   482,     0,
     483,   484,     0,     0,   485,   486,     0,     0,     0,     0,
       0,   487,     0,     0,   488,   489,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   490,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   491,   492,
     493,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
     498,   499,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   500,   501,   502,
     503,   504,     0,   505,     0,   506,   507,   508,   509,     0,
     146,    13,   510,   511,   512,   513,   514,   515,   516,    63,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   520,
     147,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   523,     0,
      15,     0,     0,   524,   525,     0,     0,   458,   459,     0,
       0,   526,     0,   527,     0,   528,   529,   464,   465,   466,
     467,   468,     0,     0,     0,     0,     0,   469,     0,   471,
       0,     0,     0,   474,     0,   420,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,   479,     0,     0,
     480,     0,     0,   481,     0,     0,     0,   484,     0,     0,
       0,     0,   421,   422,     0,     0,     0,   591,     0,     0,
     488,   489,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   490,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   491,   492,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,     0,     0,     0,   423,     0,     0,     0,
     424,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   500,   501,   502,   503,   504,     0,   505,
     745,   506,   507,   508,   509,     0,     0,     0,   510,   511,
     512,   513,   514,   515,   516,   746,   593,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   425,     0,     0,     0,   426,
       0,     0,   952,   522,   523,     0,    15,     0,     0,   524,
     525,     0,     0,     0,   458,   459,     0,   953,     0,   962,
       0,   528,   529,   429,   464,   465,   466,   467,   468,     0,
       0,     0,     0,     0,   469,     0,   471,     0,     0,     0,
     474,     0,   624,     0,     0,     0,     0,     0,   476,     0,
       0,     0,     0,     0,   479,     0,     0,   480,     0,     0,
     481,     0,     0,     0,   484,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   591,     0,     0,   488,   489,     0,
     329,   330,   331,     0,   333,   334,   335,   336,   337,   490,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,     0,   351,   352,   353,     0,     0,   356,   357,   358,
     359,   491,   492,   592,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     500,   501,   502,   503,   504,     0,   505,     0,   506,   507,
     508,   509,     0,     0,     0,   510,   511,   512,   513,   514,
     515,   516,    63,   593,   518,   519,     0,     0,     0,     0,
       0,     0,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   625,     0,     0,   521,
     522,   523,     0,    15,     0,     0,   524,   525,     0,     0,
       0,   458,   459,     0,  1208,     0,   527,     0,   528,   529,
     627,   464,   465,   466,   467,   468,     0,     0,     0,     0,
       0,   469,     0,   471,     0,     0,     0,   474,     0,     0,
       0,     0,     0,     0,     0,   476,     0,     0,     0,     0,
       0,   479,     0,     0,   480,     0,     0,   481,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   591,     0,     0,   488,   489,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   490,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   491,   492,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,   672,     0,     0,     0,     0,     0,   497,
     498,   499,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   500,   501,   502,
     503,   504,     0,   505,     0,   506,   507,   508,   509,     0,
       0,     0,   510,   511,   512,   513,   514,   515,   516,    63,
     593,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   523,     0,
      15,     0,     0,   524,   525,     0,     0,   458,   459,     0,
       0,   526,     0,   527,     0,   528,   529,   464,   465,   466,
     467,   468,     0,     0,     0,     0,     0,   469,     0,   471,
       0,     0,     0,   474,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,   479,     0,     0,
     480,     0,     0,   481,     0,     0,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   591,     0,     0,
     488,   489,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   490,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   491,   492,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,   702,
       0,     0,     0,     0,     0,   497,   498,   499,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   500,   501,   502,   503,   504,     0,   505,
       0,   506,   507,   508,   509,     0,     0,     0,   510,   511,
     512,   513,   514,   515,   516,    63,   593,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    15,     0,     0,   524,
     525,     0,     0,   458,   459,     0,     0,   526,     0,   527,
       0,   528,   529,   464,   465,   466,   467,   468,     0,     0,
       0,     0,     0,   469,  1591,   471,   472,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,   476,     0,     0,
       0,     0,     0,   479,     0,     0,   480,     0,     0,   481,
     482,     0,     0,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   591,     0,     0,   488,   489,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   490,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     491,   492,   592,  1592,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   500,
     501,   502,   503,   504,     0,   505,     0,   506,   507,   508,
     509,     0,     0,     0,   510,   511,   512,   513,   514,   515,
     516,    63,   593,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    15,     0,     0,   524,   525,     0,     0,   458,
     459,     0,     0,   526,     0,   527,     0,   528,   529,   464,
     465,   466,   467,   468,     0,     0,     0,     0,     0,   469,
       0,   471,     0,     0,     0,   474,     0,     0,     0,     0,
       0,     0,     0,   476,     0,     0,     0,     0,     0,   479,
       0,     0,   480,     0,     0,   481,     0,     0,     0,   484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
       0,     0,   488,   489,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   490,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   491,   492,   493,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   497,   498,   499,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   500,   501,   502,   503,   504,
       0,   505,     0,   506,   507,   508,   509,     0,     0,     0,
     510,   511,   512,   513,   514,   515,   516,    63,   593,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   523,     0,    15,     0,
       0,   524,   525,     0,     0,   458,   459,     0,     0,   526,
       0,   527,     0,   528,   529,   464,   465,   466,   467,   468,
       0,     0,     0,     0,     0,   469,     0,   471,     0,     0,
       0,   474,     0,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,     0,     0,   479,     0,     0,   480,     0,
       0,   481,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   591,     0,     0,   488,   489,
    1002,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     490,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   491,   492,   592,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   500,   501,   502,   503,   504,     0,   505,   745,   506,
     507,   508,   509,     0,     0,     0,   510,   511,   512,   513,
     514,   515,   516,   746,   593,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,   523,     0,    15,     0,     0,   524,   525,     0,
       0,   458,   459,     0,     0,  1003,     0,   527,  1004,   528,
     529,   464,   465,   466,   467,   468,     0,     0,     0,     0,
       0,   469,     0,   471,     0,     0,   420,   474,     0,     0,
       0,     0,     0,     0,     0,   476,     0,     0,     0,     0,
       0,   479,     0,     0,   480,     0,     0,   481,     0,     0,
       0,   484,     0,   421,   422,     0,     0,     0,     0,     0,
       0,   591,     0,     0,   488,   489,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   490,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   491,   492,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,   496,     0,   423,     0,     0,
       0,   424,     0,     0,     0,     0,     0,     0,     0,  1023,
    1024,  1025,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   500,   501,   502,
     503,   504,     0,   505,     0,   506,   507,   508,   509,     0,
       0,     0,   510,   511,   512,   513,   514,   515,   516,    63,
     593,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,     0,     0,     0,     0,     0,   425,     0,     0,     0,
     426,     0,  1362,   427,     0,     0,   521,   522,   523,     0,
      15,     0,     0,   524,   525,     0,     0,     0,   428,   458,
     459,   526,     0,   527,   429,   528,   529,   740,     0,   464,
     465,   466,   467,   468,     0,     0,     0,     0,     0,   469,
       0,   471,     0,     0,   420,   474,     0,     0,     0,     0,
       0,     0,     0,   476,     0,     0,     0,     0,     0,   479,
       0,     0,   480,   741,     0,   481,     0,     0,     0,   484,
       0,   421,   422,     0,     0,     0,     0,     0,     0,   591,
       0,     0,   488,   489,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   490,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   491,   492,   592,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,   496,     0,   423,     0,     0,     0,   424,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   500,   501,   502,   503,   504,
       0,   505,     0,   506,   507,   508,   509,     0,     0,     0,
     510,   511,   512,   513,   514,   515,   516,    63,   593,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,     0,   425,     0,     0,     0,   426,     0,
    1363,   427,     0,     0,   521,   522,   523,     0,    15,     0,
       0,   524,   525,     0,     0,     0,   428,   458,   459,   526,
     628,   527,   429,   528,   529,   740,     0,   464,   465,   466,
     467,   468,     0,     0,     0,     0,     0,   469,     0,   471,
       0,     0,   420,   474,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,   479,     0,     0,
     480,   741,     0,   481,     0,     0,     0,   484,     0,   421,
     422,     0,     0,     0,     0,     0,     0,   591,     0,     0,
     488,   489,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   490,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   491,   492,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,   423,     0,     0,     0,   424,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   500,   501,   502,   503,   504,     0,   505,
     745,   506,   507,   508,   509,     0,     0,     0,   510,   511,
     512,   513,   514,   515,   516,   746,   593,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,   425,     0,     0,     0,   426,     0,  1364,   427,
       0,     0,   521,   522,   523,     0,    15,     0,     0,   524,
     525,     0,     0,     0,   428,   458,   459,   526,     0,   527,
     429,   528,   529,   740,     0,   464,   465,   466,   467,   468,
       0,     0,     0,     0,     0,   469,     0,   471,     0,     0,
     420,   474,     0,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,     0,     0,   479,     0,     0,   480,   741,
       0,   481,     0,     0,     0,   484,     0,   421,   422,     0,
       0,     0,     0,     0,     0,   591,     0,     0,   488,   489,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     490,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   491,   492,   592,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,   496,
       0,   423,     0,     0,     0,   424,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   500,   501,   502,   503,   504,     0,   505,     0,   506,
     507,   508,   509,     0,     0,     0,   510,   511,   512,   513,
     514,   515,   516,    63,   593,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,     0,     0,
     425,     0,     0,     0,   426,     0,  1366,   427,     0,     0,
     521,   522,   523,     0,    15,     0,     0,   524,   525,     0,
       0,     0,   428,   458,   459,   526,   864,   527,   429,   528,
     529,   740,     0,   464,   465,   466,   467,   468,     0,     0,
       0,     0,     0,   469,     0,   471,     0,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,   476,     0,     0,
       0,     0,     0,   479,     0,     0,   480,   741,     0,   481,
       0,     0,     0,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   591,     0,     0,   488,   489,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   490,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     491,   492,   592,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   500,
     501,   502,   503,   504,     0,   505,     0,   506,   507,   508,
     509,     0,     0,     0,   510,   511,   512,   513,   514,   515,
     516,    63,   593,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    15,     0,     0,   524,   525,     0,     0,   458,
     459,     0,     0,   526,     0,   527,     0,   528,   529,   464,
     465,   466,   467,   468,     0,     0,     0,     0,     0,   469,
       0,   471,     0,     0,     0,   474,     0,     0,     0,     0,
       0,     0,     0,   476,     0,     0,     0,     0,     0,   479,
       0,     0,   480,     0,     0,   481,     0,     0,     0,   484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
       0,     0,   488,   489,  1149,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   490,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   491,   492,   592,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   500,   501,   502,   503,   504,
       0,   505,   745,   506,   507,   508,   509,     0,     0,     0,
     510,   511,   512,   513,   514,   515,   516,   746,   593,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   523,     0,    15,     0,
       0,   524,   525,     0,     0,   458,   459,     0,     0,   526,
       0,   527,     0,   528,   529,   464,   465,   466,   467,   468,
       0,     0,     0,     0,     0,   469,     0,   471,     0,     0,
       0,   474,     0,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,     0,     0,   479,     0,     0,   480,     0,
       0,   481,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   591,     0,     0,   488,   489,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     490,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   491,   492,   592,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   500,   501,   502,   503,   504,     0,   505,   745,   506,
     507,   508,   509,     0,     0,     0,   510,   511,   512,   513,
     514,   515,   516,   746,   593,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,   523,     0,    15,     0,     0,   524,   525,     0,
       0,   458,   459,     0,     0,   526,     0,   527,  1210,   528,
     529,   464,   465,   466,   467,   468,     0,     0,     0,     0,
       0,   469,     0,   471,     0,     0,     0,   474,     0,     0,
       0,     0,     0,     0,     0,   476,     0,     0,     0,     0,
       0,   479,     0,     0,   480,     0,     0,   481,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   591,     0,     0,   488,   489,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   490,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   491,   492,
     592,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   500,   501,   502,
     503,   504,     0,   505,   745,   506,   507,   508,   509,     0,
       0,     0,   510,   511,   512,   513,   514,   515,   516,   746,
     593,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   523,     0,
      15,     0,     0,   524,   525,     0,     0,   458,   459,     0,
       0,   526,     0,   527,  1225,   528,   529,   464,   465,   466,
     467,   468,     0,     0,     0,     0,     0,   469,     0,   471,
       0,     0,     0,   474,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,   479,     0,     0,
     480,     0,     0,   481,     0,     0,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   591,     0,     0,
     488,   489,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   490,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   491,   492,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   500,   501,   502,   503,   504,     0,   505,
       0,   506,   507,   508,   509,     0,     0,     0,   510,   511,
     512,   513,   514,   515,   516,    63,   593,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    15,     0,     0,   524,
     525,     0,     0,   458,   459,     0,     0,   526,   628,   527,
       0,   528,   529,   464,   465,   466,   467,   468,     0,     0,
       0,     0,     0,   469,     0,   471,     0,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,   476,     0,     0,
       0,     0,     0,   479,     0,     0,   480,     0,     0,   481,
       0,     0,     0,   484,     0,     0,     0,     0,     0,   680,
       0,     0,     0,   591,     0,     0,   488,   489,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   490,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     491,   492,   592,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   500,
     501,   502,   503,   504,     0,   505,     0,   506,   507,   508,
     509,     0,     0,     0,   510,   511,   512,   513,   514,   515,
     516,    63,   593,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    15,     0,     0,   524,   525,     0,     0,   458,
     459,     0,     0,   526,     0,   527,     0,   528,   529,   464,
     465,   466,   467,   468,     0,     0,     0,     0,     0,   469,
       0,   471,     0,     0,     0,   474,     0,     0,     0,     0,
       0,     0,     0,   476,     0,     0,     0,     0,     0,   479,
       0,     0,   480,     0,     0,   481,     0,     0,     0,   484,
       0,     0,   691,     0,     0,     0,     0,     0,     0,   591,
       0,     0,   488,   489,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   490,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   491,   492,   592,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   500,   501,   502,   503,   504,
       0,   505,     0,   506,   507,   508,   509,     0,     0,     0,
     510,   511,   512,   513,   514,   515,   516,    63,   593,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   523,     0,    15,     0,
       0,   524,   525,     0,     0,     0,     0,   458,   459,   526,
       0,   527,     0,   528,   529,   723,     0,   464,   465,   466,
     467,   468,     0,     0,     0,     0,     0,   469,     0,   471,
       0,     0,     0,   474,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,   479,     0,     0,
     480,     0,     0,   481,     0,     0,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   591,     0,     0,
     488,   489,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   490,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   491,   492,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   500,   501,   502,   503,   504,     0,   505,
       0,   506,   507,   508,   509,     0,     0,     0,   510,   511,
     512,   513,   514,   515,   516,    63,   593,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    15,     0,     0,   524,
     525,     0,     0,   458,   459,     0,     0,   526,     0,   527,
       0,   528,   529,   464,   465,   466,   467,   468,     0,     0,
       0,     0,     0,   469,     0,   471,     0,     0,   420,   474,
       0,     0,     0,     0,     0,     0,     0,   476,     0,     0,
       0,     0,     0,   479,     0,     0,   480,     0,     0,   481,
       0,     0,     0,   484,     0,   421,   422,     0,     0,     0,
       0,     0,     0,   591,     0,     0,   488,   489,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   490,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     491,   492,   592,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   496,     0,   423,
       0,     0,     0,   424,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   500,
     501,   502,   503,   504,     0,   505,     0,   506,   507,   508,
     509,     0,     0,     0,   510,   511,   512,   513,   514,   515,
     516,    63,   593,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,   425,     0,
       0,     0,   426,     0,  1472,   427,   727,     0,   521,   522,
     523,     0,    15,     0,     0,   524,   525,     0,     0,     0,
     428,   458,   459,   526,     0,   527,   429,   528,   529,   731,
       0,   464,   465,   466,   467,   468,     0,     0,     0,     0,
       0,   469,     0,   471,     0,     0,     0,   474,     0,     0,
       0,     0,     0,     0,     0,   476,     0,     0,     0,     0,
       0,   479,     0,     0,   480,     0,     0,   481,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   591,     0,     0,   488,   489,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   490,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   491,   492,
     592,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   500,   501,   502,
     503,   504,     0,   505,     0,   506,   507,   508,   509,     0,
       0,     0,   510,   511,   512,   513,   514,   515,   516,    63,
     593,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   523,     0,
      15,     0,     0,   524,   525,     0,     0,   458,   459,     0,
       0,   526,     0,   527,     0,   528,   529,   464,   465,   466,
     467,   468,     0,     0,     0,     0,     0,   469,     0,   471,
       0,     0,     0,   474,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,   479,     0,     0,
     480,     0,     0,   481,     0,     0,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   591,     0,     0,
     488,   489,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   490,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   491,   492,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   500,   501,   502,   503,   504,     0,   505,
     745,   506,   507,   508,   509,     0,     0,     0,   510,   511,
     512,   513,   514,   515,   516,   746,   593,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    15,     0,     0,   524,
     525,     0,     0,   458,   459,     0,     0,   526,     0,   527,
       0,   528,   529,   464,   465,   466,   467,   468,     0,     0,
    1082,     0,     0,   469,     0,   471,     0,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,   476,     0,     0,
       0,     0,     0,   479,     0,     0,   480,     0,     0,   481,
       0,     0,     0,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   591,     0,     0,   488,   489,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   490,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     491,   492,   592,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   500,
     501,   502,   503,   504,     0,   505,     0,   506,   507,   508,
     509,     0,     0,     0,   510,   511,   512,   513,   514,   515,
     516,    63,   593,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    15,     0,     0,   524,   525,     0,     0,   458,
     459,     0,     0,   526,     0,   527,     0,   528,   529,   464,
     465,   466,   467,   468,     0,     0,     0,     0,     0,   469,
       0,   471,     0,     0,     0,   474,     0,     0,     0,     0,
       0,     0,     0,   476,     0,     0,     0,     0,     0,   479,
       0,     0,   480,     0,     0,   481,     0,     0,     0,   484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
       0,     0,   488,   489,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   490,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   491,   492,   592,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   500,   501,   502,   503,   504,
       0,   505,     0,   506,   507,   508,   509,     0,     0,     0,
     510,   511,   512,   513,   514,   515,   516,    63,   593,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   523,     0,    15,     0,
       0,   524,   525,     0,     0,   458,   459,     0,     0,   526,
       0,   527,  1103,   528,   529,   464,   465,   466,   467,   468,
       0,     0,     0,     0,     0,   469,     0,   471,     0,     0,
       0,   474,     0,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,     0,     0,   479,     0,     0,   480,     0,
       0,   481,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   591,     0,     0,   488,   489,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     490,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   491,   492,   592,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   500,   501,   502,   503,   504,     0,   505,     0,   506,
     507,   508,   509,     0,     0,     0,   510,   511,   512,   513,
     514,   515,   516,    63,   593,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1153,     0,
     521,   522,   523,     0,    15,     0,     0,   524,   525,     0,
       0,   458,   459,     0,     0,   526,     0,   527,     0,   528,
     529,   464,   465,   466,   467,   468,     0,     0,     0,     0,
       0,   469,     0,   471,     0,     0,     0,   474,     0,     0,
       0,     0,     0,     0,     0,   476,     0,     0,     0,     0,
       0,   479,     0,     0,   480,     0,     0,   481,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   591,     0,     0,   488,   489,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   490,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   491,   492,
     592,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   500,   501,   502,
     503,   504,     0,   505,     0,   506,   507,   508,   509,     0,
       0,     0,   510,   511,   512,   513,   514,   515,   516,    63,
     593,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   523,     0,
      15,     0,     0,   524,   525,     0,     0,   458,   459,     0,
       0,   526,     0,   527,  1431,   528,   529,   464,   465,   466,
     467,   468,     0,     0,     0,     0,     0,   469,     0,   471,
       0,     0,     0,   474,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,   479,     0,     0,
     480,     0,     0,   481,     0,     0,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   591,     0,     0,
     488,   489,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   490,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   491,   492,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   500,   501,   502,   503,   504,     0,   505,
       0,   506,   507,   508,   509,     0,     0,     0,   510,   511,
     512,   513,   514,   515,   516,    63,   593,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    15,     0,     0,   524,
     525,     0,     0,   458,   459,     0,     0,  1440,     0,   527,
    1441,   528,   529,   464,   465,   466,   467,   468,     0,     0,
       0,     0,     0,   469,     0,   471,     0,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,   476,     0,     0,
       0,     0,     0,   479,     0,     0,   480,     0,     0,   481,
       0,     0,     0,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   591,     0,     0,   488,   489,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   490,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     491,   492,   592,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   500,
     501,   502,   503,   504,     0,   505,     0,   506,   507,   508,
     509,     0,     0,     0,   510,   511,   512,   513,   514,   515,
     516,    63,   593,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    15,     0,     0,   524,   525,     0,     0,   458,
     459,     0,     0,   526,     0,   527,  1446,   528,   529,   464,
     465,   466,   467,   468,     0,     0,     0,     0,     0,   469,
       0,   471,     0,     0,     0,   474,     0,     0,     0,     0,
       0,     0,     0,   476,     0,     0,     0,     0,     0,   479,
       0,     0,   480,     0,     0,   481,     0,     0,     0,   484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
       0,     0,   488,   489,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   490,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   491,   492,   592,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   500,   501,   502,   503,   504,
       0,   505,     0,   506,   507,   508,   509,     0,     0,     0,
     510,   511,   512,   513,   514,   515,   516,    63,   593,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   523,     0,    15,     0,
       0,   524,   525,     0,     0,   458,   459,     0,     0,   526,
       0,   527,  1488,   528,   529,   464,   465,   466,   467,   468,
       0,     0,     0,     0,     0,   469,     0,   471,     0,     0,
       0,   474,     0,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,     0,     0,   479,     0,     0,   480,     0,
       0,   481,     0,     0,     0,   484,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   591,     0,     0,   488,   489,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     490,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   491,   492,   592,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   500,   501,   502,   503,   504,     0,   505,     0,   506,
     507,   508,   509,     0,     0,     0,   510,   511,   512,   513,
     514,   515,   516,    63,   593,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,   523,     0,    15,     0,     0,   524,   525,     0,
       0,   458,   459,     0,     0,   526,     0,   527,  1569,   528,
     529,   464,   465,   466,   467,   468,     0,     0,     0,     0,
       0,   469,     0,   471,     0,     0,     0,   474,     0,     0,
       0,     0,     0,     0,     0,   476,     0,     0,     0,     0,
       0,   479,     0,     0,   480,     0,     0,   481,     0,     0,
       0,   484,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   591,     0,     0,   488,   489,     0,   329,   330,   331,
       0,   333,   334,   335,   336,   337,   490,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,     0,   351,
     352,   353,     0,     0,   356,   357,   358,   359,   491,   492,
     592,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,  1606,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,   500,   501,   502,
     503,   504,     0,   505,     0,   506,   507,   508,   509,     0,
       0,     0,   510,   511,   512,   513,   514,   515,   516,    63,
     593,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   523,     0,
      15,     0,     0,   524,   525,     0,     0,   458,   459,     0,
       0,   526,     0,   527,     0,   528,   529,   464,   465,   466,
     467,   468,     0,     0,     0,     0,     0,   469,     0,   471,
       0,     0,     0,   474,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,   479,     0,     0,
     480,     0,     0,   481,     0,     0,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   591,     0,     0,
     488,   489,     0,   329,   330,   331,     0,   333,   334,   335,
     336,   337,   490,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,   351,   352,   353,     0,     0,
     356,   357,   358,   359,   491,   492,   592,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,  1649,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,   500,   501,   502,   503,   504,     0,   505,
       0,   506,   507,   508,   509,     0,     0,     0,   510,   511,
     512,   513,   514,   515,   516,    63,   593,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   523,     0,    15,     0,     0,   524,
     525,     0,     0,   458,   459,     0,     0,   526,     0,   527,
       0,   528,   529,   464,   465,   466,   467,   468,     0,     0,
       0,     0,     0,   469,     0,   471,     0,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,   476,     0,     0,
       0,     0,     0,   479,     0,     0,   480,     0,     0,   481,
       0,     0,     0,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   591,     0,     0,   488,   489,     0,   329,
     330,   331,     0,   333,   334,   335,   336,   337,   490,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,   351,   352,   353,     0,     0,   356,   357,   358,   359,
     491,   492,   592,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,   496,     0,     0,
       0,     0,     0,     0,     0,  1650,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   500,
     501,   502,   503,   504,     0,   505,     0,   506,   507,   508,
     509,     0,     0,     0,   510,   511,   512,   513,   514,   515,
     516,    63,   593,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     523,     0,    15,     0,     0,   524,   525,     0,     0,   458,
     459,     0,     0,   526,     0,   527,     0,   528,   529,   464,
     465,   466,   467,   468,     0,     0,     0,     0,     0,   469,
       0,   471,     0,     0,     0,   474,     0,     0,     0,     0,
       0,     0,     0,   476,     0,     0,     0,     0,     0,   479,
       0,     0,   480,     0,     0,   481,     0,     0,     0,   484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
       0,     0,   488,   489,     0,   329,   330,   331,     0,   333,
     334,   335,   336,   337,   490,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,   351,   352,   353,
       0,     0,   356,   357,   358,   359,   491,   492,   592,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,   500,   501,   502,   503,   504,
       0,   505,     0,   506,   507,   508,   509,     0,     0,     0,
     510,   511,   512,   513,   514,   515,   516,    63,   593,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   523,     0,    15,     0,
       0,   524,   525,     0,     0,   458,   459,     0,     0,   526,
       0,   527,     0,   528,   529,   464,   465,   466,   467,   468,
       0,     0,     0,     0,     0,   469,     0,   471,     0,     0,
     420,   474,     0,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,     0,     0,   479,     0,     0,   480,     0,
       0,   481,     0,     0,     0,   484,     0,   421,   422,     0,
       0,     0,     0,     0,     0,   591,     0,     0,   488,   489,
       0,   329,   330,   331,     0,   333,   334,   335,   336,   337,
     490,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   491,   492,   592,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   495,   496,
       0,   423,     0,   -76,     0,   424,     0,     0,     0,     0,
       0,     0,     0,     0,   770,   771,     0,     0,     0,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,   500,   501,   502,   503,   504,     0,   505,     0,   506,
     507,   508,   509,     0,     0,     0,   510,   511,   512,   513,
     514,   515,   516,    63,   593,   518,   519,     0,     0,     0,
       0,   770,   771,   520,     0,     0,     0,     0,     0,     0,
     425,     0,     0,     0,   426,     0,  1477,   427,     0,     0,
     521,   522,   523,     0,    15,     0,     0,   524,   525,     0,
       0,     0,   428,     0,     0,  1414,     0,   527,   429,   528,
     529,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,   786,   787,   788,   789,   790,
     791,   792,   793,   794,   795,     0,     0,     0,     0,   796,
     797,   798,   799,   800,   801,     0,     0,   802,   803,   804,
     805,   806,   807,   808,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   780,   781,   782,   783,
     784,     0,     0,   787,   788,   789,   790,     0,   792,   793,
     794,   795,     0,     0,     0,     0,   796,     0,   798,   799,
       0,     0,     0,     0,   802,   809,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   721,     0,
       0,     0,     0,     0,   323,     0,     0,   821,   822,     0,
     324,     0,   528,   707,     0,     0,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,     0,     0,     0,
       0,     0,     0,     0,   327,   811,   812,   813,   814,   815,
     816,   817,   818,   819,   820,     0,     0,     0,     0,   328,
       0,     0,     0,     0,   821,   822,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,     0,     0,   324,     0,     0,     0,     0,     0,   325,
       0,     0,     0,     0,     0,     0,     0,     0,    62,   326,
       0,     0,     0,     0,     0,     0,     0,   327,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   328,     0,     0,     0,     0,     0,    63,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,     0,   420,     0,     0,     0,
     323,     0,     0,     0,     0,     0,   324,     0,     0,   420,
       0,     0,   325,     0,     0,     0,     0,     0,     0,     0,
       0,    62,   326,   421,   422,     0,     0,     0,     0,     0,
     327,     0,     0,     0,   362,     0,   421,   422,     0,     0,
       0,     0,     0,     0,     0,   328,     0,     0,     0,     0,
       0,    63,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,     0,     0,   423,     0,     0,
       0,   424,     0,     0,     0,     0,     0,   363,     0,   631,
     423,     0,     0,     0,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,    13,     0,     0,   323,
       0,     0,     0,     0,   636,   324,   425,     0,     0,     0,
     426,   325,  1511,   427,     0,     0,    14,     0,     0,   425,
       0,   326,   637,   426,     0,  1608,   427,     0,   428,   327,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,   428,     0,     0,   328,     0,     0,   429,     0,     0,
     363,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   323,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,   325,     0,     0,     0,     0,     0,
       0,     0,     0,    62,   326,     0,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,   770,   771,   328,     0,     0,
       0,     0,     0,    63,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,     0,   770,   771,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   362,
     780,   781,   782,   783,   784,     0,     0,   787,   788,   789,
     790,     0,   792,   793,   794,   795,   636,     0,     0,     0,
     796,     0,   798,   799,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   902,   903,   904,   905,   906,
     907,   908,   909,   780,   781,   782,   783,   784,   910,   911,
     787,   788,   789,   790,   912,   792,   793,   794,   795,     0,
       0,     0,   363,   796,   797,   798,   799,   913,   914,   770,
     771,   802,   803,   804,   915,   916,   917,   808,     0,   811,
     812,   813,   814,   815,   816,   817,   818,   819,   820,     0,
       0,     0,    13,     0,     0,     0,     0,     0,   821,   822,
       0,     0,     0,     0,   770,   771,     0,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,   918,
     810,     0,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   821,   822,     0,     0,     0,   528,   707,     0,     0,
       0,     0,     0,     0,     0,     0,   902,   903,   904,   905,
     906,   907,   908,   909,   780,   781,   782,   783,   784,   910,
     911,   787,   788,   789,   790,   912,   792,   793,   794,   795,
    -389,     0,     0,     0,   796,   797,   798,   799,   913,   914,
       0,     0,   802,   803,   804,   915,   916,   917,   808,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,   770,   771,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,     0,     0,     0,     0,     0,     0,
     918,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   770,   771,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,     0,   528,   707,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   821,   822,     0,
       0,   835,     0,     0,     0,     0,     0,     0,     0,     0,
     780,   781,   782,   783,   784,     0,     0,   787,   788,   789,
     790,     0,   792,   793,   794,   795,     0,     0,     0,     0,
     796,     0,   798,   799,     0,     0,     0,     0,   802,   803,
     804,     0,     0,     0,   808,     0,     0,     0,   780,   781,
     782,   783,   784,     0,     0,   787,   788,   789,   790,     0,
     792,   793,   794,   795,   770,   771,     0,     0,   796,     0,
     798,   799,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   808,     0,     0,     0,     0,   810,     0,   811,
     812,   813,   814,   815,   816,   817,   818,   819,   820,   770,
     771,     0,     0,     0,     0,     0,     0,     0,   821,   822,
       0,     0,   845,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   810,     0,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   821,   822,     0,     0,
     861,     0,     0,     0,     0,     0,     0,     0,     0,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,     0,     0,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
     770,   771,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   770,   771,
       0,     0,     0,     0,     0,     0,     0,   821,   822,     0,
       0,   875,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,  1185,     0,     0,     0,
       0,     0,     0,     0,     0,   780,   781,   782,   783,   784,
       0,     0,   787,   788,   789,   790,     0,   792,   793,   794,
     795,     0,     0,     0,     0,   796,     0,   798,   799,     0,
       0,     0,     0,   802,   803,   804,     0,     0,     0,   808,
       0,     0,     0,   780,   781,   782,   783,   784,     0,     0,
     787,   788,   789,   790,     0,   792,   793,   794,   795,   770,
     771,     0,     0,   796,     0,   798,   799,     0,     0,     0,
       0,   802,   803,   804,     0,     0,     0,   808,     0,     0,
       0,     0,   810,     0,   811,   812,   813,   814,   815,   816,
     817,   818,   819,   820,   770,   771,     0,     0,     0,     0,
       0,     0,     0,   821,   822,     0,     0,  1189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     810,     0,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   821,   822,     0,     0,  1191,     0,     0,     0,     0,
       0,     0,     0,     0,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
       0,     0,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,   770,   771,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   770,   771,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,  1200,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   821,   822,     0,
       0,  1201,     0,     0,     0,     0,     0,     0,     0,     0,
     780,   781,   782,   783,   784,     0,     0,   787,   788,   789,
     790,     0,   792,   793,   794,   795,     0,     0,     0,     0,
     796,     0,   798,   799,     0,     0,     0,     0,   802,   803,
     804,     0,     0,     0,   808,     0,     0,     0,   780,   781,
     782,   783,   784,     0,     0,   787,   788,   789,   790,     0,
     792,   793,   794,   795,   770,   771,     0,     0,   796,     0,
     798,   799,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   808,     0,     0,     0,     0,   810,     0,   811,
     812,   813,   814,   815,   816,   817,   818,   819,   820,   770,
     771,     0,     0,     0,     0,     0,     0,     0,   821,   822,
       0,     0,  1202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   810,     0,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   821,   822,     0,     0,
    1203,     0,     0,     0,     0,     0,     0,     0,     0,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,     0,     0,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
     770,   771,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   770,   771,
       0,     0,     0,     0,     0,     0,     0,   821,   822,     0,
       0,  1204,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,  1205,     0,     0,     0,
       0,     0,     0,     0,     0,   780,   781,   782,   783,   784,
       0,     0,   787,   788,   789,   790,     0,   792,   793,   794,
     795,     0,     0,     0,     0,   796,     0,   798,   799,     0,
       0,     0,     0,   802,   803,   804,     0,     0,     0,   808,
       0,     0,     0,   780,   781,   782,   783,   784,     0,     0,
     787,   788,   789,   790,     0,   792,   793,   794,   795,   770,
     771,     0,     0,   796,     0,   798,   799,     0,     0,     0,
       0,   802,   803,   804,     0,     0,     0,   808,     0,     0,
       0,     0,   810,     0,   811,   812,   813,   814,   815,   816,
     817,   818,   819,   820,   770,   771,     0,     0,     0,     0,
       0,     0,     0,   821,   822,     0,     0,  1336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     810,     0,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   821,   822,     0,     0,  1348,     0,     0,     0,     0,
       0,     0,     0,     0,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
       0,     0,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,   770,   771,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   770,   771,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,  1351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   821,   822,     0,
       0,  1398,     0,     0,     0,     0,     0,     0,     0,     0,
     780,   781,   782,   783,   784,     0,     0,   787,   788,   789,
     790,     0,   792,   793,   794,   795,     0,     0,     0,     0,
     796,     0,   798,   799,     0,     0,     0,     0,   802,   803,
     804,     0,     0,     0,   808,     0,     0,     0,   780,   781,
     782,   783,   784,     0,     0,   787,   788,   789,   790,     0,
     792,   793,   794,   795,   770,   771,     0,     0,   796,     0,
     798,   799,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   808,     0,     0,     0,     0,   810,     0,   811,
     812,   813,   814,   815,   816,   817,   818,   819,   820,   770,
     771,     0,     0,     0,     0,     0,     0,     0,   821,   822,
       0,     0,  1509,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   810,     0,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   821,   822,     0,     0,
    1510,     0,     0,     0,     0,     0,     0,     0,     0,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,     0,     0,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
     770,   771,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   770,   771,
       0,     0,     0,     0,     0,     0,     0,   821,   822,     0,
       0,  1531,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,  1533,     0,     0,     0,
       0,     0,     0,     0,     0,   780,   781,   782,   783,   784,
       0,     0,   787,   788,   789,   790,     0,   792,   793,   794,
     795,     0,     0,     0,     0,   796,     0,   798,   799,     0,
       0,     0,     0,   802,   803,   804,     0,     0,     0,   808,
       0,     0,     0,   780,   781,   782,   783,   784,     0,     0,
     787,   788,   789,   790,     0,   792,   793,   794,   795,   770,
     771,     0,     0,   796,     0,   798,   799,     0,     0,     0,
       0,   802,   803,   804,     0,     0,     0,   808,     0,     0,
       0,     0,   810,     0,   811,   812,   813,   814,   815,   816,
     817,   818,   819,   820,   770,   771,     0,     0,     0,     0,
       0,     0,     0,   821,   822,     0,     0,  1535,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     810,     0,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   821,   822,     0,     0,  1539,     0,     0,     0,     0,
       0,     0,     0,     0,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
       0,     0,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,   770,   771,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   770,   771,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,  1587,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   821,   822,     0,
       0,  1611,     0,     0,     0,     0,     0,     0,     0,     0,
     780,   781,   782,   783,   784,     0,     0,   787,   788,   789,
     790,     0,   792,   793,   794,   795,     0,     0,     0,     0,
     796,     0,   798,   799,     0,     0,     0,     0,   802,   803,
     804,     0,     0,     0,   808,     0,     0,     0,   780,   781,
     782,   783,   784,     0,     0,   787,   788,   789,   790,     0,
     792,   793,   794,   795,   770,   771,     0,     0,   796,     0,
     798,   799,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   808,     0,     0,     0,     0,   810,     0,   811,
     812,   813,   814,   815,   816,   817,   818,   819,   820,   770,
     771,     0,     0,     0,     0,     0,     0,     0,   821,   822,
       0,     0,  1612,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   810,     0,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   821,   822,     0,     0,
    1614,     0,     0,     0,     0,     0,     0,     0,     0,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,     0,     0,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
     770,   771,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   770,   771,
       0,     0,     0,     0,     0,     0,     0,   821,   822,     0,
       0,  1623,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,  1626,     0,     0,     0,
       0,     0,     0,     0,     0,   780,   781,   782,   783,   784,
       0,     0,   787,   788,   789,   790,     0,   792,   793,   794,
     795,     0,     0,     0,     0,   796,     0,   798,   799,     0,
       0,     0,     0,   802,   803,   804,     0,     0,     0,   808,
       0,     0,     0,   780,   781,   782,   783,   784,     0,     0,
     787,   788,   789,   790,     0,   792,   793,   794,   795,   770,
     771,     0,     0,   796,     0,   798,   799,     0,     0,     0,
       0,   802,   803,   804,     0,     0,     0,   808,     0,     0,
       0,     0,   810,     0,   811,   812,   813,   814,   815,   816,
     817,   818,   819,   820,   770,   771,     0,     0,     0,     0,
       0,     0,     0,   821,   822,     0,     0,  1635,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     810,     0,   811,   812,   813,   814,   815,   816,   817,   818,
     819,   820,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   821,   822,     0,     0,  1710,     0,     0,     0,     0,
       0,     0,     0,     0,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
       0,     0,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,   770,   771,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   770,   771,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,  1711,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   821,   822,   865,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     780,   781,   782,   783,   784,     0,     0,   787,   788,   789,
     790,     0,   792,   793,   794,   795,     0,     0,     0,     0,
     796,     0,   798,   799,     0,     0,     0,     0,   802,   803,
     804,     0,     0,     0,   808,     0,     0,     0,   780,   781,
     782,   783,   784,     0,     0,   787,   788,   789,   790,     0,
     792,   793,   794,   795,   770,   771,     0,     0,   796,     0,
     798,   799,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   808,     0,     0,     0,     0,   810,     0,   811,
     812,   813,   814,   815,   816,   817,   818,   819,   820,   770,
     771,     0,     0,     0,     0,     0,     0,     0,   821,   822,
    1137,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   810,     0,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   821,   822,  1287,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   780,
     781,   782,   783,   784,     0,     0,   787,   788,   789,   790,
       0,   792,   793,   794,   795,     0,     0,     0,     0,   796,
       0,   798,   799,     0,     0,     0,     0,   802,   803,   804,
       0,     0,     0,   808,   780,   781,   782,   783,   784,     0,
       0,   787,   788,   789,   790,     0,   792,   793,   794,   795,
     770,   771,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,     0,
       0,     0,     0,     0,     0,     0,   810,     0,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   821,   822,  1303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,  1438,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   780,   781,   782,   783,   784,
       0,     0,   787,   788,   789,   790,     0,   792,   793,   794,
     795,   242,   243,     0,     0,   796,     0,   798,   799,     0,
       0,     0,     0,   802,   803,   804,     0,     0,   244,   808,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   770,   771,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   810,     0,   811,   812,   813,   814,   815,   816,
     817,   818,   819,   820,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   821,   822,  1444,     0,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,     0,     0,   263,   264,
     265,     0,     0,     0,     0,     0,     0,   266,   267,   268,
     269,   270,     0,     0,   271,   272,   273,   274,   275,   276,
     277,     0,     0,     0,     0,     0,   770,   771,   780,   781,
     782,   783,   784,     0,     0,   787,   788,   789,   790,     0,
     792,   793,   794,   795,     0,     0,     0,     0,   796,     0,
     798,   799,     0,     0,     0,     0,   802,   803,   804,     0,
       0,     0,   808,   278,     0,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,    50,     0,   289,   290,     0,
       0,     0,     0,     0,   291,   292,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   810,     0,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,     0,     0,   770,
     771,   780,   781,   782,   783,   784,   821,   822,   787,   788,
     789,   790,     0,   792,   793,   794,   795,     0,     0,     0,
       0,   796,     0,   798,   799,     0,     0,     0,     0,   802,
     803,   804,     0,     0,     0,   808,   770,   771,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,   810,     0,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
       0,     0,     0,     0,   780,   781,   782,   783,   784,   821,
     822,   787,   788,   789,   790,     0,   792,   793,   794,   795,
       0,     0,     0,     0,   796,     0,   798,   799,     0,     0,
     966,     0,   802,   803,   804,     0,     0,     0,   808,   770,
     771,   780,   781,   782,   783,   784,     0,     0,   787,   788,
     789,   790,     0,   792,   793,   794,   795,     0,     0,     0,
       0,   796,     0,   798,   799,     0,     0,  1211,     0,   802,
     803,   804,     0,     0,     0,   808,   770,   771,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,     0,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
       0,     0,     0,     0,   780,   781,   782,   783,   784,   821,
     822,   787,   788,   789,   790,     0,   792,   793,   794,   795,
       0,     0,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,   770,
     771,   780,   781,   782,   783,   784,     0,     0,   787,   788,
     789,   790,     0,   792,   793,   794,   795,     0,     0,     0,
       0,   796,     0,   798,   799,     0,     0,     0,     0,   802,
     803,   804,     0,     0,     0,   808,   770,   771,     0,     0,
       0,   810,  1292,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,  1393,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
       0,     0,     0,     0,   780,   781,   782,   783,   784,   821,
     822,   787,   788,   789,   790,     0,   792,   793,   794,   795,
       0,     0,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,     0,     0,   808,   770,
     771,   780,   781,   782,   783,   784,     0,     0,   787,   788,
     789,   790,     0,   792,   793,   794,   795,     0,     0,     0,
       0,   796,     0,   798,   799,     0,     0,     0,     0,   802,
     803,   804,     0,  1636,     0,   808,   770,   771,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   821,   822,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   810,     0,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
       0,     0,   770,   771,   780,   781,   782,   783,   784,   821,
     822,   787,   788,   789,   790,     0,   792,   793,   794,   795,
       0,     0,     0,     0,   796,     0,   798,   799,     0,     0,
       0,     0,   802,   803,   804,     0,   770,   771,  -796,     0,
       0,   780,   781,   782,   783,   784,     0,     0,   787,   788,
     789,   790,     0,   792,   793,   794,   795,     0,     0,     0,
       0,   796,     0,   798,   799,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   810,     0,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,     0,     0,     0,     0,   780,   781,   782,
     783,   784,   821,   822,   787,   788,   789,   790,     0,   792,
     793,   794,   795,     0,     0,     0,     0,   796,     0,   798,
     799,   812,   813,   814,   815,   816,   817,   818,   819,   820,
       0,   780,   781,   782,   783,   784,     0,     0,   787,   821,
     822,   790,     0,   792,   793,   794,   795,     0,     0,     0,
       0,   796,     0,   798,   799,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   813,   814,
     815,   816,   817,   818,   819,   820,  1015,     0,     0,     0,
       0,     0,     0,     0,     0,   821,   822,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   814,   815,   816,   817,   818,   819,   820,
    1019,     0,     0,     0,     0,     0,     0,     0,     0,   821,
     822,     0,     0,     0,     0,     0,     0,     0,   329,   330,
     331,     0,   333,   334,   335,   336,   337,   490,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,     0,
     351,   352,   353,     0,     0,   356,   357,   358,   359,     0,
       0,     0,   329,   330,   331,     0,   333,   334,   335,   336,
     337,   490,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,     0,   351,   352,   353,     0,     0,   356,
     357,   358,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1288,
       0,     0,     0,     0,     0,  1016,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1017,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1020,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   329,   330,   331,  1021,   333,   334,   335,   336,   337,
     490,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,     0,   351,   352,   353,     0,     0,   356,   357,
     358,   359,   329,   330,   331,     0,   333,   334,   335,   336,
     337,   490,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,     0,   351,   352,   353,     0,   192,   356,
     357,   358,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1289,     0,
    1094,  1095,     0,     0,   193,     0,   194,     0,   195,   196,
     197,   198,   199,  1290,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,     0,   211,   212,   213,  1096,
       0,   214,   215,   216,   217,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1097,     0,     0,     0,     0,     0,
       0,   218,   219,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1098,  1099,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   220
};

static const yytype_int16 yycheck[] =
{
       1,   679,   598,   222,   389,   547,   509,   470,     7,   743,
     621,   652,   389,   747,   748,   434,   171,   433,    19,    15,
      16,   759,   228,   564,   943,   722,  1214,   396,    86,   659,
     936,   494,    53,   470,   938,    33,     5,     8,    33,   957,
      22,    20,   852,   238,   854,   963,   856,   588,    20,   143,
      15,    16,    20,    20,   612,   402,   614,   109,   616,     4,
      20,   389,    20,    53,    19,    20,   498,   499,  1417,   144,
     145,   146,    68,    69,    70,    57,    20,    34,   143,   173,
       5,     6,   155,     8,   431,   470,   166,   160,   127,   719,
     527,   721,     7,   723,   173,    96,   126,    98,    62,   129,
     130,   731,   155,   433,   434,   173,    63,   160,   127,   494,
     740,    36,   129,   130,   110,   111,   112,   113,   137,    53,
     720,   165,   722,   217,   238,     0,   192,    15,    16,   194,
     129,   158,     7,   102,   103,    50,   160,   158,    57,   602,
     219,   185,   470,   218,    63,   218,   173,   220,   149,   217,
     107,   889,   173,   192,   173,    30,   195,    32,   313,    34,
     507,   508,    46,    97,   898,    40,   494,   220,   709,   168,
     173,  1520,   230,   192,    57,    50,   220,   229,   197,   136,
      63,    56,   107,   389,   214,   215,   143,  1375,   187,   188,
     189,   218,   387,   164,   165,   185,   220,   214,   215,   197,
     934,   166,   197,   937,   191,    80,   171,   402,   173,   672,
     158,   176,   191,   184,   185,   160,   173,   602,   173,   191,
     215,   203,   106,   191,   191,   173,   221,   102,   103,   198,
     231,   191,   214,   191,   671,   672,   431,   127,   433,   702,
      21,    22,   238,  1615,   215,   164,   163,   191,   217,   191,
     191,   208,   217,  1159,   473,   152,  1160,   158,   316,   696,
     217,   127,   699,   160,  1002,   184,   183,  1588,   487,   217,
     127,   137,   173,   387,   602,   216,   173,   143,   166,  1198,
     137,   143,   195,   171,   194,   173,   199,   672,   176,  1575,
     165,  1577,  1210,  1665,   217,   127,  1582,  1583,   165,   163,
     217,   191,   192,  1491,   194,   137,   127,   197,   307,   639,
     185,   310,   507,   508,   644,   191,   137,   702,   185,   183,
     217,   163,   759,   132,   127,   142,   192,  1648,   191,   204,
     194,   678,   199,   127,   137,   192,   768,   118,   119,  1258,
     215,   183,   218,   137,   672,   126,   163,   128,   129,   130,
     131,   198,   982,   216,   701,   136,   165,   195,  1644,   165,
     192,   199,  1622,   173,   174,   175,   183,   363,   166,   143,
     217,   192,   143,  1633,   702,   838,   185,   219,   220,   185,
     586,   152,   588,   384,   385,   191,   157,   824,   389,   192,
     391,   387,   191,   199,   204,  1314,   392,   766,   192,   173,
     188,   189,   173,  1003,   165,   152,   402,   185,   603,   604,
     605,  1149,   607,   608,  1674,  1675,   611,   214,   613,   218,
     615,   199,   617,   163,   185,   173,   173,   198,   216,   635,
     191,   432,   143,   214,   215,   431,   642,   433,   199,   658,
     646,   152,   191,   183,   772,   773,   774,   775,   776,   777,
     778,   779,   889,   838,  1482,  1013,  1374,   785,   786,   191,
    1001,   173,   173,   791,   683,   968,  1384,  1277,   191,   218,
     466,   467,   800,   801,     5,     6,   979,   805,   806,   807,
     163,   809,   483,   678,   173,   185,   218,   834,   188,   603,
     604,   605,   173,   607,   608,   218,   191,   611,   165,   613,
     183,   615,  1143,    68,    69,    70,   701,   165,   191,   728,
     838,   507,   508,  1210,   844,   165,   216,   954,   185,   951,
      57,  1549,  1550,   218,   191,   962,    63,   185,  1225,   173,
     860,   532,   533,   191,   753,   185,   165,   884,   885,  1567,
    1568,   199,   192,   165,   191,   110,   111,   112,   113,   550,
     897,   552,   553,   554,   191,   556,   185,   191,   191,   158,
     173,  1371,   191,   185,   191,   177,   567,    57,   173,   191,
     199,   218,    57,    63,   173,   770,   771,   199,   165,   926,
     927,   218,   929,   584,   218,   218,   933,   588,   935,   216,
     920,  1023,  1024,  1025,   165,  1405,   165,   598,   185,  1627,
    1628,   796,   191,   165,   191,  1246,   191,   603,   604,   605,
    1210,   607,   608,  1213,   185,   611,   185,   613,   165,   615,
     191,   617,   191,   185,   191,  1225,   821,   216,   199,   191,
     199,   165,   633,   218,    33,   165,   191,   199,   185,   834,
     165,  1199,   191,   152,   191,   144,   191,   191,   144,   216,
     146,   185,   199,     5,     6,   185,   191,   191,   219,   220,
     185,    60,    61,   218,   173,   199,   191,   862,   664,   218,
     195,   216,   216,    25,   869,    21,    22,   872,   833,    31,
    1358,   216,   678,   165,   184,   185,   881,   187,   164,   884,
     885,   173,  1250,   888,   177,   971,   194,   973,   893,   194,
     919,   199,   897,   185,   199,   701,  1516,   194,   184,   191,
    1147,   106,   199,   194,  1133,  1131,    68,    69,   199,   217,
     198,   192,   217,   164,   165,   124,    47,   165,    35,   128,
     217,   926,   927,   195,   929,   173,   217,   199,   933,   217,
     935,   936,    35,   184,   165,   158,    67,   185,   862,   198,
     102,   103,   173,    57,   750,   869,   755,  1398,   872,    63,
     173,    33,   198,   198,   185,   217,   195,   881,   217,  1206,
     199,   158,   118,   119,   888,   173,   971,   158,   973,   893,
     126,   217,   217,   129,   130,   131,   173,  1224,    60,    61,
     136,   143,   173,  1230,   193,    12,   173,    57,   197,    57,
    1237,   200,  1239,    63,  1134,    63,    23,    24,   160,   161,
     162,  1158,   191,    57,  1414,   194,   215,   184,   197,    63,
     187,   173,   221,   190,   825,   216,   185,    22,  1370,   185,
     195,   183,   191,   829,   199,   191,  1183,   185,   834,   840,
    1440,   177,   178,   191,   185,   185,   198,  1458,  1285,   850,
     191,   191,   124,  1411,   173,   185,   128,   971,   195,   973,
    1297,   191,   199,   215,   217,  1302,   862,  1197,   214,   215,
      92,    93,  1257,   869,    21,    22,   872,   195,   173,   173,
    1257,   199,  1560,    33,   216,   881,   195,  1082,   884,   885,
     199,   194,   888,   165,   197,    43,   195,   893,  1632,  1105,
     199,   897,  1249,   195,  1234,  1235,  1640,   199,   195,   194,
      60,    61,   199,   185,   177,   178,   179,   180,   194,   191,
     217,   193,   219,   220,  1119,   197,  1522,   199,   200,  1257,
     926,   927,   198,   929,  1668,  1372,  1131,   933,   194,   935,
     936,   197,   217,   215,   198,   198,  1587,  1558,   198,   221,
     198,   108,   109,   110,   111,   112,   113,   114,   115,   198,
    1397,   198,   217,  1158,  1159,   497,   498,   499,  1579,    33,
     198,   118,   119,   198,   124,   971,   217,   973,   128,   126,
     137,   128,   129,   130,   131,   198,    21,    22,  1183,   136,
     147,   148,   149,  1496,   198,   195,    60,    61,  1501,   144,
     145,   146,   173,   174,   175,  1119,  1602,  1603,   177,   178,
     179,   173,    75,   218,   217,   547,    79,  1347,   177,   178,
     179,  1621,  1622,   216,   173,  1355,   184,   185,   186,  1629,
      93,    94,  1251,  1633,  1634,    98,    99,   100,   101,    10,
      11,    12,   574,   193,   452,   453,   454,   197,   173,   199,
     200,  1257,   173,   217,  1249,   202,   203,   204,  1654,   215,
     124,   198,   198,   127,   128,   215,  1666,   214,   215,   198,
     173,   221,   198,   137,  1674,  1675,   198,   217,   198,   166,
    1427,   116,   117,   118,   119,   198,   198,   198,   198,   217,
     217,   126,    10,   128,   129,   130,   131,   220,   217,   217,
     217,   136,   217,   138,   139,  1106,    37,    66,   215,  1110,
     198,   217,   217,  1114,   198,    43,   218,   173,   173,    33,
     216,   194,  1341,  1119,   217,   217,    43,  1564,   192,   193,
    1730,   198,   198,   197,   191,  1131,   200,   198,   132,    14,
    1335,  1604,    21,    22,   676,   194,    60,    61,   192,   166,
     220,   215,   184,   191,  1591,    13,   216,   221,   191,   217,
     191,   173,  1158,  1159,   218,   200,   201,   202,   203,   204,
       8,   173,   704,   199,  1521,   218,   173,   173,   173,   214,
     215,   218,   191,   191,   184,   173,     1,  1183,  1407,   218,
     217,   198,  1655,  1656,   217,    21,    22,   218,   217,   217,
     198,   217,   217,   217,   217,    67,   217,  1426,   173,   199,
     124,   217,  1649,   199,   128,   199,    43,   218,   173,  1604,
     173,  1335,   173,   173,   173,   218,   218,   218,   217,   198,
     173,   218,  1427,  1696,  1429,   217,   768,   116,   117,   118,
     119,   120,   216,   216,   123,   124,   125,   126,   216,   128,
     129,   130,   131,  1249,  1601,   217,  1257,   136,   218,   138,
     139,   217,   173,   217,   216,   144,   145,   146,  1487,   217,
    1655,  1656,   217,   217,   181,   217,  1604,   218,   218,   193,
     173,   216,   173,   197,   216,   199,   200,    33,   217,   173,
     116,   117,   118,   119,   120,   217,   217,   123,   124,   125,
     126,   215,   128,   129,   130,   131,  1307,   221,   840,   199,
     136,  1696,   138,   139,   193,  1429,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,  1521,  1655,  1656,   217,
      70,   218,   217,   199,   217,   214,   215,   217,   199,  1335,
     217,  1536,   389,   217,   185,   218,   218,   217,   217,   217,
     217,    53,   399,   177,   218,   218,   184,   191,   218,   218,
     216,   408,   184,   184,   216,   218,   191,   218,  1696,   218,
     218,   418,   198,   199,   200,   201,   202,   203,   204,   218,
    1599,   428,   218,   216,   216,   184,   216,   218,   214,   215,
      85,   730,     1,    46,   217,    88,   141,  1513,   218,   218,
    1000,   448,  1514,   829,  1514,  1514,  1601,   236,  1409,   941,
    1514,  1514,     1,   460,    33,  1008,  1487,  1424,  1269,   951,
    1546,  1450,  1536,   470,  1310,  1453,  1547,    33,   475,  1547,
     477,  1427,    56,  1429,  1634,  1494,   466,   583,   466,   486,
    1220,    60,    61,   722,    -1,    -1,    -1,   494,   495,   496,
      -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,   509,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     517,    -1,    -1,   520,   521,   522,   523,   524,   525,    -1,
      -1,    -1,    -1,    -1,    -1,  1704,    -1,    -1,   535,    -1,
      -1,  1023,  1024,  1025,    -1,    -1,  1028,    -1,  1030,    -1,
    1032,    -1,  1034,    -1,  1036,   124,  1038,    -1,  1040,   128,
    1042,    -1,    -1,    -1,    -1,  1047,    -1,  1049,   124,    -1,
      -1,   127,   128,  1055,    -1,  1521,    -1,    -1,    -1,    21,
      22,   137,    -1,    -1,    -1,  1067,    -1,  1069,    -1,    -1,
    1536,    -1,  1074,    -1,  1076,    -1,  1078,    -1,    -1,  1081,
      -1,    -1,    -1,    -1,    -1,   602,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,   185,
     199,   200,  1114,   630,    -1,    -1,   192,   193,    -1,    -1,
      -1,   197,  1588,    -1,   200,    -1,   215,    -1,    -1,    -1,
      -1,    -1,   221,    -1,    -1,  1601,    -1,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   672,   118,   119,    -1,    -1,
      -1,    -1,   679,   680,   126,   682,   128,   129,   130,   131,
     687,    -1,    -1,    -1,   136,    -1,   693,    -1,    -1,    -1,
      -1,    -1,  1648,    -1,    -1,   702,    -1,  1658,    -1,    -1,
      -1,    -1,    -1,  1664,    -1,    -1,   713,   714,   715,   716,
     717,   718,    -1,   720,    -1,   722,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,  1698,    -1,  1700,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,  1725,   772,   773,   774,   775,   776,
     777,   778,   779,   780,   781,    -1,    -1,   784,   785,   786,
     787,   788,   789,    -1,   791,   792,    -1,   794,   795,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,   806,
     807,   808,   809,   810,   811,   812,   813,   814,   815,   816,
     817,   818,   819,   820,    -1,   822,    -1,    -1,    -1,   826,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   838,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,   876,
     150,    -1,    -1,    -1,    -1,    -1,   883,    -1,  1370,    -1,
      -1,    -1,    -1,   890,    -1,   892,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   901,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,   914,   915,   916,
     917,   918,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   952,   953,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,   966,
      -1,   968,    -1,    -1,    -1,    -1,    -1,    -1,   975,    -1,
      -1,    -1,   979,    -1,    -1,    -1,    -1,   984,    -1,   986,
     987,   988,    -1,    -1,    -1,     5,     6,    -1,    -1,    -1,
      -1,    -1,   999,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    31,    -1,    33,    -1,    -1,    -1,    -1,   124,    39,
      -1,    -1,   128,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      60,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,   193,  1095,    -1,
      -1,   197,  1099,   199,   200,    -1,    -1,    -1,   118,   119,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   128,   215,
      -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,  1658,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,  1186,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,   215,    -1,   217,    -1,   219,
     220,   221,    -1,    -1,  1211,    -1,  1698,    -1,  1700,    -1,
      -1,    -1,  1219,  1220,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1725,    -1,    -1,    -1,    13,    21,    22,
    1247,  1248,    -1,    19,    -1,    -1,    -1,  1254,    -1,    25,
    1257,    -1,    -1,    -1,    -1,    31,  1263,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,  1279,    49,  1281,    -1,    -1,    -1,    -1,  1286,
      -1,    -1,    -1,    -1,    -1,  1292,    -1,    -1,    64,  1296,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,  1338,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    33,   128,   129,   130,   131,    -1,
      -1,  1358,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,    -1,   146,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    60,    61,   773,   774,   775,   776,   777,   778,   779,
     156,    -1,    -1,    -1,    -1,   785,   786,    -1,    -1,    -1,
      -1,   791,  1399,  1400,  1401,    -1,    33,   173,    -1,    -1,
     800,   801,    -1,    -1,    -1,   805,   806,   807,    -1,   809,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    60,    61,    -1,    21,    22,  1435,    -1,
    1437,   214,   215,    -1,    -1,   124,  1443,    -1,    -1,   128,
      -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1467,    -1,    -1,  1470,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1478,  1479,  1480,    -1,    -1,    -1,    -1,  1485,    -1,
      -1,    -1,    -1,  1490,    -1,    -1,  1493,   124,    -1,  1496,
    1497,   128,    -1,    -1,  1501,  1502,    -1,  1504,  1505,    -1,
      -1,    -1,    -1,    -1,   193,    -1,    -1,  1514,   197,    -1,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1527,   116,   117,   118,   119,   120,   215,    -1,   123,   124,
     125,   126,   221,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,  1560,    -1,   150,   193,    -1,    -1,    -1,
     197,    -1,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1584,   215,    -1,
      -1,    -1,    -1,    -1,   221,  1592,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1604,   193,  1606,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1624,  1625,   214,
     215,    -1,    -1,    -1,   219,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1645,  1646,
      -1,    -1,    -1,  1650,    -1,    -1,    -1,     1,  1655,  1656,
      -1,     5,     6,     7,    -1,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,  1683,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    42,  1696,
      44,    45,    -1,    -1,    48,    -1,    50,    51,    52,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,    -1,
      -1,   215,    -1,   217,    -1,   219,   220,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
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
     158,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,    -1,     5,     6,    -1,   215,    -1,   217,
      -1,   219,   220,   221,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
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
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,     5,     6,    -1,   215,    -1,   217,    -1,   219,   220,
     221,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
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
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
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
      -1,     5,     6,    -1,    -1,   215,    -1,   217,   218,   219,
     220,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    33,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
      -1,    -1,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,
     197,    -1,   199,   200,    -1,    -1,   200,   201,   202,    -1,
     204,    -1,    -1,   207,   208,    -1,    -1,    -1,   215,     5,
       6,   215,    -1,   217,   221,   219,   220,    13,    -1,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    33,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
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
     199,   200,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
      -1,   207,   208,    -1,    -1,    -1,   215,     5,     6,   215,
     216,   217,   221,   219,   220,    13,    -1,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,
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
     158,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
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
      -1,    -1,    68,    69,    70,    71,    72,    73,    -1,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,    -1,    -1,    -1,
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
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
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
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,   216,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,
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
      -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,
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
      -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,
     208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,
      -1,   219,   220,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,
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
     152,   153,   154,   155,    -1,   157,    -1,   159,   160,   161,
     162,    -1,    -1,    -1,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
      -1,    -1,   197,    -1,   199,   200,   198,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,
     215,     5,     6,   215,    -1,   217,   221,   219,   220,    13,
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
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
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
     158,   159,   160,   161,   162,    -1,    -1,    -1,   166,   167,
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
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     154,   155,    -1,   157,    -1,   159,   160,   161,   162,    -1,
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
      33,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
      -1,   124,    -1,    10,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    21,    22,   183,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,
     200,   201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,
      -1,    -1,   215,    -1,    -1,   215,    -1,   217,   221,   219,
     220,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,
     137,   138,   139,   140,   141,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   192,   193,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   214,   215,    -1,
      25,    -1,   219,   220,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,   214,   215,    71,    72,    73,    74,
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
      -1,    -1,    -1,    -1,   219,    -1,    33,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    33,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    41,    60,    61,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,   156,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   173,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,   219,    -1,   221,
     124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    19,
      -1,    -1,    -1,    -1,   173,    25,   193,    -1,    -1,    -1,
     197,    31,   199,   200,    -1,    -1,   185,    -1,    -1,   193,
      -1,    41,   191,   197,    -1,   199,   200,    -1,   215,    49,
      -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,    -1,
      -1,   215,    -1,    -1,    64,    -1,    -1,   221,    -1,    -1,
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
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
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
     145,   146,    -1,    -1,    -1,   150,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,   214,
     215,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
     142,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    21,
      22,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,   142,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    21,    22,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,   214,
     215,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    21,
      22,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    21,    22,    -1,    -1,
      -1,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,   214,
     215,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,    21,
      22,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,   185,    -1,   150,    21,    22,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   214,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    21,    22,   116,   117,   118,   119,   120,   214,
     215,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,
      -1,    -1,   144,   145,   146,    -1,    21,    22,   150,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,   214,   215,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   214,
     215,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   198,
     199,   200,   201,   202,   203,   204,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,   199,   200,   201,   202,   203,   204,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,    -1,
      -1,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     433,   195,   427,   380,   383,   173,   108,   109,   110,   111,
     112,   113,   114,   115,   137,   147,   148,   149,   217,     7,
      50,   314,   251,   191,   251,   218,   462,     1,     5,     6,
       9,    10,    11,    13,    15,    16,    17,    18,    19,    25,
      26,    27,    28,    29,    31,    38,    39,    42,    44,    45,
      48,    51,    52,    54,    55,    58,    59,    65,    68,    69,
      80,   102,   103,   104,   105,   118,   119,   133,   134,   135,
     151,   152,   153,   154,   155,   157,   159,   160,   161,   162,
     166,   167,   168,   169,   170,   171,   172,   174,   175,   176,
     183,   200,   201,   202,   207,   208,   215,   217,   219,   220,
     234,   236,   247,   248,   251,   252,   255,   256,   258,   260,
     261,   262,   263,   285,   287,   288,   290,   295,   300,   301,
     302,   306,   307,   308,   309,   310,   311,   312,   313,   315,
     319,   320,   327,   330,   333,   338,   341,   342,   344,   345,
     346,   348,   353,   356,   357,   364,   422,   478,   493,   504,
     508,   521,   524,   410,   173,   251,   409,   301,   371,   391,
     216,    65,   104,   174,   295,   357,   173,   173,   434,   127,
     137,   192,   388,   435,   440,   442,   357,   444,   438,   173,
     429,   446,   448,   450,   452,   454,   456,   458,   460,   357,
     198,   217,    33,   197,    33,   197,   215,   221,   216,   357,
     215,   221,   434,   173,   251,   477,   173,   191,   251,   378,
     431,   462,   467,   173,   381,   431,   472,   357,   152,   173,
     385,   386,   421,   377,   377,   377,   198,   198,   357,   259,
     198,   303,   424,   478,   217,   301,   198,     5,   102,   103,
     198,   217,   127,   300,   331,   342,   357,   286,   198,   217,
      61,   357,   217,   357,   173,   198,   198,   217,   251,   198,
     166,    58,   357,   217,   286,   198,   217,   198,   198,   217,
     198,   198,   127,   300,   357,   357,   357,   220,   286,   333,
     337,   337,   337,   217,   217,   217,   217,   217,   217,    13,
     434,    13,   434,    13,   357,   503,   519,   198,   357,   198,
     233,    13,   293,   503,   520,   357,   357,   357,   357,   357,
      13,    49,   291,   331,   357,   158,   173,   331,   479,   481,
     220,   251,   251,   357,    10,    37,   333,   339,   173,   217,
     251,   251,   251,   251,   251,    66,   316,   275,   132,   251,
      21,    22,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   136,   137,   138,   139,
     140,   141,   144,   145,   146,   147,   148,   149,   150,   192,
     193,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   214,   215,   339,   217,   398,   192,   251,   285,   215,
     251,   275,   285,   372,   198,   218,    43,   251,   388,   300,
     357,   462,   462,   432,   462,   218,   462,   462,   218,   173,
     426,   462,   276,   462,   276,   462,   276,   378,   379,   381,
     382,   218,   437,   291,   216,   216,   357,   251,   285,   194,
     431,   285,   194,   431,   285,   218,   217,    43,   127,   191,
     192,   194,   197,   384,   494,   496,   286,   421,   304,   217,
     301,   198,   217,   328,   198,   198,   198,   515,   331,   300,
     331,   191,   108,   109,   110,   111,   112,   113,   114,   115,
     121,   122,   127,   140,   141,   147,   148,   149,   192,    14,
     434,   293,   357,   357,   286,   192,   321,   323,   357,   325,
     194,   166,   357,   517,   331,   500,   505,   331,   498,   434,
     300,   357,   220,   275,   357,   357,   357,   357,   357,   357,
     421,    53,   200,   215,   217,   357,   479,   482,   486,   502,
     507,   421,   217,   482,   507,   421,   142,   184,   185,   186,
     487,   296,   286,   298,   179,   180,   228,   421,   184,   191,
     523,   421,    13,   216,   191,   523,   217,   137,   384,   523,
     191,   523,   218,   152,   157,   198,   301,   347,   286,   257,
     284,   340,    70,   215,   218,   331,   481,   160,   217,   318,
     391,     4,   160,   336,   337,    19,   158,   173,   422,    19,
     158,   173,   422,   133,   134,   135,   287,   343,   357,   343,
     357,   343,   357,   343,   357,   343,   357,   343,   357,   343,
     357,   343,   357,   357,   357,   357,   343,   357,   343,   357,
     357,   357,   357,   173,   343,   357,   357,   158,   173,   357,
     357,   357,   422,   357,   357,   357,   343,   357,   343,   357,
     357,   357,   357,   343,   357,   343,   357,   343,   357,   357,
     343,   357,    22,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   129,   130,   158,   173,   214,   215,
     354,   422,   357,   218,   331,   411,   357,   274,     8,   365,
     370,   434,   173,   300,   357,   251,   199,   199,   199,   431,
     199,   199,   199,   251,   199,   277,   199,   277,   199,   277,
     199,   431,   199,   431,   294,   462,   218,   216,   462,   462,
     357,   173,   173,   462,   357,   434,   434,    20,   462,    70,
     331,   481,   492,   198,   357,   173,   357,   462,   509,   511,
     513,   434,   523,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   286,   199,   431,   218,   218,   264,   434,   434,   218,
     434,   218,   434,   523,   434,   379,   523,   382,   199,   336,
     218,   218,   218,   218,   218,   218,    20,   337,   215,   357,
     218,   142,   191,   185,   486,   188,   189,   216,   490,   191,
     185,   188,   216,   489,    20,   218,   486,   184,   187,   488,
      20,   357,   184,   503,   294,   294,   357,    20,   503,    20,
     421,   357,   357,   357,   218,   158,   173,   217,   217,   349,
     351,    12,    23,    24,   249,   250,   357,   289,   275,   173,
     218,   481,   479,   191,   218,   218,   173,   317,   317,   217,
     127,   137,   173,   192,   197,   334,   335,   276,   198,   217,
     198,   217,   337,   337,   337,   217,   217,   216,    19,   158,
     173,   422,   194,   158,   173,   357,   217,   217,   158,   173,
     357,     1,   217,   216,   218,   285,   251,   216,    57,    63,
     368,    67,   369,   251,   199,   251,   436,   441,   443,   462,
     445,   439,   430,   173,   447,   199,   451,   199,   455,   199,
     459,   378,   461,   381,   199,   431,   218,    43,   384,   199,
     199,   331,   199,   481,   218,   218,   218,   173,   218,   185,
     199,   218,   199,   434,   379,   382,   199,   218,   217,   434,
     357,   199,   199,   199,   199,   218,   199,   199,   218,   199,
     336,   276,   217,   331,   482,   486,   357,   479,   490,   216,
     357,   502,   216,   331,   482,   184,   187,   190,   491,   216,
     331,   199,   199,   194,   231,   331,   331,    20,   218,   217,
     137,   384,   357,   357,   434,   276,   286,   357,    12,   253,
     285,   336,   218,   216,   215,   191,   216,   218,   335,   173,
     173,   217,   173,   173,   191,   216,   277,   358,   357,   360,
     357,   218,   331,   357,   198,   217,   357,   217,   216,   357,
     215,   218,   331,   217,   216,   355,   218,   331,   251,    47,
     369,    46,   106,   366,   336,   449,   453,   457,   217,   462,
     173,   357,   495,   497,   286,   305,   218,   199,   431,   173,
     217,   329,   199,   199,   199,   516,   293,   199,   322,   324,
     326,   518,   501,   506,   499,   217,   339,   277,   218,   331,
     185,   486,   490,   185,   486,   216,   185,   297,   299,   232,
     181,   185,   185,   331,   137,   384,   357,   357,   357,   218,
     218,   199,   277,   286,   254,   251,   276,   218,   479,   173,
     216,   194,   389,   218,   173,   334,   216,   142,   286,   332,
     434,   218,   462,   218,   218,   218,   362,   357,   357,   218,
     479,   218,   357,   218,    33,   367,   366,   368,   291,   217,
     217,   357,   173,   199,   357,   510,   512,   514,   217,   218,
     217,   357,   357,   357,   217,    70,   492,   217,   217,   218,
     357,   332,   218,   357,   490,   357,   491,   503,   357,   217,
     292,   230,   503,   357,   185,   357,   357,   218,   350,   199,
     250,    26,   105,   255,   307,   308,   309,   311,   357,   277,
     216,   194,   389,   434,   388,   218,   127,   357,   199,   199,
     462,   218,   218,   216,   218,   373,   367,   385,   386,   387,
     218,   492,   492,   218,   199,   217,   218,   217,   217,   217,
     291,   293,   331,   492,   492,   218,   185,   522,   522,   522,
     291,   177,   522,   522,   357,   137,   384,   347,   352,   127,
     127,   357,   286,   218,   434,   388,   388,   300,   357,   357,
     359,   361,   199,   218,   281,   374,   217,   479,   483,   484,
     485,   485,   357,   357,   492,   492,   479,   480,   218,   218,
     523,   485,   480,    53,   216,   184,   184,   191,   523,   184,
     216,   522,   357,   357,   347,   357,   388,   300,   357,   300,
     357,   251,   363,   251,   281,   479,   191,   523,   218,   218,
     218,   218,   485,   485,   218,   218,   218,   218,   357,   216,
     216,   184,   218,   216,   300,   357,   251,   251,   286,   218,
     217,   218,   218,   251,   479,   218
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
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   279,   280,   280,   280,   281,   283,   282,
     284,   284,   285,   285,   286,   286,   287,   287,   287,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     289,   289,   289,   290,   291,   291,   292,   292,   293,   293,
     294,   294,   296,   297,   295,   298,   299,   295,   300,   300,
     300,   300,   300,   301,   301,   301,   302,   302,   304,   305,
     303,   303,   306,   306,   306,   306,   306,   306,   307,   308,
     309,   309,   309,   310,   310,   310,   311,   311,   312,   312,
     312,   313,   314,   314,   314,   315,   315,   316,   316,   317,
     317,   318,   318,   318,   318,   318,   318,   318,   318,   319,
     319,   321,   322,   320,   323,   324,   320,   325,   326,   320,
     328,   329,   327,   330,   330,   330,   330,   330,   330,   331,
     331,   332,   332,   332,   333,   333,   333,   334,   334,   334,
     334,   334,   335,   335,   336,   336,   336,   337,   337,   338,
     340,   339,   341,   341,   341,   341,   341,   341,   341,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   343,   343,
     343,   343,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   345,
     345,   346,   346,   347,   347,   348,   349,   350,   348,   351,
     352,   348,   353,   353,   353,   353,   353,   353,   353,   354,
     355,   353,   356,   356,   356,   356,   356,   356,   356,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   358,   359,   357,   357,
     357,   357,   360,   361,   357,   357,   357,   362,   363,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   364,   364,   364,   364,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   364,
     365,   365,   365,   366,   366,   366,   367,   367,   368,   368,
     368,   369,   369,   370,   371,   371,   372,   371,   373,   371,
     374,   371,   371,   375,   376,   376,   377,   377,   377,   377,
     377,   378,   378,   379,   379,   380,   380,   380,   381,   382,
     382,   383,   383,   383,   384,   384,   385,   385,   385,   386,
     386,   387,   387,   388,   388,   388,   389,   389,   390,   390,
     390,   390,   390,   391,   391,   391,   391,   391,   392,   392,
     393,   392,   394,   394,   395,   395,   395,   396,   397,   396,
     398,   398,   398,   398,   399,   399,   399,   401,   400,   402,
     402,   403,   404,   403,   405,   405,   405,   406,   408,   409,
     407,   410,   411,   407,   412,   412,   413,   413,   414,   415,
     415,   415,   415,   416,   416,   416,   417,   417,   419,   420,
     418,   421,   421,   421,   421,   421,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   423,   423,   423,   423,   423,   423,   423,
     423,   424,   425,   425,   425,   426,   426,   427,   427,   427,
     429,   430,   428,   431,   431,   432,   432,   433,   433,   434,
     434,   434,   434,   434,   434,   435,   436,   434,   434,   434,
     437,   434,   434,   434,   434,   434,   434,   434,   434,   434,
     434,   434,   434,   434,   438,   439,   434,   434,   440,   441,
     434,   442,   443,   434,   444,   445,   434,   434,   446,   447,
     434,   448,   449,   434,   434,   450,   451,   434,   452,   453,
     434,   434,   454,   455,   434,   456,   457,   434,   458,   459,
     434,   460,   461,   434,   462,   462,   462,   464,   465,   466,
     467,   463,   469,   470,   471,   472,   468,   474,   475,   476,
     477,   473,   478,   478,   478,   478,   478,   479,   479,   479,
     479,   479,   479,   479,   479,   480,   480,   481,   482,   482,
     483,   483,   484,   484,   485,   485,   486,   486,   487,   487,
     488,   488,   489,   489,   490,   490,   490,   491,   491,   491,
     492,   492,   493,   493,   493,   493,   493,   493,   494,   495,
     493,   496,   497,   493,   498,   499,   493,   500,   501,   493,
     502,   502,   502,   503,   503,   504,   505,   506,   504,   507,
     507,   508,   508,   508,   509,   510,   508,   511,   512,   508,
     513,   514,   508,   508,   515,   516,   508,   508,   517,   518,
     508,   519,   519,   520,   520,   521,   521,   521,   521,   521,
     522,   522,   523,   523,   524,   524,   524,   524,   524,   524
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
       2,     3,     3,     2,     2,     3,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     3,     2,
       2,     2,     2,     2,     3,     3,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     1,     3,     0,     4,
       1,     1,     1,     1,     3,     7,     2,     2,     6,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     2,     2,     2,
       0,     2,     2,     3,     0,     2,     0,     4,     0,     2,
       1,     3,     0,     0,     7,     0,     0,     7,     3,     2,
       2,     2,     1,     1,     3,     2,     2,     3,     0,     0,
       5,     1,     2,     5,     5,     5,     6,     2,     1,     1,
       1,     2,     3,     2,     2,     3,     2,     3,     2,     2,
       3,     4,     1,     1,     0,     1,     1,     1,     0,     1,
       3,     9,     8,     8,     7,     8,     7,     7,     6,     3,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     4,
       3,     4,     5,     4,     5,     3,     4,     1,     1,     2,
       4,     4,     7,     8,     3,     5,     0,     0,     8,     3,
       3,     3,     0,     0,     8,     3,     4,     0,     0,     9,
       4,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     2,     4,     1,     4,     4,     4,     4,     4,     1,
       6,     7,     6,     6,     7,     7,     6,     7,     6,     6,
       0,     4,     1,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     1,     5,     0,     2,     0,     4,     0,     9,
       0,    10,     5,     3,     3,     4,     1,     1,     3,     3,
       3,     1,     3,     1,     3,     0,     2,     3,     3,     1,
       3,     0,     2,     3,     1,     1,     1,     2,     3,     3,
       5,     1,     1,     1,     1,     1,     0,     1,     1,     4,
       3,     3,     5,     4,     6,     5,     5,     4,     0,     2,
       0,     4,     0,     1,     0,     1,     1,     6,     0,     6,
       0,     2,     3,     5,     0,     1,     1,     0,     5,     2,
       3,     4,     0,     4,     0,     1,     1,     1,     0,     0,
       9,     0,     0,    11,     0,     2,     0,     1,     3,     1,
       1,     2,     2,     0,     1,     1,     0,     3,     0,     0,
       7,     1,     4,     3,     3,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     1,     3,     0,     2,     3,
       0,     0,     6,     1,     1,     1,     3,     3,     4,     1,
       1,     1,     1,     2,     3,     0,     0,     6,     4,     5,
       0,     9,     4,     2,     2,     3,     2,     3,     2,     2,
       3,     3,     3,     2,     0,     0,     6,     2,     0,     0,
       6,     0,     0,     6,     0,     0,     6,     1,     0,     0,
       6,     0,     0,     7,     1,     0,     0,     6,     0,     0,
       7,     1,     0,     0,     6,     0,     0,     7,     0,     0,
       6,     0,     0,     6,     1,     3,     3,     0,     0,     0,
       0,    10,     0,     0,     0,     0,    10,     0,     0,     0,
       0,    10,     1,     1,     1,     1,     1,     3,     3,     5,
       5,     6,     6,     8,     8,     0,     1,     2,     1,     3,
       3,     5,     1,     2,     1,     0,     0,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     2,     1,     1,
       0,     1,     5,     4,     6,     7,     5,     7,     0,     0,
      10,     0,     0,    10,     0,     0,    10,     0,     0,     7,
       1,     3,     3,     3,     1,     5,     0,     0,    10,     1,
       3,     3,     4,     4,     0,     0,    11,     0,     0,    11,
       0,     0,    10,     5,     0,     0,     9,     5,     0,     0,
      10,     1,     3,     1,     3,     3,     3,     4,     7,     9,
       0,     3,     0,     1,     9,    10,    10,    10,     9,    10
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

  case 187: /* function_name: "operator" '.' "name" "+="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`+="); delete (yyvsp[-1].s); }
    break;

  case 188: /* function_name: "operator" '.' "name" "-="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`-="); delete (yyvsp[-1].s); }
    break;

  case 189: /* function_name: "operator" '.' "name" "*="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`*="); delete (yyvsp[-1].s); }
    break;

  case 190: /* function_name: "operator" '.' "name" "/="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`/="); delete (yyvsp[-1].s); }
    break;

  case 191: /* function_name: "operator" '.' "name" "%="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`%="); delete (yyvsp[-1].s); }
    break;

  case 192: /* function_name: "operator" '.' "name" "&="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&="); delete (yyvsp[-1].s); }
    break;

  case 193: /* function_name: "operator" '.' "name" "|="  */
                                          { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`|="); delete (yyvsp[-1].s); }
    break;

  case 194: /* function_name: "operator" '.' "name" "^="  */
                                           { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^="); delete (yyvsp[-1].s); }
    break;

  case 195: /* function_name: "operator" '.' "name" "&&="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`&&="); delete (yyvsp[-1].s); }
    break;

  case 196: /* function_name: "operator" '.' "name" "||="  */
                                            { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`||="); delete (yyvsp[-1].s); }
    break;

  case 197: /* function_name: "operator" '.' "name" "^^="  */
                                              { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`^^="); delete (yyvsp[-1].s); }
    break;

  case 198: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 199: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 200: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 201: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 202: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 203: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 204: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 205: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 206: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 207: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 208: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 209: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 210: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 211: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 212: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 213: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 214: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 215: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 216: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 217: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 218: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 219: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 220: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 221: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 222: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 223: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 224: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 225: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 226: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 227: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 228: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 229: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 230: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 231: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 232: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 233: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 234: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 235: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 236: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 237: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 238: /* $@7: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 239: /* function_declaration: optional_public_or_private_function $@7 function_declaration_header expression_block  */
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

  case 244: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 245: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 246: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 247: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 248: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 249: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 250: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 251: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 252: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 253: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 254: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 255: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 257: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 258: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 259: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 260: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 261: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 262: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 263: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 264: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 265: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 266: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 267: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 268: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 269: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 270: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 271: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 272: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 273: /* expr_keyword: "keyword" expr expression_block  */
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

  case 274: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 275: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 276: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 277: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 278: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 279: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 280: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 281: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 282: /* $@8: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 283: /* $@9: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 284: /* expression_keyword: "keyword" '<' $@8 type_declaration_no_options_list '>' $@9 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 285: /* $@10: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 286: /* $@11: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 287: /* expression_keyword: "type function" '<' $@10 type_declaration_no_options_list '>' $@11 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 288: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 289: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 290: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 291: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 292: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 293: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 294: /* name_in_namespace: "name" "::" "name"  */
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

  case 295: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 296: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 297: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 298: /* $@12: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 299: /* $@13: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 300: /* new_type_declaration: '<' $@12 type_declaration '>' $@13  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 301: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 302: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 303: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 304: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 305: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 306: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 307: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 308: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 309: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 310: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 311: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 312: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 313: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 314: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 315: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 316: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 317: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 318: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 319: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 320: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 321: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 322: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 323: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 324: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 325: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 326: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 327: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 328: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 329: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 330: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 331: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 332: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 333: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 334: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 335: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 336: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 337: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 338: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 339: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 340: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 341: /* $@14: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 342: /* $@15: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 343: /* expr_cast: "cast" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 344: /* $@16: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 345: /* $@17: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 346: /* expr_cast: "upcast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 347: /* $@18: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 348: /* $@19: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 349: /* expr_cast: "reinterpret" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 350: /* $@20: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 351: /* $@21: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 352: /* expr_type_decl: "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 353: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 354: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 355: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 356: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
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

  case 357: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
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

  case 358: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 359: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 360: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 361: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 362: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 363: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 364: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 365: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 366: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 367: /* capture_entry: '&' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 368: /* capture_entry: '=' "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 369: /* capture_entry: "<-" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 370: /* capture_entry: ":=" "name"  */
                                    { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 371: /* capture_entry: "name" '(' "name" ')'  */
                                    { (yyval.pCapt) = ast_makeCaptureEntry(scanner,tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s),*(yyvsp[-1].s)); delete (yyvsp[-3].s); delete (yyvsp[-1].s); }
    break;

  case 372: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 373: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 374: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 375: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 376: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 377: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),(yyvsp[0].pExpression));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 378: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 379: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 380: /* $@22: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 381: /* expr_full_block_assumed_piped: block_or_lambda $@22 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 382: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 383: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 384: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 385: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 386: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 387: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 388: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 389: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 390: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 391: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 392: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 393: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 394: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 395: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 396: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 397: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 398: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 399: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 400: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 401: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 402: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 403: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 404: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 405: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 406: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 407: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 408: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 409: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 410: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 411: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 412: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 413: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 414: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 415: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 416: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 417: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 418: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 419: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 420: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 421: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 422: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 423: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 424: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 425: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 426: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 427: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 428: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 429: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 430: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 431: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 432: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 433: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 434: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 435: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 436: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 437: /* $@24: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 438: /* func_addr_expr: '@' '@' '<' $@23 type_declaration_no_options '>' $@24 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 439: /* $@25: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 440: /* $@26: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 441: /* func_addr_expr: '@' '@' '<' $@25 optional_function_argument_list optional_function_type '>' $@26 func_addr_name  */
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

  case 442: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 443: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 444: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 445: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 446: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 447: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 448: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 449: /* $@27: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 450: /* $@28: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 451: /* expr_field: expr '.' $@27 error $@28  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), "");
        yyerrok;
    }
    break;

  case 452: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 453: /* expr_call: name_in_namespace '(' "uninitialized" ')'  */
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

  case 454: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 455: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-4].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 456: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 457: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 458: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 459: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 460: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 461: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 462: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 463: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 464: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 465: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 466: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 467: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 468: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 469: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 470: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 471: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 472: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 473: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 485: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 488: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 489: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 490: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 491: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 492: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 493: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 494: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 495: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 496: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 497: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 498: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 499: /* expr: '(' expr_list optional_comma ')'  */
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

  case 500: /* expr: '(' make_struct_single ')'  */
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

  case 501: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 502: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 503: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 504: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 505: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 506: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 507: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 508: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 509: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 510: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 511: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 512: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 513: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 514: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 515: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 516: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 517: /* $@30: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 518: /* expr: expr "is" "type" '<' $@29 type_declaration_no_options '>' $@30  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 519: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 520: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 521: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 522: /* $@31: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 523: /* $@32: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 524: /* expr: expr "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 525: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 526: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 527: /* $@33: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 528: /* $@34: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 529: /* expr: expr '?' "as" "type" '<' $@33 type_declaration '>' $@34  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 530: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 531: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 532: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 533: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 534: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 535: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 536: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 537: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 538: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 539: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 540: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 541: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 542: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 543: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 544: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 545: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 546: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 547: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 548: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 549: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 550: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 551: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 552: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 553: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 554: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 555: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 556: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 557: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 558: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 559: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 560: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 561: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 562: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 563: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 564: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 565: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 566: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 567: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 568: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 569: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 570: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 571: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 572: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 573: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 574: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 575: /* struct_variable_declaration_list: struct_variable_declaration_list semicolon  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 576: /* $@35: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 577: /* struct_variable_declaration_list: struct_variable_declaration_list $@35 structure_variable_declaration semicolon  */
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

  case 578: /* $@36: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 579: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@36 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 580: /* $@37: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 581: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@37 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 582: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 583: /* function_argument_declaration_no_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_no_type  */
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

  case 584: /* function_argument_declaration_type: optional_field_annotation kwd_let_var_or_nothing variable_declaration_type  */
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

  case 585: /* function_argument_declaration_type: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))));
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 586: /* function_argument_list: function_argument_declaration_no_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 587: /* function_argument_list: function_argument_declaration_type  */
                                                                                      { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 588: /* function_argument_list: function_argument_declaration_no_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 589: /* function_argument_list: function_argument_declaration_type semicolon function_argument_list  */
                                                                                            { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 590: /* function_argument_list: function_argument_declaration_type ',' function_argument_list  */
                                                                                      { (yyval.pVarDeclList) = (yyvsp[0].pVarDeclList); (yyvsp[0].pVarDeclList)->insert((yyvsp[0].pVarDeclList)->begin(),(yyvsp[-2].pVarDecl)); }
    break;

  case 591: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 592: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 593: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 594: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 595: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 596: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 597: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 598: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition(*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))));
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 599: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 600: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 601: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 602: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 603: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 604: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 605: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 606: /* variable_declaration_no_type: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 607: /* variable_declaration_no_type: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 608: /* variable_declaration_no_type: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 609: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 610: /* variable_declaration_type: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 611: /* variable_declaration: variable_declaration_type  */
                                        {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 612: /* variable_declaration: variable_declaration_no_type  */
                                           {
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 613: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 614: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 615: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 616: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 617: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 618: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 619: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 620: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 621: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 622: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 623: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options semicolon  */
                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 624: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 625: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 626: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr semicolon  */
                                                                                                                {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 627: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 628: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 629: /* global_variable_declaration_list: global_variable_declaration_list "end of line"  */
                                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 630: /* $@38: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 631: /* global_variable_declaration_list: global_variable_declaration_list $@38 optional_field_annotation let_variable_declaration  */
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

  case 632: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 633: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 634: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 635: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 636: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 637: /* global_let: kwd_let optional_shared optional_public_or_private_variable open_block global_variable_declaration_list close_block  */
                                                                                                                                                      {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 638: /* $@39: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 639: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@39 optional_field_annotation let_variable_declaration  */
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

  case 640: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 641: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 642: /* enum_list: enum_list "name" semicolon  */
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

  case 643: /* enum_list: enum_list "name" '=' expr semicolon  */
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

  case 644: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 645: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 646: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 647: /* $@40: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 648: /* single_alias: optional_public_or_private_alias "name" $@40 '=' type_declaration  */
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

  case 652: /* $@41: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 654: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 655: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 656: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 657: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 658: /* $@42: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 659: /* $@43: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 660: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@42 enum_list $@43 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 661: /* $@44: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 662: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 663: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 664: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 665: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 666: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 667: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 668: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 669: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 670: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 671: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 672: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 673: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 674: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 675: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 676: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 677: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 678: /* $@46: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 679: /* $@47: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 680: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@46 structure_name $@47 optional_struct_variable_declaration_list  */
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

  case 681: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 682: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition("``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)));
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 683: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 684: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))));
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 685: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))));
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 686: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 687: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 688: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 689: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 690: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 691: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 692: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 693: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 694: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 695: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 696: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 697: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 698: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 699: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 700: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 701: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 702: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 703: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 704: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 705: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 706: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 707: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 708: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 709: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 710: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 711: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 712: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 713: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 714: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 715: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 716: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 717: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 718: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 719: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 720: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 721: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 722: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 723: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 724: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 725: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 726: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 727: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 728: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 729: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
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

  case 730: /* $@48: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 731: /* $@49: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 732: /* bitfield_type_declaration: "bitfield" '<' $@48 bitfield_bits '>' $@49  */
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

  case 735: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 736: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 737: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 738: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 739: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 740: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 741: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 742: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 743: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 744: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 745: /* $@50: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 746: /* $@51: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 747: /* type_declaration_no_options: "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 748: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 749: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 750: /* $@52: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 751: /* type_declaration_no_options: '$' name_in_namespace '<' $@52 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 752: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 753: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 754: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 755: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 756: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 757: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 758: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 759: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 760: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 761: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 762: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 763: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 764: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 765: /* $@54: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 766: /* type_declaration_no_options: "smart_ptr" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 767: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 768: /* $@55: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 769: /* $@56: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 770: /* type_declaration_no_options: "array" '<' $@55 type_declaration '>' $@56  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 771: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 772: /* $@58: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 773: /* type_declaration_no_options: "table" '<' $@57 table_type_pair '>' $@58  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 774: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 775: /* $@60: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 776: /* type_declaration_no_options: "iterator" '<' $@59 type_declaration '>' $@60  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 777: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 778: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 779: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 780: /* type_declaration_no_options: "block" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 781: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 782: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 783: /* type_declaration_no_options: "block" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 784: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 785: /* $@65: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 786: /* $@66: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 787: /* type_declaration_no_options: "function" '<' $@65 type_declaration '>' $@66  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 788: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 789: /* $@68: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 790: /* type_declaration_no_options: "function" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 791: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 792: /* $@69: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 793: /* $@70: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 794: /* type_declaration_no_options: "lambda" '<' $@69 type_declaration '>' $@70  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 795: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 796: /* $@72: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 797: /* type_declaration_no_options: "lambda" '<' $@71 optional_function_argument_list optional_function_type '>' $@72  */
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

  case 798: /* $@73: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 799: /* $@74: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 800: /* type_declaration_no_options: "tuple" '<' $@73 tuple_type_list '>' $@74  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 801: /* $@75: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 802: /* $@76: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 803: /* type_declaration_no_options: "variant" '<' $@75 variant_type_list '>' $@76  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 804: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 805: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 806: /* type_declaration: type_declaration '|' '#'  */
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

  case 807: /* $@77: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 808: /* $@78: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 809: /* $@79: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 810: /* $@80: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 811: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@77 "name" $@78 open_block $@79 tuple_alias_type_list $@80 close_block  */
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

  case 812: /* $@81: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 813: /* $@82: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 814: /* $@83: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 815: /* $@84: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 816: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@81 "name" $@82 open_block $@83 variant_alias_type_list $@84 close_block  */
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

  case 817: /* $@85: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 818: /* $@86: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 819: /* $@87: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 820: /* $@88: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 821: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@85 "name" $@86 open_block $@87 bitfield_alias_bits $@88 close_block  */
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
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 828: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 829: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 830: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 831: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 832: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 833: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 834: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
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

  case 839: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
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
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 860: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 861: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 862: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 863: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 864: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
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
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 866: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
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
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 868: /* $@89: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 869: /* $@90: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 870: /* make_struct_decl: "struct" '<' $@89 type_declaration_no_options '>' $@90 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 871: /* $@91: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 872: /* $@92: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 873: /* make_struct_decl: "class" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 874: /* $@93: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 875: /* $@94: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 876: /* make_struct_decl: "variant" '<' $@93 variant_type_list '>' $@94 '(' use_initializer make_variant_dim ')'  */
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

  case 877: /* $@95: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 878: /* $@96: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 879: /* make_struct_decl: "default" '<' $@95 type_declaration_no_options '>' $@96 use_initializer  */
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
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 881: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 882: /* make_tuple: make_tuple ',' expr  */
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

  case 883: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
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

  case 886: /* $@97: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 887: /* $@98: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 888: /* make_tuple_call: "tuple" '<' $@97 tuple_type_list '>' $@98 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 890: /* make_dim: make_dim "end of expression" make_tuple  */
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
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 893: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 894: /* $@99: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 895: /* $@100: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 896: /* make_dim_decl: "array" "struct" '<' $@99 type_declaration_no_options '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 897: /* $@101: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 898: /* $@102: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 899: /* make_dim_decl: "array" "tuple" '<' $@101 tuple_type_list '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 900: /* $@103: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 901: /* $@104: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 902: /* make_dim_decl: "array" "variant" '<' $@103 variant_type_list '>' $@104 '(' make_variant_dim ')'  */
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

  case 904: /* $@105: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 905: /* $@106: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 906: /* make_dim_decl: "array" '<' $@105 type_declaration_no_options '>' $@106 '(' optional_expr_list ')'  */
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

  case 908: /* $@107: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 909: /* $@108: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 910: /* make_dim_decl: "fixed_array" '<' $@107 type_declaration_no_options '>' $@108 '(' expr_list optional_comma ')'  */
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

  case 912: /* make_table: make_table "end of expression" make_map_tuple  */
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

  case 915: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
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

  case 921: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 922: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 923: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 924: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 925: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 926: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 927: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 928: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 929: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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
