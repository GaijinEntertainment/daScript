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
  YYSYMBOL_function_argument_declaration = 375, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 376,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 377,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 378,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 379,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 380,             /* variant_type  */
  YYSYMBOL_variant_type_list = 381,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 382,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 383,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 384,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 385,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 386,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 387, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 388, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 389, /* global_variable_declaration_list  */
  YYSYMBOL_390_38 = 390,                   /* $@38  */
  YYSYMBOL_optional_shared = 391,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 392, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 393,               /* global_let  */
  YYSYMBOL_394_39 = 394,                   /* $@39  */
  YYSYMBOL_enum_list = 395,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 396, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 397,             /* single_alias  */
  YYSYMBOL_398_40 = 398,                   /* $@40  */
  YYSYMBOL_alias_list = 399,               /* alias_list  */
  YYSYMBOL_alias_declaration = 400,        /* alias_declaration  */
  YYSYMBOL_401_41 = 401,                   /* $@41  */
  YYSYMBOL_optional_public_or_private_enum = 402, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 403,                /* enum_name  */
  YYSYMBOL_enum_declaration = 404,         /* enum_declaration  */
  YYSYMBOL_405_42 = 405,                   /* $@42  */
  YYSYMBOL_406_43 = 406,                   /* $@43  */
  YYSYMBOL_407_44 = 407,                   /* $@44  */
  YYSYMBOL_408_45 = 408,                   /* $@45  */
  YYSYMBOL_optional_structure_parent = 409, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 410,          /* optional_sealed  */
  YYSYMBOL_structure_name = 411,           /* structure_name  */
  YYSYMBOL_class_or_struct = 412,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 413, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 414, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 415,    /* structure_declaration  */
  YYSYMBOL_416_46 = 416,                   /* $@46  */
  YYSYMBOL_417_47 = 417,                   /* $@47  */
  YYSYMBOL_variable_name_with_pos_list = 418, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 419,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 420, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 421, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 422,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 423,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 424,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 425, /* bitfield_type_declaration  */
  YYSYMBOL_426_48 = 426,                   /* $@48  */
  YYSYMBOL_427_49 = 427,                   /* $@49  */
  YYSYMBOL_c_or_s = 428,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 429,          /* table_type_pair  */
  YYSYMBOL_dim_list = 430,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 431, /* type_declaration_no_options  */
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
  YYSYMBOL_455_73 = 455,                   /* $@73  */
  YYSYMBOL_456_74 = 456,                   /* $@74  */
  YYSYMBOL_457_75 = 457,                   /* $@75  */
  YYSYMBOL_458_76 = 458,                   /* $@76  */
  YYSYMBOL_type_declaration = 459,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 460,  /* tuple_alias_declaration  */
  YYSYMBOL_461_77 = 461,                   /* $@77  */
  YYSYMBOL_462_78 = 462,                   /* $@78  */
  YYSYMBOL_463_79 = 463,                   /* $@79  */
  YYSYMBOL_464_80 = 464,                   /* $@80  */
  YYSYMBOL_variant_alias_declaration = 465, /* variant_alias_declaration  */
  YYSYMBOL_466_81 = 466,                   /* $@81  */
  YYSYMBOL_467_82 = 467,                   /* $@82  */
  YYSYMBOL_468_83 = 468,                   /* $@83  */
  YYSYMBOL_469_84 = 469,                   /* $@84  */
  YYSYMBOL_bitfield_alias_declaration = 470, /* bitfield_alias_declaration  */
  YYSYMBOL_471_85 = 471,                   /* $@85  */
  YYSYMBOL_472_86 = 472,                   /* $@86  */
  YYSYMBOL_473_87 = 473,                   /* $@87  */
  YYSYMBOL_474_88 = 474,                   /* $@88  */
  YYSYMBOL_make_decl = 475,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 476,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 477,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 478,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 479,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 480,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 481,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 482, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 483,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 484, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 485, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 486, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 487, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 488, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 489,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 490,         /* make_struct_decl  */
  YYSYMBOL_491_89 = 491,                   /* $@89  */
  YYSYMBOL_492_90 = 492,                   /* $@90  */
  YYSYMBOL_493_91 = 493,                   /* $@91  */
  YYSYMBOL_494_92 = 494,                   /* $@92  */
  YYSYMBOL_495_93 = 495,                   /* $@93  */
  YYSYMBOL_496_94 = 496,                   /* $@94  */
  YYSYMBOL_497_95 = 497,                   /* $@95  */
  YYSYMBOL_498_96 = 498,                   /* $@96  */
  YYSYMBOL_make_tuple = 499,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 500,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 501,          /* make_tuple_call  */
  YYSYMBOL_502_97 = 502,                   /* $@97  */
  YYSYMBOL_503_98 = 503,                   /* $@98  */
  YYSYMBOL_make_dim = 504,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 505,            /* make_dim_decl  */
  YYSYMBOL_506_99 = 506,                   /* $@99  */
  YYSYMBOL_507_100 = 507,                  /* $@100  */
  YYSYMBOL_508_101 = 508,                  /* $@101  */
  YYSYMBOL_509_102 = 509,                  /* $@102  */
  YYSYMBOL_510_103 = 510,                  /* $@103  */
  YYSYMBOL_511_104 = 511,                  /* $@104  */
  YYSYMBOL_512_105 = 512,                  /* $@105  */
  YYSYMBOL_513_106 = 513,                  /* $@106  */
  YYSYMBOL_514_107 = 514,                  /* $@107  */
  YYSYMBOL_515_108 = 515,                  /* $@108  */
  YYSYMBOL_make_table = 516,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 517,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 518,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 519, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 520,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 521       /* array_comprehension  */
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
#define YYLAST   15017

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  222
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  300
/* YYNRULES -- Number of rules.  */
#define YYNRULES  913
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1718

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
       0,   558,   558,   559,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   574,   575,   576,   580,   586,   587,
     588,   592,   593,   597,   615,   616,   617,   618,   622,   623,
     627,   628,   632,   633,   633,   637,   642,   651,   666,   682,
     687,   695,   695,   734,   764,   768,   769,   770,   774,   777,
     781,   787,   796,   799,   805,   806,   810,   814,   815,   819,
     822,   828,   834,   837,   843,   844,   848,   849,   850,   859,
     860,   864,   865,   869,   870,   870,   876,   877,   878,   879,
     880,   884,   890,   890,   896,   896,   902,   910,   920,   929,
     929,   936,   937,   938,   939,   940,   941,   945,   950,   958,
     959,   960,   964,   965,   966,   967,   968,   969,   970,   971,
     977,   980,   986,   989,   992,   998,   999,  1000,  1001,  1005,
    1018,  1036,  1039,  1047,  1058,  1069,  1080,  1083,  1090,  1094,
    1101,  1102,  1106,  1107,  1108,  1112,  1115,  1119,  1128,  1132,
    1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,
    1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,
    1153,  1154,  1155,  1156,  1157,  1158,  1159,  1160,  1161,  1162,
    1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,  1172,
    1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,  1182,
    1183,  1184,  1185,  1186,  1187,  1188,  1189,  1190,  1191,  1192,
    1193,  1194,  1195,  1196,  1197,  1198,  1199,  1200,  1201,  1202,
    1203,  1204,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,
    1213,  1214,  1215,  1220,  1238,  1239,  1240,  1244,  1250,  1250,
    1268,  1269,  1272,  1273,  1276,  1280,  1291,  1300,  1309,  1315,
    1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1326,  1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,
    1339,  1344,  1350,  1356,  1367,  1368,  1372,  1373,  1377,  1378,
    1382,  1386,  1393,  1393,  1393,  1399,  1399,  1399,  1408,  1442,
    1445,  1448,  1451,  1457,  1458,  1469,  1473,  1476,  1484,  1484,
    1484,  1487,  1493,  1496,  1500,  1504,  1511,  1518,  1524,  1528,
    1532,  1535,  1538,  1546,  1549,  1552,  1560,  1563,  1571,  1574,
    1577,  1585,  1591,  1592,  1593,  1597,  1598,  1602,  1603,  1607,
    1612,  1620,  1626,  1632,  1638,  1644,  1653,  1662,  1671,  1683,
    1686,  1692,  1692,  1692,  1695,  1695,  1695,  1700,  1700,  1700,
    1708,  1708,  1708,  1714,  1724,  1735,  1748,  1758,  1769,  1784,
    1787,  1793,  1794,  1801,  1812,  1813,  1814,  1818,  1819,  1820,
    1821,  1822,  1826,  1831,  1839,  1840,  1841,  1845,  1850,  1857,
    1864,  1864,  1873,  1874,  1875,  1876,  1877,  1878,  1879,  1883,
    1884,  1885,  1886,  1887,  1888,  1889,  1890,  1891,  1892,  1893,
    1894,  1895,  1896,  1897,  1898,  1899,  1900,  1901,  1905,  1906,
    1907,  1908,  1913,  1914,  1915,  1916,  1917,  1918,  1919,  1920,
    1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,  1929,  1934,
    1941,  1953,  1958,  1968,  1972,  1979,  1982,  1982,  1982,  1987,
    1987,  1987,  2000,  2004,  2008,  2013,  2020,  2029,  2034,  2041,
    2041,  2041,  2048,  2052,  2061,  2069,  2077,  2081,  2084,  2090,
    2091,  2092,  2093,  2094,  2095,  2096,  2097,  2098,  2099,  2100,
    2101,  2102,  2103,  2104,  2105,  2106,  2107,  2108,  2109,  2110,
    2111,  2112,  2113,  2114,  2115,  2116,  2117,  2118,  2119,  2120,
    2121,  2122,  2123,  2124,  2125,  2131,  2132,  2133,  2134,  2135,
    2148,  2157,  2158,  2159,  2160,  2161,  2162,  2163,  2164,  2165,
    2166,  2167,  2168,  2171,  2174,  2175,  2178,  2178,  2178,  2181,
    2186,  2190,  2194,  2194,  2194,  2199,  2202,  2206,  2206,  2206,
    2211,  2214,  2215,  2216,  2217,  2218,  2219,  2220,  2221,  2222,
    2224,  2228,  2229,  2234,  2238,  2239,  2240,  2241,  2242,  2243,
    2244,  2248,  2252,  2256,  2260,  2264,  2268,  2272,  2276,  2280,
    2287,  2288,  2289,  2293,  2294,  2295,  2299,  2300,  2304,  2305,
    2306,  2310,  2311,  2315,  2326,  2329,  2332,  2332,  2351,  2350,
    2366,  2365,  2379,  2388,  2397,  2407,  2408,  2412,  2415,  2424,
    2425,  2429,  2432,  2435,  2451,  2460,  2461,  2465,  2468,  2471,
    2485,  2486,  2490,  2496,  2502,  2505,  2509,  2515,  2524,  2525,
    2526,  2530,  2531,  2535,  2542,  2547,  2556,  2562,  2573,  2576,
    2581,  2586,  2594,  2605,  2608,  2608,  2628,  2629,  2633,  2634,
    2635,  2639,  2642,  2642,  2661,  2664,  2667,  2682,  2701,  2702,
    2703,  2708,  2708,  2734,  2735,  2739,  2740,  2740,  2744,  2745,
    2746,  2750,  2760,  2765,  2760,  2777,  2782,  2777,  2797,  2798,
    2802,  2803,  2807,  2813,  2814,  2815,  2816,  2820,  2821,  2822,
    2826,  2829,  2835,  2840,  2835,  2860,  2867,  2872,  2881,  2887,
    2898,  2899,  2900,  2901,  2902,  2903,  2904,  2905,  2906,  2907,
    2908,  2909,  2910,  2911,  2912,  2913,  2914,  2915,  2916,  2917,
    2918,  2919,  2920,  2921,  2922,  2923,  2924,  2928,  2929,  2930,
    2931,  2932,  2933,  2934,  2935,  2939,  2950,  2954,  2961,  2973,
    2980,  2989,  2994,  2997,  3010,  3010,  3010,  3023,  3024,  3028,
    3032,  3039,  3043,  3050,  3051,  3052,  3053,  3054,  3069,  3075,
    3075,  3075,  3079,  3084,  3091,  3091,  3098,  3102,  3106,  3111,
    3116,  3121,  3126,  3130,  3134,  3139,  3143,  3147,  3152,  3152,
    3152,  3158,  3165,  3165,  3165,  3170,  3170,  3170,  3176,  3176,
    3176,  3181,  3185,  3185,  3185,  3190,  3190,  3190,  3199,  3203,
    3203,  3203,  3208,  3208,  3208,  3217,  3221,  3221,  3221,  3226,
    3226,  3226,  3235,  3235,  3235,  3241,  3241,  3241,  3250,  3253,
    3264,  3280,  3280,  3285,  3290,  3280,  3315,  3315,  3320,  3326,
    3315,  3351,  3351,  3356,  3361,  3351,  3391,  3392,  3393,  3394,
    3395,  3399,  3406,  3413,  3419,  3425,  3432,  3439,  3445,  3454,
    3457,  3463,  3471,  3476,  3483,  3488,  3495,  3500,  3506,  3507,
    3511,  3512,  3517,  3518,  3522,  3523,  3527,  3528,  3532,  3533,
    3534,  3538,  3539,  3540,  3544,  3545,  3549,  3555,  3562,  3570,
    3577,  3585,  3594,  3594,  3594,  3602,  3602,  3602,  3609,  3609,
    3609,  3620,  3620,  3620,  3631,  3634,  3640,  3654,  3660,  3666,
    3672,  3672,  3672,  3686,  3691,  3698,  3717,  3722,  3729,  3729,
    3729,  3739,  3739,  3739,  3753,  3753,  3753,  3767,  3776,  3776,
    3776,  3796,  3803,  3803,  3803,  3813,  3818,  3825,  3828,  3834,
    3853,  3862,  3870,  3890,  3915,  3916,  3920,  3921,  3926,  3929,
    3932,  3935,  3938,  3941
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
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
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

#define YYPACT_NINF (-1488)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-780)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1488,    84, -1488, -1488,    32,   -86,   371,   366, -1488,   -97,
     293,   293,   293, -1488, -1488,   223,   125, -1488, -1488,   449,
   -1488, -1488, -1488, -1488,   312, -1488,    23, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488,   -99, -1488,   -30,
     -62,   -25, -1488, -1488, -1488, -1488,   371, -1488,    21, -1488,
   -1488, -1488,   293,   293, -1488, -1488,    23, -1488, -1488, -1488,
   -1488, -1488,    90,    51, -1488, -1488, -1488, -1488,   125,   125,
     125,    -6, -1488,   831,   -44, -1488, -1488, -1488, -1488,   378,
     414,   545,   426, -1488,   648,    31,    32,   252,   -86,    72,
     216, -1488,   810,   810, -1488,   340,   449,     7,   449,   707,
     348,   413,   437, -1488,   486,   492, -1488, -1488,   -36,    32,
     125,   125,   125,   125, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488,   525, -1488, -1488, -1488, -1488, -1488, -1488, -1488,   366,
   -1488, -1488, -1488, -1488, -1488,   817,   140, -1488, -1488, -1488,
   -1488,   727, -1488, -1488, -1488, -1488, -1488, -1488, -1488,   449,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,   530,
   -1488,   263, -1488,   732,   643,   831, 14844, -1488,   299,   710,
   -1488,   -68, -1488, -1488, -1488,   845, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488,   342, -1488,   658, -1488,   366,   366,   366,
   -1488, -1488, 13792, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,   836,   853,
   -1488,   677,   366,   679, -1488, -1488,   724, -1488,   550,    32,
      32,   213,   118, -1488, -1488, -1488,   140, -1488, 10572, -1488,
   -1488, -1488,   728,   729, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488,   734,   689, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,   858, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488,   736,   699, -1488, -1488,   119,   722, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488,   366, -1488,   726,
     366, -1488,   -68,   -38, -1488,    32, -1488,   704,   888,   240,
   -1488, -1488,   735,   740,   755,   730,   758,   759, -1488, -1488,
   -1488,   742, -1488, -1488, -1488, -1488, -1488,   766, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,   772,
   -1488, -1488, -1488,   774,   780, -1488, -1488, -1488, -1488,   782,
     783,   769,   223, -1488, -1488, -1488, -1488, -1488,  1310,   751,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488,   819,   854, -1488,
     794, -1488,   314, -1488,   120, 10469, -1488,  2585, -1488,   647,
     223, -1488, -1488, -1488,   118,   789, -1488,  9603,   840,   843,
   10572, -1488,    -2, -1488, -1488, -1488,  9603, -1488, -1488,   846,
   -1488,   577,   605,   609, -1488, -1488,  9603,   -28, -1488, -1488,
   -1488,    17, -1488, -1488, -1488,     6,  5891, -1488,   813, 10217,
     650, 10320,   539, -1488, -1488,  9603, -1488, -1488,   137, -1488,
     -11, 10675,   751, -1488,   837,   839,  9603, -1488, -1488, -1488,
   -1488, -1488,    82,   -74,   842,    36,  3205, -1488, -1488,   366,
      60,  6097,   812,  9603,   866,   844,   849,   824, -1488,   449,
     852,   877,  6303,   151,    75,   856, -1488,   338,   857,   859,
    3411,  9603,  9603,   124,   124,   124,   834,   835,   847,   848,
     850,   851, -1488,  2166, 10114,  6511, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488,  6717,   864, -1488,  6925,  9603,  9603,  9603,
    9603,  9603,  5067,  7131, -1488,   860, -1488, -1488,   449,   449,
   -1488,  9603,  1053, -1488, -1488, -1488, -1488, -1488, -1488,  1032,
   -1488, -1488, -1488,   518, -1488,    39,   449, -1488,   449,   449,
     449, -1488,   449, -1488, -1488,  1010, -1488, -1488, -1488, -1488,
     863, -1488, -1488,   310, -1488, -1488, -1488, -1488, -1488, -1488,
    9922, -1488,   862, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
     443, -1488,   550, -1488,   111, -1488, -1488,   865,   889,   891,
   -1488, 10915, -1488,  1048,   985, -1488, -1488, -1488,  3823, 10572,
   10572, 10572, 11026, 10572, 10572,   874,   920, 10572,   677, 10572,
     677, 10572,   677, 10675,   923, 11064, -1488,  9603, -1488, -1488,
   -1488, -1488,   881, -1488, -1488, 13275,  9603, -1488,  1310,   449,
   -1488,   550,   -66, -1488, -1488,   202, -1488,   751,   550,   909,
     202, -1488,   550, 11175,   887,  1064, -1488,   -71, -1488, -1488,
     273, -1488, -1488, 13834,   137, -1488,   893, -1488, -1488,   223,
     504, -1488,   910,   913,   916, -1488,  9603,  3823, -1488,   924,
     984, 10880,  1103, 10572,  9603,  9603, 14317,  9603, 13834,   926,
   -1488, -1488,  9603, -1488, -1488,   925,   954, 14317,  9603, -1488,
   -1488,  9603, -1488, -1488,  9603, -1488, 10572,  3823, -1488, 10880,
    1135,  1135,   901, -1488,   863, -1488, -1488, -1488,  9603,  9603,
    9603,  9603,  9603,  9603,   137,  1913,   137,  2791,   137, 14020,
   -1488,   841, -1488, 13834, -1488,   749,   137,   939, -1488,   933,
    1135,  1135,   -40,  1135,  1135,   137,  1113,   911,   937, 14317,
     921,   123,   937,   948,   928,   317, -1488, -1488, 13834, -1488,
     366, -1488, -1488, -1488,  4029, -1488, -1488, -1488, -1488, -1488,
   -1488,   189,    38,   124, -1488, 14596, 14633,  4235,  4235,  4235,
    4235,  4235,  4235,  4235,  4235,  9603,  9603, -1488, -1488,  9603,
    4235,  4235,  9603,  9603,  9603,   970,  4235,  9603,   399,  9603,
    9603,  9603,  9603,  9603,  9603,  4235,  4235,  9603,  9603,  9603,
    4235,  4235,  4235,  9603,  4235,  7337,  9603,  9603,  9603,  9603,
    9603,  9603,  9603,  9603,  9603,  9603, 14699,  9603, -1488,  7543,
     647,  9603, -1488, -1488,   125, -1488,  1139, -1488,   -68, 10572,
   -1488,   975, -1488,  3823, -1488, 10759,   463,   511,   952,   441,
   -1488,   540,   551, -1488, -1488,   377,   619,   722,   669,   722,
     686,   722,   186, -1488,   281, -1488, 10572,   931, -1488, -1488,
   13386, -1488, -1488, 10572, -1488, -1488, 10572, -1488, -1488, -1488,
    9603,   981, -1488,   982, -1488, 10572, -1488,  3823, -1488, 10675,
   10572, 10572, -1488,    24, 10572,  5273,  7749,   986,  9603, 10572,
   -1488, -1488, -1488, 10572,   937, -1488,   924,  9603,  9603,  9603,
    9603,  9603,  9603,  9603,  9603,  9603,  9603,  9603,  9603,  9603,
    9603,  9603,  9603,  9603,  9603,   366,  2258,   940, 14317, 11210,
   -1488, -1488, 10572, 10572, 11321, 10572, -1488, -1488, 11359, 10572,
     937, 10572, 10675,   937,   923,   592, -1488, 10880, -1488,    38,
   11470, 11505, 11616, 11654, 11765, 11800,    35,   124,  2998,  4443,
    5479, 14057,   948,    -4,   382,   971,   291,    42,  5685,    -4,
     756,    46,  9603,   977,  9603, -1488, -1488, 10572, -1488, 10572,
   -1488,  9603,   585,    47, -1488,  9603, -1488,    54,   137, -1488,
    9603, -1488,  9603,  9603,  9603,   945,   575, -1488,   947,   949,
     612, -1488, -1488,   818,  9603, -1488,   863,   196,  4651, -1488,
     460,   951,   992,   992, -1488, -1488,   956,   135,   677, -1488,
     973,   957, -1488, -1488,   978,   962, -1488, -1488,   124,   124,
     124, -1488, -1488,  2346, -1488,  2346, -1488,  2346, -1488,  2346,
   -1488,  2346, -1488,  2346, -1488,  2346, -1488,  2346,   930,   930,
     871, -1488,  2346, -1488,  2346,   871,  1346,  1346,   963, -1488,
    2346,   806,   966, -1488, 13424,   -47,   -47,   862, 14317,   930,
     930, -1488,  2346, -1488,  2346,  1473, 10716,  9969, -1488,  2346,
   -1488,  2346, -1488,  2346, 14410, -1488,  2346, 14732, 14150,  1120,
    1079, 14447,   871,   871,  1565,  1565,   806,   806,   806,   617,
    9603,   967,   972,   623,  9603,  1167,   976, 13535, -1488,   473,
     550, 13927,   154,   788,  1121,   449,   884, -1488, -1488, 10759,
   -1488, -1488, -1488, -1488, 10572, -1488, -1488, -1488,  1017, -1488,
     993, -1488,   995, -1488,  1002, -1488, -1488,   923,   321,   751,
   -1488, -1488,   751,   751, 11911, -1488,  1168,   -59, -1488, 10880,
   -1488,  1319,  2304,  9603,   691,   625,   476,   994,   996,  1049,
   11949,   418, 12060,   692, 10572, 10675,   923,  4232,  1006, 14317,
   14317, 14317, 14317, 14317, 14317, 14317, 14317, 14317, 14317, 14317,
   14317, 14317, 14317, 14317, 14317, 14317, 14317, -1488,  1009, 10572,
   -1488, -1488,  9603,  4440,  4648, -1488,  4856, -1488,  6714,  1012,
    9806,   323,  1013,   362,    38,   677, -1488, -1488, -1488, -1488,
   -1488,  1011,  9603, -1488,  4859, 13275,     4,  9603,   625,   382,
   -1488, -1488,  1016, -1488,  9603,  9603, -1488,  1019, -1488,  9603,
     625,   656,  1026, -1488, -1488,  9603, 14317, -1488, -1488,   402,
     439, 14187,  9603, -1488,  9603,    55, 12095, 14317, 14317, -1488,
    1030,   144,  9603,  9603, 10572,   677,   366, -1488, -1488,  9603,
   -1488,  1668,  2585,    38,   163, -1488,  1015,   254,  9809, -1488,
   -1488, -1488,   373,   477,   135,  1082,  1084,  1043,  1094,  1095,
   -1488,   411,   722, -1488,  9603, -1488,  9603, -1488, -1488, -1488,
    7955,  9603, -1488,  1071,  1055, -1488, -1488,  9603,  1056, -1488,
   13570,  9603,  8161,  1057, -1488, 13681, -1488,  8367, -1488, -1488,
   -1488, -1488,   449, -1488, -1488,   698, -1488,    40, -1488,    38,
   -1488, -1488, -1488, -1488,   751, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,  1069, 10572,
   -1488,  1097,  9603, -1488, -1488,   211, -1488,  1070, -1488, -1488,
   -1488,   446, -1488,  1114,  1073, -1488, -1488, 10302,   448,   458,
   -1488, -1488,  9603, 10315, 14317, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488,   748,   722,  8573,   663,    -4,   382,
   14317,   948, -1488, -1488, 14317,   971, -1488,   664,    -4,  1080,
   -1488, -1488, -1488, -1488,   667, -1488, -1488, -1488,  1116,   668,
     681,  9603,   220,  9603,  9603,  9603, 12206, 12244, 10396,   722,
   -1488, 13834, -1488,   449, -1488,   677, -1488,  1077,  4651,  1125,
    1083,   185,   479, -1488, -1488,  1127, -1488, -1488,   135,  1085,
       0, 10572, 12355, 10572, 12390, -1488,   481, 12501, -1488,  9603,
    2159,  9603, -1488, 12539,  4651, -1488,   482,  9603, -1488, -1488,
   -1488,   483, -1488,  1269,    40, -1488, -1488,   788,  1087, -1488,
   -1488, -1488,  9603,   751, -1488, 14317,  1088,  1089, -1488, -1488,
   -1488,  9603,  1136,  1109,  9603, -1488, -1488, -1488, -1488,  1108,
    1093,  1110,  9603,  9603,  9603,  1112,  1242,  1119,  1122,  8779,
   -1488,     0, -1488,   491,  9603,   382, -1488,  9603,   656, -1488,
    9603,  9603,  1123, -1488, -1488,  9603,  9603,   684,  9603,  9603,
   12650, 14317, 14317, -1488, -1488, -1488,  1132,   818,  3617, -1488,
     722, -1488,   497, -1488,   242, 10572,    -2, -1488,  1124, -1488,
   -1488,  8985, -1488, -1488, 10401, -1488,   700, -1488, -1488, -1488,
   10572, 12685, 12796, -1488,   512, -1488, 12834, -1488, -1488, -1488,
    1269,   137,  1126,  1242,  1242, 12945,  1133,  1128, 12980,  1129,
    1131,  1134,  9603, -1488,  9603,   871,   871,   871,  9603, -1488,
   -1488,  1242,  1242, -1488, 13091, -1488, -1488, 14280, -1488, 14280,
   -1488,  1141,   871,  9603, -1488,  1156,  1141, 14280,  9603, 14317,
   14317,   238,   503, -1488, -1488,  9191,  9397, -1488, -1488, -1488,
   -1488, -1488, 14317,   366,  1137, 10572,    -2,  1092,  3823, -1488,
    9603, 14410, -1488, -1488,   701, -1488, -1488,  1140, -1488, 14844,
   -1488, -1488, -1488,  -100,  -100, -1488,  9603,  9603, -1488,  1242,
    1242,   625,  1143,  1144,   937,  -100,   625, -1488,  1285,  1138,
    1157,  1169,  1166, -1488,  1175,  1147, 14280,  9603,  9603, -1488,
     503,  9603,  9603, 14317, -1488, -1488,  1092,  3823,  3823, -1488,
   10759,  2159, -1488, -1488, -1488, -1488,   449, 14844,   625,   948,
    1174, -1488,  1151,  1154, 13129, 13240,  -100,  -100,   948,  1155,
   -1488, -1488,  1163,  1165,  1170,  9603,  1159,  1171,  1200, -1488,
    1172, -1488, -1488,  1173, 14317, 14317, -1488, 14317,  3823, -1488,
   10759, -1488, 10759, -1488, -1488, -1488,   366,   499,  1176, -1488,
   -1488, -1488, -1488, -1488,  1179,  1180, -1488, -1488, -1488, -1488,
   14317, -1488, -1488, -1488, -1488, -1488, -1488, 10759, -1488, -1488,
   -1488, -1488,   625, -1488, -1488, -1488,   501, -1488
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   130,     1,   315,     0,     0,     0,   636,   316,     0,
     628,   628,   628,    69,    70,     0,     0,    15,     3,     0,
      10,     9,     8,    16,     0,     7,   616,     6,    11,     5,
       4,    13,    12,    14,   100,   101,    99,   108,   110,    43,
      59,    56,    57,    45,    46,    47,     0,    48,    54,    44,
     231,   230,   628,   628,    22,    21,   616,   630,   629,   801,
     791,   796,     0,   283,    41,   116,   117,   118,     0,     0,
       0,   119,   121,   128,     0,   115,    17,   654,   653,   224,
     638,     0,   657,   617,   618,     0,     0,     0,     0,    49,
       0,    55,     0,     0,    52,     0,     0,   628,     0,    18,
       0,     0,     0,   285,     0,     0,   127,   122,     0,     0,
       0,     0,     0,     0,   131,   226,   225,   228,   223,   640,
     639,     0,   656,   655,   659,   658,   662,   620,   619,   622,
     106,   107,   104,   105,   103,     0,     0,   102,   111,    60,
      58,    54,    51,    50,   631,   633,   233,   232,   635,     0,
     637,    19,    20,    23,   802,   792,   797,   284,    39,    42,
     126,     0,   123,   124,   125,   129,     0,   641,     0,   650,
     613,   550,    24,    25,    29,     0,    95,    96,    93,    94,
      92,    91,    97,     0,    53,     0,   634,     0,     0,     0,
      40,   120,     0,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,     0,     0,
     138,   132,     0,     0,   642,   651,     0,   663,   614,     0,
       0,   552,     0,    26,    27,    28,     0,   109,     0,   803,
     793,   798,   192,   193,   190,   141,   142,   144,   143,   145,
     146,   147,   148,   174,   175,   172,   173,   165,   176,   177,
     166,   163,   164,   191,   185,     0,   189,   178,   179,   180,
     181,   152,   153,   154,   149,   150,   151,   162,     0,   168,
     169,   167,   160,   161,   156,   155,   157,   158,   159,   140,
     139,   184,     0,   170,   171,   550,   135,   260,   229,   697,
     700,   703,   704,   698,   701,   699,   702,     0,   624,   648,
     660,   621,   550,     0,   112,     0,   114,     0,   603,   601,
     623,    98,     0,     0,     0,     0,     0,     0,   670,   690,
     671,   706,   672,   676,   677,   678,   679,   696,   683,   684,
     685,   686,   687,   688,   689,   691,   692,   693,   694,   761,
     675,   682,   695,   768,   775,   673,   680,   674,   681,     0,
       0,     0,     0,   705,   723,   726,   724,   725,   788,   632,
     711,   581,   587,   194,   195,   188,   183,   196,   186,   182,
       0,   133,   314,   575,     0,     0,   227,     0,   645,   643,
       0,   652,   564,   664,     0,     0,   113,     0,     0,     0,
       0,   602,     0,   729,   752,   755,     0,   758,   748,     0,
     714,   762,   769,   776,   782,   785,     0,     0,   738,   743,
     737,     0,   751,   747,   740,     0,     0,   742,   727,     0,
     804,   794,   799,   197,   187,     0,   312,   313,     0,   134,
     550,     0,   136,   262,     0,     0,     0,    71,    72,    84,
     455,   456,     0,     0,     0,     0,   300,   449,   298,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   299,     0,
       0,     0,     0,     0,     0,     0,   696,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   539,     0,     0,     0,   372,   374,   373,   375,
     376,   377,   378,     0,     0,    35,   268,     0,     0,     0,
       0,     0,   264,     0,   354,   355,   453,   452,     0,     0,
     239,     0,     0,   255,   250,   247,   246,   248,   249,   234,
     282,   261,   241,   533,   240,   450,     0,   524,    79,    80,
      77,   253,    78,   254,   256,   318,   245,   523,   522,   521,
     130,   527,   451,     0,   242,   526,   525,   497,   457,   498,
     379,   458,     0,   454,   806,   810,   807,   808,   809,   624,
       0,   625,     0,   649,   566,   615,   551,     0,     0,     0,
     533,     0,   605,   606,     0,   599,   600,   598,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   132,     0,
     132,     0,   132,     0,     0,     0,   734,   264,   745,   746,
     739,   741,     0,   744,   728,     0,     0,   790,   789,     0,
     712,     0,   283,   717,   718,     0,   582,   577,     0,     0,
       0,   588,     0,     0,     0,   665,   573,   592,   576,   579,
       0,   852,   855,     0,     0,   288,   292,   291,   297,     0,
       0,   340,     0,     0,     0,   888,     0,     0,   304,   301,
       0,   349,     0,     0,   268,     0,   286,     0,     0,     0,
     331,   334,     0,   259,   337,     0,     0,    63,     0,    86,
     892,     0,   861,   870,     0,   858,     0,     0,   309,   306,
     485,   486,   355,   367,   130,   281,   279,   280,     0,     0,
       0,     0,     0,     0,     0,   830,     0,     0,     0,   868,
     895,     0,   272,     0,   275,     0,     0,     0,   897,   906,
     462,   461,   499,   460,   459,     0,     0,     0,   906,   349,
       0,   283,   906,   821,     0,   356,   257,   258,     0,    82,
       0,   370,   237,   531,     0,   244,   251,   252,   303,   308,
     317,     0,   364,     0,   243,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   487,   488,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,   236,     0,
     646,     0,   626,   644,     0,   565,     0,   661,   550,     0,
     604,     0,   608,     0,   612,   379,     0,     0,     0,   719,
     732,     0,     0,   707,   709,     0,     0,   135,     0,   135,
       0,   135,     0,   585,     0,   708,     0,     0,   736,   721,
       0,   713,   805,     0,   583,   795,     0,   589,   800,   574,
       0,     0,   591,     0,   590,     0,   593,     0,   137,     0,
       0,     0,    87,     0,     0,   844,     0,     0,     0,     0,
     878,   881,   884,     0,   906,   305,   302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   287,     0,
      88,    89,     0,     0,     0,     0,    61,    62,     0,     0,
     906,     0,     0,   906,     0,     0,   310,   307,   356,   364,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   864,   822,   830,     0,   873,     0,     0,     0,   830,
       0,     0,     0,     0,     0,   833,   900,     0,   263,     0,
      38,     0,    36,     0,   899,   907,   269,     0,     0,   875,
     907,   265,     0,     0,     0,     0,     0,   490,     0,     0,
     426,   423,   425,    66,     0,   260,   130,     0,   264,   442,
       0,     0,     0,     0,   330,   329,     0,     0,   132,   278,
       0,     0,   510,   509,     0,     0,   511,   515,     0,     0,
       0,   401,   410,   389,   411,   390,   413,   392,   412,   391,
     414,   393,   404,   383,   405,   384,   406,   385,   463,   464,
     476,   415,   394,   416,   395,   477,   474,   475,     0,   403,
     381,   504,     0,   495,     0,   528,   529,   530,   382,   465,
     466,   417,   396,   418,   397,   481,   482,   483,   407,   386,
     408,   387,   409,   388,   484,   402,   380,     0,     0,   479,
     480,   478,   472,   473,   468,   467,   469,   470,   471,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   447,     0,
       0,     0,     0,   558,   561,     0,     0,   607,   610,   379,
     611,   730,   753,   756,     0,   759,   749,   715,     0,   763,
       0,   770,     0,   777,     0,   783,   786,     0,     0,   270,
     733,   722,   578,   584,     0,   667,   668,   594,   597,   596,
     580,     0,     0,     0,     0,   845,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   350,
     389,   390,   392,   391,   393,   383,   384,   385,   394,   395,
     381,   396,   397,   386,   387,   388,   380,   311,     0,     0,
     901,   500,     0,     0,     0,   501,     0,   532,     0,     0,
       0,     0,     0,     0,   364,   132,   534,   535,   536,   537,
     538,     0,     0,   831,     0,   349,   830,     0,     0,     0,
     839,   840,     0,   847,     0,     0,   837,     0,   876,     0,
       0,     0,     0,   835,   877,     0,   867,   832,   896,     0,
       0,    32,     0,   898,     0,     0,     0,   812,   811,   489,
       0,     0,     0,     0,     0,   132,     0,    64,    65,     0,
      81,    73,     0,   364,     0,   443,     0,     0,     0,   446,
     444,   319,     0,     0,     0,     0,     0,     0,     0,     0,
     362,     0,   135,   506,     0,   512,     0,   400,   398,   399,
       0,     0,   493,     0,     0,   516,   520,     0,     0,   496,
       0,     0,     0,     0,   433,     0,   440,     0,   491,   448,
     647,   627,   131,   559,   560,   561,   562,   553,   567,   364,
     609,   731,   754,   757,   720,   760,   750,   716,   710,   764,
     766,   771,   773,   778,   780,   784,   787,   586,     0,     0,
     666,     0,     0,   853,   856,     0,   289,     0,   294,   295,
     293,     0,   343,     0,     0,   346,   341,     0,     0,     0,
     889,   887,   268,     0,    90,   332,   335,   338,   893,   891,
     862,   871,   869,   859,     0,   135,     0,     0,   830,     0,
     865,   823,   846,   838,   866,   874,   836,     0,   830,     0,
     842,   843,   850,   834,     0,   273,   276,    33,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   135,
      67,     0,    74,     0,   235,   132,   445,     0,   264,     0,
       0,   601,     0,   359,   360,     0,   358,   357,     0,     0,
       0,     0,     0,     0,     0,   421,     0,     0,   517,     0,
     505,     0,   494,     0,   264,   434,     0,     0,   492,   441,
     437,     0,   572,   556,   553,   554,   555,   558,     0,   767,
     774,   781,   264,   271,   669,   595,     0,     0,    85,   290,
     296,     0,     0,     0,     0,   342,   879,   882,   885,     0,
       0,     0,     0,     0,     0,     0,   844,     0,     0,     0,
     238,     0,   540,     0,     0,     0,   848,     0,     0,   841,
       0,     0,   266,    30,    37,     0,     0,     0,     0,     0,
       0,   814,   813,   424,   549,   427,     0,    66,     0,    83,
     135,   419,     0,   320,   601,     0,     0,   366,     0,   363,
     365,     0,   351,   369,     0,   548,     0,   546,   422,   543,
       0,     0,     0,   542,     0,   435,     0,   438,   557,   568,
     556,     0,     0,   844,   844,     0,     0,     0,     0,     0,
       0,     0,   264,   902,   268,   333,   336,   339,     0,   845,
     863,   844,   844,   502,     0,   368,   541,   904,   849,   904,
     851,   904,   274,   264,   277,    34,   904,   904,     0,   816,
     815,     0,     0,   430,    68,   300,     0,    75,    79,    80,
      77,    78,    76,     0,     0,     0,     0,     0,     0,   361,
       0,   352,   507,   513,     0,   547,   545,     0,   544,     0,
     570,   563,   735,   829,   829,   344,     0,     0,   347,   844,
     844,   819,     0,     0,   906,   829,   819,   503,     0,     0,
       0,     0,   906,    31,     0,     0,   904,     0,     0,   428,
       0,     0,     0,   306,   371,   420,     0,     0,     0,   328,
     379,   353,   508,   514,   518,   436,     0,     0,     0,   826,
     906,   828,     0,     0,     0,     0,   829,   829,   820,     0,
     890,   903,     0,     0,     0,     0,     0,     0,     0,   907,
       0,   912,   908,     0,   818,   817,   431,   307,     0,   326,
     379,   324,   379,   327,   519,   569,     0,     0,   907,   827,
     854,   857,   345,   348,     0,     0,   886,   894,   872,   860,
     905,   910,   911,   913,   267,   909,   322,   379,   325,   323,
     571,   824,     0,   880,   883,   321,     0,   825
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1488, -1488, -1488, -1488, -1488, -1488,   671,  1306, -1488, -1488,
   -1488, -1488, -1488, -1488,  1393, -1488, -1488, -1488,   709,  1353,
   -1488,  1259, -1488, -1488,  1313, -1488, -1488, -1488,   -94,    -1,
   -1488, -1488, -1488,   -93, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488,  1181, -1488, -1488,   -51,   -56, -1488, -1488,
   -1488,   509,   593,  -529,  -570,  -815, -1488, -1488, -1488, -1487,
   -1488, -1488,    12,  -211,  -219,  -375, -1488,   423, -1488,  -603,
   -1488,  -655,  -121,  -260, -1488, -1488, -1488, -1488,  -451,    -9,
   -1488, -1488, -1488, -1488, -1488,   -89,   -88,   -83, -1488,   -78,
   -1488, -1488, -1488,  1411, -1488,   428, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,   227,
     -48,  -266,    16,   171,  -891,  -413, -1488,  -523, -1488, -1488,
    -385,  1115, -1488, -1488, -1488, -1461, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488,   950, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488,  -158,    -8,  -103,     2,   133, -1488, -1488,
   -1488, -1488, -1488,   989, -1488,  -404,  -587, -1488,  -421,  -887,
   -1488,  -622,   -98,  -583, -1347, -1488,  -371, -1488, -1488,  1379,
   -1488, -1488, -1488,   868,  1022,   155, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488,  -610,  -190, -1488,   988,
   -1488, -1488, -1488, -1488, -1488, -1488,  -301, -1488, -1488,  -271,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488,  -138, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488,   998,  -642,  -172,  -730,  -687, -1488, -1488,
   -1231,  -918, -1488, -1488, -1488, -1166,   -32, -1250, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488,   243,  -487, -1488,
   -1488, -1488,   745, -1488, -1488, -1488, -1488, -1488, -1488, -1488,
   -1488, -1488, -1488, -1488, -1488, -1488, -1488, -1236,  -692, -1488
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    17,   153,    56,    18,   175,   181,  1565,  1378,
    1483,   715,   516,   159,   517,   105,    20,    21,    47,    48,
      49,    94,    22,    41,    42,   518,   519,  1239,  1240,   624,
     521,  1393,  1498,   522,   523,   984,   524,   644,   525,   526,
     527,   528,  1172,   182,   183,    37,    38,    39,   231,    71,
      72,    73,    74,    24,   296,   386,   221,    25,   117,   222,
     118,   166,   297,   148,   693,  1011,   531,   387,   532,   727,
    1564,   717,  1118,   580,   957,  1481,   959,  1482,   534,   535,
     536,   646,   874,  1449,   537,   538,   539,   540,   541,   542,
     543,   544,   438,   545,   751,  1252,   994,   546,   547,   912,
    1462,   913,  1463,   915,  1464,   548,   879,  1455,   549,   728,
    1513,   550,  1260,  1261,   998,   695,   551,   808,   986,   552,
     660,  1012,   554,   555,   556,   982,   557,  1234,  1572,  1235,
    1630,   558,  1085,  1429,   559,   729,  1411,  1642,  1413,  1643,
    1520,  1684,   561,   382,  1437,  1529,  1295,  1297,  1095,   574,
     818,  1599,  1647,   383,   384,   639,   640,   431,   843,   844,
     432,   974,   636,   588,   402,   319,   320,   228,   312,    84,
     129,    27,   171,   389,    95,    96,   185,    97,    28,    53,
     121,   168,    29,   308,   572,   569,  1090,   391,   226,   227,
      82,   126,   393,    30,   169,   310,   637,   562,   307,   365,
     366,   835,   430,   367,   596,  1307,   869,   828,   428,   368,
     589,  1301,   846,   594,  1306,   590,  1302,   591,  1303,   593,
    1305,   597,  1309,   598,  1439,   599,  1311,   600,  1440,   601,
    1313,   602,  1441,   603,  1315,   604,  1316,   627,    31,   101,
     188,   371,   628,    32,   102,   189,   372,   632,    33,   100,
     187,   370,   621,   563,   733,  1659,   734,   943,  1650,  1651,
    1652,   944,   956,  1214,  1208,  1203,  1372,  1138,   564,   870,
    1446,   871,  1447,   924,  1468,   921,  1466,   945,   718,   565,
     922,  1467,   946,   566,  1144,  1539,  1145,  1540,  1146,  1541,
     883,  1459,   919,  1465,   711,   719,   567,  1619,   966,   568
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,   823,   553,   298,   847,   658,    64,    75,   710,   907,
     742,   630,   530,   232,   991,   867,   842,   311,    76,    52,
     949,   752,  1110,   575,  1112,  1199,  1114,   625,   837,   688,
     839,  1211,   841,  1362,   873,   138,   971,  1183,  1185,   610,
     975,   652,   996,    90,  1133,   816,   130,   131,   364,   937,
     608,    34,    35,   161,  1506,  1192,   862,   937,   730,    75,
      75,    75,  1209,   942,    57,   942,  1215,  1222,   862,    62,
      58,   696,   697,  1244,  1224,  1381,    54,   104,    91,   775,
      40,   530,   778,   779,     2,    83,  1435,   444,   445,   778,
     779,     3,   229,    85,   936,   145,   947,   150,   951,    63,
     369,    75,    75,    75,    75,   530,   963,   455,   110,   111,
     112,  1629,  1646,   460,     4,   967,     5,  1648,     6,  -130,
     863,   864,    55,   865,     7,   585,   866,   533,   853,   584,
     626,   631,    87,   864,     8,   586,   429,   824,   653,   654,
       9,   170,  1511,   649,   380,  1137,  1436,   113,   186,   229,
     474,   475,   230,    86,   394,   176,   177,  1586,   618,    65,
    1686,    86,   730,    50,    10,   929,    88,   806,   807,  1676,
     606,   146,   114,   313,   806,   807,   529,  1244,   395,   314,
     224,  1198,   160,    51,   477,   478,    11,    12,    66,   607,
     587,   147,  1148,  1476,   104,   364,   533,   132,   997,   239,
     240,   241,   133,   611,   134,    36,   885,   135,    98,   230,
     364,   109,   743,   530,   609,   863,  1550,   694,   694,   694,
     533,   612,   705,   707,    92,    62,   863,   613,  1179,   363,
     316,  1182,    67,   863,   655,    93,   926,   863,   863,   364,
     662,   364,   493,   494,   495,   863,   863,   442,   136,    13,
     862,   364,   149,   656,   679,    63,   744,  1246,   663,  1339,
     973,    68,  1255,   103,   396,   506,   104,   741,    62,    14,
     317,   862,  1256,   680,   380,   146,    13,   664,  1359,   229,
     645,  1384,   530,  1603,  1604,    13,    93,    50,    15,   634,
     862,   318,   681,  1354,   741,   147,    14,   512,    63,    16,
     973,  1615,  1616,   364,   364,    14,   178,    51,  1257,  1558,
     635,   179,   530,   180,    50,   864,   135,    77,    78,   388,
      79,   436,   392,  1620,   854,  1621,   814,  1258,   533,   857,
    1624,  1625,  1259,    69,    51,  1181,   864,   381,   439,   230,
     999,   317,    70,   514,   692,   113,  1247,   862,    80,   992,
      57,    13,  1395,   417,   730,   864,    58,  1488,  1225,  1656,
    1657,   813,   318,   817,   437,   862,    62,    13,   678,  1244,
    1292,    14,  1098,  1653,    50,  1627,   363,   623,    13,  1505,
     995,   573,   401,   440,  1663,  1115,   520,    14,   571,   141,
    1673,   363,   906,   623,    51,   741,    63,   533,    14,   364,
     364,   364,   887,   364,   364,  1327,   993,   364,  1438,   364,
     852,   364,   864,   364,  1245,   925,  1128,   855,   139,    81,
     363,   858,   363,   741,   872,  1694,  1695,   533,  1262,   620,
     864,   399,   363,   315,   400,   115,  1585,   401,    13,   401,
    1475,   116,   753,   363,   650,   976,    13,  1410,   530,   910,
    1478,   826,   827,   829,    86,   831,   832,  1243,    14,   836,
      62,   838,    50,   840,   623,  1130,    14,  1218,   673,   978,
    1397,   119,   623,   364,   979,    13,  1205,   120,  1223,  1206,
    1116,   191,    51,   124,   363,   363,    13,   694,    13,   125,
      63,   868,   530,   223,   958,    14,   364,   533,   533,   533,
     533,   533,   533,   533,   533,  1322,    14,  1207,    14,  1358,
     533,   533,   623,   144,   623,   980,   533,   736,   737,   983,
    1318,   154,  1351,  1368,  1193,   533,   533,    13,  1104,    50,
     533,   533,   533,   236,   533,   745,   683,   746,   747,   748,
    1471,   749,    13,  1117,    43,    44,    45,    14,  1096,    51,
     122,   123,   754,   623,   942,   684,  1361,  1042,  1338,   741,
     237,  1353,    14,   533,  1399,  1003,  1007,    13,   942,   812,
    1200,  1201,  1043,   815,  1496,    46,  1107,   106,   107,   108,
     363,   363,   363,   822,   363,   363,   155,    14,   363,  1400,
     363,  1047,   363,   623,   363,  1267,  1268,  1269,  1202,  1131,
    1132,  1375,  1408,  1333,    13,  1169,    13,   533,    13,  1385,
     156,    13,  1147,    13,    13,  1355,  1086,  1334,   851,   162,
     163,   164,   165,    13,    14,   418,    14,  1409,    14,   364,
     623,    14,   623,    14,    14,   811,   429,   623,  1376,   623,
     876,  1173,  1174,    14,  1176,  1451,    62,  1457,  1178,   623,
    1180,  1248,   419,   420,   363,   978,   364,  1458,   429,   157,
    1094,   741,  1101,   364,   887,  1389,   364,   887,  1399,   158,
    1408,   694,   887,   887,   887,   364,    63,   363,  1249,   364,
     364,   364,   887,   659,   364,  1583,  1167,  1460,   976,   364,
     976,  1289,   976,   364,  1328,  1401,  1317,  1507,   167,  1518,
    1525,  1527,   877,   976,    13,   127,   429,   190,  1119,  1556,
    1102,   128,   629,  1584,   146,  1122,   421,  1711,  1123,  1717,
     422,   878,   364,   364,    14,   364,   981,  1127,  1597,   364,
     623,   364,   364,  1230,   147,   429,  1134,   514,   692,  1105,
     732,  1143,   694,   694,   694,  1433,   429,   741,  1231,   741,
    1106,   741,   985,   741,   299,   741,  1502,   741,   300,   741,
    1489,   741,   233,   234,   151,  1296,   741,   364,   741,   364,
     152,  -765,   301,   302,   741,  1278,  -765,   303,   304,   305,
     306,  1283,  1524,   730,    91,   423,   741,   110,   741,   424,
    1279,  1184,   425,   741,  -765,   741,  1284,   741,  1244,  -772,
     741,   142,   143,  -779,  -772,    75,  -429,   426,  -779,   571,
     363,  -429,    13,   427,   429,    13,   225,  1319,  1109,  1119,
     570,  1119,  -772,   619,  1100,  1500,  -779,   755,   756,  -429,
    1236,  1470,    14,   741,  1108,    14,  1219,   363,  1220,  1532,
    1369,  1237,  1238,  1370,   363,  1293,  1371,   363,  1474,  1477,
     238,  1294,  1480,  1485,   887,   887,   363,   553,   887,   887,
     363,   363,   363,   741,   429,   363,  1486,   530,  1111,  1568,
     363,   293,   887,  1337,   363,   887,   110,  1276,   112,  1290,
     377,   429,  1117,   884,   886,  1113,   429,   429,   294,  1613,
    1326,  1336,   755,   756,   295,   429,   429,   309,  1343,  1593,
    1644,   373,   374,   363,   363,   376,   363,   375,   920,   378,
     363,   923,   363,   363,   364,   379,   385,   418,  1319,  1319,
     390,   397,  1662,  1588,   767,   768,   172,   173,   960,   961,
    1670,   398,   775,   403,   777,   778,   779,   780,   404,  1612,
    1212,  1205,   781,  1213,   419,   420,   429,   406,   363,  1628,
     363,   755,   756,   405,   364,   364,   407,   408,  1689,   409,
    1622,  1649,  1649,  1388,   410,  1469,  1304,   514,   692,  1658,
     411,   990,   412,  1649,  1658,   110,   111,   112,   413,   364,
     414,   415,   533,    43,    44,    45,   416,   765,   766,   767,
     768,   434,   433,  1561,   172,   173,   174,   775,  1566,   777,
     778,   779,   780,  1637,  1638,   576,  1687,   781,   421,   783,
     784,   435,   422,   582,  1649,  1649,   583,  1390,   418,   595,
     806,   807,   233,   234,   235,   953,   954,   955,   616,   667,
    1452,  1394,    59,    60,    61,   641,  1089,   642,  1117,   669,
     651,   672,   670,   675,   364,   419,   420,   671,   767,   768,
     674,   698,   699,  1678,   682,   685,   775,   686,   777,   778,
     779,   780,   714,   739,   700,   701,   781,   702,   703,   740,
    1716,   801,   802,   803,   804,   805,   750,   423,    16,   809,
     735,   424,   678,  1299,   425,   806,   807,   819,   741,   712,
    1291,   821,   833,   834,  1298,   363,   629,   848,  1300,   426,
     755,   756,  1136,   856,   860,   427,  1448,   861,   880,   421,
     875,   881,   585,   422,   882,   887,   753,   905,   911,   916,
     917,   928,   586,   964,   965,   418,   968,   969,   970,   364,
     801,   802,   803,   804,   805,   363,   363,  1639,   972,   976,
    1514,   755,   756,  1038,   806,   807,   977,  1093,  1097,  1120,
      13,  1103,   419,   420,  1125,  1126,   755,   756,  1170,  1141,
     363,  1217,  1204,  1229,  1232,  1251,  1233,   732,  1286,  1250,
      14,  1263,  1497,  1254,  1264,   732,  1265,   587,   423,  1266,
    1270,  1443,   424,  1271,  1281,   425,  1679,  1681,  1296,  1282,
    1308,  1512,  1310,  1287,  1312,   765,   766,   767,   768,   769,
     426,  1314,   772,   773,   774,   775,   427,   777,   778,   779,
     780,  1321,  1329,   530,  1330,   781,   421,   783,   784,   585,
     422,   364,  1331,   364,  1341,   363,  1342,  1706,  1356,   586,
    1349,  1352,  1363,  1396,  1587,  1366,   765,   766,   767,   768,
     769,   520,  1373,   772,   773,   774,   775,  1383,   777,   778,
     779,   780,  1512,   767,   768,  1403,   781,  1404,   783,   784,
    1405,   775,   530,   530,   778,   779,   780,  1406,  1407,  1418,
    1444,   781,  1419,  1421,  1427,  1516,   798,   799,   800,   801,
     802,   803,   804,   805,   587,   423,  1442,  1453,  1450,   424,
    1454,  1432,   425,   806,   807,  1501,  1479,  1484,  1503,  1504,
    1508,  1510,  1528,   530,  1469,  1533,  1534,   426,  1537,  1536,
     363,  1543,  1549,   427,  1636,   364,   797,   798,   799,   800,
     801,   802,   803,   804,   805,  1542,  1618,  1544,   533,  1548,
     364,  1573,  1606,  1623,   806,   807,  1551,   560,  1665,  1552,
    1563,  1667,  1589,   418,  1602,  1607,  1609,   581,  1610,   806,
     807,  1611,   418,  1668,  1666,  1635,   592,  1669,  1645,  1671,
    1325,  1660,  1661,  1672,  1634,  1688,   605,   755,   756,  1690,
     419,   420,  1691,  1696,   741,  1701,   615,   533,   533,   419,
     420,  1697,  1594,  1698,  1703,   633,   962,  1702,  1699,  1705,
    1704,   137,  1499,  1712,    19,   364,   643,  1713,  1714,    89,
     184,   140,   363,  1574,   363,  1577,   661,  1092,  1242,  1578,
    1579,   666,    26,   668,   741,  1580,   741,   321,   533,  1357,
    1581,  1253,   677,  1555,  1509,  1402,  1530,  1600,  1434,   638,
     689,   690,   691,  1601,   421,    99,  1367,   810,   422,  1531,
     647,   741,  1374,   421,  1664,   709,  1560,   422,  1365,  1379,
     648,  1380,   950,   713,     0,     0,   709,   720,   721,   722,
     723,   724,   765,   766,   767,   768,   769,  1710,     0,   772,
       0,   738,   775,     0,   777,   778,   779,   780,     0,     0,
       0,     0,   781,     0,   783,   784,     0,     0,     0,     0,
       0,     0,     0,     0,   755,   756,   363,  1416,     0,     0,
       0,     0,     0,   423,     0,     0,     0,   424,     0,  1426,
     425,   363,   423,     0,  1431,     0,   424,     0,  1323,   425,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,   427,     0,     0,   426,     0,     0,     0,   825,     0,
     427,     0,     0,     0,   799,   800,   801,   802,   803,   804,
     805,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,   807,     0,   981,     0,     0,   850,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,  1473,     0,     0,   755,   756,     0,   765,
     766,   767,   768,   769,     0,     0,   772,   773,   774,   775,
       0,   777,   778,   779,   780,     0,     0,   661,  1487,   781,
       0,   783,   784,     0,   709,   908,     0,   909,     0,     0,
       0,   981,   914,     0,     0,     0,     0,     0,   918,     0,
       0,     0,     0,     0,     0,     0,     0,   927,     0,  1683,
       0,     0,     0,     0,     0,  1685,     0,     0,   930,   931,
     932,   933,   934,   935,     0,   941,     0,   941,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   796,   797,
     798,   799,   800,   801,   802,   803,   804,   805,     0,  1708,
    1392,  1709,     0,   767,   768,     0,     0,   806,   807,   755,
     756,   775,     0,   777,   778,   779,   780,     0,     0,     0,
       0,   781,     0,     0,     0,     0,  1715,  1013,  1015,  1017,
    1019,  1021,  1023,  1025,  1027,  1028,  1029,     0,     0,  1030,
    1032,  1034,  1035,  1036,  1037,     0,  1040,  1041,     0,  1044,
    1045,  1046,  1048,  1049,  1050,  1052,  1054,  1055,  1056,  1057,
    1059,  1061,  1063,  1064,  1066,  1068,  1069,  1070,  1071,  1072,
    1073,  1074,  1075,  1076,  1077,  1078,     0,  1087,     0,     0,
       0,  1091,     0,     0,     0,     0,     0,   803,   804,   805,
       0,     0,     0,  1099,     0,  1614,     0,     0,     0,   806,
     807,     0,     0,     0,   765,   766,   767,   768,   769,     0,
       0,   772,   773,   774,   775,     0,   777,   778,   779,   780,
       0,     0,   659,     0,   781,     0,   783,   784,     0,     0,
    1124,     0,   787,   788,   789,     0,     0,  1129,   793,     0,
       0,     0,     0,     0,     0,     0,  1140,     0,  1142,     0,
       0,     0,     0,     0,     0,     0,     0,  1149,  1150,  1151,
    1152,  1153,  1154,  1155,  1156,  1157,  1158,  1159,  1160,  1161,
    1162,  1163,  1164,  1165,  1166,     0,     0,     0,   886,     0,
       0,   795,     0,   796,   797,   798,   799,   800,   801,   802,
     803,   804,   805,  1014,  1016,  1018,  1020,  1022,  1024,  1026,
       0,     0,   806,   807,     0,  1031,  1033,     0,   720,  1195,
       0,  1039,     0,     0,     0,     0,     0,     0,     0,     0,
    1051,  1053,  1216,     0,   709,  1058,  1060,  1062,     0,  1065,
       0,  1221,     0,     0,     0,   709,     0,     0,   444,   445,
    1149,     0,  1226,  1227,  1228,     0,     0,     0,   450,   451,
     452,   453,   454,     0,  1241,     0,     0,     0,   455,     0,
     457,     0,     0,     0,   460,     0,   418,     0,     0,     0,
       0,     0,   462,     0,     0,     0,     0,     0,   465,     0,
       0,   466,     0,     0,   467,     0,   937,     0,   470,     0,
       0,     0,     0,   419,   420,     0,     0,     0,   577,     0,
       0,   474,   475,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   477,   478,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1280,   481,   482,     0,  1285,     0,     0,   421,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,     0,
     491,   730,   492,   493,   494,   495,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,   731,   579,   504,   505,
       0,     0,     0,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   423,     0,     0,     0,
     424,     0,     0,   938,   508,   509,     0,    15,     0,     0,
     510,   511,  1344,     0,     0,     0,     0,     0,   939,     0,
     940,     0,   514,   515,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1360,     0,     0,
       0,     0,     0,     0,  1364,   941,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   704,
     755,   756,  1386,  1387,     0,   322,     0,     0,     0,  1391,
       0,   323,   560,     0,     0,     0,     0,   324,  1149,     0,
       0,     0,     0,     0,     0,     0,     0,   325,     0,     0,
       0,     0,     0,     0,  1412,   326,  1414,     0,     0,     0,
       0,  1417,     0,     0,     0,     0,     0,  1420,     0,     0,
     327,  1423,     0,     0,     0,     0,     0,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
       0,     0,  1445,     0,     0,   765,   766,   767,   768,   769,
       0,     0,   772,   773,   774,   775,     0,   777,   778,   779,
     780,   418,   709,     0,     0,   781,     0,   783,   784,     0,
       0,     0,     0,   787,   788,   789,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,   419,   420,
       0,     0,   361,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1490,  1491,  1492,     0,   418,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,     0,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,   419,   420,     0,   755,   756,  1521,
       0,  1522,     0,   806,   807,     0,     0,  1526,     0,     0,
       0,     0,   421,     0,     0,   362,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1535,     0,     0,  1538,     0,     0,     0,     0,     0,
       0,     0,  1545,  1546,  1547,     0,     0,     0,     0,  1554,
       0,     0,     0,    13,  1557,     0,     0,  1559,   421,     0,
     709,  1562,   422,     0,     0,   709,  1567,     0,  1569,  1570,
       0,     0,     0,    14,     0,     0,     0,     0,  1582,   623,
       0,   423,     0,     0,     0,   424,     0,  1168,   425,     0,
       0,  1591,   765,   766,   767,   768,   769,     0,     0,   772,
     773,   774,   775,   426,   777,   778,   779,   780,     0,   427,
       0,     0,   781,     0,   783,   784,     0,     0,     0,     0,
     787,   788,   789,     0,   709,     0,   793,   423,     0,     0,
       0,   424,     0,  1324,   425,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1626,   426,
       0,     0,     0,     0,     0,   427,  1633,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1640,   795,
    1641,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     805,     0,     0,     0,     0,     0,  1654,  1655,     0,     0,
     806,   807,     0,     0,     0,   514,   692,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1674,  1675,     0,
       0,     0,  1677,     0,     0,     0,   443,  1680,  1682,     0,
     444,   445,     3,     0,   446,   447,   448,     0,   449,     0,
     450,   451,   452,   453,   454,     0,     0,     0,     0,     0,
     455,   456,   457,   458,   459,  1700,   460,     0,     0,     0,
       0,     0,     0,   461,   462,     0,     0,   463,  1707,   464,
     465,     0,     0,   466,     0,     8,   467,   468,     0,   469,
     470,     0,     0,   471,   472,     0,     0,     0,     0,     0,
     473,     0,     0,   474,   475,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   476,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   477,   478,   479,
     480,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   483,   484,
     485,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,   489,
     490,     0,   491,     0,   492,   493,   494,   495,     0,   146,
      13,   496,   497,   498,   499,   500,   501,   502,    63,   503,
     504,   505,     0,     0,     0,     0,     0,     0,   506,   147,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   507,   508,   509,     0,    15,
       0,     0,   510,   511,     0,     0,   444,   445,     0,     0,
     512,     0,   513,     0,   514,   515,   450,   451,   452,   453,
     454,     0,     0,     0,     0,     0,   455,     0,   457,     0,
       0,     0,   460,     0,   418,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,   465,     0,     0,   466,
       0,     0,   467,     0,     0,     0,   470,     0,     0,     0,
       0,   419,   420,     0,     0,     0,   577,     0,     0,   474,
     475,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   476,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   477,   478,   578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,     0,     0,     0,     0,   421,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   491,   730,
     492,   493,   494,   495,     0,     0,     0,   496,   497,   498,
     499,   500,   501,   502,   731,   579,   504,   505,     0,     0,
       0,     0,     0,     0,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   423,     0,     0,     0,   424,     0,
       0,   938,   508,   509,     0,    15,     0,     0,   510,   511,
       0,     0,     0,   444,   445,     0,   939,     0,   948,     0,
     514,   515,   427,   450,   451,   452,   453,   454,     0,     0,
       0,     0,     0,   455,     0,   457,     0,     0,     0,   460,
       0,   610,     0,     0,     0,     0,     0,   462,     0,     0,
       0,     0,     0,   465,     0,     0,   466,     0,     0,   467,
       0,     0,     0,   470,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   577,     0,     0,   474,   475,     0,   328,
     329,   330,     0,   332,   333,   334,   335,   336,   476,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
       0,   350,   351,   352,     0,     0,   355,   356,   357,   358,
     477,   478,   578,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   481,   482,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,   486,
     487,   488,   489,   490,     0,   491,     0,   492,   493,   494,
     495,     0,     0,     0,   496,   497,   498,   499,   500,   501,
     502,    63,   579,   504,   505,     0,     0,     0,     0,     0,
       0,   506,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   611,     0,     0,   507,   508,
     509,     0,    15,     0,     0,   510,   511,     0,     0,     0,
     444,   445,     0,  1194,     0,   513,     0,   514,   515,   613,
     450,   451,   452,   453,   454,     0,     0,     0,     0,     0,
     455,     0,   457,     0,     0,     0,   460,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,     0,
     465,     0,     0,   466,     0,     0,   467,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,     0,     0,   474,   475,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   476,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   477,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,     0,     0,     0,     0,     0,
       0,     0,   657,     0,     0,     0,     0,     0,   483,   484,
     485,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,   489,
     490,     0,   491,     0,   492,   493,   494,   495,     0,     0,
       0,   496,   497,   498,   499,   500,   501,   502,    63,   579,
     504,   505,     0,     0,     0,     0,     0,     0,   506,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   507,   508,   509,     0,    15,
       0,     0,   510,   511,     0,     0,   444,   445,     0,     0,
     512,     0,   513,     0,   514,   515,   450,   451,   452,   453,
     454,     0,     0,     0,     0,     0,   455,     0,   457,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,   465,     0,     0,   466,
       0,     0,   467,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,   474,
     475,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   476,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   477,   478,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,     0,     0,     0,     0,     0,     0,     0,   687,     0,
       0,     0,     0,     0,   483,   484,   485,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   491,     0,
     492,   493,   494,   495,     0,     0,     0,   496,   497,   498,
     499,   500,   501,   502,    63,   579,   504,   505,     0,     0,
       0,     0,     0,     0,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   508,   509,     0,    15,     0,     0,   510,   511,
       0,     0,   444,   445,     0,     0,   512,     0,   513,     0,
     514,   515,   450,   451,   452,   453,   454,     0,     0,     0,
       0,     0,   455,  1575,   457,   458,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,     0,
       0,     0,   465,     0,     0,   466,     0,     0,   467,   468,
       0,     0,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,   474,   475,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   476,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   477,
     478,   578,  1576,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   481,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,   489,   490,     0,   491,     0,   492,   493,   494,   495,
       0,     0,     0,   496,   497,   498,   499,   500,   501,   502,
      63,   579,   504,   505,     0,     0,     0,     0,     0,     0,
     506,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,   509,
       0,    15,     0,     0,   510,   511,     0,     0,   444,   445,
       0,     0,   512,     0,   513,     0,   514,   515,   450,   451,
     452,   453,   454,     0,     0,     0,     0,     0,   455,     0,
     457,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,   462,     0,     0,     0,     0,     0,   465,     0,
       0,   466,     0,     0,   467,     0,     0,     0,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,     0,
       0,   474,   475,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   477,   478,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,     0,
     491,     0,   492,   493,   494,   495,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,    63,   579,   504,   505,
       0,     0,     0,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,   509,     0,    15,     0,     0,
     510,   511,     0,     0,   444,   445,     0,     0,   512,     0,
     513,     0,   514,   515,   450,   451,   452,   453,   454,     0,
       0,     0,     0,     0,   455,     0,   457,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,     0,   462,     0,
       0,     0,     0,     0,   465,     0,     0,   466,     0,     0,
     467,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,     0,     0,   474,   475,   987,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   476,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   477,   478,   578,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,   489,   490,     0,   491,   730,   492,   493,
     494,   495,     0,     0,     0,   496,   497,   498,   499,   500,
     501,   502,   731,   579,   504,   505,     0,     0,     0,     0,
       0,     0,   506,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   507,
     508,   509,     0,    15,     0,     0,   510,   511,     0,     0,
     444,   445,     0,     0,   988,     0,   513,   989,   514,   515,
     450,   451,   452,   453,   454,     0,     0,     0,     0,     0,
     455,     0,   457,     0,     0,   418,   460,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,     0,
     465,     0,     0,   466,     0,     0,   467,     0,     0,     0,
     470,     0,   419,   420,     0,     0,     0,     0,     0,     0,
     577,     0,     0,   474,   475,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   476,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   477,   478,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,     0,   421,     0,     0,     0,
     422,     0,     0,     0,     0,     0,     0,     0,  1008,  1009,
    1010,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,   489,
     490,     0,   491,     0,   492,   493,   494,   495,     0,     0,
       0,   496,   497,   498,   499,   500,   501,   502,    63,   579,
     504,   505,     0,     0,     0,     0,     0,     0,   506,     0,
       0,     0,     0,     0,     0,   423,     0,     0,     0,   424,
       0,  1340,   425,     0,     0,   507,   508,   509,     0,    15,
       0,     0,   510,   511,     0,     0,     0,   426,   444,   445,
     512,     0,   513,   427,   514,   515,   725,     0,   450,   451,
     452,   453,   454,     0,     0,     0,     0,     0,   455,     0,
     457,     0,     0,   418,   460,     0,     0,     0,     0,     0,
       0,     0,   462,     0,     0,     0,     0,     0,   465,     0,
       0,   466,   726,     0,   467,     0,     0,     0,   470,     0,
     419,   420,     0,     0,     0,     0,     0,     0,   577,     0,
       0,   474,   475,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   477,   478,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,     0,   421,     0,     0,     0,   422,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,     0,
     491,     0,   492,   493,   494,   495,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,    63,   579,   504,   505,
       0,     0,     0,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,   423,     0,     0,     0,   424,     0,  1345,
     425,     0,     0,   507,   508,   509,     0,    15,     0,     0,
     510,   511,     0,     0,     0,   426,   444,   445,   512,   614,
     513,   427,   514,   515,   725,     0,   450,   451,   452,   453,
     454,     0,     0,     0,     0,     0,   455,     0,   457,     0,
       0,   418,   460,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,   465,     0,     0,   466,
     726,     0,   467,     0,     0,     0,   470,     0,   419,   420,
       0,     0,     0,     0,     0,     0,   577,     0,     0,   474,
     475,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   476,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   477,   478,   578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,     0,   421,     0,     0,     0,   422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   491,   730,
     492,   493,   494,   495,     0,     0,     0,   496,   497,   498,
     499,   500,   501,   502,   731,   579,   504,   505,     0,     0,
       0,     0,     0,     0,   506,     0,     0,     0,     0,     0,
       0,   423,     0,     0,     0,   424,     0,  1346,   425,     0,
       0,   507,   508,   509,     0,    15,     0,     0,   510,   511,
       0,     0,     0,   426,   444,   445,   512,     0,   513,   427,
     514,   515,   725,     0,   450,   451,   452,   453,   454,     0,
       0,     0,     0,     0,   455,     0,   457,     0,     0,   418,
     460,     0,     0,     0,     0,     0,     0,     0,   462,     0,
       0,     0,     0,     0,   465,     0,     0,   466,   726,     0,
     467,     0,     0,     0,   470,     0,   419,   420,     0,     0,
       0,     0,     0,     0,   577,     0,     0,   474,   475,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   476,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   477,   478,   578,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,     0,
     421,     0,     0,     0,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,   489,   490,     0,   491,     0,   492,   493,
     494,   495,     0,     0,     0,   496,   497,   498,   499,   500,
     501,   502,    63,   579,   504,   505,     0,     0,     0,     0,
       0,     0,   506,     0,     0,     0,     0,     0,     0,   423,
       0,     0,     0,   424,     0,  1347,   425,     0,     0,   507,
     508,   509,     0,    15,     0,     0,   510,   511,     0,     0,
       0,   426,   444,   445,   512,   848,   513,   427,   514,   515,
     725,     0,   450,   451,   452,   453,   454,     0,     0,     0,
       0,     0,   455,     0,   457,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,     0,
       0,     0,   465,     0,     0,   466,   726,     0,   467,     0,
       0,     0,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,   474,   475,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   476,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   477,
     478,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   481,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,   489,   490,     0,   491,     0,   492,   493,   494,   495,
       0,     0,     0,   496,   497,   498,   499,   500,   501,   502,
      63,   579,   504,   505,     0,     0,     0,     0,     0,     0,
     506,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,   509,
       0,    15,     0,     0,   510,   511,     0,     0,   444,   445,
       0,     0,   512,     0,   513,     0,   514,   515,   450,   451,
     452,   453,   454,     0,     0,     0,     0,     0,   455,     0,
     457,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,   462,     0,     0,     0,     0,     0,   465,     0,
       0,   466,     0,     0,   467,     0,     0,     0,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,     0,
       0,   474,   475,  1135,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   477,   478,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,     0,
     491,   730,   492,   493,   494,   495,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,   731,   579,   504,   505,
       0,     0,     0,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,   509,     0,    15,     0,     0,
     510,   511,     0,     0,   444,   445,     0,     0,   512,     0,
     513,     0,   514,   515,   450,   451,   452,   453,   454,     0,
       0,     0,     0,     0,   455,     0,   457,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,     0,   462,     0,
       0,     0,     0,     0,   465,     0,     0,   466,     0,     0,
     467,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,     0,     0,   474,   475,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   476,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   477,   478,   578,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,   489,   490,     0,   491,   730,   492,   493,
     494,   495,     0,     0,     0,   496,   497,   498,   499,   500,
     501,   502,   731,   579,   504,   505,     0,     0,     0,     0,
       0,     0,   506,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   507,
     508,   509,     0,    15,     0,     0,   510,   511,     0,     0,
     444,   445,     0,     0,   512,     0,   513,  1196,   514,   515,
     450,   451,   452,   453,   454,     0,     0,     0,     0,     0,
     455,     0,   457,     0,     0,     0,   460,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,     0,
     465,     0,     0,   466,     0,     0,   467,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,     0,     0,   474,   475,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   476,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   477,   478,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,   489,
     490,     0,   491,   730,   492,   493,   494,   495,     0,     0,
       0,   496,   497,   498,   499,   500,   501,   502,   731,   579,
     504,   505,     0,     0,     0,     0,     0,     0,   506,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   507,   508,   509,     0,    15,
       0,     0,   510,   511,     0,     0,   444,   445,     0,     0,
     512,     0,   513,  1210,   514,   515,   450,   451,   452,   453,
     454,     0,     0,     0,     0,     0,   455,     0,   457,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,   465,     0,     0,   466,
       0,     0,   467,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,   474,
     475,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   476,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   477,   478,   578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   491,     0,
     492,   493,   494,   495,     0,     0,     0,   496,   497,   498,
     499,   500,   501,   502,    63,   579,   504,   505,     0,     0,
       0,     0,     0,     0,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   508,   509,     0,    15,     0,     0,   510,   511,
       0,     0,   444,   445,     0,     0,   512,   614,   513,     0,
     514,   515,   450,   451,   452,   453,   454,     0,     0,     0,
       0,     0,   455,     0,   457,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,     0,
       0,     0,   465,     0,     0,   466,     0,     0,   467,     0,
       0,     0,   470,     0,     0,     0,     0,     0,   665,     0,
       0,     0,   577,     0,     0,   474,   475,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   476,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   477,
     478,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   481,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,   489,   490,     0,   491,     0,   492,   493,   494,   495,
       0,     0,     0,   496,   497,   498,   499,   500,   501,   502,
      63,   579,   504,   505,     0,     0,     0,     0,     0,     0,
     506,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,   509,
       0,    15,     0,     0,   510,   511,     0,     0,   444,   445,
       0,     0,   512,     0,   513,     0,   514,   515,   450,   451,
     452,   453,   454,     0,     0,     0,     0,     0,   455,     0,
     457,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,   462,     0,     0,     0,     0,     0,   465,     0,
       0,   466,     0,     0,   467,     0,     0,     0,   470,     0,
       0,   676,     0,     0,     0,     0,     0,     0,   577,     0,
       0,   474,   475,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   477,   478,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,     0,
     491,     0,   492,   493,   494,   495,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,    63,   579,   504,   505,
       0,     0,     0,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,   509,     0,    15,     0,     0,
     510,   511,     0,     0,     0,     0,   444,   445,   512,     0,
     513,     0,   514,   515,   708,     0,   450,   451,   452,   453,
     454,     0,     0,     0,     0,     0,   455,     0,   457,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,   465,     0,     0,   466,
       0,     0,   467,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,   474,
     475,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   476,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   477,   478,   578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   491,     0,
     492,   493,   494,   495,     0,     0,     0,   496,   497,   498,
     499,   500,   501,   502,    63,   579,   504,   505,     0,     0,
       0,     0,     0,     0,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   508,   509,     0,    15,     0,     0,   510,   511,
       0,     0,   444,   445,     0,     0,   512,     0,   513,     0,
     514,   515,   450,   451,   452,   453,   454,     0,     0,     0,
       0,     0,   455,     0,   457,     0,     0,   418,   460,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,     0,
       0,     0,   465,     0,     0,   466,     0,     0,   467,     0,
       0,     0,   470,     0,   419,   420,     0,     0,     0,     0,
       0,     0,   577,     0,     0,   474,   475,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   476,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   477,
     478,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   481,   482,     0,   421,     0,
       0,     0,   422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,   489,   490,     0,   491,     0,   492,   493,   494,   495,
       0,     0,     0,   496,   497,   498,   499,   500,   501,   502,
      63,   579,   504,   505,     0,     0,     0,     0,     0,     0,
     506,     0,     0,     0,     0,     0,     0,   423,     0,     0,
       0,   424,     0,  1348,   425,   712,     0,   507,   508,   509,
       0,    15,     0,     0,   510,   511,     0,     0,     0,   426,
     444,   445,   512,     0,   513,   427,   514,   515,   716,     0,
     450,   451,   452,   453,   454,     0,     0,     0,     0,     0,
     455,     0,   457,     0,     0,     0,   460,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,     0,
     465,     0,     0,   466,     0,     0,   467,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,     0,     0,   474,   475,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   476,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   477,   478,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,   489,
     490,     0,   491,     0,   492,   493,   494,   495,     0,     0,
       0,   496,   497,   498,   499,   500,   501,   502,    63,   579,
     504,   505,     0,     0,     0,     0,     0,     0,   506,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   507,   508,   509,     0,    15,
       0,     0,   510,   511,     0,     0,   444,   445,     0,     0,
     512,     0,   513,     0,   514,   515,   450,   451,   452,   453,
     454,     0,     0,     0,     0,     0,   455,     0,   457,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,   465,     0,     0,   466,
       0,     0,   467,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,   474,
     475,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   476,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   477,   478,   578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   491,   730,
     492,   493,   494,   495,     0,     0,     0,   496,   497,   498,
     499,   500,   501,   502,   731,   579,   504,   505,     0,     0,
       0,     0,     0,     0,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   508,   509,     0,    15,     0,     0,   510,   511,
       0,     0,   444,   445,     0,     0,   512,     0,   513,     0,
     514,   515,   450,   451,   452,   453,   454,     0,     0,  1067,
       0,     0,   455,     0,   457,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,     0,
       0,     0,   465,     0,     0,   466,     0,     0,   467,     0,
       0,     0,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,   474,   475,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   476,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   477,
     478,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   481,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,   489,   490,     0,   491,     0,   492,   493,   494,   495,
       0,     0,     0,   496,   497,   498,   499,   500,   501,   502,
      63,   579,   504,   505,     0,     0,     0,     0,     0,     0,
     506,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,   509,
       0,    15,     0,     0,   510,   511,     0,     0,   444,   445,
       0,     0,   512,     0,   513,     0,   514,   515,   450,   451,
     452,   453,   454,     0,     0,     0,     0,     0,   455,     0,
     457,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,   462,     0,     0,     0,     0,     0,   465,     0,
       0,   466,     0,     0,   467,     0,     0,     0,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,     0,
       0,   474,   475,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   477,   478,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,     0,
     491,     0,   492,   493,   494,   495,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,    63,   579,   504,   505,
       0,     0,     0,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,   509,     0,    15,     0,     0,
     510,   511,     0,     0,   444,   445,     0,     0,   512,     0,
     513,  1088,   514,   515,   450,   451,   452,   453,   454,     0,
       0,     0,     0,     0,   455,     0,   457,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,     0,   462,     0,
       0,     0,     0,     0,   465,     0,     0,   466,     0,     0,
     467,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,     0,     0,   474,   475,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   476,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   477,   478,   578,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,   489,   490,     0,   491,     0,   492,   493,
     494,   495,     0,     0,     0,   496,   497,   498,   499,   500,
     501,   502,    63,   579,   504,   505,     0,     0,     0,     0,
       0,     0,   506,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1139,     0,   507,
     508,   509,     0,    15,     0,     0,   510,   511,     0,     0,
     444,   445,     0,     0,   512,     0,   513,     0,   514,   515,
     450,   451,   452,   453,   454,     0,     0,     0,     0,     0,
     455,     0,   457,     0,     0,     0,   460,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,     0,
     465,     0,     0,   466,     0,     0,   467,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,     0,     0,   474,   475,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   476,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   477,   478,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,   489,
     490,     0,   491,     0,   492,   493,   494,   495,     0,     0,
       0,   496,   497,   498,   499,   500,   501,   502,    63,   579,
     504,   505,     0,     0,     0,     0,     0,     0,   506,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   507,   508,   509,     0,    15,
       0,     0,   510,   511,     0,     0,   444,   445,     0,     0,
     512,     0,   513,  1415,   514,   515,   450,   451,   452,   453,
     454,     0,     0,     0,     0,     0,   455,     0,   457,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,   465,     0,     0,   466,
       0,     0,   467,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,   474,
     475,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   476,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   477,   478,   578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   491,     0,
     492,   493,   494,   495,     0,     0,     0,   496,   497,   498,
     499,   500,   501,   502,    63,   579,   504,   505,     0,     0,
       0,     0,     0,     0,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   508,   509,     0,    15,     0,     0,   510,   511,
       0,     0,   444,   445,     0,     0,  1424,     0,   513,  1425,
     514,   515,   450,   451,   452,   453,   454,     0,     0,     0,
       0,     0,   455,     0,   457,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,     0,
       0,     0,   465,     0,     0,   466,     0,     0,   467,     0,
       0,     0,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,   474,   475,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   476,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   477,
     478,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   481,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,   489,   490,     0,   491,     0,   492,   493,   494,   495,
       0,     0,     0,   496,   497,   498,   499,   500,   501,   502,
      63,   579,   504,   505,     0,     0,     0,     0,     0,     0,
     506,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,   509,
       0,    15,     0,     0,   510,   511,     0,     0,   444,   445,
       0,     0,   512,     0,   513,  1430,   514,   515,   450,   451,
     452,   453,   454,     0,     0,     0,     0,     0,   455,     0,
     457,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,   462,     0,     0,     0,     0,     0,   465,     0,
       0,   466,     0,     0,   467,     0,     0,     0,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,     0,
       0,   474,   475,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   477,   478,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,     0,
     491,     0,   492,   493,   494,   495,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,    63,   579,   504,   505,
       0,     0,     0,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,   509,     0,    15,     0,     0,
     510,   511,     0,     0,   444,   445,     0,     0,   512,     0,
     513,  1472,   514,   515,   450,   451,   452,   453,   454,     0,
       0,     0,     0,     0,   455,     0,   457,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,     0,   462,     0,
       0,     0,     0,     0,   465,     0,     0,   466,     0,     0,
     467,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   577,     0,     0,   474,   475,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   476,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   477,   478,   578,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,   489,   490,     0,   491,     0,   492,   493,
     494,   495,     0,     0,     0,   496,   497,   498,   499,   500,
     501,   502,    63,   579,   504,   505,     0,     0,     0,     0,
       0,     0,   506,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   507,
     508,   509,     0,    15,     0,     0,   510,   511,     0,     0,
     444,   445,     0,     0,   512,     0,   513,  1553,   514,   515,
     450,   451,   452,   453,   454,     0,     0,     0,     0,     0,
     455,     0,   457,     0,     0,     0,   460,     0,     0,     0,
       0,     0,     0,     0,   462,     0,     0,     0,     0,     0,
     465,     0,     0,   466,     0,     0,   467,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,     0,     0,   474,   475,     0,   328,   329,   330,     0,
     332,   333,   334,   335,   336,   476,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,     0,   350,   351,
     352,     0,     0,   355,   356,   357,   358,   477,   478,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,     0,     0,     0,     0,     0,
       0,     0,  1590,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,   486,   487,   488,   489,
     490,     0,   491,     0,   492,   493,   494,   495,     0,     0,
       0,   496,   497,   498,   499,   500,   501,   502,    63,   579,
     504,   505,     0,     0,     0,     0,     0,     0,   506,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   507,   508,   509,     0,    15,
       0,     0,   510,   511,     0,     0,   444,   445,     0,     0,
     512,     0,   513,     0,   514,   515,   450,   451,   452,   453,
     454,     0,     0,     0,     0,     0,   455,     0,   457,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
     462,     0,     0,     0,     0,     0,   465,     0,     0,   466,
       0,     0,   467,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   577,     0,     0,   474,
     475,     0,   328,   329,   330,     0,   332,   333,   334,   335,
     336,   476,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,     0,   350,   351,   352,     0,     0,   355,
     356,   357,   358,   477,   478,   578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,     0,     0,     0,     0,     0,     0,     0,  1631,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   491,     0,
     492,   493,   494,   495,     0,     0,     0,   496,   497,   498,
     499,   500,   501,   502,    63,   579,   504,   505,     0,     0,
       0,     0,     0,     0,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   508,   509,     0,    15,     0,     0,   510,   511,
       0,     0,   444,   445,     0,     0,   512,     0,   513,     0,
     514,   515,   450,   451,   452,   453,   454,     0,     0,     0,
       0,     0,   455,     0,   457,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,   462,     0,     0,     0,
       0,     0,   465,     0,     0,   466,     0,     0,   467,     0,
       0,     0,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   577,     0,     0,   474,   475,     0,   328,   329,
     330,     0,   332,   333,   334,   335,   336,   476,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,     0,
     350,   351,   352,     0,     0,   355,   356,   357,   358,   477,
     478,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   481,   482,     0,     0,     0,
       0,     0,     0,     0,  1632,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,   486,   487,
     488,   489,   490,     0,   491,     0,   492,   493,   494,   495,
       0,     0,     0,   496,   497,   498,   499,   500,   501,   502,
      63,   579,   504,   505,     0,     0,     0,     0,     0,     0,
     506,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,   509,
       0,    15,     0,     0,   510,   511,     0,     0,   444,   445,
       0,     0,   512,     0,   513,     0,   514,   515,   450,   451,
     452,   453,   454,     0,     0,     0,     0,     0,   455,     0,
     457,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,   462,     0,     0,     0,     0,     0,   465,     0,
       0,   466,     0,     0,   467,     0,     0,     0,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   577,     0,
       0,   474,   475,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,   477,   478,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,   486,   487,   488,   489,   490,     0,
     491,     0,   492,   493,   494,   495,     0,     0,     0,   496,
     497,   498,   499,   500,   501,   502,    63,   579,   504,   505,
       0,     0,     0,     0,     0,     0,   506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   508,   509,     0,    15,     0,     0,
     510,   511,     0,     0,   444,   445,     0,     0,   512,     0,
     513,     0,   514,   515,   450,   451,   452,   453,   454,     0,
       0,     0,     0,     0,   455,     0,   457,     0,     0,   418,
     460,     0,     0,     0,     0,     0,     0,     0,   462,     0,
       0,     0,     0,     0,   465,     0,     0,   466,     0,     0,
     467,     0,     0,     0,   470,     0,   419,   420,     0,     0,
       0,     0,     0,     0,   577,     0,     0,   474,   475,     0,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   476,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,   350,   351,   352,     0,     0,   355,   356,   357,
     358,   477,   478,   578,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,     0,
     421,     0,   -76,     0,   422,     0,     0,     0,     0,     0,
       0,     0,     0,   755,   756,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     486,   487,   488,   489,   490,     0,   491,     0,   492,   493,
     494,   495,     0,     0,     0,   496,   497,   498,   499,   500,
     501,   502,    63,   579,   504,   505,     0,     0,     0,     0,
     755,   756,   506,     0,     0,     0,     0,     0,     0,   423,
       0,     0,     0,   424,     0,  1350,   425,     0,     0,   507,
     508,   509,     0,    15,     0,     0,   510,   511,     0,     0,
       0,   426,     0,     0,  1398,     0,   513,   427,   514,   515,
     757,   758,   759,   760,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,   779,   780,     0,     0,     0,     0,   781,   782,
     783,   784,   785,   786,     0,     0,   787,   788,   789,   790,
     791,   792,   793,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   765,   766,   767,   768,   769,
       0,     0,   772,   773,   774,   775,     0,   777,   778,   779,
     780,     0,     0,     0,     0,   781,     0,   783,   784,     0,
       0,     0,     0,   787,   794,   795,     0,   796,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   706,     0,     0,
       0,     0,     0,   322,     0,     0,   806,   807,     0,   323,
       0,   514,   692,     0,     0,   324,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,     0,     0,     0,     0,
       0,     0,     0,   326,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,     0,     0,     0,     0,   327,     0,
       0,     0,     0,   806,   807,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,     0,     0,
       0,     0,   323,     0,     0,     0,     0,     0,   324,     0,
       0,     0,     0,     0,     0,     0,     0,    62,   325,     0,
       0,     0,     0,     0,     0,     0,   326,     0,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   327,     0,     0,     0,     0,     0,    63,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   362,     0,   418,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   323,     0,     0,   418,     0,
       0,   324,     0,     0,     0,     0,     0,     0,     0,     0,
      62,   325,   419,   420,     0,     0,     0,     0,     0,   326,
       0,     0,     0,   361,     0,   419,   420,     0,     0,     0,
       0,     0,     0,     0,   327,     0,     0,     0,     0,     0,
      63,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,     0,     0,   421,     0,     0,   418,
     422,     0,     0,     0,   418,     0,   362,     0,   617,   421,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   419,   420,     0,     0,
       0,   419,   420,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,     0,   322,     0,
       0,     0,     0,   622,   323,   423,     0,     0,     0,   424,
     324,  1456,   425,     0,     0,    14,     0,     0,   423,     0,
     325,   623,   424,     0,  1461,   425,     0,   426,   326,     0,
     421,     0,     0,   427,   422,   421,     0,     0,     0,   422,
     426,     0,     0,   327,     0,     0,   427,     0,     0,   362,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   423,
       0,   322,     0,   424,   423,  1495,   425,   323,   424,     0,
    1592,   425,     0,   324,     0,     0,     0,     0,     0,     0,
       0,   426,    62,   325,     0,     0,   426,   427,     0,     0,
       0,   326,   427,     0,     0,   361,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   327,     0,     0,     0,
       0,     0,    63,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,     0,   362,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
     323,     0,     0,     0,     0,     0,   324,     0,     0,     0,
       0,     0,     0,     0,     0,    62,   325,     0,     0,     0,
       0,     0,     0,     0,   326,     0,     0,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,   755,   756,   327,
       0,     0,     0,     0,     0,    63,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,     0,
     755,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,   765,   766,   767,   768,   769,     0,     0,   772,
     773,   774,   775,     0,   777,   778,   779,   780,   622,     0,
       0,     0,   781,     0,   783,   784,     0,     0,     0,     0,
     787,     0,   789,     0,     0,     0,     0,   888,   889,   890,
     891,   892,   893,   894,   895,   765,   766,   767,   768,   769,
     896,   897,   772,   773,   774,   775,   898,   777,   778,   779,
     780,     0,     0,     0,   362,   781,   782,   783,   784,   899,
     900,   755,   756,   787,   788,   789,   901,   902,   903,   793,
       0,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     805,     0,     0,     0,    13,     0,     0,     0,     0,     0,
     806,   807,     0,     0,     0,     0,   755,   756,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,   904,   795,     0,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   806,   807,     0,     0,     0,   514,   692,
       0,     0,     0,     0,     0,     0,     0,     0,   888,   889,
     890,   891,   892,   893,   894,   895,   765,   766,   767,   768,
     769,   896,   897,   772,   773,   774,   775,   898,   777,   778,
     779,   780,  -379,     0,     0,     0,   781,   782,   783,   784,
     899,   900,     0,     0,   787,   788,   789,   901,   902,   903,
     793,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,   755,   756,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,     0,     0,     0,     0,
       0,     0,   904,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   755,   756,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,     0,   514,
     692,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,   820,     0,     0,     0,     0,     0,     0,
       0,     0,   765,   766,   767,   768,   769,     0,     0,   772,
     773,   774,   775,     0,   777,   778,   779,   780,     0,     0,
       0,     0,   781,     0,   783,   784,     0,     0,     0,     0,
     787,   788,   789,     0,     0,     0,   793,     0,     0,     0,
     765,   766,   767,   768,   769,     0,     0,   772,   773,   774,
     775,     0,   777,   778,   779,   780,   755,   756,     0,     0,
     781,     0,   783,   784,     0,     0,     0,     0,   787,   788,
     789,     0,     0,     0,   793,     0,     0,     0,     0,   795,
       0,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   755,   756,     0,     0,     0,     0,     0,     0,     0,
     806,   807,     0,     0,   830,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,     0,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,   807,
       0,     0,   845,     0,     0,     0,     0,     0,     0,     0,
       0,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,     0,     0,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,   755,   756,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     755,   756,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,   859,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,  1171,     0,
       0,     0,     0,     0,     0,     0,     0,   765,   766,   767,
     768,   769,     0,     0,   772,   773,   774,   775,     0,   777,
     778,   779,   780,     0,     0,     0,     0,   781,     0,   783,
     784,     0,     0,     0,     0,   787,   788,   789,     0,     0,
       0,   793,     0,     0,     0,   765,   766,   767,   768,   769,
       0,     0,   772,   773,   774,   775,     0,   777,   778,   779,
     780,   755,   756,     0,     0,   781,     0,   783,   784,     0,
       0,     0,     0,   787,   788,   789,     0,     0,     0,   793,
       0,     0,     0,     0,   795,     0,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   755,   756,     0,     0,
       0,     0,     0,     0,     0,   806,   807,     0,     0,  1175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,     0,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   806,   807,     0,     0,  1177,     0,     0,
       0,     0,     0,     0,     0,     0,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,     0,     0,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,   755,   756,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   755,   756,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,  1186,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,  1187,     0,     0,     0,     0,     0,     0,
       0,     0,   765,   766,   767,   768,   769,     0,     0,   772,
     773,   774,   775,     0,   777,   778,   779,   780,     0,     0,
       0,     0,   781,     0,   783,   784,     0,     0,     0,     0,
     787,   788,   789,     0,     0,     0,   793,     0,     0,     0,
     765,   766,   767,   768,   769,     0,     0,   772,   773,   774,
     775,     0,   777,   778,   779,   780,   755,   756,     0,     0,
     781,     0,   783,   784,     0,     0,     0,     0,   787,   788,
     789,     0,     0,     0,   793,     0,     0,     0,     0,   795,
       0,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   755,   756,     0,     0,     0,     0,     0,     0,     0,
     806,   807,     0,     0,  1188,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,     0,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,   807,
       0,     0,  1189,     0,     0,     0,     0,     0,     0,     0,
       0,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,     0,     0,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,   755,   756,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     755,   756,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,  1190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,  1191,     0,
       0,     0,     0,     0,     0,     0,     0,   765,   766,   767,
     768,   769,     0,     0,   772,   773,   774,   775,     0,   777,
     778,   779,   780,     0,     0,     0,     0,   781,     0,   783,
     784,     0,     0,     0,     0,   787,   788,   789,     0,     0,
       0,   793,     0,     0,     0,   765,   766,   767,   768,   769,
       0,     0,   772,   773,   774,   775,     0,   777,   778,   779,
     780,   755,   756,     0,     0,   781,     0,   783,   784,     0,
       0,     0,     0,   787,   788,   789,     0,     0,     0,   793,
       0,     0,     0,     0,   795,     0,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   755,   756,     0,     0,
       0,     0,     0,     0,     0,   806,   807,     0,     0,  1320,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,     0,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   806,   807,     0,     0,  1332,     0,     0,
       0,     0,     0,     0,     0,     0,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,     0,     0,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,   755,   756,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   755,   756,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,  1335,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,  1382,     0,     0,     0,     0,     0,     0,
       0,     0,   765,   766,   767,   768,   769,     0,     0,   772,
     773,   774,   775,     0,   777,   778,   779,   780,     0,     0,
       0,     0,   781,     0,   783,   784,     0,     0,     0,     0,
     787,   788,   789,     0,     0,     0,   793,     0,     0,     0,
     765,   766,   767,   768,   769,     0,     0,   772,   773,   774,
     775,     0,   777,   778,   779,   780,   755,   756,     0,     0,
     781,     0,   783,   784,     0,     0,     0,     0,   787,   788,
     789,     0,     0,     0,   793,     0,     0,     0,     0,   795,
       0,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   755,   756,     0,     0,     0,     0,     0,     0,     0,
     806,   807,     0,     0,  1493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,     0,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,   807,
       0,     0,  1494,     0,     0,     0,     0,     0,     0,     0,
       0,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,     0,     0,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,   755,   756,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     755,   756,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,  1515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,  1517,     0,
       0,     0,     0,     0,     0,     0,     0,   765,   766,   767,
     768,   769,     0,     0,   772,   773,   774,   775,     0,   777,
     778,   779,   780,     0,     0,     0,     0,   781,     0,   783,
     784,     0,     0,     0,     0,   787,   788,   789,     0,     0,
       0,   793,     0,     0,     0,   765,   766,   767,   768,   769,
       0,     0,   772,   773,   774,   775,     0,   777,   778,   779,
     780,   755,   756,     0,     0,   781,     0,   783,   784,     0,
       0,     0,     0,   787,   788,   789,     0,     0,     0,   793,
       0,     0,     0,     0,   795,     0,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   755,   756,     0,     0,
       0,     0,     0,     0,     0,   806,   807,     0,     0,  1519,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,     0,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   806,   807,     0,     0,  1523,     0,     0,
       0,     0,     0,     0,     0,     0,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,     0,     0,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,   755,   756,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   755,   756,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,  1571,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,  1595,     0,     0,     0,     0,     0,     0,
       0,     0,   765,   766,   767,   768,   769,     0,     0,   772,
     773,   774,   775,     0,   777,   778,   779,   780,     0,     0,
       0,     0,   781,     0,   783,   784,     0,     0,     0,     0,
     787,   788,   789,     0,     0,     0,   793,     0,     0,     0,
     765,   766,   767,   768,   769,     0,     0,   772,   773,   774,
     775,     0,   777,   778,   779,   780,   755,   756,     0,     0,
     781,     0,   783,   784,     0,     0,     0,     0,   787,   788,
     789,     0,     0,     0,   793,     0,     0,     0,     0,   795,
       0,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   755,   756,     0,     0,     0,     0,     0,     0,     0,
     806,   807,     0,     0,  1596,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,     0,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,   807,
       0,     0,  1598,     0,     0,     0,     0,     0,     0,     0,
       0,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,     0,     0,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,   755,   756,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     755,   756,     0,     0,     0,     0,     0,     0,     0,   806,
     807,     0,     0,  1605,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,  1608,     0,
       0,     0,     0,     0,     0,     0,     0,   765,   766,   767,
     768,   769,     0,     0,   772,   773,   774,   775,     0,   777,
     778,   779,   780,     0,     0,     0,     0,   781,     0,   783,
     784,     0,     0,     0,     0,   787,   788,   789,     0,     0,
       0,   793,     0,     0,     0,   765,   766,   767,   768,   769,
       0,     0,   772,   773,   774,   775,     0,   777,   778,   779,
     780,   755,   756,     0,     0,   781,     0,   783,   784,     0,
       0,     0,     0,   787,   788,   789,     0,     0,     0,   793,
       0,     0,     0,     0,   795,     0,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,   755,   756,     0,     0,
       0,     0,     0,     0,     0,   806,   807,     0,     0,  1617,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   795,     0,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   806,   807,     0,     0,  1692,     0,     0,
       0,     0,     0,     0,     0,     0,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,     0,     0,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,   755,   756,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   755,   756,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,  1693,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
     807,   849,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   765,   766,   767,   768,   769,     0,     0,   772,
     773,   774,   775,     0,   777,   778,   779,   780,     0,     0,
       0,     0,   781,     0,   783,   784,     0,     0,     0,     0,
     787,   788,   789,     0,     0,     0,   793,     0,     0,     0,
     765,   766,   767,   768,   769,     0,     0,   772,   773,   774,
     775,     0,   777,   778,   779,   780,   755,   756,     0,     0,
     781,     0,   783,   784,     0,     0,     0,     0,   787,   788,
     789,     0,     0,     0,   793,     0,     0,     0,     0,   795,
       0,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   755,   756,     0,     0,     0,     0,     0,     0,     0,
     806,   807,  1121,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,     0,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   806,   807,
    1272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   765,   766,   767,   768,   769,     0,     0,   772,   773,
     774,   775,     0,   777,   778,   779,   780,     0,     0,     0,
       0,   781,     0,   783,   784,     0,     0,     0,     0,   787,
     788,   789,     0,     0,     0,   793,   765,   766,   767,   768,
     769,     0,     0,   772,   773,   774,   775,     0,   777,   778,
     779,   780,   755,   756,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,     0,     0,     0,     0,     0,     0,     0,   795,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   806,
     807,  1288,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,  1422,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   765,   766,   767,
     768,   769,     0,     0,   772,   773,   774,   775,     0,   777,
     778,   779,   780,   242,   243,     0,     0,   781,     0,   783,
     784,     0,     0,     0,     0,   787,   788,   789,     0,     0,
     244,   793,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   755,   756,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   795,     0,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   806,   807,  1428,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,     0,     0,
     263,   264,   265,     0,     0,     0,     0,     0,     0,   266,
     267,   268,   269,   270,     0,     0,   271,   272,   273,   274,
     275,   276,   277,     0,     0,     0,     0,     0,   755,   756,
     765,   766,   767,   768,   769,     0,     0,   772,   773,   774,
     775,     0,   777,   778,   779,   780,     0,     0,     0,     0,
     781,     0,   783,   784,     0,     0,     0,     0,   787,   788,
     789,     0,     0,     0,   793,   278,     0,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,    50,     0,   289,
     290,     0,     0,     0,     0,     0,   291,   292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,     0,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,     0,
       0,   755,   756,   765,   766,   767,   768,   769,   806,   807,
     772,   773,   774,   775,     0,   777,   778,   779,   780,     0,
       0,     0,     0,   781,     0,   783,   784,     0,     0,     0,
       0,   787,   788,   789,     0,     0,     0,   793,   755,   756,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    13,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
     795,     0,   796,   797,   798,   799,   800,   801,   802,   803,
     804,   805,     0,     0,     0,     0,   765,   766,   767,   768,
     769,   806,   807,   772,   773,   774,   775,     0,   777,   778,
     779,   780,     0,     0,     0,     0,   781,     0,   783,   784,
       0,     0,   952,     0,   787,   788,   789,     0,     0,     0,
     793,   755,   756,   765,   766,   767,   768,   769,     0,     0,
     772,   773,   774,   775,     0,   777,   778,   779,   780,     0,
       0,     0,     0,   781,     0,   783,   784,     0,     0,  1197,
       0,   787,   788,   789,     0,     0,     0,   793,   755,   756,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     795,     0,   796,   797,   798,   799,   800,   801,   802,   803,
     804,   805,     0,     0,     0,     0,   765,   766,   767,   768,
     769,   806,   807,   772,   773,   774,   775,     0,   777,   778,
     779,   780,     0,     0,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,   755,   756,   765,   766,   767,   768,   769,     0,     0,
     772,   773,   774,   775,     0,   777,   778,   779,   780,     0,
       0,     0,     0,   781,     0,   783,   784,     0,     0,     0,
       0,   787,   788,   789,     0,     0,     0,   793,   755,   756,
       0,     0,     0,   795,  1277,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     795,  1377,   796,   797,   798,   799,   800,   801,   802,   803,
     804,   805,     0,     0,     0,     0,   765,   766,   767,   768,
     769,   806,   807,   772,   773,   774,   775,     0,   777,   778,
     779,   780,     0,     0,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
     793,   755,   756,   765,   766,   767,   768,   769,     0,     0,
     772,   773,   774,   775,     0,   777,   778,   779,   780,     0,
       0,     0,     0,   781,     0,   783,   784,     0,     0,     0,
       0,   787,   788,   789,     0,  1618,     0,   793,   755,   756,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     795,     0,   796,   797,   798,   799,   800,   801,   802,   803,
     804,   805,     0,     0,     0,     0,   765,   766,   767,   768,
     769,   806,   807,   772,   773,   774,   775,     0,   777,   778,
     779,   780,     0,     0,     0,     0,   781,     0,   783,   784,
       0,     0,     0,     0,   787,   788,   789,     0,     0,     0,
    -780,     0,     0,   765,   766,   767,   768,   769,     0,     0,
     772,   773,   774,   775,     0,   777,   778,   779,   780,     0,
       0,     0,     0,   781,     0,   783,   784,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,  1000,     0,     0,     0,     0,
       0,     0,     0,     0,   806,   807,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   799,   800,   801,   802,   803,
     804,   805,  1004,     0,     0,     0,     0,     0,     0,     0,
       0,   806,   807,     0,     0,     0,     0,   328,   329,   330,
       0,   332,   333,   334,   335,   336,   476,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,     0,   350,
     351,   352,     0,     0,   355,   356,   357,   358,     0,     0,
       0,     0,     0,     0,   328,   329,   330,     0,   332,   333,
     334,   335,   336,   476,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,     0,   350,   351,   352,     0,
       0,   355,   356,   357,   358,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1273,     0,     0,  1001,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1002,
     328,   329,   330,     0,   332,   333,   334,   335,   336,   476,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,  1005,   350,   351,   352,     0,     0,   355,   356,   357,
     358,     0,     0,   328,   329,   330,  1006,   332,   333,   334,
     335,   336,   476,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,   350,   351,   352,  1079,  1080,
     355,   356,   357,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1081,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1082,     0,     0,     0,     0,     0,     0,   192,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1274,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1275,     0,     0,     0,     0,
       0,     0,     0,  1083,  1084,   193,     0,   194,     0,   195,
     196,   197,   198,   199,     0,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,     0,   211,   212,   213,
       0,     0,   214,   215,   216,   217,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   218,   219,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   220
};

static const yytype_int16 yycheck[] =
{
       1,   584,   387,   222,   607,   456,    15,    16,   495,   664,
     533,   432,   387,   171,   744,   637,   603,   228,    19,     7,
     707,   550,   837,   394,   839,   943,   841,   431,   598,   480,
     600,   949,   602,  1199,   644,    86,   728,   924,   929,    33,
     732,     5,     4,    22,    20,   574,    15,    16,   238,    53,
      33,    19,    20,   109,  1401,    20,   127,    53,   158,    68,
      69,    70,    20,   705,    57,   707,    20,    20,   127,   143,
      63,   484,   485,   173,    20,    20,   173,   143,    57,   126,
     166,   456,   129,   130,     0,    62,    46,     5,     6,   129,
     130,     7,   160,   192,   704,    96,   706,    98,   708,   173,
     238,   110,   111,   112,   113,   480,   716,    25,   144,   145,
     146,  1572,  1599,    31,    30,   725,    32,   217,    34,     8,
     191,   192,   219,   194,    40,   127,   197,   387,   194,   400,
     431,   432,   194,   192,    50,   137,   195,   588,   102,   103,
      56,   129,   142,   217,   155,   875,   106,   191,   149,   160,
      68,    69,   220,   191,   312,    15,    16,  1504,   429,    34,
    1647,   191,   158,   163,    80,   694,   191,   214,   215,  1630,
     198,   164,   216,   229,   214,   215,   387,   173,   216,   230,
     168,   185,   218,   183,   102,   103,   102,   103,    63,   217,
     192,   184,   884,  1359,   143,   385,   456,   166,   160,   187,
     188,   189,   171,   197,   173,   173,   657,   176,    53,   220,
     400,   217,   173,   588,   197,   191,  1466,   483,   484,   485,
     480,   215,   493,   494,   203,   143,   191,   221,   920,   238,
     231,   923,   107,   191,   198,   214,   687,   191,   191,   429,
     459,   431,   160,   161,   162,   191,   191,   385,   217,   165,
     127,   441,    97,   217,   473,   173,   217,   987,   198,  1146,
     137,   136,   127,   173,   315,   183,   143,   533,   143,   185,
     152,   127,   137,   198,   155,   164,   165,   217,  1196,   160,
     198,   137,   657,  1533,  1534,   165,   214,   163,   204,   152,
     127,   173,   217,  1184,   560,   184,   185,   215,   173,   215,
     137,  1551,  1552,   493,   494,   185,   166,   183,   173,  1475,
     173,   171,   687,   173,   163,   192,   176,     5,     6,   307,
       8,     7,   310,  1559,   625,  1561,   215,   192,   588,   630,
    1566,  1567,   197,   208,   183,   922,   192,   218,   218,   220,
     753,   152,   217,   219,   220,   191,   988,   127,    36,   160,
      57,   165,  1243,   362,   158,   192,    63,   137,   968,  1609,
    1610,   572,   173,   574,    50,   127,   143,   165,   217,   173,
     216,   185,   823,  1604,   163,   137,   385,   191,   165,   194,
     751,   390,   197,   384,  1615,   199,   387,   185,   389,   173,
    1626,   400,   663,   191,   183,   661,   173,   657,   185,   589,
     590,   591,   191,   593,   594,  1135,   217,   597,  1299,   599,
     621,   601,   192,   603,   218,   686,   867,   628,   166,   107,
     429,   632,   431,   689,   643,  1656,  1657,   687,   998,   430,
     192,   191,   441,   220,   194,    57,   194,   197,   165,   197,
    1358,    63,   132,   452,   453,   191,   165,  1262,   823,   668,
    1368,   589,   590,   591,   191,   593,   594,   986,   185,   597,
     143,   599,   163,   601,   191,   869,   185,   954,   469,   152,
     216,    57,   191,   663,   157,   165,   185,    63,   965,   188,
     199,   218,   183,    57,   493,   494,   165,   753,   165,    63,
     173,   218,   867,   194,   713,   185,   686,   757,   758,   759,
     760,   761,   762,   763,   764,  1127,   185,   216,   185,  1196,
     770,   771,   191,   173,   191,   198,   776,   518,   519,   738,
     199,   173,   199,  1210,   937,   785,   786,   165,   829,   163,
     790,   791,   792,   191,   794,   536,   198,   538,   539,   540,
    1355,   542,   165,   844,   173,   174,   175,   185,   819,   183,
       5,     6,   553,   191,  1196,   217,  1198,   158,  1145,   825,
     218,   199,   185,   823,   191,   755,   756,   165,  1210,   570,
     188,   189,   173,   574,  1389,   204,   199,    68,    69,    70,
     589,   590,   591,   584,   593,   594,   173,   185,   597,   216,
     599,   781,   601,   191,   603,  1008,  1009,  1010,   216,   870,
     871,   199,   191,   185,   165,   906,   165,   867,   165,  1231,
     173,   165,   883,   165,   165,  1185,   806,   199,   619,   110,
     111,   112,   113,   165,   185,    33,   185,   216,   185,   819,
     191,   185,   191,   185,   185,   192,   195,   191,   199,   191,
     649,   912,   913,   185,   915,   199,   143,   199,   919,   191,
     921,   191,    60,    61,   663,   152,   846,   199,   195,   173,
     818,   927,   199,   853,   191,  1235,   856,   191,   191,   177,
     191,   937,   191,   191,   191,   865,   173,   686,   218,   869,
     870,   871,   191,   456,   874,  1500,   905,  1342,   191,   879,
     191,   218,   191,   883,   218,   218,  1117,   218,   173,   218,
     218,   218,   198,   191,   165,    57,   195,   177,   846,   218,
     199,    63,   173,   216,   164,   853,   124,   218,   856,   218,
     128,   217,   912,   913,   185,   915,   735,   865,   216,   919,
     191,   921,   922,   158,   184,   195,   874,   219,   220,   199,
     513,   879,  1008,  1009,  1010,    47,   195,  1013,   173,  1015,
     199,  1017,   740,  1019,    75,  1021,  1398,  1023,    79,  1025,
    1382,  1027,   177,   178,    57,    67,  1032,   957,  1034,   959,
      63,   194,    93,    94,  1040,   158,   199,    98,    99,   100,
     101,   158,  1424,   158,    57,   193,  1052,   144,  1054,   197,
     173,   199,   200,  1059,   217,  1061,   173,  1063,   173,   194,
    1066,    92,    93,   194,   199,   814,   194,   215,   199,   810,
     819,   199,   165,   221,   195,   165,   106,  1118,   199,   957,
     173,   959,   217,   173,   825,  1395,   217,    21,    22,   217,
      12,  1354,   185,  1099,   835,   185,   957,   846,   959,  1442,
     184,    23,    24,   187,   853,    57,   190,   856,   185,   185,
     192,    63,   185,   185,   191,   191,   865,  1242,   191,   191,
     869,   870,   871,  1129,   195,   874,   185,  1242,   199,   185,
     879,    35,   191,  1144,   883,   191,   144,  1067,   146,  1090,
      22,   195,  1183,   656,   657,   199,   195,   195,    35,  1544,
     199,   199,    21,    22,   217,   195,   195,   173,  1169,   199,
     199,   173,   173,   912,   913,   216,   915,   173,   681,   173,
     919,   684,   921,   922,  1104,   216,   194,    33,  1219,  1220,
     194,   217,  1614,  1506,   118,   119,   177,   178,   179,   180,
    1622,    43,   126,   198,   128,   129,   130,   131,   198,  1542,
     184,   185,   136,   187,    60,    61,   195,   217,   957,  1571,
     959,    21,    22,   198,  1144,  1145,   198,   198,  1650,   217,
    1563,  1603,  1604,  1234,   198,   217,  1104,   219,   220,  1611,
     198,   744,   198,  1615,  1616,   144,   145,   146,   198,  1169,
     198,   198,  1242,   173,   174,   175,   217,   116,   117,   118,
     119,   137,   173,  1480,   177,   178,   179,   126,  1485,   128,
     129,   130,   131,  1586,  1587,   216,  1648,   136,   124,   138,
     139,   217,   128,   173,  1656,  1657,   173,  1236,    33,   173,
     214,   215,   177,   178,   179,   184,   185,   186,   215,   217,
    1331,  1242,    10,    11,    12,   198,   809,   198,  1339,   173,
     198,   217,   198,   166,  1234,    60,    61,   198,   118,   119,
     198,   217,   217,  1636,   198,   198,   126,   198,   128,   129,
     130,   131,   198,    10,   217,   217,   136,   217,   217,    37,
    1712,   200,   201,   202,   203,   204,    66,   193,   215,   217,
     220,   197,   217,   199,   200,   214,   215,   198,  1354,   198,
    1091,    43,   218,   173,  1095,  1104,   173,   216,  1099,   215,
      21,    22,   875,   194,   217,   221,  1325,    43,   198,   124,
     217,   198,   127,   128,   198,   191,   132,    14,   192,   194,
     166,   220,   137,   184,   191,    33,    13,   216,   191,  1319,
     200,   201,   202,   203,   204,  1144,  1145,  1588,   217,   191,
    1411,    21,    22,   173,   214,   215,   218,     8,   173,   218,
     165,   199,    60,    61,   173,   173,    21,    22,   218,   173,
    1169,   184,   191,   218,   217,   173,   217,   940,     1,   218,
     185,   198,  1391,   217,   217,   948,   198,   192,   193,   217,
     217,  1319,   197,   217,   217,   200,  1637,  1638,    67,   217,
     173,  1410,   199,   217,   199,   116,   117,   118,   119,   120,
     215,   199,   123,   124,   125,   126,   221,   128,   129,   130,
     131,    43,   218,  1588,   218,   136,   124,   138,   139,   127,
     128,  1411,   173,  1413,   218,  1234,   217,  1678,   217,   137,
     218,   218,   216,   218,  1505,   216,   116,   117,   118,   119,
     120,  1242,   216,   123,   124,   125,   126,   217,   128,   129,
     130,   131,  1471,   118,   119,   173,   136,   173,   138,   139,
     217,   126,  1637,  1638,   129,   130,   131,   173,   173,   198,
     173,   136,   217,   217,   217,  1413,   197,   198,   199,   200,
     201,   202,   203,   204,   192,   193,   217,   173,   218,   197,
     217,  1292,   200,   214,   215,   218,   216,   181,   173,   216,
     173,   216,    33,  1678,   217,   217,   217,   215,   199,   173,
    1319,   218,    70,   221,  1585,  1505,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   217,   185,   217,  1588,   217,
    1520,   199,   199,   177,   214,   215,   217,   387,    53,   217,
     217,   184,   218,    33,   218,   217,   217,   397,   217,   214,
     215,   217,    33,   184,   216,   218,   406,   191,   218,   184,
    1133,   218,   218,   216,  1583,   191,   416,    21,    22,   218,
      60,    61,   218,   218,  1640,   216,   426,  1637,  1638,    60,
      61,   218,  1520,   218,   184,   435,   715,   216,   218,   216,
     218,    85,  1393,   217,     1,  1585,   446,   218,   218,    46,
     141,    88,  1411,  1497,  1413,  1498,   456,   814,   985,  1498,
    1498,   461,     1,   463,  1680,  1498,  1682,   236,  1678,  1192,
    1498,   993,   472,  1471,  1408,  1254,  1434,  1530,  1295,   440,
     480,   481,   482,  1531,   124,    56,  1209,   569,   128,  1437,
     452,  1707,  1215,   124,  1616,   495,  1478,   128,  1205,  1222,
     452,  1224,   707,   503,    -1,    -1,   506,   507,   508,   509,
     510,   511,   116,   117,   118,   119,   120,  1686,    -1,   123,
      -1,   521,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,  1505,  1270,    -1,    -1,
      -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,  1282,
     200,  1520,   193,    -1,  1287,    -1,   197,    -1,   199,   200,
      -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,   221,    -1,    -1,   215,    -1,    -1,    -1,   588,    -1,
     221,    -1,    -1,    -1,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,  1572,    -1,    -1,   616,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1585,    -1,    -1,    -1,
      -1,    -1,    -1,  1356,    -1,    -1,    21,    22,    -1,   116,
     117,   118,   119,   120,    -1,    -1,   123,   124,   125,   126,
      -1,   128,   129,   130,   131,    -1,    -1,   657,  1381,   136,
      -1,   138,   139,    -1,   664,   665,    -1,   667,    -1,    -1,
      -1,  1630,   672,    -1,    -1,    -1,    -1,    -1,   678,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   687,    -1,  1640,
      -1,    -1,    -1,    -1,    -1,  1646,    -1,    -1,   698,   699,
     700,   701,   702,   703,    -1,   705,    -1,   707,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,    -1,  1680,
      12,  1682,    -1,   118,   119,    -1,    -1,   214,   215,    21,
      22,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,    -1,    -1,    -1,  1707,   757,   758,   759,
     760,   761,   762,   763,   764,   765,   766,    -1,    -1,   769,
     770,   771,   772,   773,   774,    -1,   776,   777,    -1,   779,
     780,   781,   782,   783,   784,   785,   786,   787,   788,   789,
     790,   791,   792,   793,   794,   795,   796,   797,   798,   799,
     800,   801,   802,   803,   804,   805,    -1,   807,    -1,    -1,
      -1,   811,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
      -1,    -1,    -1,   823,    -1,  1548,    -1,    -1,    -1,   214,
     215,    -1,    -1,    -1,   116,   117,   118,   119,   120,    -1,
      -1,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
      -1,    -1,  1575,    -1,   136,    -1,   138,   139,    -1,    -1,
     860,    -1,   144,   145,   146,    -1,    -1,   867,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   876,    -1,   878,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   887,   888,   889,
     890,   891,   892,   893,   894,   895,   896,   897,   898,   899,
     900,   901,   902,   903,   904,    -1,    -1,    -1,  1631,    -1,
      -1,   193,    -1,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   758,   759,   760,   761,   762,   763,   764,
      -1,    -1,   214,   215,    -1,   770,   771,    -1,   938,   939,
      -1,   776,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     785,   786,   952,    -1,   954,   790,   791,   792,    -1,   794,
      -1,   961,    -1,    -1,    -1,   965,    -1,    -1,     5,     6,
     970,    -1,   972,   973,   974,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,   984,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1080,   118,   119,    -1,  1084,    -1,    -1,   124,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,
     197,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,  1172,    -1,    -1,    -1,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,   221,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1197,    -1,    -1,
      -1,    -1,    -1,    -1,  1204,  1205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      21,    22,  1232,  1233,    -1,    19,    -1,    -1,    -1,  1239,
      -1,    25,  1242,    -1,    -1,    -1,    -1,    31,  1248,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,  1264,    49,  1266,    -1,    -1,    -1,
      -1,  1271,    -1,    -1,    -1,    -1,    -1,  1277,    -1,    -1,
      64,  1281,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,  1322,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    33,  1342,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,
      -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1383,  1384,  1385,    -1,    33,    -1,   173,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    60,    61,    -1,    21,    22,  1419,
      -1,  1421,    -1,   214,   215,    -1,    -1,  1427,    -1,    -1,
      -1,    -1,   124,    -1,    -1,   219,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1451,    -1,    -1,  1454,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1462,  1463,  1464,    -1,    -1,    -1,    -1,  1469,
      -1,    -1,    -1,   165,  1474,    -1,    -1,  1477,   124,    -1,
    1480,  1481,   128,    -1,    -1,  1485,  1486,    -1,  1488,  1489,
      -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,  1498,   191,
      -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,    -1,
      -1,  1511,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,   215,   128,   129,   130,   131,    -1,   221,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,  1544,    -1,   150,   193,    -1,    -1,
      -1,   197,    -1,   199,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1568,   215,
      -1,    -1,    -1,    -1,    -1,   221,  1576,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1588,   193,
    1590,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    -1,    -1,    -1,    -1,    -1,  1606,  1607,    -1,    -1,
     214,   215,    -1,    -1,    -1,   219,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1627,  1628,    -1,
      -1,    -1,  1632,    -1,    -1,    -1,     1,  1637,  1638,    -1,
       5,     6,     7,    -1,     9,    10,    11,    -1,    13,    -1,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,  1665,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    42,  1678,    44,
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
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,   200,   201,
     202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,
       5,     6,    -1,   215,    -1,   217,    -1,   219,   220,   221,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
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
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,   133,   134,   135,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    52,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
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
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
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
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
       5,     6,    -1,    -1,   215,    -1,   217,   218,   219,   220,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    33,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,   119,    -1,   124,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
      -1,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,   197,
      -1,   199,   200,    -1,    -1,   200,   201,   202,    -1,   204,
      -1,    -1,   207,   208,    -1,    -1,    -1,   215,     5,     6,
     215,    -1,   217,   221,   219,   220,    13,    -1,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    33,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,   124,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,    -1,    -1,   197,    -1,   199,
     200,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,    -1,   215,     5,     6,   215,   216,
     217,   221,   219,   220,    13,    -1,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    33,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    93,    94,    95,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
     119,    -1,   124,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,    -1,    -1,   197,    -1,   199,   200,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,    -1,   215,     5,     6,   215,    -1,   217,   221,
     219,   220,    13,    -1,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
     124,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,   215,     5,     6,   215,   216,   217,   221,   219,   220,
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
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
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
      -1,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,    -1,    -1,    -1,   166,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
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
     155,    -1,   157,   158,   159,   160,   161,   162,    -1,    -1,
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
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,   216,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,
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
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,     5,     6,
      -1,    -1,   215,    -1,   217,    -1,   219,   220,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,    -1,
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
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
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,   204,    -1,    -1,   207,   208,
      -1,    -1,     5,     6,    -1,    -1,   215,    -1,   217,    -1,
     219,   220,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    33,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    68,    69,    -1,    71,    72,
      73,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      93,    94,    95,    -1,    -1,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,    -1,   124,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,    -1,
      -1,   197,    -1,   199,   200,   198,    -1,   200,   201,   202,
      -1,   204,    -1,    -1,   207,   208,    -1,    -1,    -1,   215,
       5,     6,   215,    -1,   217,   221,   219,   220,    13,    -1,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
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
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
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
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
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
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
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
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
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
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
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
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
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
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
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
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
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
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
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
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,
     155,    -1,   157,    -1,   159,   160,   161,   162,    -1,    -1,
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
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   154,   155,    -1,   157,    -1,
     159,   160,   161,   162,    -1,    -1,    -1,   166,   167,   168,
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
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   154,   155,    -1,   157,    -1,   159,   160,   161,   162,
      -1,    -1,    -1,   166,   167,   168,   169,   170,   171,   172,
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
     157,    -1,   159,   160,   161,   162,    -1,    -1,    -1,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   200,   201,   202,    -1,   204,    -1,    -1,
     207,   208,    -1,    -1,     5,     6,    -1,    -1,   215,    -1,
     217,    -1,   219,   220,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    33,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,    -1,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,    -1,
     124,    -1,    10,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   154,   155,    -1,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      21,    22,   183,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,    -1,    -1,   197,    -1,   199,   200,    -1,    -1,   200,
     201,   202,    -1,   204,    -1,    -1,   207,   208,    -1,    -1,
      -1,   215,    -1,    -1,   215,    -1,   217,   221,   219,   220,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,    -1,    -1,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   192,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,   214,   215,    -1,    25,
      -1,   219,   220,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,   214,   215,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,   173,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   219,    -1,    33,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    33,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    41,    60,    61,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,   156,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
     173,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    -1,   124,    -1,    -1,    33,
     128,    -1,    -1,    -1,    33,    -1,   219,    -1,   221,   124,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,
      -1,    60,    61,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    19,    -1,
      -1,    -1,    -1,   173,    25,   193,    -1,    -1,    -1,   197,
      31,   199,   200,    -1,    -1,   185,    -1,    -1,   193,    -1,
      41,   191,   197,    -1,   199,   200,    -1,   215,    49,    -1,
     124,    -1,    -1,   221,   128,   124,    -1,    -1,    -1,   128,
     215,    -1,    -1,    64,    -1,    -1,   221,    -1,    -1,   219,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
      -1,    19,    -1,   197,   193,   199,   200,    25,   197,    -1,
     199,   200,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   215,   143,    41,    -1,    -1,   215,   221,    -1,    -1,
      -1,    49,   221,    -1,    -1,   156,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,   173,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,    -1,
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
     214,   215,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,    -1,   219,   220,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,   136,   137,   138,   139,
     140,   141,    -1,    -1,   144,   145,   146,   147,   148,   149,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,   192,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,   219,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,    21,    22,    -1,    -1,   136,    -1,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,    -1,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    21,    22,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    21,    22,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,   193,
      -1,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     214,   215,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,   118,   119,   120,    -1,    -1,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,
      -1,   136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   116,   117,   118,   119,
     120,    -1,    -1,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    21,    22,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
     119,   120,    -1,    -1,   123,   124,   125,   126,    -1,   128,
     129,   130,   131,    21,    22,    -1,    -1,   136,    -1,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,
      38,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,   215,   216,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    -1,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,   144,   145,   146,   147,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    21,    22,
     116,   117,   118,   119,   120,    -1,    -1,   123,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    -1,    -1,
     136,    -1,   138,   139,    -1,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,   150,   193,    -1,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   163,    -1,   207,
     208,    -1,    -1,    -1,    -1,    -1,   214,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,    -1,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,    -1,
      -1,    21,    22,   116,   117,   118,   119,   120,   214,   215,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,   142,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,   142,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    21,    22,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,   150,    21,    22,
      -1,    -1,    -1,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    21,    22,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,    -1,   185,    -1,   150,    21,    22,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,    -1,    -1,    -1,    -1,   116,   117,   118,   119,
     120,   214,   215,   123,   124,   125,   126,    -1,   128,   129,
     130,   131,    -1,    -1,    -1,    -1,   136,    -1,   138,   139,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
     150,    -1,    -1,   116,   117,   118,   119,   120,    -1,    -1,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,    -1,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,   199,   200,   201,   202,
     203,   204,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,   215,    -1,    -1,    -1,    -1,    71,    72,    73,
      -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    -1,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    -1,
      -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      71,    72,    73,    -1,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   158,    93,    94,    95,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    71,    72,    73,   173,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,   129,   130,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   214,   215,    71,    -1,    73,    -1,    75,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    -1,    93,    94,    95,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   223,     0,     7,    30,    32,    34,    40,    50,    56,
      80,   102,   103,   165,   185,   204,   215,   224,   227,   236,
     238,   239,   244,   251,   275,   279,   315,   393,   400,   404,
     415,   460,   465,   470,    19,    20,   173,   267,   268,   269,
     166,   245,   246,   173,   174,   175,   204,   240,   241,   242,
     163,   183,   284,   401,   173,   219,   226,    57,    63,   396,
     396,   396,   143,   173,   301,    34,    63,   107,   136,   208,
     217,   271,   272,   273,   274,   301,   251,     5,     6,     8,
      36,   107,   412,    62,   391,   192,   191,   194,   191,   241,
      22,    57,   203,   214,   243,   396,   397,   399,   397,   391,
     471,   461,   466,   173,   143,   237,   273,   273,   273,   217,
     144,   145,   146,   191,   216,    57,    63,   280,   282,    57,
      63,   402,     5,     6,    57,    63,   413,    57,    63,   392,
      15,    16,   166,   171,   173,   176,   217,   229,   268,   166,
     246,   173,   240,   240,   173,   251,   164,   184,   285,   397,
     251,    57,    63,   225,   173,   173,   173,   173,   177,   235,
     218,   269,   273,   273,   273,   273,   283,   173,   403,   416,
     284,   394,   177,   178,   179,   228,    15,    16,   166,   171,
     173,   229,   265,   266,   243,   398,   251,   472,   462,   467,
     177,   218,    35,    71,    73,    75,    76,    77,    78,    79,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    93,    94,    95,    98,    99,   100,   101,   118,   119,
     173,   278,   281,   194,   284,   106,   410,   411,   389,   160,
     220,   270,   365,   177,   178,   179,   191,   218,   192,   284,
     284,   284,    21,    22,    38,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   128,   129,   130,   137,   138,   139,   140,
     141,   144,   145,   146,   147,   148,   149,   150,   193,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   207,
     208,   214,   215,    35,    35,   217,   276,   284,   286,    75,
      79,    93,    94,    98,    99,   100,   101,   420,   405,   173,
     417,   285,   390,   269,   268,   220,   251,   152,   173,   387,
     388,   265,    19,    25,    31,    41,    49,    64,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   156,   219,   301,   419,   421,   422,   425,   431,   459,
     473,   463,   468,   173,   173,   173,   216,    22,   173,   216,
     155,   218,   365,   375,   376,   194,   277,   289,   284,   395,
     194,   409,   284,   414,   365,   216,   268,   217,    43,   191,
     194,   197,   386,   198,   198,   198,   217,   198,   198,   217,
     198,   198,   198,   198,   198,   198,   217,   301,    33,    60,
      61,   124,   128,   193,   197,   200,   215,   221,   430,   195,
     424,   379,   382,   173,   137,   217,     7,    50,   314,   218,
     251,   217,   459,     1,     5,     6,     9,    10,    11,    13,
      15,    16,    17,    18,    19,    25,    26,    27,    28,    29,
      31,    38,    39,    42,    44,    45,    48,    51,    52,    54,
      55,    58,    59,    65,    68,    69,    80,   102,   103,   104,
     105,   118,   119,   133,   134,   135,   151,   152,   153,   154,
     155,   157,   159,   160,   161,   162,   166,   167,   168,   169,
     170,   171,   172,   174,   175,   176,   183,   200,   201,   202,
     207,   208,   215,   217,   219,   220,   234,   236,   247,   248,
     251,   252,   255,   256,   258,   260,   261,   262,   263,   285,
     287,   288,   290,   295,   300,   301,   302,   306,   307,   308,
     309,   310,   311,   312,   313,   315,   319,   320,   327,   330,
     333,   338,   341,   342,   344,   345,   346,   348,   353,   356,
     357,   364,   419,   475,   490,   501,   505,   518,   521,   407,
     173,   251,   406,   301,   371,   388,   216,    65,   104,   174,
     295,   357,   173,   173,   431,   127,   137,   192,   385,   432,
     437,   439,   357,   441,   435,   173,   426,   443,   445,   447,
     449,   451,   453,   455,   457,   357,   198,   217,    33,   197,
      33,   197,   215,   221,   216,   357,   215,   221,   431,   173,
     251,   474,   173,   191,   251,   377,   428,   459,   464,   173,
     380,   428,   469,   357,   152,   173,   384,   418,   375,   377,
     378,   198,   198,   357,   259,   198,   303,   421,   475,   217,
     301,   198,     5,   102,   103,   198,   217,   127,   300,   331,
     342,   357,   286,   198,   217,    61,   357,   217,   357,   173,
     198,   198,   217,   251,   198,   166,    58,   357,   217,   286,
     198,   217,   198,   198,   217,   198,   198,   127,   300,   357,
     357,   357,   220,   286,   333,   337,   337,   337,   217,   217,
     217,   217,   217,   217,    13,   431,    13,   431,    13,   357,
     500,   516,   198,   357,   198,   233,    13,   293,   500,   517,
     357,   357,   357,   357,   357,    13,    49,   291,   331,   357,
     158,   173,   331,   476,   478,   220,   251,   251,   357,    10,
      37,   333,   339,   173,   217,   251,   251,   251,   251,   251,
      66,   316,   275,   132,   251,    21,    22,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   136,   137,   138,   139,   140,   141,   144,   145,   146,
     147,   148,   149,   150,   192,   193,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   214,   215,   339,   217,
     395,   192,   251,   285,   215,   251,   275,   285,   372,   198,
     218,    43,   251,   385,   300,   357,   459,   459,   429,   459,
     218,   459,   459,   218,   173,   423,   459,   276,   459,   276,
     459,   276,   378,   380,   381,   218,   434,   291,   216,   216,
     357,   251,   285,   194,   428,   285,   194,   428,   285,   218,
     217,    43,   127,   191,   192,   194,   197,   383,   218,   428,
     491,   493,   286,   418,   304,   217,   301,   198,   217,   328,
     198,   198,   198,   512,   331,   300,   331,   191,   108,   109,
     110,   111,   112,   113,   114,   115,   121,   122,   127,   140,
     141,   147,   148,   149,   192,    14,   431,   293,   357,   357,
     286,   192,   321,   323,   357,   325,   194,   166,   357,   514,
     331,   497,   502,   331,   495,   431,   300,   357,   220,   275,
     357,   357,   357,   357,   357,   357,   418,    53,   200,   215,
     217,   357,   476,   479,   483,   499,   504,   418,   217,   479,
     504,   418,   142,   184,   185,   186,   484,   296,   286,   298,
     179,   180,   228,   418,   184,   191,   520,   418,    13,   216,
     191,   520,   217,   137,   383,   520,   191,   218,   152,   157,
     198,   301,   347,   286,   257,   284,   340,    70,   215,   218,
     331,   478,   160,   217,   318,   388,     4,   160,   336,   337,
      19,   158,   173,   419,    19,   158,   173,   419,   133,   134,
     135,   287,   343,   357,   343,   357,   343,   357,   343,   357,
     343,   357,   343,   357,   343,   357,   343,   357,   357,   357,
     357,   343,   357,   343,   357,   357,   357,   357,   173,   343,
     357,   357,   158,   173,   357,   357,   357,   419,   357,   357,
     357,   343,   357,   343,   357,   357,   357,   357,   343,   357,
     343,   357,   343,   357,   357,   343,   357,    22,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   129,
     130,   158,   173,   214,   215,   354,   419,   357,   218,   331,
     408,   357,   274,     8,   365,   370,   431,   173,   300,   357,
     251,   199,   199,   199,   428,   199,   199,   199,   251,   199,
     277,   199,   277,   199,   277,   199,   199,   428,   294,   459,
     218,   216,   459,   459,   357,   173,   173,   459,   300,   357,
     377,   431,   431,    20,   459,    70,   331,   478,   489,   198,
     357,   173,   357,   459,   506,   508,   510,   431,   520,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   286,   199,   428,
     218,   218,   264,   431,   431,   218,   431,   218,   431,   520,
     431,   378,   520,   381,   199,   336,   218,   218,   218,   218,
     218,   218,    20,   337,   215,   357,   218,   142,   185,   483,
     188,   189,   216,   487,   191,   185,   188,   216,   486,    20,
     218,   483,   184,   187,   485,    20,   357,   184,   500,   294,
     294,   357,    20,   500,    20,   418,   357,   357,   357,   218,
     158,   173,   217,   217,   349,   351,    12,    23,    24,   249,
     250,   357,   289,   275,   173,   218,   478,   476,   191,   218,
     218,   173,   317,   317,   217,   127,   137,   173,   192,   197,
     334,   335,   276,   198,   217,   198,   217,   337,   337,   337,
     217,   217,   216,    19,   158,   173,   419,   194,   158,   173,
     357,   217,   217,   158,   173,   357,     1,   217,   216,   218,
     285,   251,   216,    57,    63,   368,    67,   369,   251,   199,
     251,   433,   438,   440,   459,   442,   436,   427,   173,   444,
     199,   448,   199,   452,   199,   456,   458,   380,   199,   428,
     218,    43,   383,   199,   199,   331,   199,   478,   218,   218,
     218,   173,   218,   185,   199,   218,   199,   431,   378,   381,
     199,   218,   217,   431,   357,   199,   199,   199,   199,   218,
     199,   199,   218,   199,   336,   276,   217,   331,   479,   483,
     357,   476,   487,   216,   357,   499,   216,   331,   479,   184,
     187,   190,   488,   216,   331,   199,   199,   194,   231,   331,
     331,    20,   218,   217,   137,   383,   357,   357,   431,   276,
     286,   357,    12,   253,   285,   336,   218,   216,   215,   191,
     216,   218,   335,   173,   173,   217,   173,   173,   191,   216,
     277,   358,   357,   360,   357,   218,   331,   357,   198,   217,
     357,   217,   216,   357,   215,   218,   331,   217,   216,   355,
     218,   331,   251,    47,   369,    46,   106,   366,   336,   446,
     450,   454,   217,   459,   173,   357,   492,   494,   286,   305,
     218,   199,   428,   173,   217,   329,   199,   199,   199,   513,
     293,   199,   322,   324,   326,   515,   498,   503,   496,   217,
     339,   277,   218,   331,   185,   483,   487,   185,   483,   216,
     185,   297,   299,   232,   181,   185,   185,   331,   137,   383,
     357,   357,   357,   218,   218,   199,   277,   286,   254,   251,
     276,   218,   476,   173,   216,   194,   386,   218,   173,   334,
     216,   142,   286,   332,   431,   218,   459,   218,   218,   218,
     362,   357,   357,   218,   476,   218,   357,   218,    33,   367,
     366,   368,   291,   217,   217,   357,   173,   199,   357,   507,
     509,   511,   217,   218,   217,   357,   357,   357,   217,    70,
     489,   217,   217,   218,   357,   332,   218,   357,   487,   357,
     488,   500,   357,   217,   292,   230,   500,   357,   185,   357,
     357,   218,   350,   199,   250,    26,   105,   255,   307,   308,
     309,   311,   357,   277,   216,   194,   386,   431,   385,   218,
     127,   357,   199,   199,   459,   218,   218,   216,   218,   373,
     367,   384,   218,   489,   489,   218,   199,   217,   218,   217,
     217,   217,   291,   293,   331,   489,   489,   218,   185,   519,
     519,   519,   291,   177,   519,   519,   357,   137,   383,   347,
     352,   127,   127,   357,   286,   218,   431,   385,   385,   300,
     357,   357,   359,   361,   199,   218,   281,   374,   217,   476,
     480,   481,   482,   482,   357,   357,   489,   489,   476,   477,
     218,   218,   520,   482,   477,    53,   216,   184,   184,   191,
     520,   184,   216,   519,   357,   357,   347,   357,   385,   300,
     357,   300,   357,   251,   363,   251,   281,   476,   191,   520,
     218,   218,   218,   218,   482,   482,   218,   218,   218,   218,
     357,   216,   216,   184,   218,   216,   300,   357,   251,   251,
     286,   218,   217,   218,   218,   251,   476,   218
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
     275,   275,   276,   276,   276,   277,   277,   277,   278,   278,
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
     374,   371,   371,   375,   375,   376,   376,   377,   377,   378,
     378,   379,   379,   379,   380,   381,   381,   382,   382,   382,
     383,   383,   384,   384,   384,   384,   384,   384,   385,   385,
     385,   386,   386,   387,   387,   387,   387,   387,   388,   388,
     388,   388,   388,   389,   390,   389,   391,   391,   392,   392,
     392,   393,   394,   393,   395,   395,   395,   395,   396,   396,
     396,   398,   397,   399,   399,   400,   401,   400,   402,   402,
     402,   403,   405,   406,   404,   407,   408,   404,   409,   409,
     410,   410,   411,   412,   412,   412,   412,   413,   413,   413,
     414,   414,   416,   417,   415,   418,   418,   418,   418,   418,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   420,   420,   420,
     420,   420,   420,   420,   420,   421,   422,   422,   422,   423,
     423,   424,   424,   424,   426,   427,   425,   428,   428,   429,
     429,   430,   430,   431,   431,   431,   431,   431,   431,   432,
     433,   431,   431,   431,   434,   431,   431,   431,   431,   431,
     431,   431,   431,   431,   431,   431,   431,   431,   435,   436,
     431,   431,   437,   438,   431,   439,   440,   431,   441,   442,
     431,   431,   443,   444,   431,   445,   446,   431,   431,   447,
     448,   431,   449,   450,   431,   431,   451,   452,   431,   453,
     454,   431,   455,   456,   431,   457,   458,   431,   459,   459,
     459,   461,   462,   463,   464,   460,   466,   467,   468,   469,
     465,   471,   472,   473,   474,   470,   475,   475,   475,   475,
     475,   476,   476,   476,   476,   476,   476,   476,   476,   477,
     477,   478,   479,   479,   480,   480,   481,   481,   482,   482,
     483,   483,   484,   484,   485,   485,   486,   486,   487,   487,
     487,   488,   488,   488,   489,   489,   490,   490,   490,   490,
     490,   490,   491,   492,   490,   493,   494,   490,   495,   496,
     490,   497,   498,   490,   499,   499,   499,   500,   500,   501,
     502,   503,   501,   504,   504,   505,   505,   505,   506,   507,
     505,   508,   509,   505,   510,   511,   505,   505,   512,   513,
     505,   505,   514,   515,   505,   516,   516,   517,   517,   518,
     518,   518,   518,   518,   519,   519,   520,   520,   521,   521,
     521,   521,   521,   521
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
       0,     3,     0,     2,     3,     0,     2,     4,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     2,     2,     3,     4,     3,     2,
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
       0,    10,     5,     3,     4,     1,     3,     1,     3,     1,
       3,     0,     2,     3,     3,     1,     3,     0,     2,     3,
       1,     1,     1,     2,     3,     5,     3,     3,     1,     1,
       1,     0,     1,     1,     4,     3,     3,     5,     4,     6,
       5,     5,     4,     0,     0,     4,     0,     1,     0,     1,
       1,     6,     0,     6,     0,     2,     3,     5,     0,     1,
       1,     0,     5,     2,     3,     4,     0,     4,     0,     1,
       1,     1,     0,     0,     9,     0,     0,    11,     0,     2,
       0,     1,     3,     1,     1,     2,     2,     0,     1,     1,
       0,     3,     0,     0,     7,     1,     4,     3,     3,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     1,
       3,     0,     2,     3,     0,     0,     6,     1,     1,     1,
       3,     3,     4,     1,     1,     1,     1,     2,     3,     0,
       0,     6,     4,     5,     0,     9,     4,     2,     2,     3,
       2,     3,     2,     2,     3,     3,     3,     2,     0,     0,
       6,     2,     0,     0,     6,     0,     0,     6,     0,     0,
       6,     1,     0,     0,     6,     0,     0,     7,     1,     0,
       0,     6,     0,     0,     7,     1,     0,     0,     6,     0,
       0,     7,     0,     0,     6,     0,     0,     6,     1,     3,
       3,     0,     0,     0,     0,    10,     0,     0,     0,     0,
      10,     0,     0,     0,     0,    10,     1,     1,     1,     1,
       1,     3,     3,     5,     5,     6,     6,     8,     8,     0,
       1,     1,     1,     3,     3,     5,     1,     2,     1,     0,
       0,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     2,     1,     1,     0,     1,     5,     4,     6,     7,
       5,     7,     0,     0,    10,     0,     0,    10,     0,     0,
      10,     0,     0,     7,     1,     3,     3,     3,     1,     5,
       0,     0,    10,     1,     3,     3,     4,     4,     0,     0,
      11,     0,     0,    11,     0,     0,    10,     5,     0,     0,
       9,     5,     0,     0,    10,     1,     3,     1,     3,     3,
       3,     4,     7,     9,     0,     3,     0,     1,     9,    10,
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

  case 137: /* optional_function_type: ':' '(' tuple_type_list ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-2]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-1].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-1].pVarDeclList));
    }
    break;

  case 138: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 139: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 140: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 141: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 142: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 143: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 144: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 145: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 146: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 147: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 148: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 149: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 150: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 151: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 152: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 153: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 154: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 155: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 156: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 157: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 158: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 159: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 160: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 161: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 162: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 163: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 164: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 165: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 166: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 167: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 168: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 169: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 170: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 171: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 172: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 173: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 174: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 175: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 176: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 177: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 178: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 179: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 180: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 181: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 182: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 183: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 184: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 185: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 186: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 187: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 188: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 189: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 190: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 191: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 192: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 193: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 194: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 195: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 196: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 197: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 198: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 199: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 200: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 201: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 202: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 203: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 204: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 205: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 206: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 207: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 208: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 209: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 210: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 211: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 212: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 213: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 214: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 215: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 216: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 217: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 218: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 219: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 220: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 221: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 222: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 223: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 224: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 225: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 226: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 227: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 228: /* $@7: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 229: /* function_declaration: optional_public_or_private_function $@7 function_declaration_header expression_block  */
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

  case 234: /* expression_block: open_block expressions close_block  */
                                                                  {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 235: /* expression_block: open_block expressions close_block "finally" open_block expressions close_block  */
                                                                                                                        {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 236: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
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

  case 237: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
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

  case 238: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 239: /* expression_any: semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 240: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expr_assign semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 244: /* expression_any: expression_delete semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 245: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 246: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 247: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 248: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 249: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 250: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 251: /* expression_any: expression_break semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* expression_any: expression_continue semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 253: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 254: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 255: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 256: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 257: /* expression_any: expression_label semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 258: /* expression_any: expression_goto semicolon  */
                                                  { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 259: /* expression_any: "pass" semicolon  */
                                                  { (yyval.pExpression) = nullptr; }
    break;

  case 260: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 261: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back((yyvsp[0].pExpression));
        }
    }
    break;

  case 262: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 263: /* expr_keyword: "keyword" expr expression_block  */
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

  case 264: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 265: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 266: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 267: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 268: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 269: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 270: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 271: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 272: /* $@8: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 273: /* $@9: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 274: /* expression_keyword: "keyword" '<' $@8 type_declaration_no_options_list '>' $@9 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 275: /* $@10: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 276: /* $@11: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 277: /* expression_keyword: "type function" '<' $@10 type_declaration_no_options_list '>' $@11 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 278: /* expr_pipe: expr_assign " <|" expr_block  */
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

  case 279: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 280: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 281: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 282: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 283: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 284: /* name_in_namespace: "name" "::" "name"  */
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

  case 285: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 286: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), (yyvsp[0].pExpression));
    }
    break;

  case 287: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), (yyvsp[0].pExpression));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 288: /* $@12: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 289: /* $@13: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 290: /* new_type_declaration: '<' $@12 type_declaration '>' $@13  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 291: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 292: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pTypeDecl),false);
    }
    break;

  case 293: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 294: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),(yyvsp[-3].pTypeDecl),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 295: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),(yyvsp[-1].pExpression));
    }
    break;

  case 296: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),(yyvsp[-1].pExpression));
    }
    break;

  case 297: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 298: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 299: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 300: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 301: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 302: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 303: /* expression_return: expression_return_no_pipe semicolon  */
                                                    {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 304: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 305: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 306: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 307: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 308: /* expression_yield: expression_yield_no_pipe semicolon  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 309: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression));
    }
    break;

  case 310: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),(yyvsp[0].pExpression));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 311: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 312: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 313: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 314: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 315: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 316: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 317: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 318: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 319: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 320: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 321: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                        {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 322: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 323: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr semicolon  */
                                                                                                                                {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 324: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                           {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameList),tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 325: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 326: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 327: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr semicolon  */
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

  case 328: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr_pipe  */
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

  case 331: /* $@14: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 332: /* $@15: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 333: /* expr_cast: "cast" '<' $@14 type_declaration_no_options '>' $@15 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
    }
    break;

  case 334: /* $@16: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 335: /* $@17: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 336: /* expr_cast: "upcast" '<' $@16 type_declaration_no_options '>' $@17 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 337: /* $@18: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 338: /* $@19: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 339: /* expr_cast: "reinterpret" '<' $@18 type_declaration_no_options '>' $@19 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),(yyvsp[0].pExpression),(yyvsp[-3].pTypeDecl));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 340: /* $@20: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 341: /* $@21: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 342: /* expr_type_decl: "type" '<' $@20 type_declaration '>' $@21  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 343: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
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

  case 344: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
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

  case 345: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
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

  case 348: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
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

  case 349: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 350: /* expr_list: expr_list ',' expr  */
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
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 366: /* optional_capture_list: "capture" '(' capture_list ')'  */
                                             { (yyval.pCaptList) = (yyvsp[-1].pCaptList); }
    break;

  case 367: /* expr_block: expression_block  */
                                            {
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

  case 370: /* $@22: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 371: /* expr_full_block_assumed_piped: block_or_lambda $@22 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
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
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 399: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 400: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
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

  case 421: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 422: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
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

  case 424: /* func_addr_name: "$i" '(' expr ')'  */
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

  case 426: /* $@23: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 427: /* $@24: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 428: /* func_addr_expr: '@' '@' '<' $@23 type_declaration_no_options '>' $@24 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = (yyvsp[-3].pTypeDecl);
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 429: /* $@25: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 430: /* $@26: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 431: /* func_addr_expr: '@' '@' '<' $@25 optional_function_argument_list optional_function_type '>' $@26 func_addr_name  */
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

  case 432: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 433: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 434: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 435: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 436: /* expr_field: expr '.' "name" '(' '[' make_struct_fields ']' ')'  */
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

  case 437: /* expr_field: expr '.' basic_type_declaration '(' ')'  */
                                                                        {
        auto method_name = das_to_string((yyvsp[-2].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), method_name);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 438: /* expr_field: expr '.' basic_type_declaration '(' expr_list ')'  */
                                                                                             {
        auto method_name = das_to_string((yyvsp[-3].type));
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), method_name);
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 439: /* $@27: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 440: /* $@28: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 441: /* expr_field: expr '.' $@27 error $@28  */
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
            dd->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            dd->useInitializer = false;
            dd->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = dd;
    }
    break;

  case 444: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
                                                               {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-3])),*(yyvsp[-3].s));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
            delete (yyvsp[-3].s);
            (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 445: /* expr_call: name_in_namespace '(' "uninitialized" make_struct_single ')'  */
                                                                                 {
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
            ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[-4])),*(yyvsp[-4].s));
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

  case 449: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 450: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 451: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 452: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 453: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 454: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 455: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 456: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 457: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 458: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 459: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",(yyvsp[0].pExpression)); }
    break;

  case 460: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",(yyvsp[0].pExpression)); }
    break;

  case 461: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",(yyvsp[0].pExpression)); }
    break;

  case 462: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",(yyvsp[0].pExpression)); }
    break;

  case 463: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 464: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 465: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 466: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 467: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 468: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 469: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 470: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 471: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 472: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 473: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 474: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 475: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 476: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 477: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 478: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 479: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 480: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 481: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 482: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 483: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", (yyvsp[-2].pExpression), (yyvsp[0].pExpression)); }
    break;

  case 484: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back((yyvsp[-2].pExpression));
        itv->arguments.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = itv;
    }
    break;

  case 485: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", (yyvsp[0].pExpression)); }
    break;

  case 486: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", (yyvsp[0].pExpression)); }
    break;

  case 487: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", (yyvsp[-1].pExpression)); }
    break;

  case 488: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", (yyvsp[-1].pExpression)); }
    break;

  case 489: /* expr: '(' expr_list optional_comma ')'  */
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

  case 490: /* expr: '(' make_struct_single ')'  */
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

  case 491: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 492: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 493: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-3].pExpression), (yyvsp[-1].pExpression)); }
    break;

  case 494: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), (yyvsp[-4].pExpression), (yyvsp[-1].pExpression), true); }
    break;

  case 495: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-2].pExpression), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 496: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), (yyvsp[-3].pExpression), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 497: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 498: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 499: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),(yyvsp[0].pExpression)); }
    break;

  case 500: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 501: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression)); }
    break;

  case 502: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 503: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 504: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression)); }
    break;

  case 505: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
        }
    break;

  case 506: /* $@29: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 507: /* $@30: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 508: /* expr: expr "is" "type" '<' $@29 type_declaration_no_options '>' $@30  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),(yyvsp[-2].pTypeDecl));
    }
    break;

  case 509: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),vdecl);
    }
    break;

  case 510: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 511: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 512: /* $@31: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 513: /* $@32: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 514: /* expr: expr "as" "type" '<' $@31 type_declaration '>' $@32  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-7].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 515: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-2].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 516: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 517: /* $@33: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 518: /* $@34: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 519: /* expr: expr '?' "as" "type" '<' $@33 type_declaration '>' $@34  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),(yyvsp[-8].pExpression),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 520: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),(yyvsp[-3].pExpression),das_to_string((yyvsp[0].type)));
    }
    break;

  case 521: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 522: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 523: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 524: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 525: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 526: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 527: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 528: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 529: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 530: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 531: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 532: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 533: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 534: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 535: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 536: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 537: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 538: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 539: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 540: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 541: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 542: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 543: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-5].pExpression), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 544: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 545: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), (yyvsp[-6].pExpression), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 546: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 547: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-6].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 548: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),(yyvsp[-5].pExpression),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 549: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 550: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 551: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
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

  case 566: /* $@35: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 567: /* struct_variable_declaration_list: struct_variable_declaration_list $@35 structure_variable_declaration semicolon  */
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

  case 568: /* $@36: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 569: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@36 function_declaration_header semicolon  */
                                                          {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 570: /* $@37: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 571: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@37 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyvsp[-1].pFuncDecl)->isTemplate = yyextra->g_thisStructure ? yyextra->g_thisStructure->isTemplate : false;
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 572: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' semicolon  */
                                                                                       {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
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

  case 614: /* $@38: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 615: /* global_variable_declaration_list: global_variable_declaration_list $@38 optional_field_annotation let_variable_declaration  */
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
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 622: /* $@39: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 623: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@39 optional_field_annotation let_variable_declaration  */
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

  case 625: /* enum_list: enum_list semicolon  */
                                {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 626: /* enum_list: enum_list "name" semicolon  */
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

  case 627: /* enum_list: enum_list "name" '=' expr semicolon  */
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

  case 628: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 629: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 630: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 631: /* $@40: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 632: /* single_alias: optional_public_or_private_alias "name" $@40 '=' type_declaration  */
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

  case 636: /* $@41: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 638: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 639: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 640: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 641: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 642: /* $@42: %empty  */
                                                                                                                       {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 643: /* $@43: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 644: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name open_block $@42 enum_list $@43 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 645: /* $@44: %empty  */
                                                                                                                                                            {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 646: /* $@45: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 647: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration open_block $@44 enum_list $@45 close_block  */
                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 648: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 649: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 650: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 651: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 652: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 653: /* class_or_struct: "class"  */
                    { (yyval.i) = CorS_Class; }
    break;

  case 654: /* class_or_struct: "struct"  */
                    { (yyval.i) = CorS_Struct; }
    break;

  case 655: /* class_or_struct: "template" "class"  */
                                 { (yyval.i) = CorS_ClassTemplate; }
    break;

  case 656: /* class_or_struct: "template" "struct"  */
                                 { (yyval.i) = CorS_StructTemplate; }
    break;

  case 657: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 658: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 659: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 660: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 661: /* optional_struct_variable_declaration_list: open_block struct_variable_declaration_list close_block  */
                                                                      {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 662: /* $@46: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 663: /* $@47: %empty  */
                         {
        if ( (yyvsp[0].pStructure) ) {
            (yyvsp[0].pStructure)->isClass = (yyvsp[-3].i)==CorS_Class || (yyvsp[-3].i)==CorS_ClassTemplate;
            (yyvsp[0].pStructure)->isTemplate = (yyvsp[-3].i)==CorS_ClassTemplate || (yyvsp[-3].i)==CorS_StructTemplate;
            (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b);
        }
    }
    break;

  case 664: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@46 structure_name $@47 optional_struct_variable_declaration_list  */
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

  case 665: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 666: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 667: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 668: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 669: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 670: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 671: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 672: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 673: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 674: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 675: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 676: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 677: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 678: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 679: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 680: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 681: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 682: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 683: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 684: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 685: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 686: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 687: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 688: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 689: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 690: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 691: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 692: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 693: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 694: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 695: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 696: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 697: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 698: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 699: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 700: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 701: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 702: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 703: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 704: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 705: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 706: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 707: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 708: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 709: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 710: /* bitfield_bits: bitfield_bits semicolon "name"  */
                                                 {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 711: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 712: /* bitfield_alias_bits: bitfield_alias_bits semicolon  */
                                            {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 713: /* bitfield_alias_bits: bitfield_alias_bits "name" semicolon  */
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

  case 714: /* $@48: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 715: /* $@49: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 716: /* bitfield_type_declaration: "bitfield" '<' $@48 bitfield_bits '>' $@49  */
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

  case 719: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 720: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 721: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 722: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 723: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 724: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 725: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 726: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 727: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 728: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 729: /* $@50: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 730: /* $@51: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 731: /* type_declaration_no_options: "type" '<' $@50 type_declaration '>' $@51  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 732: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 733: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 734: /* $@52: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 735: /* type_declaration_no_options: '$' name_in_namespace '<' $@52 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 736: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 737: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 738: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 739: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 740: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 741: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 742: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 743: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 744: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 745: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 746: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 747: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 748: /* $@53: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 749: /* $@54: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 750: /* type_declaration_no_options: "smart_ptr" '<' $@53 type_declaration '>' $@54  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 751: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 752: /* $@55: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 753: /* $@56: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 754: /* type_declaration_no_options: "array" '<' $@55 type_declaration '>' $@56  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 755: /* $@57: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 756: /* $@58: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 757: /* type_declaration_no_options: "table" '<' $@57 table_type_pair '>' $@58  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].aTypePair).firstType;
        (yyval.pTypeDecl)->secondType = (yyvsp[-2].aTypePair).secondType;
    }
    break;

  case 758: /* $@59: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 759: /* $@60: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 760: /* type_declaration_no_options: "iterator" '<' $@59 type_declaration '>' $@60  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 761: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 762: /* $@61: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 763: /* $@62: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 764: /* type_declaration_no_options: "block" '<' $@61 type_declaration '>' $@62  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 765: /* $@63: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 766: /* $@64: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 767: /* type_declaration_no_options: "block" '<' $@63 optional_function_argument_list optional_function_type '>' $@64  */
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

  case 768: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 769: /* $@65: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 770: /* $@66: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 771: /* type_declaration_no_options: "function" '<' $@65 type_declaration '>' $@66  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 772: /* $@67: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 773: /* $@68: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 774: /* type_declaration_no_options: "function" '<' $@67 optional_function_argument_list optional_function_type '>' $@68  */
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

  case 775: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 776: /* $@69: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 777: /* $@70: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 778: /* type_declaration_no_options: "lambda" '<' $@69 type_declaration '>' $@70  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 779: /* $@71: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 780: /* $@72: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 781: /* type_declaration_no_options: "lambda" '<' $@71 optional_function_argument_list optional_function_type '>' $@72  */
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

  case 782: /* $@73: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 783: /* $@74: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 784: /* type_declaration_no_options: "tuple" '<' $@73 tuple_type_list '>' $@74  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 785: /* $@75: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 786: /* $@76: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 787: /* type_declaration_no_options: "variant" '<' $@75 variant_type_list '>' $@76  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 788: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 789: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 790: /* type_declaration: type_declaration '|' '#'  */
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

  case 791: /* $@77: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 792: /* $@78: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 793: /* $@79: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 794: /* $@80: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 795: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@77 "name" $@78 open_block $@79 tuple_alias_type_list $@80 close_block  */
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

  case 796: /* $@81: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 797: /* $@82: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 798: /* $@83: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 799: /* $@84: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 800: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@81 "name" $@82 open_block $@83 variant_alias_type_list $@84 close_block  */
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

  case 801: /* $@85: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 802: /* $@86: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 803: /* $@87: %empty  */
                 {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 804: /* $@88: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 805: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@85 "name" $@86 open_block $@87 bitfield_alias_bits $@88 close_block  */
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

  case 806: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 807: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 808: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 809: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 810: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 811: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 812: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 813: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 814: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),(yyvsp[0].pExpression),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 815: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 816: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 817: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),(yyvsp[-1].b),false);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 818: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",(yyvsp[0].pExpression),false,true);
        mfd->tag = (yyvsp[-3].pExpression);
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 819: /* make_variant_dim: %empty  */
       {
        (yyval.pExpression) = ast_makeStructToMakeVariant(nullptr, LineInfo());
    }
    break;

  case 820: /* make_variant_dim: make_struct_fields  */
                              {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 821: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 822: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 823: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 824: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 825: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 826: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 827: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 828: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 829: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 830: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 831: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 844: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 845: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 846: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 847: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-2].pTypeDecl);
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 848: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = (yyvsp[-4].pTypeDecl);
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 849: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-5].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 850: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back((yyvsp[-2].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 851: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
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

  case 852: /* $@89: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 853: /* $@90: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 854: /* make_struct_decl: "struct" '<' $@89 type_declaration_no_options '>' $@90 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 855: /* $@91: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 856: /* $@92: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 857: /* make_struct_decl: "class" '<' $@91 type_declaration_no_options '>' $@92 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-6].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 858: /* $@93: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 859: /* $@94: %empty  */
                                                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 860: /* make_struct_decl: "variant" '<' $@93 variant_type_list '>' $@94 '(' use_initializer make_variant_dim ')'  */
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

  case 861: /* $@95: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 862: /* $@96: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 863: /* make_struct_decl: "default" '<' $@95 type_declaration_no_options '>' $@96 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = (yyvsp[-3].pTypeDecl);
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 864: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 865: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 866: /* make_tuple: make_tuple ',' expr  */
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

  case 867: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back((yyvsp[-2].pExpression));
        mt->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mt;
    }
    break;

  case 868: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 869: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 870: /* $@97: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 871: /* $@98: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 872: /* make_tuple_call: "tuple" '<' $@97 tuple_type_list '>' $@98 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 873: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 874: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 875: /* make_dim_decl: '[' optional_expr_list ']'  */
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

  case 876: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 877: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-2].pTypeDecl);
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 878: /* $@99: %empty  */
                                       { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 879: /* $@100: %empty  */
                                                                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 880: /* make_dim_decl: "array" "struct" '<' $@99 type_declaration_no_options '>' $@100 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 881: /* $@101: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 882: /* $@102: %empty  */
                                                                                                  { yyextra->das_arrow_depth --; }
    break;

  case 883: /* make_dim_decl: "array" "tuple" '<' $@101 tuple_type_list '>' $@102 '(' use_initializer optional_make_struct_dim_decl ')'  */
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

  case 884: /* $@103: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 885: /* $@104: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 886: /* make_dim_decl: "array" "variant" '<' $@103 variant_type_list '>' $@104 '(' make_variant_dim ')'  */
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

  case 887: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
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

  case 888: /* $@105: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 889: /* $@106: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 890: /* make_dim_decl: "array" '<' $@105 type_declaration_no_options '>' $@106 '(' optional_expr_list ')'  */
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

  case 891: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 892: /* $@107: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 893: /* $@108: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 894: /* make_dim_decl: "fixed_array" '<' $@107 type_declaration_no_options '>' $@108 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = (yyvsp[-6].pTypeDecl);
        mka->gen2 = true;
        (yyval.pExpression) = mka;
    }
    break;

  case 895: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = mka;
    }
    break;

  case 896: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back((yyvsp[0].pExpression));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 897: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 898: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),(yyvsp[-2].pExpression),(yyvsp[0].pExpression));
    }
    break;

  case 899: /* make_table_decl: "begin of code block" optional_expr_map_tuple_list "end of code block"  */
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

  case 900: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
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

  case 901: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 902: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 903: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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

  case 904: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 905: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 906: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 907: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 908: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 909: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 910: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 911: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 912: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 913: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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
