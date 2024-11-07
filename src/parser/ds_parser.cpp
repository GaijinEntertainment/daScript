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
  YYSYMBOL_DAS_STRUCT = 4,                 /* "struct"  */
  YYSYMBOL_DAS_CLASS = 5,                  /* "class"  */
  YYSYMBOL_DAS_LET = 6,                    /* "let"  */
  YYSYMBOL_DAS_DEF = 7,                    /* "def"  */
  YYSYMBOL_DAS_WHILE = 8,                  /* "while"  */
  YYSYMBOL_DAS_IF = 9,                     /* "if"  */
  YYSYMBOL_DAS_STATIC_IF = 10,             /* "static_if"  */
  YYSYMBOL_DAS_ELSE = 11,                  /* "else"  */
  YYSYMBOL_DAS_FOR = 12,                   /* "for"  */
  YYSYMBOL_DAS_CATCH = 13,                 /* "recover"  */
  YYSYMBOL_DAS_TRUE = 14,                  /* "true"  */
  YYSYMBOL_DAS_FALSE = 15,                 /* "false"  */
  YYSYMBOL_DAS_NEWT = 16,                  /* "new"  */
  YYSYMBOL_DAS_TYPEINFO = 17,              /* "typeinfo"  */
  YYSYMBOL_DAS_TYPE = 18,                  /* "type"  */
  YYSYMBOL_DAS_IN = 19,                    /* "in"  */
  YYSYMBOL_DAS_IS = 20,                    /* "is"  */
  YYSYMBOL_DAS_AS = 21,                    /* "as"  */
  YYSYMBOL_DAS_ELIF = 22,                  /* "elif"  */
  YYSYMBOL_DAS_STATIC_ELIF = 23,           /* "static_elif"  */
  YYSYMBOL_DAS_ARRAY = 24,                 /* "array"  */
  YYSYMBOL_DAS_RETURN = 25,                /* "return"  */
  YYSYMBOL_DAS_NULL = 26,                  /* "null"  */
  YYSYMBOL_DAS_BREAK = 27,                 /* "break"  */
  YYSYMBOL_DAS_TRY = 28,                   /* "try"  */
  YYSYMBOL_DAS_OPTIONS = 29,               /* "options"  */
  YYSYMBOL_DAS_TABLE = 30,                 /* "table"  */
  YYSYMBOL_DAS_EXPECT = 31,                /* "expect"  */
  YYSYMBOL_DAS_CONST = 32,                 /* "const"  */
  YYSYMBOL_DAS_REQUIRE = 33,               /* "require"  */
  YYSYMBOL_DAS_OPERATOR = 34,              /* "operator"  */
  YYSYMBOL_DAS_ENUM = 35,                  /* "enum"  */
  YYSYMBOL_DAS_FINALLY = 36,               /* "finally"  */
  YYSYMBOL_DAS_DELETE = 37,                /* "delete"  */
  YYSYMBOL_DAS_DEREF = 38,                 /* "deref"  */
  YYSYMBOL_DAS_TYPEDEF = 39,               /* "typedef"  */
  YYSYMBOL_DAS_TYPEDECL = 40,              /* "typedecl"  */
  YYSYMBOL_DAS_WITH = 41,                  /* "with"  */
  YYSYMBOL_DAS_AKA = 42,                   /* "aka"  */
  YYSYMBOL_DAS_ASSUME = 43,                /* "assume"  */
  YYSYMBOL_DAS_CAST = 44,                  /* "cast"  */
  YYSYMBOL_DAS_OVERRIDE = 45,              /* "override"  */
  YYSYMBOL_DAS_ABSTRACT = 46,              /* "abstract"  */
  YYSYMBOL_DAS_UPCAST = 47,                /* "upcast"  */
  YYSYMBOL_DAS_ITERATOR = 48,              /* "iterator"  */
  YYSYMBOL_DAS_VAR = 49,                   /* "var"  */
  YYSYMBOL_DAS_ADDR = 50,                  /* "addr"  */
  YYSYMBOL_DAS_CONTINUE = 51,              /* "continue"  */
  YYSYMBOL_DAS_WHERE = 52,                 /* "where"  */
  YYSYMBOL_DAS_PASS = 53,                  /* "pass"  */
  YYSYMBOL_DAS_REINTERPRET = 54,           /* "reinterpret"  */
  YYSYMBOL_DAS_MODULE = 55,                /* "module"  */
  YYSYMBOL_DAS_PUBLIC = 56,                /* "public"  */
  YYSYMBOL_DAS_LABEL = 57,                 /* "label"  */
  YYSYMBOL_DAS_GOTO = 58,                  /* "goto"  */
  YYSYMBOL_DAS_IMPLICIT = 59,              /* "implicit"  */
  YYSYMBOL_DAS_EXPLICIT = 60,              /* "explicit"  */
  YYSYMBOL_DAS_SHARED = 61,                /* "shared"  */
  YYSYMBOL_DAS_PRIVATE = 62,               /* "private"  */
  YYSYMBOL_DAS_SMART_PTR = 63,             /* "smart_ptr"  */
  YYSYMBOL_DAS_UNSAFE = 64,                /* "unsafe"  */
  YYSYMBOL_DAS_INSCOPE = 65,               /* "inscope"  */
  YYSYMBOL_DAS_STATIC = 66,                /* "static"  */
  YYSYMBOL_DAS_FIXED_ARRAY = 67,           /* "fixed_array"  */
  YYSYMBOL_DAS_DEFAULT = 68,               /* "default"  */
  YYSYMBOL_DAS_UNINITIALIZED = 69,         /* "uninitialized"  */
  YYSYMBOL_DAS_TBOOL = 70,                 /* "bool"  */
  YYSYMBOL_DAS_TVOID = 71,                 /* "void"  */
  YYSYMBOL_DAS_TSTRING = 72,               /* "string"  */
  YYSYMBOL_DAS_TAUTO = 73,                 /* "auto"  */
  YYSYMBOL_DAS_TINT = 74,                  /* "int"  */
  YYSYMBOL_DAS_TINT2 = 75,                 /* "int2"  */
  YYSYMBOL_DAS_TINT3 = 76,                 /* "int3"  */
  YYSYMBOL_DAS_TINT4 = 77,                 /* "int4"  */
  YYSYMBOL_DAS_TUINT = 78,                 /* "uint"  */
  YYSYMBOL_DAS_TBITFIELD = 79,             /* "bitfield"  */
  YYSYMBOL_DAS_TUINT2 = 80,                /* "uint2"  */
  YYSYMBOL_DAS_TUINT3 = 81,                /* "uint3"  */
  YYSYMBOL_DAS_TUINT4 = 82,                /* "uint4"  */
  YYSYMBOL_DAS_TFLOAT = 83,                /* "float"  */
  YYSYMBOL_DAS_TFLOAT2 = 84,               /* "float2"  */
  YYSYMBOL_DAS_TFLOAT3 = 85,               /* "float3"  */
  YYSYMBOL_DAS_TFLOAT4 = 86,               /* "float4"  */
  YYSYMBOL_DAS_TRANGE = 87,                /* "range"  */
  YYSYMBOL_DAS_TURANGE = 88,               /* "urange"  */
  YYSYMBOL_DAS_TRANGE64 = 89,              /* "range64"  */
  YYSYMBOL_DAS_TURANGE64 = 90,             /* "urange64"  */
  YYSYMBOL_DAS_TBLOCK = 91,                /* "block"  */
  YYSYMBOL_DAS_TINT64 = 92,                /* "int64"  */
  YYSYMBOL_DAS_TUINT64 = 93,               /* "uint64"  */
  YYSYMBOL_DAS_TDOUBLE = 94,               /* "double"  */
  YYSYMBOL_DAS_TFUNCTION = 95,             /* "function"  */
  YYSYMBOL_DAS_TLAMBDA = 96,               /* "lambda"  */
  YYSYMBOL_DAS_TINT8 = 97,                 /* "int8"  */
  YYSYMBOL_DAS_TUINT8 = 98,                /* "uint8"  */
  YYSYMBOL_DAS_TINT16 = 99,                /* "int16"  */
  YYSYMBOL_DAS_TUINT16 = 100,              /* "uint16"  */
  YYSYMBOL_DAS_TTUPLE = 101,               /* "tuple"  */
  YYSYMBOL_DAS_TVARIANT = 102,             /* "variant"  */
  YYSYMBOL_DAS_GENERATOR = 103,            /* "generator"  */
  YYSYMBOL_DAS_YIELD = 104,                /* "yield"  */
  YYSYMBOL_DAS_SEALED = 105,               /* "sealed"  */
  YYSYMBOL_ADDEQU = 106,                   /* "+="  */
  YYSYMBOL_SUBEQU = 107,                   /* "-="  */
  YYSYMBOL_DIVEQU = 108,                   /* "/="  */
  YYSYMBOL_MULEQU = 109,                   /* "*="  */
  YYSYMBOL_MODEQU = 110,                   /* "%="  */
  YYSYMBOL_ANDEQU = 111,                   /* "&="  */
  YYSYMBOL_OREQU = 112,                    /* "|="  */
  YYSYMBOL_XOREQU = 113,                   /* "^="  */
  YYSYMBOL_SHL = 114,                      /* "<<"  */
  YYSYMBOL_SHR = 115,                      /* ">>"  */
  YYSYMBOL_ADDADD = 116,                   /* "++"  */
  YYSYMBOL_SUBSUB = 117,                   /* "--"  */
  YYSYMBOL_LEEQU = 118,                    /* "<="  */
  YYSYMBOL_SHLEQU = 119,                   /* "<<="  */
  YYSYMBOL_SHREQU = 120,                   /* ">>="  */
  YYSYMBOL_GREQU = 121,                    /* ">="  */
  YYSYMBOL_EQUEQU = 122,                   /* "=="  */
  YYSYMBOL_NOTEQU = 123,                   /* "!="  */
  YYSYMBOL_RARROW = 124,                   /* "->"  */
  YYSYMBOL_LARROW = 125,                   /* "<-"  */
  YYSYMBOL_QQ = 126,                       /* "??"  */
  YYSYMBOL_QDOT = 127,                     /* "?."  */
  YYSYMBOL_QBRA = 128,                     /* "?["  */
  YYSYMBOL_LPIPE = 129,                    /* "<|"  */
  YYSYMBOL_LBPIPE = 130,                   /* " <|"  */
  YYSYMBOL_LLPIPE = 131,                   /* "$ <|"  */
  YYSYMBOL_LAPIPE = 132,                   /* "@ <|"  */
  YYSYMBOL_LFPIPE = 133,                   /* "@@ <|"  */
  YYSYMBOL_RPIPE = 134,                    /* "|>"  */
  YYSYMBOL_CLONEEQU = 135,                 /* ":="  */
  YYSYMBOL_ROTL = 136,                     /* "<<<"  */
  YYSYMBOL_ROTR = 137,                     /* ">>>"  */
  YYSYMBOL_ROTLEQU = 138,                  /* "<<<="  */
  YYSYMBOL_ROTREQU = 139,                  /* ">>>="  */
  YYSYMBOL_MAPTO = 140,                    /* "=>"  */
  YYSYMBOL_COLCOL = 141,                   /* "::"  */
  YYSYMBOL_ANDAND = 142,                   /* "&&"  */
  YYSYMBOL_OROR = 143,                     /* "||"  */
  YYSYMBOL_XORXOR = 144,                   /* "^^"  */
  YYSYMBOL_ANDANDEQU = 145,                /* "&&="  */
  YYSYMBOL_OROREQU = 146,                  /* "||="  */
  YYSYMBOL_XORXOREQU = 147,                /* "^^="  */
  YYSYMBOL_DOTDOT = 148,                   /* ".."  */
  YYSYMBOL_MTAG_E = 149,                   /* "$$"  */
  YYSYMBOL_MTAG_I = 150,                   /* "$i"  */
  YYSYMBOL_MTAG_V = 151,                   /* "$v"  */
  YYSYMBOL_MTAG_B = 152,                   /* "$b"  */
  YYSYMBOL_MTAG_A = 153,                   /* "$a"  */
  YYSYMBOL_MTAG_T = 154,                   /* "$t"  */
  YYSYMBOL_MTAG_C = 155,                   /* "$c"  */
  YYSYMBOL_MTAG_F = 156,                   /* "$f"  */
  YYSYMBOL_MTAG_DOTDOTDOT = 157,           /* "..."  */
  YYSYMBOL_BRABRAB = 158,                  /* "[["  */
  YYSYMBOL_BRACBRB = 159,                  /* "[{"  */
  YYSYMBOL_CBRCBRB = 160,                  /* "{{"  */
  YYSYMBOL_INTEGER = 161,                  /* "integer constant"  */
  YYSYMBOL_LONG_INTEGER = 162,             /* "long integer constant"  */
  YYSYMBOL_UNSIGNED_INTEGER = 163,         /* "unsigned integer constant"  */
  YYSYMBOL_UNSIGNED_LONG_INTEGER = 164,    /* "unsigned long integer constant"  */
  YYSYMBOL_UNSIGNED_INT8 = 165,            /* "unsigned int8 constant"  */
  YYSYMBOL_FLOAT = 166,                    /* "floating point constant"  */
  YYSYMBOL_DOUBLE = 167,                   /* "double constant"  */
  YYSYMBOL_NAME = 168,                     /* "name"  */
  YYSYMBOL_KEYWORD = 169,                  /* "keyword"  */
  YYSYMBOL_TYPE_FUNCTION = 170,            /* "type function"  */
  YYSYMBOL_BEGIN_STRING = 171,             /* "start of the string"  */
  YYSYMBOL_STRING_CHARACTER = 172,         /* STRING_CHARACTER  */
  YYSYMBOL_STRING_CHARACTER_ESC = 173,     /* STRING_CHARACTER_ESC  */
  YYSYMBOL_END_STRING = 174,               /* "end of the string"  */
  YYSYMBOL_BEGIN_STRING_EXPR = 175,        /* "{"  */
  YYSYMBOL_END_STRING_EXPR = 176,          /* "}"  */
  YYSYMBOL_END_OF_READ = 177,              /* "end of failed eader macro"  */
  YYSYMBOL_178_begin_of_code_block_ = 178, /* "begin of code block"  */
  YYSYMBOL_179_end_of_code_block_ = 179,   /* "end of code block"  */
  YYSYMBOL_180_end_of_expression_ = 180,   /* "end of expression"  */
  YYSYMBOL_SEMICOLON_CUR_CUR = 181,        /* ";}}"  */
  YYSYMBOL_SEMICOLON_CUR_SQR = 182,        /* ";}]"  */
  YYSYMBOL_SEMICOLON_SQR_SQR = 183,        /* ";]]"  */
  YYSYMBOL_COMMA_SQR_SQR = 184,            /* ",]]"  */
  YYSYMBOL_COMMA_CUR_SQR = 185,            /* ",}]"  */
  YYSYMBOL_186_ = 186,                     /* ','  */
  YYSYMBOL_187_ = 187,                     /* '='  */
  YYSYMBOL_188_ = 188,                     /* '?'  */
  YYSYMBOL_189_ = 189,                     /* ':'  */
  YYSYMBOL_190_ = 190,                     /* '|'  */
  YYSYMBOL_191_ = 191,                     /* '^'  */
  YYSYMBOL_192_ = 192,                     /* '&'  */
  YYSYMBOL_193_ = 193,                     /* '<'  */
  YYSYMBOL_194_ = 194,                     /* '>'  */
  YYSYMBOL_195_ = 195,                     /* '-'  */
  YYSYMBOL_196_ = 196,                     /* '+'  */
  YYSYMBOL_197_ = 197,                     /* '*'  */
  YYSYMBOL_198_ = 198,                     /* '/'  */
  YYSYMBOL_199_ = 199,                     /* '%'  */
  YYSYMBOL_UNARY_MINUS = 200,              /* UNARY_MINUS  */
  YYSYMBOL_UNARY_PLUS = 201,               /* UNARY_PLUS  */
  YYSYMBOL_202_ = 202,                     /* '~'  */
  YYSYMBOL_203_ = 203,                     /* '!'  */
  YYSYMBOL_PRE_INC = 204,                  /* PRE_INC  */
  YYSYMBOL_PRE_DEC = 205,                  /* PRE_DEC  */
  YYSYMBOL_POST_INC = 206,                 /* POST_INC  */
  YYSYMBOL_POST_DEC = 207,                 /* POST_DEC  */
  YYSYMBOL_DEREF = 208,                    /* DEREF  */
  YYSYMBOL_209_ = 209,                     /* '.'  */
  YYSYMBOL_210_ = 210,                     /* '['  */
  YYSYMBOL_211_ = 211,                     /* ']'  */
  YYSYMBOL_212_ = 212,                     /* '('  */
  YYSYMBOL_213_ = 213,                     /* ')'  */
  YYSYMBOL_214_ = 214,                     /* '$'  */
  YYSYMBOL_215_ = 215,                     /* '@'  */
  YYSYMBOL_216_ = 216,                     /* '#'  */
  YYSYMBOL_YYACCEPT = 217,                 /* $accept  */
  YYSYMBOL_program = 218,                  /* program  */
  YYSYMBOL_top_level_reader_macro = 219,   /* top_level_reader_macro  */
  YYSYMBOL_optional_public_or_private_module = 220, /* optional_public_or_private_module  */
  YYSYMBOL_module_name = 221,              /* module_name  */
  YYSYMBOL_module_declaration = 222,       /* module_declaration  */
  YYSYMBOL_character_sequence = 223,       /* character_sequence  */
  YYSYMBOL_string_constant = 224,          /* string_constant  */
  YYSYMBOL_string_builder_body = 225,      /* string_builder_body  */
  YYSYMBOL_string_builder = 226,           /* string_builder  */
  YYSYMBOL_reader_character_sequence = 227, /* reader_character_sequence  */
  YYSYMBOL_expr_reader = 228,              /* expr_reader  */
  YYSYMBOL_229_1 = 229,                    /* $@1  */
  YYSYMBOL_options_declaration = 230,      /* options_declaration  */
  YYSYMBOL_require_declaration = 231,      /* require_declaration  */
  YYSYMBOL_keyword_or_name = 232,          /* keyword_or_name  */
  YYSYMBOL_require_module_name = 233,      /* require_module_name  */
  YYSYMBOL_require_module = 234,           /* require_module  */
  YYSYMBOL_is_public_module = 235,         /* is_public_module  */
  YYSYMBOL_expect_declaration = 236,       /* expect_declaration  */
  YYSYMBOL_expect_list = 237,              /* expect_list  */
  YYSYMBOL_expect_error = 238,             /* expect_error  */
  YYSYMBOL_expression_label = 239,         /* expression_label  */
  YYSYMBOL_expression_goto = 240,          /* expression_goto  */
  YYSYMBOL_elif_or_static_elif = 241,      /* elif_or_static_elif  */
  YYSYMBOL_expression_else = 242,          /* expression_else  */
  YYSYMBOL_if_or_static_if = 243,          /* if_or_static_if  */
  YYSYMBOL_expression_else_one_liner = 244, /* expression_else_one_liner  */
  YYSYMBOL_245_2 = 245,                    /* $@2  */
  YYSYMBOL_expression_if_one_liner = 246,  /* expression_if_one_liner  */
  YYSYMBOL_expression_if_then_else = 247,  /* expression_if_then_else  */
  YYSYMBOL_248_3 = 248,                    /* $@3  */
  YYSYMBOL_expression_for_loop = 249,      /* expression_for_loop  */
  YYSYMBOL_250_4 = 250,                    /* $@4  */
  YYSYMBOL_expression_unsafe = 251,        /* expression_unsafe  */
  YYSYMBOL_expression_while_loop = 252,    /* expression_while_loop  */
  YYSYMBOL_expression_with = 253,          /* expression_with  */
  YYSYMBOL_expression_with_alias = 254,    /* expression_with_alias  */
  YYSYMBOL_255_5 = 255,                    /* $@5  */
  YYSYMBOL_annotation_argument_value = 256, /* annotation_argument_value  */
  YYSYMBOL_annotation_argument_value_list = 257, /* annotation_argument_value_list  */
  YYSYMBOL_annotation_argument_name = 258, /* annotation_argument_name  */
  YYSYMBOL_annotation_argument = 259,      /* annotation_argument  */
  YYSYMBOL_annotation_argument_list = 260, /* annotation_argument_list  */
  YYSYMBOL_metadata_argument_list = 261,   /* metadata_argument_list  */
  YYSYMBOL_annotation_declaration_name = 262, /* annotation_declaration_name  */
  YYSYMBOL_annotation_declaration_basic = 263, /* annotation_declaration_basic  */
  YYSYMBOL_annotation_declaration = 264,   /* annotation_declaration  */
  YYSYMBOL_annotation_list = 265,          /* annotation_list  */
  YYSYMBOL_optional_annotation_list = 266, /* optional_annotation_list  */
  YYSYMBOL_optional_function_argument_list = 267, /* optional_function_argument_list  */
  YYSYMBOL_optional_function_type = 268,   /* optional_function_type  */
  YYSYMBOL_function_name = 269,            /* function_name  */
  YYSYMBOL_global_function_declaration = 270, /* global_function_declaration  */
  YYSYMBOL_optional_public_or_private_function = 271, /* optional_public_or_private_function  */
  YYSYMBOL_function_declaration_header = 272, /* function_declaration_header  */
  YYSYMBOL_function_declaration = 273,     /* function_declaration  */
  YYSYMBOL_274_6 = 274,                    /* $@6  */
  YYSYMBOL_expression_block = 275,         /* expression_block  */
  YYSYMBOL_expr_call_pipe = 276,           /* expr_call_pipe  */
  YYSYMBOL_expression_any = 277,           /* expression_any  */
  YYSYMBOL_expressions = 278,              /* expressions  */
  YYSYMBOL_expr_keyword = 279,             /* expr_keyword  */
  YYSYMBOL_optional_expr_list = 280,       /* optional_expr_list  */
  YYSYMBOL_optional_expr_list_in_braces = 281, /* optional_expr_list_in_braces  */
  YYSYMBOL_optional_expr_map_tuple_list = 282, /* optional_expr_map_tuple_list  */
  YYSYMBOL_type_declaration_no_options_list = 283, /* type_declaration_no_options_list  */
  YYSYMBOL_expression_keyword = 284,       /* expression_keyword  */
  YYSYMBOL_285_7 = 285,                    /* $@7  */
  YYSYMBOL_286_8 = 286,                    /* $@8  */
  YYSYMBOL_287_9 = 287,                    /* $@9  */
  YYSYMBOL_288_10 = 288,                   /* $@10  */
  YYSYMBOL_expr_pipe = 289,                /* expr_pipe  */
  YYSYMBOL_name_in_namespace = 290,        /* name_in_namespace  */
  YYSYMBOL_expression_delete = 291,        /* expression_delete  */
  YYSYMBOL_new_type_declaration = 292,     /* new_type_declaration  */
  YYSYMBOL_293_11 = 293,                   /* $@11  */
  YYSYMBOL_294_12 = 294,                   /* $@12  */
  YYSYMBOL_expr_new = 295,                 /* expr_new  */
  YYSYMBOL_expression_break = 296,         /* expression_break  */
  YYSYMBOL_expression_continue = 297,      /* expression_continue  */
  YYSYMBOL_expression_return_no_pipe = 298, /* expression_return_no_pipe  */
  YYSYMBOL_expression_return = 299,        /* expression_return  */
  YYSYMBOL_expression_yield_no_pipe = 300, /* expression_yield_no_pipe  */
  YYSYMBOL_expression_yield = 301,         /* expression_yield  */
  YYSYMBOL_expression_try_catch = 302,     /* expression_try_catch  */
  YYSYMBOL_kwd_let_var_or_nothing = 303,   /* kwd_let_var_or_nothing  */
  YYSYMBOL_kwd_let = 304,                  /* kwd_let  */
  YYSYMBOL_optional_in_scope = 305,        /* optional_in_scope  */
  YYSYMBOL_tuple_expansion = 306,          /* tuple_expansion  */
  YYSYMBOL_tuple_expansion_variable_declaration = 307, /* tuple_expansion_variable_declaration  */
  YYSYMBOL_expression_let = 308,           /* expression_let  */
  YYSYMBOL_expr_cast = 309,                /* expr_cast  */
  YYSYMBOL_310_13 = 310,                   /* $@13  */
  YYSYMBOL_311_14 = 311,                   /* $@14  */
  YYSYMBOL_312_15 = 312,                   /* $@15  */
  YYSYMBOL_313_16 = 313,                   /* $@16  */
  YYSYMBOL_314_17 = 314,                   /* $@17  */
  YYSYMBOL_315_18 = 315,                   /* $@18  */
  YYSYMBOL_expr_type_decl = 316,           /* expr_type_decl  */
  YYSYMBOL_317_19 = 317,                   /* $@19  */
  YYSYMBOL_318_20 = 318,                   /* $@20  */
  YYSYMBOL_expr_type_info = 319,           /* expr_type_info  */
  YYSYMBOL_expr_list = 320,                /* expr_list  */
  YYSYMBOL_block_or_simple_block = 321,    /* block_or_simple_block  */
  YYSYMBOL_block_or_lambda = 322,          /* block_or_lambda  */
  YYSYMBOL_capture_entry = 323,            /* capture_entry  */
  YYSYMBOL_capture_list = 324,             /* capture_list  */
  YYSYMBOL_optional_capture_list = 325,    /* optional_capture_list  */
  YYSYMBOL_expr_block = 326,               /* expr_block  */
  YYSYMBOL_expr_full_block = 327,          /* expr_full_block  */
  YYSYMBOL_expr_full_block_assumed_piped = 328, /* expr_full_block_assumed_piped  */
  YYSYMBOL_329_21 = 329,                   /* $@21  */
  YYSYMBOL_expr_numeric_const = 330,       /* expr_numeric_const  */
  YYSYMBOL_expr_assign = 331,              /* expr_assign  */
  YYSYMBOL_expr_assign_pipe_right = 332,   /* expr_assign_pipe_right  */
  YYSYMBOL_expr_assign_pipe = 333,         /* expr_assign_pipe  */
  YYSYMBOL_expr_named_call = 334,          /* expr_named_call  */
  YYSYMBOL_expr_method_call = 335,         /* expr_method_call  */
  YYSYMBOL_func_addr_name = 336,           /* func_addr_name  */
  YYSYMBOL_func_addr_expr = 337,           /* func_addr_expr  */
  YYSYMBOL_338_22 = 338,                   /* $@22  */
  YYSYMBOL_339_23 = 339,                   /* $@23  */
  YYSYMBOL_340_24 = 340,                   /* $@24  */
  YYSYMBOL_341_25 = 341,                   /* $@25  */
  YYSYMBOL_expr_field = 342,               /* expr_field  */
  YYSYMBOL_343_26 = 343,                   /* $@26  */
  YYSYMBOL_344_27 = 344,                   /* $@27  */
  YYSYMBOL_expr_call = 345,                /* expr_call  */
  YYSYMBOL_expr = 346,                     /* expr  */
  YYSYMBOL_347_28 = 347,                   /* $@28  */
  YYSYMBOL_348_29 = 348,                   /* $@29  */
  YYSYMBOL_349_30 = 349,                   /* $@30  */
  YYSYMBOL_350_31 = 350,                   /* $@31  */
  YYSYMBOL_351_32 = 351,                   /* $@32  */
  YYSYMBOL_352_33 = 352,                   /* $@33  */
  YYSYMBOL_expr_mtag = 353,                /* expr_mtag  */
  YYSYMBOL_optional_field_annotation = 354, /* optional_field_annotation  */
  YYSYMBOL_optional_override = 355,        /* optional_override  */
  YYSYMBOL_optional_constant = 356,        /* optional_constant  */
  YYSYMBOL_optional_public_or_private_member_variable = 357, /* optional_public_or_private_member_variable  */
  YYSYMBOL_optional_static_member_variable = 358, /* optional_static_member_variable  */
  YYSYMBOL_structure_variable_declaration = 359, /* structure_variable_declaration  */
  YYSYMBOL_struct_variable_declaration_list = 360, /* struct_variable_declaration_list  */
  YYSYMBOL_361_34 = 361,                   /* $@34  */
  YYSYMBOL_362_35 = 362,                   /* $@35  */
  YYSYMBOL_363_36 = 363,                   /* $@36  */
  YYSYMBOL_function_argument_declaration = 364, /* function_argument_declaration  */
  YYSYMBOL_function_argument_list = 365,   /* function_argument_list  */
  YYSYMBOL_tuple_type = 366,               /* tuple_type  */
  YYSYMBOL_tuple_type_list = 367,          /* tuple_type_list  */
  YYSYMBOL_tuple_alias_type_list = 368,    /* tuple_alias_type_list  */
  YYSYMBOL_variant_type = 369,             /* variant_type  */
  YYSYMBOL_variant_type_list = 370,        /* variant_type_list  */
  YYSYMBOL_variant_alias_type_list = 371,  /* variant_alias_type_list  */
  YYSYMBOL_copy_or_move = 372,             /* copy_or_move  */
  YYSYMBOL_variable_declaration = 373,     /* variable_declaration  */
  YYSYMBOL_copy_or_move_or_clone = 374,    /* copy_or_move_or_clone  */
  YYSYMBOL_optional_ref = 375,             /* optional_ref  */
  YYSYMBOL_let_variable_name_with_pos_list = 376, /* let_variable_name_with_pos_list  */
  YYSYMBOL_let_variable_declaration = 377, /* let_variable_declaration  */
  YYSYMBOL_global_variable_declaration_list = 378, /* global_variable_declaration_list  */
  YYSYMBOL_379_37 = 379,                   /* $@37  */
  YYSYMBOL_optional_shared = 380,          /* optional_shared  */
  YYSYMBOL_optional_public_or_private_variable = 381, /* optional_public_or_private_variable  */
  YYSYMBOL_global_let = 382,               /* global_let  */
  YYSYMBOL_383_38 = 383,                   /* $@38  */
  YYSYMBOL_enum_list = 384,                /* enum_list  */
  YYSYMBOL_optional_public_or_private_alias = 385, /* optional_public_or_private_alias  */
  YYSYMBOL_single_alias = 386,             /* single_alias  */
  YYSYMBOL_387_39 = 387,                   /* $@39  */
  YYSYMBOL_alias_list = 388,               /* alias_list  */
  YYSYMBOL_alias_declaration = 389,        /* alias_declaration  */
  YYSYMBOL_390_40 = 390,                   /* $@40  */
  YYSYMBOL_optional_public_or_private_enum = 391, /* optional_public_or_private_enum  */
  YYSYMBOL_enum_name = 392,                /* enum_name  */
  YYSYMBOL_enum_declaration = 393,         /* enum_declaration  */
  YYSYMBOL_394_41 = 394,                   /* $@41  */
  YYSYMBOL_395_42 = 395,                   /* $@42  */
  YYSYMBOL_396_43 = 396,                   /* $@43  */
  YYSYMBOL_397_44 = 397,                   /* $@44  */
  YYSYMBOL_optional_structure_parent = 398, /* optional_structure_parent  */
  YYSYMBOL_optional_sealed = 399,          /* optional_sealed  */
  YYSYMBOL_structure_name = 400,           /* structure_name  */
  YYSYMBOL_class_or_struct = 401,          /* class_or_struct  */
  YYSYMBOL_optional_public_or_private_structure = 402, /* optional_public_or_private_structure  */
  YYSYMBOL_optional_struct_variable_declaration_list = 403, /* optional_struct_variable_declaration_list  */
  YYSYMBOL_structure_declaration = 404,    /* structure_declaration  */
  YYSYMBOL_405_45 = 405,                   /* $@45  */
  YYSYMBOL_406_46 = 406,                   /* $@46  */
  YYSYMBOL_variable_name_with_pos_list = 407, /* variable_name_with_pos_list  */
  YYSYMBOL_basic_type_declaration = 408,   /* basic_type_declaration  */
  YYSYMBOL_enum_basic_type_declaration = 409, /* enum_basic_type_declaration  */
  YYSYMBOL_structure_type_declaration = 410, /* structure_type_declaration  */
  YYSYMBOL_auto_type_declaration = 411,    /* auto_type_declaration  */
  YYSYMBOL_bitfield_bits = 412,            /* bitfield_bits  */
  YYSYMBOL_bitfield_alias_bits = 413,      /* bitfield_alias_bits  */
  YYSYMBOL_bitfield_type_declaration = 414, /* bitfield_type_declaration  */
  YYSYMBOL_415_47 = 415,                   /* $@47  */
  YYSYMBOL_416_48 = 416,                   /* $@48  */
  YYSYMBOL_c_or_s = 417,                   /* c_or_s  */
  YYSYMBOL_table_type_pair = 418,          /* table_type_pair  */
  YYSYMBOL_dim_list = 419,                 /* dim_list  */
  YYSYMBOL_type_declaration_no_options = 420, /* type_declaration_no_options  */
  YYSYMBOL_421_49 = 421,                   /* $@49  */
  YYSYMBOL_422_50 = 422,                   /* $@50  */
  YYSYMBOL_423_51 = 423,                   /* $@51  */
  YYSYMBOL_424_52 = 424,                   /* $@52  */
  YYSYMBOL_425_53 = 425,                   /* $@53  */
  YYSYMBOL_426_54 = 426,                   /* $@54  */
  YYSYMBOL_427_55 = 427,                   /* $@55  */
  YYSYMBOL_428_56 = 428,                   /* $@56  */
  YYSYMBOL_429_57 = 429,                   /* $@57  */
  YYSYMBOL_430_58 = 430,                   /* $@58  */
  YYSYMBOL_431_59 = 431,                   /* $@59  */
  YYSYMBOL_432_60 = 432,                   /* $@60  */
  YYSYMBOL_433_61 = 433,                   /* $@61  */
  YYSYMBOL_434_62 = 434,                   /* $@62  */
  YYSYMBOL_435_63 = 435,                   /* $@63  */
  YYSYMBOL_436_64 = 436,                   /* $@64  */
  YYSYMBOL_437_65 = 437,                   /* $@65  */
  YYSYMBOL_438_66 = 438,                   /* $@66  */
  YYSYMBOL_439_67 = 439,                   /* $@67  */
  YYSYMBOL_440_68 = 440,                   /* $@68  */
  YYSYMBOL_441_69 = 441,                   /* $@69  */
  YYSYMBOL_442_70 = 442,                   /* $@70  */
  YYSYMBOL_443_71 = 443,                   /* $@71  */
  YYSYMBOL_444_72 = 444,                   /* $@72  */
  YYSYMBOL_445_73 = 445,                   /* $@73  */
  YYSYMBOL_446_74 = 446,                   /* $@74  */
  YYSYMBOL_447_75 = 447,                   /* $@75  */
  YYSYMBOL_type_declaration = 448,         /* type_declaration  */
  YYSYMBOL_tuple_alias_declaration = 449,  /* tuple_alias_declaration  */
  YYSYMBOL_450_76 = 450,                   /* $@76  */
  YYSYMBOL_451_77 = 451,                   /* $@77  */
  YYSYMBOL_452_78 = 452,                   /* $@78  */
  YYSYMBOL_453_79 = 453,                   /* $@79  */
  YYSYMBOL_variant_alias_declaration = 454, /* variant_alias_declaration  */
  YYSYMBOL_455_80 = 455,                   /* $@80  */
  YYSYMBOL_456_81 = 456,                   /* $@81  */
  YYSYMBOL_457_82 = 457,                   /* $@82  */
  YYSYMBOL_458_83 = 458,                   /* $@83  */
  YYSYMBOL_bitfield_alias_declaration = 459, /* bitfield_alias_declaration  */
  YYSYMBOL_460_84 = 460,                   /* $@84  */
  YYSYMBOL_461_85 = 461,                   /* $@85  */
  YYSYMBOL_462_86 = 462,                   /* $@86  */
  YYSYMBOL_463_87 = 463,                   /* $@87  */
  YYSYMBOL_make_decl = 464,                /* make_decl  */
  YYSYMBOL_make_struct_fields = 465,       /* make_struct_fields  */
  YYSYMBOL_make_variant_dim = 466,         /* make_variant_dim  */
  YYSYMBOL_make_struct_single = 467,       /* make_struct_single  */
  YYSYMBOL_make_struct_dim = 468,          /* make_struct_dim  */
  YYSYMBOL_make_struct_dim_list = 469,     /* make_struct_dim_list  */
  YYSYMBOL_make_struct_dim_decl = 470,     /* make_struct_dim_decl  */
  YYSYMBOL_optional_make_struct_dim_decl = 471, /* optional_make_struct_dim_decl  */
  YYSYMBOL_optional_block = 472,           /* optional_block  */
  YYSYMBOL_optional_trailing_semicolon_cur_cur = 473, /* optional_trailing_semicolon_cur_cur  */
  YYSYMBOL_optional_trailing_semicolon_cur_sqr = 474, /* optional_trailing_semicolon_cur_sqr  */
  YYSYMBOL_optional_trailing_semicolon_sqr_sqr = 475, /* optional_trailing_semicolon_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_sqr_sqr = 476, /* optional_trailing_delim_sqr_sqr  */
  YYSYMBOL_optional_trailing_delim_cur_sqr = 477, /* optional_trailing_delim_cur_sqr  */
  YYSYMBOL_use_initializer = 478,          /* use_initializer  */
  YYSYMBOL_make_struct_decl = 479,         /* make_struct_decl  */
  YYSYMBOL_480_88 = 480,                   /* $@88  */
  YYSYMBOL_481_89 = 481,                   /* $@89  */
  YYSYMBOL_482_90 = 482,                   /* $@90  */
  YYSYMBOL_483_91 = 483,                   /* $@91  */
  YYSYMBOL_484_92 = 484,                   /* $@92  */
  YYSYMBOL_485_93 = 485,                   /* $@93  */
  YYSYMBOL_486_94 = 486,                   /* $@94  */
  YYSYMBOL_487_95 = 487,                   /* $@95  */
  YYSYMBOL_make_tuple = 488,               /* make_tuple  */
  YYSYMBOL_make_map_tuple = 489,           /* make_map_tuple  */
  YYSYMBOL_make_tuple_call = 490,          /* make_tuple_call  */
  YYSYMBOL_491_96 = 491,                   /* $@96  */
  YYSYMBOL_492_97 = 492,                   /* $@97  */
  YYSYMBOL_make_dim = 493,                 /* make_dim  */
  YYSYMBOL_make_dim_decl = 494,            /* make_dim_decl  */
  YYSYMBOL_495_98 = 495,                   /* $@98  */
  YYSYMBOL_496_99 = 496,                   /* $@99  */
  YYSYMBOL_497_100 = 497,                  /* $@100  */
  YYSYMBOL_498_101 = 498,                  /* $@101  */
  YYSYMBOL_499_102 = 499,                  /* $@102  */
  YYSYMBOL_500_103 = 500,                  /* $@103  */
  YYSYMBOL_501_104 = 501,                  /* $@104  */
  YYSYMBOL_502_105 = 502,                  /* $@105  */
  YYSYMBOL_503_106 = 503,                  /* $@106  */
  YYSYMBOL_504_107 = 504,                  /* $@107  */
  YYSYMBOL_make_table = 505,               /* make_table  */
  YYSYMBOL_expr_map_tuple_list = 506,      /* expr_map_tuple_list  */
  YYSYMBOL_make_table_decl = 507,          /* make_table_decl  */
  YYSYMBOL_array_comprehension_where = 508, /* array_comprehension_where  */
  YYSYMBOL_optional_comma = 509,           /* optional_comma  */
  YYSYMBOL_array_comprehension = 510       /* array_comprehension  */
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
#define YYLAST   14661

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  217
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  294
/* YYNRULES -- Number of rules.  */
#define YYNRULES  883
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1672

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   444


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
       2,     2,     2,   203,     2,   216,   214,   199,   192,     2,
     212,   213,   197,   196,   186,   195,   209,   198,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   189,   180,
     193,   187,   194,   188,   215,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   210,     2,   211,   191,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   178,   190,   179,   202,     2,     2,     2,
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
     175,   176,   177,   181,   182,   183,   184,   185,   200,   201,
     204,   205,   206,   207,   208
};

#if DAS_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   542,   542,   543,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   564,   570,   571,
     572,   576,   577,   581,   599,   600,   601,   602,   606,   610,
     615,   624,   632,   648,   653,   661,   661,   700,   730,   734,
     735,   736,   740,   743,   747,   753,   762,   765,   771,   772,
     776,   780,   781,   785,   788,   794,   800,   803,   809,   810,
     814,   815,   816,   825,   826,   830,   831,   831,   837,   838,
     839,   840,   841,   845,   851,   851,   857,   857,   863,   871,
     881,   890,   890,   897,   898,   899,   900,   901,   902,   906,
     911,   919,   920,   921,   925,   926,   927,   928,   929,   930,
     931,   932,   938,   941,   947,   950,   953,   959,   960,   961,
     965,   978,   996,   999,  1007,  1018,  1029,  1040,  1043,  1050,
    1054,  1061,  1062,  1066,  1067,  1068,  1072,  1075,  1082,  1086,
    1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,
    1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,
    1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,
    1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
    1147,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,  1156,
    1157,  1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,
    1167,  1168,  1169,  1174,  1192,  1193,  1194,  1198,  1204,  1204,
    1221,  1225,  1236,  1245,  1254,  1260,  1261,  1262,  1263,  1264,
    1265,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,
    1275,  1276,  1277,  1278,  1279,  1280,  1284,  1289,  1295,  1301,
    1312,  1313,  1317,  1318,  1322,  1323,  1327,  1331,  1338,  1338,
    1338,  1344,  1344,  1344,  1353,  1387,  1390,  1393,  1396,  1402,
    1403,  1414,  1418,  1421,  1429,  1429,  1429,  1432,  1438,  1441,
    1445,  1449,  1456,  1463,  1469,  1473,  1477,  1480,  1483,  1491,
    1494,  1497,  1505,  1508,  1516,  1519,  1522,  1530,  1536,  1537,
    1538,  1542,  1543,  1547,  1548,  1552,  1557,  1565,  1571,  1577,
    1586,  1598,  1601,  1607,  1607,  1607,  1610,  1610,  1610,  1615,
    1615,  1615,  1623,  1623,  1623,  1629,  1639,  1650,  1663,  1673,
    1684,  1699,  1702,  1708,  1709,  1716,  1727,  1728,  1729,  1733,
    1734,  1735,  1736,  1740,  1745,  1753,  1754,  1758,  1763,  1770,
    1777,  1777,  1786,  1787,  1788,  1789,  1790,  1791,  1792,  1796,
    1797,  1798,  1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,
    1807,  1808,  1809,  1810,  1811,  1812,  1813,  1814,  1818,  1819,
    1820,  1821,  1826,  1827,  1828,  1829,  1830,  1831,  1832,  1833,
    1834,  1835,  1836,  1837,  1838,  1839,  1840,  1841,  1842,  1847,
    1854,  1866,  1871,  1881,  1885,  1892,  1895,  1895,  1895,  1900,
    1900,  1900,  1913,  1917,  1921,  1926,  1933,  1933,  1933,  1940,
    1944,  1953,  1957,  1960,  1966,  1967,  1968,  1969,  1970,  1971,
    1972,  1973,  1974,  1975,  1976,  1977,  1978,  1979,  1980,  1981,
    1982,  1983,  1984,  1985,  1986,  1987,  1988,  1989,  1990,  1991,
    1992,  1993,  1994,  1995,  1996,  1997,  1998,  1999,  2000,  2001,
    2007,  2008,  2009,  2010,  2011,  2024,  2025,  2026,  2027,  2028,
    2029,  2030,  2031,  2032,  2033,  2034,  2035,  2038,  2041,  2042,
    2045,  2045,  2045,  2048,  2053,  2057,  2061,  2061,  2061,  2066,
    2069,  2073,  2073,  2073,  2078,  2081,  2082,  2083,  2084,  2085,
    2086,  2087,  2088,  2089,  2091,  2095,  2096,  2101,  2105,  2106,
    2107,  2108,  2109,  2110,  2111,  2115,  2119,  2123,  2127,  2131,
    2135,  2139,  2143,  2147,  2154,  2155,  2156,  2160,  2161,  2162,
    2166,  2167,  2171,  2172,  2173,  2177,  2178,  2182,  2193,  2196,
    2196,  2215,  2214,  2229,  2228,  2241,  2250,  2259,  2269,  2270,
    2274,  2277,  2286,  2287,  2291,  2294,  2297,  2313,  2322,  2323,
    2327,  2330,  2333,  2347,  2348,  2352,  2358,  2364,  2367,  2371,
    2377,  2386,  2387,  2388,  2392,  2393,  2397,  2404,  2409,  2418,
    2424,  2435,  2438,  2443,  2448,  2456,  2467,  2470,  2470,  2490,
    2491,  2495,  2496,  2497,  2501,  2504,  2504,  2523,  2526,  2529,
    2544,  2563,  2564,  2565,  2570,  2570,  2596,  2597,  2601,  2602,
    2602,  2606,  2607,  2608,  2612,  2622,  2627,  2622,  2639,  2644,
    2639,  2659,  2660,  2664,  2665,  2669,  2675,  2676,  2680,  2681,
    2682,  2686,  2689,  2695,  2700,  2695,  2714,  2721,  2726,  2735,
    2741,  2752,  2753,  2754,  2755,  2756,  2757,  2758,  2759,  2760,
    2761,  2762,  2763,  2764,  2765,  2766,  2767,  2768,  2769,  2770,
    2771,  2772,  2773,  2774,  2775,  2776,  2777,  2778,  2782,  2783,
    2784,  2785,  2786,  2787,  2788,  2789,  2793,  2804,  2808,  2815,
    2827,  2834,  2843,  2848,  2851,  2864,  2864,  2864,  2877,  2878,
    2882,  2886,  2893,  2897,  2904,  2905,  2906,  2907,  2908,  2923,
    2929,  2929,  2929,  2933,  2938,  2945,  2945,  2952,  2956,  2960,
    2965,  2970,  2975,  2980,  2984,  2988,  2993,  2997,  3001,  3006,
    3006,  3006,  3012,  3019,  3019,  3019,  3024,  3024,  3024,  3030,
    3030,  3030,  3035,  3039,  3039,  3039,  3044,  3044,  3044,  3053,
    3057,  3057,  3057,  3062,  3062,  3062,  3071,  3075,  3075,  3075,
    3080,  3080,  3080,  3089,  3089,  3089,  3095,  3095,  3095,  3104,
    3107,  3118,  3134,  3134,  3139,  3144,  3134,  3169,  3169,  3174,
    3180,  3169,  3205,  3205,  3210,  3215,  3205,  3245,  3246,  3247,
    3248,  3249,  3253,  3260,  3267,  3273,  3279,  3286,  3293,  3299,
    3308,  3314,  3322,  3327,  3334,  3339,  3346,  3351,  3357,  3358,
    3362,  3363,  3368,  3369,  3373,  3374,  3378,  3379,  3383,  3384,
    3385,  3389,  3390,  3391,  3395,  3396,  3400,  3406,  3413,  3421,
    3428,  3436,  3445,  3445,  3445,  3453,  3453,  3453,  3460,  3460,
    3460,  3467,  3467,  3467,  3478,  3481,  3487,  3501,  3507,  3513,
    3519,  3519,  3519,  3529,  3534,  3541,  3549,  3554,  3561,  3561,
    3561,  3571,  3571,  3571,  3581,  3581,  3581,  3591,  3599,  3599,
    3599,  3618,  3625,  3625,  3625,  3635,  3640,  3647,  3650,  3656,
    3664,  3673,  3681,  3701,  3726,  3727,  3731,  3732,  3737,  3740,
    3743,  3746,  3749,  3752
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
  "\"struct\"", "\"class\"", "\"let\"", "\"def\"", "\"while\"", "\"if\"",
  "\"static_if\"", "\"else\"", "\"for\"", "\"recover\"", "\"true\"",
  "\"false\"", "\"new\"", "\"typeinfo\"", "\"type\"", "\"in\"", "\"is\"",
  "\"as\"", "\"elif\"", "\"static_elif\"", "\"array\"", "\"return\"",
  "\"null\"", "\"break\"", "\"try\"", "\"options\"", "\"table\"",
  "\"expect\"", "\"const\"", "\"require\"", "\"operator\"", "\"enum\"",
  "\"finally\"", "\"delete\"", "\"deref\"", "\"typedef\"", "\"typedecl\"",
  "\"with\"", "\"aka\"", "\"assume\"", "\"cast\"", "\"override\"",
  "\"abstract\"", "\"upcast\"", "\"iterator\"", "\"var\"", "\"addr\"",
  "\"continue\"", "\"where\"", "\"pass\"", "\"reinterpret\"", "\"module\"",
  "\"public\"", "\"label\"", "\"goto\"", "\"implicit\"", "\"explicit\"",
  "\"shared\"", "\"private\"", "\"smart_ptr\"", "\"unsafe\"",
  "\"inscope\"", "\"static\"", "\"fixed_array\"", "\"default\"",
  "\"uninitialized\"", "\"bool\"", "\"void\"", "\"string\"", "\"auto\"",
  "\"int\"", "\"int2\"", "\"int3\"", "\"int4\"", "\"uint\"",
  "\"bitfield\"", "\"uint2\"", "\"uint3\"", "\"uint4\"", "\"float\"",
  "\"float2\"", "\"float3\"", "\"float4\"", "\"range\"", "\"urange\"",
  "\"range64\"", "\"urange64\"", "\"block\"", "\"int64\"", "\"uint64\"",
  "\"double\"", "\"function\"", "\"lambda\"", "\"int8\"", "\"uint8\"",
  "\"int16\"", "\"uint16\"", "\"tuple\"", "\"variant\"", "\"generator\"",
  "\"yield\"", "\"sealed\"", "\"+=\"", "\"-=\"", "\"/=\"", "\"*=\"",
  "\"%=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<\"", "\">>\"", "\"++\"",
  "\"--\"", "\"<=\"", "\"<<=\"", "\">>=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"->\"", "\"<-\"", "\"??\"", "\"?.\"", "\"?[\"", "\"<|\"", "\" <|\"",
  "\"$ <|\"", "\"@ <|\"", "\"@@ <|\"", "\"|>\"", "\":=\"", "\"<<<\"",
  "\">>>\"", "\"<<<=\"", "\">>>=\"", "\"=>\"", "\"::\"", "\"&&\"",
  "\"||\"", "\"^^\"", "\"&&=\"", "\"||=\"", "\"^^=\"", "\"..\"", "\"$$\"",
  "\"$i\"", "\"$v\"", "\"$b\"", "\"$a\"", "\"$t\"", "\"$c\"", "\"$f\"",
  "\"...\"", "\"[[\"", "\"[{\"", "\"{{\"", "\"integer constant\"",
  "\"long integer constant\"", "\"unsigned integer constant\"",
  "\"unsigned long integer constant\"", "\"unsigned int8 constant\"",
  "\"floating point constant\"", "\"double constant\"", "\"name\"",
  "\"keyword\"", "\"type function\"", "\"start of the string\"",
  "STRING_CHARACTER", "STRING_CHARACTER_ESC", "\"end of the string\"",
  "\"{\"", "\"}\"", "\"end of failed eader macro\"",
  "\"begin of code block\"", "\"end of code block\"",
  "\"end of expression\"", "\";}}\"", "\";}]\"", "\";]]\"", "\",]]\"",
  "\",}]\"", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'",
  "'-'", "'+'", "'*'", "'/'", "'%'", "UNARY_MINUS", "UNARY_PLUS", "'~'",
  "'!'", "PRE_INC", "PRE_DEC", "POST_INC", "POST_DEC", "DEREF", "'.'",
  "'['", "']'", "'('", "')'", "'$'", "'@'", "'#'", "$accept", "program",
  "top_level_reader_macro", "optional_public_or_private_module",
  "module_name", "module_declaration", "character_sequence",
  "string_constant", "string_builder_body", "string_builder",
  "reader_character_sequence", "expr_reader", "$@1", "options_declaration",
  "require_declaration", "keyword_or_name", "require_module_name",
  "require_module", "is_public_module", "expect_declaration",
  "expect_list", "expect_error", "expression_label", "expression_goto",
  "elif_or_static_elif", "expression_else", "if_or_static_if",
  "expression_else_one_liner", "$@2", "expression_if_one_liner",
  "expression_if_then_else", "$@3", "expression_for_loop", "$@4",
  "expression_unsafe", "expression_while_loop", "expression_with",
  "expression_with_alias", "$@5", "annotation_argument_value",
  "annotation_argument_value_list", "annotation_argument_name",
  "annotation_argument", "annotation_argument_list",
  "metadata_argument_list", "annotation_declaration_name",
  "annotation_declaration_basic", "annotation_declaration",
  "annotation_list", "optional_annotation_list",
  "optional_function_argument_list", "optional_function_type",
  "function_name", "global_function_declaration",
  "optional_public_or_private_function", "function_declaration_header",
  "function_declaration", "$@6", "expression_block", "expr_call_pipe",
  "expression_any", "expressions", "expr_keyword", "optional_expr_list",
  "optional_expr_list_in_braces", "optional_expr_map_tuple_list",
  "type_declaration_no_options_list", "expression_keyword", "$@7", "$@8",
  "$@9", "$@10", "expr_pipe", "name_in_namespace", "expression_delete",
  "new_type_declaration", "$@11", "$@12", "expr_new", "expression_break",
  "expression_continue", "expression_return_no_pipe", "expression_return",
  "expression_yield_no_pipe", "expression_yield", "expression_try_catch",
  "kwd_let_var_or_nothing", "kwd_let", "optional_in_scope",
  "tuple_expansion", "tuple_expansion_variable_declaration",
  "expression_let", "expr_cast", "$@13", "$@14", "$@15", "$@16", "$@17",
  "$@18", "expr_type_decl", "$@19", "$@20", "expr_type_info", "expr_list",
  "block_or_simple_block", "block_or_lambda", "capture_entry",
  "capture_list", "optional_capture_list", "expr_block", "expr_full_block",
  "expr_full_block_assumed_piped", "$@21", "expr_numeric_const",
  "expr_assign", "expr_assign_pipe_right", "expr_assign_pipe",
  "expr_named_call", "expr_method_call", "func_addr_name",
  "func_addr_expr", "$@22", "$@23", "$@24", "$@25", "expr_field", "$@26",
  "$@27", "expr_call", "expr", "$@28", "$@29", "$@30", "$@31", "$@32",
  "$@33", "expr_mtag", "optional_field_annotation", "optional_override",
  "optional_constant", "optional_public_or_private_member_variable",
  "optional_static_member_variable", "structure_variable_declaration",
  "struct_variable_declaration_list", "$@34", "$@35", "$@36",
  "function_argument_declaration", "function_argument_list", "tuple_type",
  "tuple_type_list", "tuple_alias_type_list", "variant_type",
  "variant_type_list", "variant_alias_type_list", "copy_or_move",
  "variable_declaration", "copy_or_move_or_clone", "optional_ref",
  "let_variable_name_with_pos_list", "let_variable_declaration",
  "global_variable_declaration_list", "$@37", "optional_shared",
  "optional_public_or_private_variable", "global_let", "$@38", "enum_list",
  "optional_public_or_private_alias", "single_alias", "$@39", "alias_list",
  "alias_declaration", "$@40", "optional_public_or_private_enum",
  "enum_name", "enum_declaration", "$@41", "$@42", "$@43", "$@44",
  "optional_structure_parent", "optional_sealed", "structure_name",
  "class_or_struct", "optional_public_or_private_structure",
  "optional_struct_variable_declaration_list", "structure_declaration",
  "$@45", "$@46", "variable_name_with_pos_list", "basic_type_declaration",
  "enum_basic_type_declaration", "structure_type_declaration",
  "auto_type_declaration", "bitfield_bits", "bitfield_alias_bits",
  "bitfield_type_declaration", "$@47", "$@48", "c_or_s", "table_type_pair",
  "dim_list", "type_declaration_no_options", "$@49", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "$@59", "$@60",
  "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69",
  "$@70", "$@71", "$@72", "$@73", "$@74", "$@75", "type_declaration",
  "tuple_alias_declaration", "$@76", "$@77", "$@78", "$@79",
  "variant_alias_declaration", "$@80", "$@81", "$@82", "$@83",
  "bitfield_alias_declaration", "$@84", "$@85", "$@86", "$@87",
  "make_decl", "make_struct_fields", "make_variant_dim",
  "make_struct_single", "make_struct_dim", "make_struct_dim_list",
  "make_struct_dim_decl", "optional_make_struct_dim_decl",
  "optional_block", "optional_trailing_semicolon_cur_cur",
  "optional_trailing_semicolon_cur_sqr",
  "optional_trailing_semicolon_sqr_sqr", "optional_trailing_delim_sqr_sqr",
  "optional_trailing_delim_cur_sqr", "use_initializer", "make_struct_decl",
  "$@88", "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95",
  "make_tuple", "make_map_tuple", "make_tuple_call", "$@96", "$@97",
  "make_dim", "make_dim_decl", "$@98", "$@99", "$@100", "$@101", "$@102",
  "$@103", "$@104", "$@105", "$@106", "$@107", "make_table",
  "expr_map_tuple_list", "make_table_decl", "array_comprehension_where",
  "optional_comma", "array_comprehension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1477)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-751)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1477,   103, -1477, -1477,    35,   -60,   188,     1, -1477,   -69,
     584,   584,   584, -1477,    60,    23, -1477, -1477,    49, -1477,
   -1477, -1477,   523, -1477,   127, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477,    83, -1477,    56,   161,   224,
   -1477, -1477, -1477, -1477,   188, -1477,     9, -1477,   584,   584,
   -1477, -1477,   127, -1477, -1477, -1477, -1477, -1477,   247,   233,
   -1477, -1477, -1477,    23,    23,    23,   236, -1477,   675,   -72,
   -1477, -1477, -1477, -1477,   607,   616,   625, -1477,   659,    48,
      35,   292,   -60,   260,   325, -1477,   660,   660, -1477,   348,
     379,    -4,   402,   683,   430,   435,   446, -1477,   469,   481,
   -1477, -1477,   -67,    35,    23,    23,    23,    23, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477,   515, -1477, -1477, -1477, -1477,
   -1477,   486, -1477, -1477, -1477, -1477, -1477,   -55,   123, -1477,
   -1477, -1477, -1477,   630, -1477, -1477, -1477, -1477, -1477,   511,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,   522,
   -1477,   206, -1477,   412,   565,   675, 14493, -1477,   368,   627,
   -1477,   -91, -1477, -1477,   688, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477,   209, -1477,   555, -1477,   571,   590,   598, -1477,
   -1477, 12982, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477,   746,   751, -1477,
     588,   615, -1477,   492, -1477,   628, -1477,   642,    35,    35,
      75,   113, -1477, -1477, -1477,   123, -1477,  9827, -1477, -1477,
   -1477,   663,   666, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477,   669,   631, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477,   824, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
     679,   646, -1477, -1477,    90,   664, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477,   680,   684,   694,
   -1477,   -91,   100, -1477, -1477,    35,   665,   838,   539, -1477,
   -1477,   689,   693,   696,   681,   698,   701, -1477, -1477, -1477,
     685, -1477, -1477, -1477, -1477, -1477,   702, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,   703, -1477,
   -1477, -1477,   705,   708, -1477, -1477, -1477, -1477,   710,   711,
     700,    60, -1477, -1477, -1477, -1477, -1477,  1303,   715, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477,   739,   775, -1477,   706,
   -1477,   107, -1477,   129,  9827, -1477,  1788,   183, -1477,    60,
   -1477, -1477, -1477,   113,   697, -1477,  9121,   745,   747,  9827,
   -1477,   109, -1477, -1477, -1477,  9121, -1477, -1477,   754, -1477,
     389,   406,   417, -1477, -1477,  9121,   -93, -1477, -1477, -1477,
      14, -1477, -1477, -1477,    29,  5481, -1477,   709,  9583,   401,
    9682,   474, -1477, -1477,  9121, -1477, -1477,   426,   114, -1477,
     715, -1477,   733,   736,  9121, -1477, -1477, -1477, -1477, -1477,
     773,   -57,   737,    39,  3251, -1477, -1477,   615,   155,  5683,
     722,  9121,   767,   743,   744,   726, -1477,   762,   750,   783,
    5885,   -82,   290,   753, -1477,   293,   755,   756,  3453,  9121,
    9121,    84,    84,    84,   735,   738,   740,   742,   748,   749,
   -1477,  2022,  9484,  6089, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477,  6291,   764, -1477,  6495,   919, -1477,  9121,  9121,  9121,
    9121,  9121,  4875,  9121, -1477,   741, -1477, -1477,   778,   782,
    9121,   954, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477,   101, -1477,   -53,   784, -1477,   785,   787,   788, -1477,
     792, -1477, -1477,   909, -1477, -1477, -1477, -1477,   765, -1477,
   -1477,    18, -1477, -1477, -1477, -1477, -1477, -1477,  1265, -1477,
     768, -1477, -1477, -1477, -1477, -1477, -1477,   229, -1477,   803,
   -1477, -1477,    21, -1477, -1477,   772,   794,   795, -1477,  2012,
   -1477,   943,   729, -1477, -1477, -1477,  3857,  9827,  9827,  9827,
    2203,  9827,  9827,   779,   823,  9827,   588,  9827,   588,  9827,
     588,  9926,   825, 10266, -1477,  9121, -1477, -1477, -1477, -1477,
     786, -1477, -1477, 12441,  9121, -1477,  1303,   815, -1477,   819,
      19, -1477, -1477,   561, -1477,   715,   820,   811,   561, -1477,
     822, 10376,   790,   961, -1477,   149, -1477, -1477, -1477, 13017,
     426, -1477,   793, -1477, -1477,    60,   362, -1477,   813,   814,
     816, -1477,  9121,  3857, -1477,   828,   878, 10009,   997,  9827,
    9121,  9121, 13758,  9121, 13017,   840, -1477, -1477,  9121, -1477,
   -1477,   826,   850, 13758,  9121, -1477, -1477,  9121, -1477, -1477,
    9121, -1477,  9827,  3857, -1477, 10009,   190,   190,   810, -1477,
     765, -1477, -1477, -1477,  9121,  9121,  9121,  9121,  9121,  9121,
     426,  2642,   426,  2845,   426, 13116, -1477,   686, -1477, 13017,
   -1477,   641,   426, -1477,   846,   855,   190,   190,   -39,   190,
     190,   426,  1022,   849, 13758,   849,   320, -1477, -1477, 13017,
   -1477, -1477, -1477, -1477,  5077, -1477, -1477, -1477, -1477, -1477,
   -1477,   196,   879,    84, -1477, 12856, 14385,  4059,  4059,  4059,
    4059,  4059,  4059,  4059,  4059,  9121,  9121, -1477, -1477,  9121,
    4059,  4059,  9121,  9121,  9121,   870,  4059,  9121,   441,  9121,
    9121,  9121,  9121,  9121,  9121,  4059,  4059,  9121,  9121,  9121,
    4059,  4059,  4059,  9121,  4059,  6697,  9121,  9121,  9121,  9121,
    9121,  9121,  9121,  9121,  9121,  9121,   -33,  9121, -1477,  6899,
   -1477,  9121, -1477,   183, -1477,    23,  1032,   -91,  9827, -1477,
     872, -1477,  3857, -1477, 10121,   358,   454,   847,   530, -1477,
     509,   529, -1477, -1477,    99,   580,   664,   581,   664,   589,
     664, -1477,   416, -1477,   485, -1477,  9827,   829,   849, -1477,
   -1477, 12540, -1477, -1477,  9827, -1477, -1477,  9827, -1477, -1477,
   -1477,  9121,   875, -1477,   876, -1477,  9827, -1477,  3857,  9827,
    9827, -1477,    36,  9827,  5279,  7101,   880,  9121,  9827, -1477,
   -1477, -1477,  9827,   849, -1477,   828,  9121,  9121,  9121,  9121,
    9121,  9121,  9121,  9121,  9121,  9121,  9121,  9121,  9121,  9121,
    9121,  9121,  9121,  9121,   615,   263,   834,   846, 13758, 10411,
   -1477, -1477,  9827,  9827, 10521,  9827, -1477, -1477, 10556,  9827,
     849,  9827,  9827,   849,  9827,   337, -1477, 10009, -1477,   879,
   10666, 10701, 10811, 10846, 10956, 10991,    38,    84,   837,     8,
    3048,  4263,  7303, 13215,   867,    -8,   207,   869,   151,    41,
    7505,    -8,   622,    47,  9121,   877,  9121, -1477, -1477,  9827,
   -1477,  9827, -1477,  9121,   638,    50,  9121,   881, -1477,    51,
     426,  9121,   848,   844,   852,   853,   419, -1477, -1477,   582,
    9121,   765,  4467, -1477,   226,   867,   845,   893,   893, -1477,
   -1477,    -9,   588, -1477,   874,   856, -1477, -1477,   883,   857,
   -1477, -1477,    84,    84,    84, -1477, -1477,  2051, -1477,  2051,
   -1477,  2051, -1477,  2051, -1477,  2051, -1477,  2051, -1477,  2051,
   -1477,  2051,   992,   992,  1008, -1477,  2051, -1477,  2051,  1008,
    9971,  9971,   859, -1477,  2051,   130,   860, -1477, 12639,    66,
      66,   768, 13758,   992,   992, -1477,  2051, -1477,  2051, 14098,
   13973, 14008, -1477,  2051, -1477,  2051, -1477,  2051, 13848, -1477,
    2051, 14416, 13250, 14126, 14216, 14244,  1008,  1008,   969,   969,
     130,   130,   130,   451,  9121,   861,   862,   482,  9121,  1069,
   12674, -1477,   268, 13340,   898,   159,   695,  1012,   899,   501,
   -1477, -1477, 10231, -1477, -1477, -1477, -1477,  9827, -1477, -1477,
     912, -1477, -1477,   887, -1477,   894, -1477,   896, -1477,  9926,
   -1477,   825,   496,   715, -1477, -1477, -1477,   715,   715, 11101,
   -1477,  1049,    -3, -1477, 10009,  1232,  1708,  9121,   592,   499,
     278,   886,   888,   924, 11136,   237, 11246,   597,  9827,  9827,
    9827,  1718,   889, 13758, 13758, 13758, 13758, 13758, 13758, 13758,
   13758, 13758, 13758, 13758, 13758, 13758, 13758, 13758, 13758, 13758,
   13758, -1477,   882,  9827, -1477, -1477, -1477,  9121,  2360,  2562,
   -1477,  2764, -1477,  2967,   900,  3170,  3373,   901,  3575,   879,
     588, -1477, -1477, -1477, -1477, -1477,   895,  9121, -1477,  9121,
    9121,  9121,  4671, 12441,    12,  9121,   506,   499,   207, -1477,
   -1477,   904, -1477,  9121,  9121, -1477,   906, -1477,  9121,   499,
     548,   916, -1477, -1477,  9121, 13758, -1477, -1477,   512,   528,
   13375,  9121, -1477, -1477,  2440,  9121,    55, -1477, -1477,  9121,
    9121,  9827,   588,   615, -1477, -1477,  9121, -1477,  1535,   879,
     187,  9323, -1477, -1477, -1477,   266,   288,   932,   944,   962,
     963, -1477,   274,   664, -1477,  9121, -1477,  9121, -1477, -1477,
   -1477,  7707,  9121, -1477,   940,   926, -1477, -1477,  9121,   927,
   -1477, 12773,  9121,  7909,   928, -1477, 12872, -1477, -1477, -1477,
   -1477, -1477,   966, -1477, -1477,   347, -1477,    26, -1477,   879,
   -1477, -1477, -1477, -1477,   715, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,   931,
    9827, -1477,   979,  9121, -1477, -1477,    11, -1477,   143,   935,
   -1477, -1477, -1477,   532, -1477,   981,   938, -1477, -1477,  3777,
    3979,  4056, -1477, -1477,  9121,  4183, 13758, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477,   593,   664,  8111,   574,
   11281, 13758, 13758,    -8,   207, 13758,   939,   162,   867, -1477,
   -1477, 13758,   869, -1477,   576,    -8,   941, -1477, -1477, -1477,
   -1477,   578, -1477, -1477, -1477,   583, -1477,   586,  9121, 11391,
   11426,  4260,   664, -1477, 13017, -1477,   973,   588,   945,  4467,
     986,   946,   163, -1477, -1477, -1477, -1477,    -9,   949,    45,
    9827, 11536,  9827, 11571, -1477,   313, 11681, -1477,  9121, 13883,
    9121, -1477, 11716, -1477,   336,  9121, -1477, -1477, -1477,  1124,
      26, -1477, -1477,   695,   950, -1477, -1477, -1477,  9121,   715,
   -1477, 13758,   951,   952, -1477, -1477, -1477,  9121,  1001,   977,
    9121, -1477, -1477, -1477, -1477,   960,   967,   970,  9121,  9121,
    9121,   971,  1105,   972,   980,  8313, -1477,    45, -1477,   338,
    9121,   185,   207, -1477,  9121,  9121,  9121,  9121,   548, -1477,
    9121,  9121,   982,  9121,  9121,   587, -1477, -1477, -1477,  1002,
     582,  3655, -1477,   664, -1477,   296, -1477,   314,  9827,   109,
   -1477, -1477,  8515, -1477, -1477,  4387, -1477,   604, -1477, -1477,
   -1477,  9827, 11826, 11861, -1477, -1477, 11971, -1477, -1477,  1124,
     426,   985,  1105,  1105, 12006,  1005,   983, 12116,   988,   996,
     998,  9121, -1477,  9121,  1008,  1008,  1008,  9121, -1477, -1477,
    1105,   499, -1477, 12151, -1477, -1477, 13465,  9121,  9121, -1477,
   12261, 13758, 13758, 13465, -1477,   995,  1008,  9121, -1477,   995,
   13465,  9121,   330, -1477, -1477,  8717,  8919, -1477, -1477, -1477,
   -1477, -1477, 13758,   615,   999,  9827,   109,  1051,  9121,  9121,
   13848, -1477, -1477,   605, -1477, -1477, -1477, 14493, -1477, -1477,
   -1477,   191,   191, -1477,  9121,  9121, -1477,  1105,  1105,   499,
    1000,  1003,   849,   191,   867,  1006, -1477,  1157,  1009, 13758,
   13758,   218,  1035,  1042,  1025,  1043,  1013, 13465, -1477,   330,
    9121,  9121, 13758, -1477, -1477,  1051,  9121,  9121, 13504, 13883,
   -1477, -1477, -1477,  1046, 14493,   499,   867,  1037, -1477,  1014,
    1016, 12296, 12406,   191,   191,  1017, -1477, -1477,  1019,  1023,
   -1477,  9121,  1024,  9121,  9121,  1026,  1061, -1477,  1028, -1477,
   -1477,  1031, -1477, 13758,  9121, 13594, 13629, -1477, -1477, -1477,
     615,   349,  1033, -1477, -1477, -1477, -1477, -1477,  1034,  1036,
   -1477, -1477, -1477, 13758, -1477, 13758, 13758, -1477, -1477, -1477,
   -1477, 13719, -1477, -1477, -1477, -1477,   499, -1477, -1477, -1477,
     359, -1477
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   121,     1,   301,     0,     0,     0,   609,   302,     0,
     601,   601,   601,    16,     0,     0,    15,     3,     0,    10,
       9,     8,     0,     7,   589,     6,    11,     5,     4,    13,
      12,    14,    92,    93,    91,   100,   102,    37,    53,    50,
      51,    39,    40,    41,     0,    42,    48,    38,   601,   601,
      22,    21,   589,   603,   602,   772,   762,   767,     0,   269,
      35,   108,   109,     0,     0,     0,   110,   112,   119,     0,
     107,    17,   627,   626,   214,   611,   628,   590,   591,     0,
       0,     0,     0,    43,     0,    49,     0,     0,    46,     0,
       0,   601,     0,    18,     0,     0,     0,   271,     0,     0,
     118,   113,     0,     0,     0,     0,     0,     0,   122,   216,
     215,   218,   213,   613,   612,     0,   630,   629,   633,   593,
     592,   595,    98,    99,    96,    97,    95,     0,     0,    94,
     103,    54,    52,    48,    45,    44,   604,   606,   608,     0,
     610,    19,    20,    23,   773,   763,   768,   270,    33,    36,
     117,     0,   114,   115,   116,   120,     0,   614,     0,   623,
     586,   524,    24,    25,     0,    87,    88,    85,    86,    84,
      83,    89,     0,    47,     0,   607,     0,     0,     0,    34,
     111,     0,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,     0,     0,   128,
     123,     0,   615,     0,   624,     0,   634,   587,     0,     0,
     526,     0,    26,    27,    28,     0,   101,     0,   774,   764,
     769,   182,   183,   180,   131,   132,   134,   133,   135,   136,
     137,   138,   164,   165,   162,   163,   155,   166,   167,   156,
     153,   154,   181,   175,     0,   179,   168,   169,   170,   171,
     142,   143,   144,   139,   140,   141,   152,     0,   158,   159,
     157,   150,   151,   146,   145,   147,   148,   149,   130,   129,
     174,     0,   160,   161,   524,   126,   246,   219,   597,   668,
     671,   674,   675,   669,   672,   670,   673,     0,   621,   631,
     594,   524,     0,   104,   106,     0,     0,   576,   574,   596,
      90,     0,     0,     0,     0,     0,     0,   641,   661,   642,
     677,   643,   647,   648,   649,   650,   667,   654,   655,   656,
     657,   658,   659,   660,   662,   663,   664,   665,   732,   646,
     653,   666,   739,   746,   644,   651,   645,   652,     0,     0,
       0,     0,   676,   694,   697,   695,   696,   759,   605,   682,
     554,   560,   184,   185,   178,   173,   186,   176,   172,     0,
     124,   300,   548,     0,     0,   217,     0,   616,   618,     0,
     625,   538,   635,     0,     0,   105,     0,     0,     0,     0,
     575,     0,   700,   723,   726,     0,   729,   719,     0,   685,
     733,   740,   747,   753,   756,     0,     0,   709,   714,   708,
       0,   722,   718,   711,     0,     0,   713,   698,     0,   775,
     765,   770,   187,   177,     0,   298,   299,     0,   524,   125,
     127,   248,     0,     0,     0,    63,    64,    76,   430,   431,
       0,     0,     0,     0,   286,   424,   284,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   285,     0,     0,     0,
       0,     0,     0,     0,   667,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,     0,     0,     0,   352,   354,   353,   355,   356,   357,
     358,     0,     0,    29,     0,   220,   225,     0,     0,     0,
       0,     0,     0,     0,   336,   337,   428,   427,     0,     0,
       0,     0,   241,   236,   233,   232,   234,   235,   268,   247,
     227,   507,   226,   425,     0,   498,    71,    72,    69,   239,
      70,   240,   242,   304,   231,   497,   496,   495,   121,   501,
     426,     0,   228,   500,   499,   471,   432,   472,   359,   433,
       0,   429,   777,   781,   778,   779,   780,     0,   598,     0,
     597,   622,   539,   588,   525,     0,     0,     0,   507,     0,
     578,   579,     0,   572,   573,   571,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   123,     0,   123,     0,
     123,     0,     0,     0,   705,   250,   716,   717,   710,   712,
       0,   715,   699,     0,     0,   761,   760,     0,   683,     0,
     269,   689,   688,     0,   555,   550,     0,     0,     0,   561,
       0,     0,     0,   636,   546,   565,   549,   822,   825,     0,
       0,   274,   278,   277,   283,     0,     0,   322,     0,     0,
       0,   858,     0,     0,   290,   287,     0,   331,     0,     0,
     254,     0,   272,     0,     0,     0,   313,   316,     0,   245,
     319,     0,     0,    57,     0,    78,   862,     0,   831,   840,
       0,   828,     0,     0,   295,   292,   460,   461,   337,   347,
     121,   267,   265,   266,     0,     0,     0,     0,     0,     0,
       0,   800,     0,     0,     0,   838,   865,     0,   258,     0,
     261,     0,     0,   867,   876,     0,   437,   436,   473,   435,
     434,     0,     0,   876,   331,   876,   338,   243,   244,     0,
      74,   350,   223,   505,     0,   230,   237,   238,   289,   294,
     303,     0,   345,     0,   229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   462,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   416,     0,   222,     0,
     599,     0,   617,   619,   632,     0,     0,   524,     0,   577,
       0,   581,     0,   585,   359,     0,     0,     0,   690,   703,
       0,     0,   678,   680,     0,     0,   126,     0,   126,     0,
     126,   552,     0,   558,     0,   679,     0,     0,   876,   707,
     692,     0,   684,   776,     0,   556,   766,     0,   562,   771,
     547,     0,     0,   564,     0,   563,     0,   566,     0,     0,
       0,    79,     0,     0,   814,     0,     0,     0,     0,   848,
     851,   854,     0,   876,   291,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   876,   273,     0,
      80,    81,     0,     0,     0,     0,    55,    56,     0,     0,
     876,     0,     0,   876,     0,     0,   296,   293,   338,   345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   269,
       0,     0,     0,   834,   792,   800,     0,   843,     0,     0,
       0,   800,     0,     0,     0,     0,     0,   803,   870,     0,
     249,     0,    32,     0,    30,     0,   877,     0,   246,     0,
       0,   877,     0,     0,     0,     0,   406,   403,   405,    60,
       0,   121,     0,   419,     0,   791,     0,     0,     0,   312,
     311,     0,   123,   264,     0,     0,   484,   483,     0,     0,
     485,   489,     0,     0,     0,   381,   390,   369,   391,   370,
     393,   372,   392,   371,   394,   373,   384,   363,   385,   364,
     386,   365,   438,   439,   451,   395,   374,   396,   375,   452,
     449,   450,     0,   383,   361,   478,     0,   469,     0,   502,
     503,   504,   362,   440,   441,   397,   376,   398,   377,   456,
     457,   458,   387,   366,   388,   367,   389,   368,   459,   382,
     360,     0,     0,   454,   455,   453,   447,   448,   443,   442,
     444,   445,   446,     0,     0,     0,   412,     0,     0,     0,
       0,   422,     0,     0,     0,     0,   532,   535,     0,     0,
     580,   583,   359,   584,   701,   724,   727,     0,   730,   720,
       0,   686,   734,     0,   741,     0,   748,     0,   754,     0,
     757,     0,     0,   256,   704,   251,   693,   551,   557,     0,
     638,   639,   567,   570,   569,     0,     0,     0,     0,   815,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,   369,   370,   372,   371,   373,   363,
     364,   365,   374,   375,   361,   376,   377,   366,   367,   368,
     360,   297,     0,     0,   871,   255,   474,     0,     0,     0,
     475,     0,   506,     0,     0,     0,     0,     0,     0,   345,
     123,   508,   509,   510,   511,   512,     0,     0,   801,     0,
       0,     0,     0,   331,   800,     0,     0,     0,     0,   809,
     810,     0,   817,     0,     0,   807,     0,   846,     0,     0,
       0,     0,   805,   847,     0,   837,   802,   866,     0,     0,
       0,     0,   868,   869,     0,     0,     0,   845,   464,     0,
       0,     0,   123,     0,    58,    59,     0,    73,    65,   345,
       0,     0,   421,   420,   305,     0,     0,     0,     0,     0,
       0,   343,     0,   126,   480,     0,   486,     0,   380,   378,
     379,     0,     0,   467,     0,     0,   490,   494,     0,     0,
     470,     0,     0,     0,     0,   413,     0,   417,   465,   423,
     600,   620,   122,   533,   534,   535,   536,   527,   540,   345,
     582,   702,   725,   728,   691,   731,   721,   681,   687,   735,
     737,   742,   744,   749,   751,   755,   553,   758,   559,     0,
       0,   637,     0,     0,   823,   826,     0,   275,     0,     0,
     280,   281,   279,     0,   325,     0,     0,   328,   323,     0,
       0,     0,   859,   857,   254,     0,    82,   314,   317,   320,
     863,   861,   832,   841,   839,   829,     0,   126,     0,     0,
       0,   783,   782,   800,     0,   835,     0,     0,   793,   816,
     808,   836,   844,   806,     0,   800,     0,   812,   813,   820,
     804,     0,   259,   262,    31,     0,   221,     0,     0,     0,
       0,     0,   126,    61,     0,    66,     0,   123,     0,     0,
       0,     0,   574,   341,   342,   340,   339,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,   491,     0,   479,
       0,   468,     0,   414,     0,     0,   466,   418,   545,   530,
     527,   528,   529,   532,     0,   738,   745,   752,   250,   257,
     640,   568,     0,     0,    77,   276,   282,     0,     0,     0,
       0,   324,   849,   852,   855,     0,     0,     0,     0,     0,
       0,     0,   814,     0,     0,     0,   224,     0,   514,     0,
       0,     0,     0,   818,     0,     0,     0,     0,     0,   811,
       0,     0,   252,     0,     0,     0,   404,   523,   407,     0,
      60,     0,    75,   126,   399,     0,   306,   574,     0,     0,
     344,   346,     0,   333,   349,     0,   522,     0,   520,   402,
     517,     0,     0,     0,   516,   415,     0,   531,   541,   530,
       0,     0,   814,   814,     0,     0,     0,     0,     0,     0,
       0,   250,   872,   254,   315,   318,   321,     0,   815,   833,
     814,     0,   476,     0,   348,   515,   874,     0,     0,   819,
       0,   785,   784,   874,   821,   874,   260,   250,   263,   874,
     874,     0,     0,   410,    62,   286,     0,    67,    71,    72,
      69,    70,    68,     0,     0,     0,     0,     0,     0,     0,
     334,   481,   487,     0,   521,   519,   518,     0,   543,   537,
     706,   799,   799,   326,     0,     0,   329,   814,   814,     0,
       0,     0,   876,   799,   790,     0,   477,     0,     0,   787,
     786,     0,     0,     0,   876,     0,     0,   874,   408,     0,
       0,     0,   292,   351,   400,     0,     0,     0,     0,   335,
     482,   488,   492,     0,     0,     0,   796,   876,   798,     0,
       0,     0,     0,   799,   799,     0,   860,   873,     0,     0,
     830,     0,     0,     0,     0,     0,     0,   877,     0,   882,
     878,     0,   411,   293,     0,     0,     0,   310,   493,   542,
       0,     0,   877,   797,   824,   827,   327,   330,     0,     0,
     856,   864,   842,   875,   880,   789,   788,   881,   883,   253,
     879,     0,   309,   308,   544,   794,     0,   850,   853,   307,
       0,   795
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1477, -1477, -1477, -1477, -1477, -1477,   549,  1172, -1477, -1477,
   -1477,  1251, -1477, -1477, -1477,   763,  1209, -1477,  1122, -1477,
   -1477,  1174, -1477, -1477, -1477,  -203, -1477, -1477, -1477,  -202,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,  1040,
   -1477, -1477,   -56,   -44, -1477, -1477, -1477,   432,   463,  -515,
    -561,  -783, -1477, -1477, -1477, -1476, -1477, -1477,  -209,  -361,
   -1477,   312, -1477, -1335, -1477, -1273,  -117,   280, -1477, -1477,
   -1477, -1477,  -423,   -14, -1477, -1477, -1477, -1477, -1477,  -199,
    -195,  -193, -1477,  -192, -1477, -1477, -1477,  1269, -1477,   303,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477,  -342,  -165,  -367,  -102, -1477,  -898,  -441,
   -1477,  -517, -1477, -1477,  -366,   797, -1477, -1477, -1477, -1464,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,   839,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477,  -154,  -123,  -211,
    -124,    16, -1477, -1477, -1477, -1477, -1477,   854, -1477,  -575,
   -1477, -1477,  -570, -1477, -1477,  -608,  -206,  -569, -1300, -1477,
    -370, -1477, -1477,  1235, -1477, -1477, -1477,   734,   858,    30,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
    -582,  -149, -1477,   863, -1477, -1477, -1477, -1477, -1477, -1477,
    -401, -1477, -1477,  -384, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477,  -201, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477,   864,  -685,  -280,
    -836,  -655, -1477, -1477, -1179,  -889, -1477, -1477, -1477, -1138,
    -155, -1166, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,
   -1477,   111,  -474, -1477, -1477, -1477,   603, -1477, -1477, -1477,
   -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477, -1477,   804,
   -1477, -1443,  -701, -1477
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    16,   143,    52,    17,   164,   170,   701,   506,
     149,   507,    99,    19,    20,    45,    46,    47,    88,    21,
      39,    40,   508,   509,  1216,  1217,   510,  1366,  1461,   511,
     512,   960,   513,   630,   514,   515,   516,   517,  1147,   171,
     172,    35,    36,    37,   220,    66,    67,    68,    69,    22,
     285,   375,   210,    23,   111,   211,   112,   156,   679,   985,
     519,   376,   520,   827,  1528,   886,  1092,   568,   939,  1451,
     941,  1452,   522,   523,   524,   632,   853,  1415,   525,   526,
     527,   528,   529,   530,   531,   532,   427,   533,   731,  1225,
     969,   534,   535,   892,  1428,   893,  1429,   895,  1430,   536,
     858,  1421,   537,   713,  1474,   538,  1231,  1232,   972,   681,
     539,   788,   961,   540,   646,   986,   542,   543,   544,   958,
     545,  1211,  1532,  1212,  1589,   546,  1059,  1397,   547,   714,
    1380,  1600,  1382,  1601,  1481,  1638,   549,   371,  1403,  1488,
    1265,  1267,  1068,   562,   797,  1557,  1604,   372,   373,   613,
     822,   420,   618,   824,   421,  1171,   624,   576,   391,   308,
     309,   217,   301,    78,   121,    25,   161,   377,    89,    90,
     174,    91,    26,    49,   115,   158,    27,   288,   559,   560,
    1064,   380,   215,   216,    76,   118,   382,    28,   159,   299,
     625,   550,   297,   354,   355,   814,   419,   356,   584,  1278,
    1290,   807,   417,   357,   577,  1271,   826,   582,  1276,   578,
    1272,   579,  1273,   581,  1275,   585,  1279,   586,  1405,   587,
    1281,   588,  1406,   589,  1283,   590,  1407,   591,  1285,   592,
    1287,   615,    29,    95,   177,   360,   616,    30,    96,   178,
     361,   620,    31,    94,   176,   359,   609,   551,  1606,  1575,
     966,   925,  1607,  1608,  1609,   926,   938,  1193,  1187,  1182,
    1349,  1112,   552,   849,  1412,   850,  1413,   904,  1434,   901,
    1432,   927,   703,   553,   902,  1433,   928,   554,  1118,  1498,
    1119,  1499,  1120,  1500,   862,  1425,   899,  1431,   697,   887,
     555,  1578,   947,   556
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    70,   287,   802,   722,   572,   924,   221,   924,   696,
     541,  1160,   952,   563,   953,   518,   821,   848,  1111,   614,
     619,   644,   823,   732,   130,   816,   358,   818,  -121,   820,
      84,   682,   683,  1083,   606,  1085,  1178,  1087,   931,   965,
    1339,  1426,  1190,   638,   917,   674,   596,   796,   852,    70,
      70,    70,    53,    32,    33,  1107,    61,  1167,    54,   151,
    1188,   598,   122,   123,   917,    85,  1194,   218,  1588,  1201,
    1205,  1401,  1469,  1491,  1358,   104,   105,   106,   353,    92,
    1582,  1603,  1583,   518,    58,    62,  1585,  1586,   758,   759,
      70,    70,    70,    70,  1053,  1054,   286,   691,   693,    50,
     594,    38,   645,     2,   680,   680,   680,   518,   916,     3,
     929,    59,   933,   425,   107,   723,  1227,   162,   163,   595,
     945,   139,   843,  1055,   219,  1632,  1228,  1095,  1640,   949,
     664,  1402,     4,   843,     5,  1056,     6,   165,   166,   108,
     639,   640,     7,  1170,  1631,    51,   150,   383,   733,    98,
     735,   736,     8,   803,   721,   635,   426,    63,     9,   724,
      98,   715,  1122,   303,    58,   909,  1570,  1546,   918,   965,
     786,   787,  1177,   430,   302,   138,  1057,  1058,  1229,    48,
    1298,   721,    10,  1230,   845,  1472,  1145,   418,    77,   286,
     755,    59,  1584,   758,   759,   845,  1443,   866,   734,  1154,
     794,    58,  1157,    34,    11,    12,   597,    86,   834,   124,
     735,   736,   835,   352,   125,   518,   126,   838,    87,   127,
     864,   599,   844,   286,   844,   353,    64,   844,    59,    71,
    1571,   795,   641,   844,   573,    65,   844,   844,   648,   600,
     353,   844,    80,   369,   574,   601,   747,   748,   218,   385,
     906,   642,   665,   828,   755,   304,   757,   758,   759,   760,
     128,  1326,   286,   306,   761,   885,  1509,   369,   843,   353,
      79,   353,   218,  1299,   843,   786,   787,  1220,  1170,  1080,
     721,   307,   518,    13,   167,  1334,    80,   843,   905,   168,
     305,   169,   973,  1081,   127,   407,   575,  1445,   504,   678,
     863,   865,    14,   370,  1519,   219,   747,   748,   721,   428,
     843,   384,   518,    15,   755,   504,   678,   758,   759,   760,
    1517,  1367,   408,   409,   761,   900,  1561,  1562,   903,   219,
     845,  1184,   353,   353,  1185,   844,   845,   406,   846,   786,
     787,   847,   429,   843,  1573,   107,   306,   918,   649,   845,
      81,   557,  1468,  1623,   967,   390,    41,    42,    43,  1298,
     352,   970,  1186,   558,   307,   561,   680,   650,  1206,   407,
    1262,  1404,   845,  1176,    98,   352,   805,   806,   808,  1071,
     810,   811,   964,  1610,   815,   410,   817,    44,   819,   411,
    1179,  1180,    80,  1399,  1619,   225,   408,   409,  1368,   786,
     787,  1613,  1614,  1605,   352,   845,   352,  1077,   968,   790,
      82,  1233,  1221,  1266,  1069,    97,   791,  1305,  1181,   180,
     851,  1089,   226,  1091,   965,  1103,   352,   636,   353,   353,
     353,  1306,   353,   353,  1648,  1649,   353,   721,   353,  1222,
     353,   518,   353,   611,  1442,   890,  1219,  1062,   103,   612,
    1379,   412,  1370,   131,   866,   413,  1448,  1142,   414,   410,
    1377,    58,  1197,   411,   866,  1105,  1106,   352,   352,    87,
     954,    58,  1202,   415,  1370,   955,  1168,  1371,  1121,   416,
     954,  1259,  1176,   666,  1143,  1378,   669,   518,    59,   924,
     940,  1300,  1338,   133,  1293,   100,   101,   102,    59,   866,
     353,  1372,   667,  1545,   924,   670,   390,  1544,  1148,  1149,
     959,  1151,  1110,   956,  1286,  1153,   136,  1155,  1156,  1333,
    1158,  1288,   866,   353,   866,   412,  1479,    72,    73,   413,
      74,  1159,   414,   407,  1345,  1176,   152,   153,   154,   155,
     721,  1238,  1239,  1240,  1437,  1176,   212,   415,   418,  1485,
     680,  1515,  1074,   416,   104,   856,   106,   213,    75,   137,
     408,   409,  1665,   352,   352,   352,   289,   352,   352,   607,
     290,   352,  1671,   352,   857,   352,   622,   352,  -736,  1459,
     715,   608,   140,  -736,   291,   292,   977,   981,   715,   293,
     294,   295,   296,  1213,   623,  -743,   611,  1016,   144,  1327,
    -743,  -736,   612,   145,  1214,  1215,  -750,  1249,  -409,  1017,
    1088,  -750,  1021,  -409,   146,   680,   680,   680,  -743,  1250,
     721,   855,   721,   410,   721,  1093,   721,   411,   721,  -750,
     721,  -409,   721,  1097,   721,   352,  1098,   147,  1254,   721,
      53,   721,   617,  1067,   418,  1102,    54,   721,  1075,   353,
    1255,  1362,  1108,   148,   611,   918,   521,  1117,   352,   721,
     612,   721,  1336,   109,   160,   611,   721,  1298,   721,   110,
     721,   612,   113,   721,  1337,  1141,   611,   353,   114,  1090,
    1543,   116,   612,   157,  1465,   353,    85,   117,   353,   412,
    1289,   175,   611,   413,   179,  1269,   414,   353,   612,   418,
     353,   353,   957,  1078,   353,   721,  1352,   104,   611,   353,
     611,   415,   611,   353,   612,   119,   612,   416,   612,   418,
     418,   120,  1353,  1079,   521,   388,  1417,  1346,   389,  1446,
    1347,   390,   214,  1348,  1309,  1310,  1311,   721,  1093,   141,
    1093,   611,   227,   353,   353,   142,   353,   612,   521,   228,
     353,  1263,   353,   353,  1440,   353,  1447,  1264,  1450,  1315,
     866,   407,   866,  1453,   866,  1296,  1454,  1531,   229,   866,
     418,   418,   866,   866,  1082,  1084,   230,   432,   433,   418,
     282,    70,   418,  1086,   352,   283,  1297,   418,   408,   409,
     353,  1308,   353,   286,   418,   418,   298,   443,  1552,  1602,
     284,  1191,  1184,   448,  1192,  1435,  1463,   504,   678,  1436,
     222,   223,   352,   162,   163,   942,   943,   104,   105,   106,
     352,   300,  1198,   352,  1199,  1329,  1574,  1361,    41,    42,
      43,   362,   352,  1518,   363,   352,   352,   364,   541,   352,
     462,   463,   365,   518,   352,   366,  1344,   367,   352,   134,
     135,   410,  1351,   374,   573,   411,   521,   368,   378,  1355,
     222,   223,   224,  1357,   574,   935,   936,   937,    55,    56,
      57,  1618,   381,   379,   465,   466,  1274,   386,   352,   352,
     387,   352,   392,  1628,  1574,   352,   393,   352,   352,   394,
     352,   396,  1247,   395,   397,   399,   400,   398,   401,  1385,
    1548,   402,  1418,   403,   404,   418,  1643,   422,   564,   801,
     423,  1394,   405,   570,    58,   571,   575,   412,   424,   604,
    1641,   413,   583,   521,   414,   352,   627,   352,   353,   628,
     637,   481,   482,   483,   653,   655,   656,   657,   658,   415,
     353,    59,   659,   660,   661,   416,   668,   684,   671,   672,
     685,   494,   686,   521,   687,   705,   716,   700,   717,   721,
     688,   689,   718,   720,   725,   726,   631,   727,   728,   353,
     353,   353,   729,  1624,   730,    15,  1525,  1596,  1597,  1529,
     789,  1670,   792,   502,   664,   800,  1439,   798,   698,   735,
     736,   813,   812,   617,   353,   832,  1475,   829,   833,   836,
     837,   839,   841,   842,  1363,   854,   859,   860,   733,   861,
     884,   897,   735,   736,   866,   896,  1455,   521,   521,   521,
     521,   521,   521,   521,   521,   908,  1634,   891,   735,   736,
     521,   521,   946,   948,   950,   951,   521,   971,  1012,  1066,
    1070,  1076,  1094,  1100,  1101,   521,   521,  1144,  1115,  1169,
     521,   521,   521,  1176,   521,  1183,  1196,  1208,  1223,  1207,
    1203,  1224,   353,   352,  1209,  1210,   828,  1234,  1235,  1237,
    1257,  1241,  1242,  1252,  1253,   352,  1236,  1261,  1266,  1268,
    1277,  1280,   521,   407,  1547,   747,   748,  1414,  1282,  1409,
    1284,  1292,  1303,   755,  1314,   757,   758,   759,   760,  1301,
    1373,  1302,  1313,   761,   352,   352,   352,  1328,   747,   748,
     408,   409,  1374,  1321,  1324,  1340,   755,  1343,   757,   758,
     759,   760,   745,   746,   747,   748,   761,  1350,   521,   352,
    1375,  1376,   755,  1387,   757,   758,   759,   760,  1388,  1390,
    1395,   353,   761,  1408,   763,   764,  1398,  1410,  1416,  1419,
    1420,  1444,  1449,  1462,  1466,  1460,  1487,  1467,  1464,   828,
    1471,  1595,  1435,  1492,  1493,  1572,   783,   784,   785,  1495,
    1473,  1496,  1501,   410,  1508,  1577,   573,   411,   786,   787,
    1502,  1477,  1503,  1507,  1510,   828,   574,   781,   782,   783,
     784,   785,  1511,   645,  1527,  1565,  1533,   352,  1560,  1564,
    1567,   786,   787,   781,   782,   783,   784,   785,  1568,  1621,
    1569,  1627,  1594,  1616,  1625,   548,  1617,   786,   787,  1620,
    1622,  1626,  1629,  1642,  1630,   569,  1639,  1644,  1473,  1645,
    1650,   353,  1651,   353,   580,  1654,  1652,  1657,   575,   412,
    1658,  1659,  1660,   413,   593,  1666,   414,  1667,   865,  1668,
     944,   129,    18,    83,   603,   173,   132,  1534,  1065,  1537,
    1204,   415,  1538,   621,   407,   310,  1539,   416,  1540,  1541,
      24,  1226,  1514,   629,   -68,  1470,   352,  1489,  1558,  1490,
    1553,  1400,   626,   647,  1559,   735,   736,    93,   652,  1615,
     654,   408,   409,  1524,   793,  1342,   932,     0,   704,   663,
       0,     0,     0,   633,   634,     0,     0,   675,   676,   677,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,     0,   695,     0,     0,     0,     0,     0,     0,     0,
     699,     0,   353,   695,  1593,   407,   706,   707,   708,   709,
     710,     0,     0,     0,     0,     0,     0,     0,     0,   719,
       0,     0,     0,     0,   410,     0,     0,     0,   411,     0,
       0,     0,   408,   409,     0,     0,   352,     0,   352,     0,
       0,   737,   738,   739,   740,   741,   742,   743,   744,   745,
     746,   747,   748,   749,   750,   751,   752,   753,   754,   755,
     756,   757,   758,   759,   760,     0,   353,     0,     0,   761,
     762,   763,   764,   765,   766,     0,     0,   767,   768,   769,
     770,   771,   772,   773,     0,   804,     0,     0,     0,     0,
     412,     0,     0,     0,   413,   410,  1294,   414,     0,   411,
       0,  1664,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   415,   831,     0,     0,     0,     0,   416,     0,
       0,     0,   774,   775,   352,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,     0,     0,   352,     0,     0,
       0,     0,     0,     0,   786,   787,     0,     0,     0,   504,
     678,     0,   647,     0,   521,     0,     0,     0,     0,   695,
     888,   412,   889,     0,     0,   413,     0,   894,   414,     0,
       0,     0,     0,   898,     0,     0,     0,     0,     0,     0,
       0,     0,   907,   415,     0,     0,     0,     0,   957,   416,
       0,     0,     0,   910,   911,   912,   913,   914,   915,     0,
     923,   352,   923,     0,     0,   988,   990,   992,   994,   996,
     998,  1000,     0,     0,     0,     0,  1365,  1005,  1007,     0,
       0,     0,     0,  1013,     0,   735,   736,     0,     0,     0,
       0,     0,  1025,  1027,     0,     0,     0,  1032,  1034,  1036,
       0,  1039,     0,     0,     0,   957,   987,   989,   991,   993,
     995,   997,   999,  1001,  1002,  1003,     0,     0,  1004,  1006,
    1008,  1009,  1010,  1011,     0,  1014,  1015,     0,  1018,  1019,
    1020,  1022,  1023,  1024,  1026,  1028,  1029,  1030,  1031,  1033,
    1035,  1037,  1038,  1040,  1042,  1043,  1044,  1045,  1046,  1047,
    1048,  1049,  1050,  1051,  1052,     0,  1060,     0,     0,     0,
    1063,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1072,     0,     0,     0,     0,     0,     0,     0,   745,
     746,   747,   748,   749,     0,     0,   752,   753,   754,   755,
       0,   757,   758,   759,   760,     0,     0,     0,     0,   761,
       0,   763,   764,     0,     0,     0,     0,   767,   768,   769,
    1099,     0,     0,   773,     0,     0,     0,  1104,     0,     0,
       0,     0,     0,     0,  1114,     0,  1116,     0,     0,     0,
       0,     0,     0,     0,     0,  1123,  1124,  1125,  1126,  1127,
    1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,
    1138,  1139,  1140,   775,     0,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,     0,     0,     0,     0,     0,
     407,     0,     0,     0,   786,   787,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   706,
    1173,     0,     0,     0,     0,     0,     0,   408,   409,     0,
       0,     0,     0,  1195,     0,   695,     0,   408,   409,     0,
       0,     0,  1200,     0,     0,   695,     0,     0,     0,   431,
    1123,     0,   432,   433,     3,     0,   434,   435,   436,  1218,
     437,     0,   438,   439,   440,   441,   442,     0,     0,     0,
       0,     0,   443,   444,   445,   446,   447,     0,   448,     0,
       0,     0,     0,     0,     0,   449,   450,     0,     0,   451,
     410,   452,   453,     0,   411,   454,     0,     8,   455,   456,
     410,   457,   458,     0,   411,   459,   460,     0,     0,     0,
       0,     0,   461,     0,     0,   462,   463,     0,   317,   318,
     319,     0,   321,   322,   323,   324,   325,   464,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,     0,
     339,   340,   341,     0,     0,   344,   345,   346,   347,   465,
     466,   467,   468,  1251,     0,     0,   412,  1256,     0,     0,
     413,     0,  1295,   414,   469,   470,   412,     0,     0,     0,
     413,     0,  1312,   414,     0,     0,     0,     0,   415,   471,
     472,   473,     0,     0,   416,     0,     0,     0,   415,    58,
       0,     0,     0,     0,   416,     0,     0,   474,   475,   476,
     477,   478,     0,   479,     0,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,    59,   491,   492,   493,
       0,     0,     0,     0,     0,     0,   494,   495,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   497,   498,   499,  1316,    14,     0,     0,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
     503,     0,   504,   505,     0,     0,     0,     0,  1330,  1331,
    1332,     0,     0,     0,  1335,     0,     0,     0,     0,     0,
       0,     0,  1341,   923,     0,     0,     0,     0,     0,     0,
       0,     0,   735,   736,   690,     0,     0,     0,     0,     0,
     311,     0,     0,   548,     0,     0,   312,     0,  1359,  1360,
       0,     0,   313,     0,     0,  1364,     0,     0,     0,     0,
    1123,     0,   314,     0,     0,     0,     0,     0,     0,     0,
     315,   735,   736,     0,  1381,     0,  1383,     0,     0,     0,
       0,  1386,     0,     0,     0,   316,     0,  1389,     0,     0,
       0,  1392,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,     0,   745,   746,   747,   748,
     749,     0,  1411,   752,   753,   754,   755,     0,   757,   758,
     759,   760,     0,     0,     0,     0,   761,     0,   763,   764,
       0,     0,     0,   695,   767,   768,   769,     0,     0,     0,
     773,     0,     0,    58,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,   350,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
      59,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     775,     0,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   786,   787,   735,   736,   799,     0,  1482,     0,  1483,
       0,     0,     0,     0,  1486,     0,   351,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,  1494,     0,     0,  1497,
     786,   787,     0,     0,     0,   504,   678,  1504,  1505,  1506,
       0,     0,     0,     0,  1513,     0,     0,     0,     0,  1516,
       0,     0,     0,  1520,  1521,  1522,  1523,     0,     0,   695,
    1526,     0,   695,  1530,     0,     0,     0,     0,     0,     0,
    1542,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1550,     0,     0,     0,     0,     0,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,   695,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,  1579,  1580,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1587,     0,     0,     0,     0,  1592,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1598,  1599,     0,
       0,   775,   407,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,  1611,  1612,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,   809,     0,     0,   408,
     409,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1633,     0,     0,     0,     0,  1635,  1636,     0,     0,     0,
       0,   431,     0,     0,   432,   433,     3,     0,   434,   435,
     436,     0,   437,     0,   438,   439,   440,   441,   442,     0,
    1653,     0,  1655,  1656,   443,   444,   445,   446,   447,     0,
     448,     0,     0,  1661,     0,     0,     0,   449,   450,     0,
       0,   451,   410,   452,   453,     0,   411,   454,     0,     8,
     455,   456,     0,   457,   458,     0,     0,   459,   460,     0,
       0,     0,     0,     0,   461,     0,     0,   462,   463,     0,
     317,   318,   319,     0,   321,   322,   323,   324,   325,   464,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,     0,   339,   340,   341,     0,     0,   344,   345,   346,
     347,   465,   466,   467,   468,     0,     0,     0,   412,     0,
       0,     0,   413,     0,  1317,   414,   469,   470,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     415,   471,   472,   473,     0,     0,   416,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,   474,
     475,   476,   477,   478,   407,   479,     0,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,    59,   491,
     492,   493,     0,     0,     0,     0,     0,     0,   494,  1356,
     496,   408,   409,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   497,   498,   499,     0,    14,
       0,     0,   500,   501,     0,     0,   432,   433,     0,     0,
     502,     0,   503,     0,   504,   505,   438,   439,   440,   441,
     442,     0,     0,     0,     0,     0,   443,     0,   445,     0,
       0,     0,   448,     0,   407,     0,     0,     0,     0,     0,
     450,     0,     0,     0,   410,     0,   453,     0,   411,   454,
       0,     0,   455,     0,   917,     0,   458,     0,     0,     0,
       0,   408,   409,     0,     0,     0,   565,     0,     0,   462,
     463,     0,   317,   318,   319,     0,   321,   322,   323,   324,
     325,   464,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,     0,   339,   340,   341,     0,     0,   344,
     345,   346,   347,   465,   466,   566,     0,     0,     0,     0,
     412,     0,     0,     0,   413,     0,  1318,   414,   469,   470,
       0,     0,     0,     0,   410,     0,     0,     0,   411,     0,
       0,     0,   415,     0,     0,     0,     0,     0,   416,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,   474,   475,   476,   477,   478,   407,   479,   918,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     919,   567,   492,   493,     0,     0,     0,     0,     0,     0,
     494,     0,     0,   408,   409,     0,     0,     0,     0,     0,
     412,     0,     0,     0,   413,     0,     0,   920,   498,   499,
       0,    14,     0,     0,   500,   501,     0,     0,     0,   432,
     433,     0,   921,     0,   922,     0,   504,   505,   416,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,   407,     0,     0,
       0,     0,     0,   450,     0,     0,   410,     0,     0,   453,
     411,     0,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,   408,   409,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,   412,     0,     0,     0,   413,     0,  1319,   414,
       0,   469,   470,     0,     0,     0,     0,   410,     0,     0,
       0,   411,     0,     0,   415,     0,     0,     0,     0,     0,
     416,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,   407,
     479,   918,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   919,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,   408,   409,     0,     0,
       0,     0,     0,   412,     0,     0,     0,   413,     0,     0,
     920,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,     0,   432,   433,     0,   921,     0,   930,     0,   504,
     505,   416,   438,   439,   440,   441,   442,     0,     0,     0,
       0,     0,   443,     0,   445,     0,     0,     0,   448,     0,
     598,     0,     0,     0,     0,     0,   450,     0,     0,   410,
       0,     0,   453,   411,     0,   454,     0,     0,   455,     0,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   565,     0,     0,   462,   463,     0,   317,   318,
     319,     0,   321,   322,   323,   324,   325,   464,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,     0,
     339,   340,   341,     0,     0,   344,   345,   346,   347,   465,
     466,   566,     0,     0,     0,   412,     0,     0,     0,   413,
       0,  1320,   414,     0,   469,   470,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   415,     0,     0,
       0,     0,     0,   416,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,     0,   474,   475,   476,
     477,   478,   407,   479,     0,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,    59,   567,   492,   493,
       0,     0,     0,     0,     0,     0,   494,     0,     0,   408,
     409,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,     0,     0,   497,   498,   499,     0,    14,     0,     0,
     500,   501,     0,     0,     0,   432,   433,     0,  1172,     0,
     503,     0,   504,   505,   601,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,   410,     0,     0,   453,   411,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   467,     0,     0,     0,   412,     0,
       0,     0,   413,     0,  1322,   414,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,   643,     0,     0,     0,
     415,     0,   471,   472,   473,     0,   416,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,   407,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,   408,   409,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,     0,   503,     0,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,   410,     0,   453,     0,   411,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   467,     0,     0,     0,
       0,   412,     0,     0,     0,   413,     0,  1323,   414,   469,
     470,     0,     0,     0,     0,     0,     0,     0,   673,     0,
       0,     0,     0,   415,   471,   472,   473,     0,     0,   416,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,   407,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,   408,   409,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,   432,
     433,     0,     0,   502,     0,   503,     0,   504,   505,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
    1535,   445,   446,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,   410,     0,   453,
       0,   411,   454,     0,     0,   455,   456,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,  1536,
       0,     0,     0,   412,     0,     0,     0,   413,     0,  1325,
     414,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,     0,     0,     0,     0,
       0,   416,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,   407,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,   408,   409,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,     0,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,   410,
       0,   453,     0,   411,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     467,     0,     0,     0,     0,   412,     0,     0,     0,   413,
       0,  1422,   414,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   415,   471,   472,
     473,     0,     0,   416,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,   407,   479,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,    59,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,   408,   409,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
       0,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,   407,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,   410,     0,   453,     0,   411,   454,     0,     0,   455,
       0,     0,     0,   458,     0,   408,   409,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   467,     0,     0,     0,     0,   412,     0,     0,
       0,   413,     0,  1423,   414,   469,   470,     0,   410,     0,
       0,     0,   411,     0,     0,     0,     0,     0,     0,   415,
     982,   983,   984,     0,     0,   416,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,   407,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,   408,   409,   412,     0,     0,     0,   413,     0,
    1424,   414,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,     0,   415,   432,   433,   502,
       0,   503,   416,   504,   505,   711,     0,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,   407,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,   410,     0,   453,     0,   411,
     454,   712,     0,   455,     0,     0,     0,   458,     0,   408,
     409,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,   412,     0,     0,     0,   413,     0,  1427,   414,   469,
     470,     0,   410,     0,     0,     0,   411,     0,     0,     0,
       0,     0,     0,   415,     0,     0,     0,     0,     0,   416,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,   407,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,   408,   409,   412,     0,
       0,     0,   413,     0,  1458,   414,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,     0,
     415,   432,   433,   502,   602,   503,   416,   504,   505,   711,
       0,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,   410,
       0,   453,     0,   411,   454,   712,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,   412,     0,     0,     0,   413,
       0,  1551,   414,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   415,     0,     0,
       0,     0,     0,   416,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,   918,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   919,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,     0,     0,   432,   433,   502,     0,   503,
       0,   504,   505,   711,     0,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,   712,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,     0,     0,   432,
     433,   502,   829,   503,     0,   504,   505,   711,     0,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,   712,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,     0,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,   918,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   919,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   962,     0,   503,
     963,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,  1109,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,   918,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   919,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,     0,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,   602,   503,     0,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,   453,     0,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,   651,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,   432,
     433,     0,     0,   502,     0,   503,     0,   504,   505,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,   662,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,     0,     0,   432,   433,   502,     0,   503,     0,   504,
     505,   694,     0,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,     0,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   698,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,     0,     0,   432,
     433,   502,     0,   503,     0,   504,   505,   702,     0,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,     0,   504,
     505,   438,   439,   440,   441,   442,     0,     0,  1041,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,    59,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
       0,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,  1061,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1113,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,     0,   503,     0,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,   453,     0,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,   432,
     433,     0,     0,   502,     0,   503,  1174,   504,   505,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,  1189,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,    59,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
    1384,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,  1393,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,     0,   503,  1438,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,   453,     0,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,   498,
     499,     0,    14,     0,     0,   500,   501,     0,     0,   432,
     433,     0,     0,   502,     0,   503,  1512,   504,   505,   438,
     439,   440,   441,   442,     0,     0,     0,     0,     0,   443,
       0,   445,     0,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,   450,     0,     0,     0,     0,     0,   453,
       0,     0,   454,     0,     0,   455,     0,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,     0,   462,   463,     0,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   465,   466,   566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,   470,     0,     0,     0,     0,     0,     0,     0,
    1549,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,     0,
     479,     0,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,    59,   567,   492,   493,     0,     0,     0,
       0,     0,     0,   494,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,   498,   499,     0,    14,     0,     0,   500,   501,     0,
       0,   432,   433,     0,     0,   502,     0,   503,     0,   504,
     505,   438,   439,   440,   441,   442,     0,     0,     0,     0,
       0,   443,     0,   445,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,   450,     0,     0,     0,     0,
       0,   453,     0,     0,   454,     0,     0,   455,     0,     0,
       0,   458,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   565,     0,     0,   462,   463,     0,   317,   318,   319,
       0,   321,   322,   323,   324,   325,   464,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,   339,
     340,   341,     0,     0,   344,   345,   346,   347,   465,   466,
     566,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   469,   470,     0,     0,     0,     0,     0,
       0,     0,  1590,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   474,   475,   476,   477,
     478,     0,   479,     0,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,    59,   567,   492,   493,     0,
       0,     0,     0,     0,     0,   494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   498,   499,     0,    14,     0,     0,   500,
     501,     0,     0,   432,   433,     0,     0,   502,     0,   503,
       0,   504,   505,   438,   439,   440,   441,   442,     0,     0,
       0,     0,     0,   443,     0,   445,     0,     0,     0,   448,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   453,     0,     0,   454,     0,     0,   455,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   565,     0,     0,   462,   463,     0,   317,
     318,   319,     0,   321,   322,   323,   324,   325,   464,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,   339,   340,   341,     0,     0,   344,   345,   346,   347,
     465,   466,   566,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   469,   470,     0,     0,     0,
       0,     0,     0,     0,  1591,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,   474,   475,
     476,   477,   478,     0,   479,     0,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,    59,   567,   492,
     493,     0,     0,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,   498,   499,     0,    14,     0,
       0,   500,   501,     0,     0,   432,   433,     0,     0,   502,
       0,   503,     0,   504,   505,   438,   439,   440,   441,   442,
       0,     0,     0,     0,     0,   443,     0,   445,     0,     0,
       0,   448,     0,     0,     0,     0,     0,     0,     0,   450,
       0,     0,     0,     0,     0,   453,     0,     0,   454,     0,
       0,   455,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   565,     0,     0,   462,   463,
       0,   317,   318,   319,     0,   321,   322,   323,   324,   325,
     464,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   339,   340,   341,     0,     0,   344,   345,
     346,   347,   465,   466,   566,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
     474,   475,   476,   477,   478,     0,   479,     0,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,    59,
     567,   492,   493,     0,     0,     0,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,   498,   499,     0,
      14,     0,     0,   500,   501,     0,     0,   432,   433,     0,
       0,   502,     0,   503,     0,   504,   505,   438,   439,   440,
     441,   442,     0,     0,     0,     0,     0,   443,     0,   445,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   450,     0,     0,     0,     0,     0,   453,     0,     0,
     454,     0,     0,   455,     0,     0,     0,   458,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     462,   463,     0,   317,   318,   319,     0,   321,   322,   323,
     324,   325,   464,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,     0,   339,   340,   341,     0,     0,
     344,   345,   346,   347,   465,   466,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,     0,   479,     0,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,    59,   567,   492,   493,     0,   692,     0,     0,     0,
       0,   494,   311,     0,     0,     0,     0,     0,   312,     0,
       0,     0,     0,     0,   313,     0,     0,     0,   497,   498,
     499,     0,    14,     0,   314,   500,   501,     0,     0,     0,
       0,     0,   315,  1369,     0,   503,     0,   504,   505,     0,
       0,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,     0,     0,     0,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   314,     0,    58,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,   316,     0,     0,     0,
       0,     0,    59,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,     0,
     311,     0,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   314,     0,    58,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,   316,     0,     0,     0,     0,
       0,    59,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,     0,   605,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
     610,   312,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,   611,     0,     0,     0,     0,   314,   612,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,     0,     0,     0,     0,     0,   351,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,     0,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,   313,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   314,     0,    58,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,     0,   316,
       0,   735,   736,     0,     0,    59,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   735,
     736,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,     0,   610,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,     0,     0,   867,   868,   869,   870,   871,
     872,   873,   874,   745,   746,   747,   748,   749,   875,   876,
     752,   753,   754,   755,   877,   757,   758,   759,   760,  -359,
     351,   735,   736,   761,   762,   763,   764,   878,   879,     0,
       0,   767,   768,   769,   880,   881,   882,   773,     0,     0,
       0,     0,     0,     0,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   883,   775,     0,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   785,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,   787,
       0,     0,     0,   504,   678,     0,     0,   867,   868,   869,
     870,   871,   872,   873,   874,   745,   746,   747,   748,   749,
     875,   876,   752,   753,   754,   755,   877,   757,   758,   759,
     760,   735,   736,     0,     0,   761,   762,   763,   764,   878,
     879,     0,     0,   767,   768,   769,   880,   881,   882,   773,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   735,   736,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1073,     0,     0,     0,     0,     0,     0,   883,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,     0,   504,   678,   867,   868,   869,
     870,   871,   872,   873,   874,   745,   746,   747,   748,   749,
     875,   876,   752,   753,   754,   755,   877,   757,   758,   759,
     760,     0,     0,     0,     0,   761,   762,   763,   764,   878,
     879,     0,     0,   767,   768,   769,   880,   881,   882,   773,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,   735,   736,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,  1270,     0,     0,   773,     0,     0,     0,   883,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   735,   736,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,     0,   504,   678,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,   825,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,   735,   736,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   735,   736,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,   840,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1146,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,   735,   736,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   735,   736,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1150,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1152,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,   735,   736,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   735,   736,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,   735,   736,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   735,   736,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1163,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1164,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,   735,   736,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   735,   736,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,   735,   736,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   735,   736,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1304,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,   735,   736,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   735,   736,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,   735,   736,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   735,   736,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1457,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,   735,   736,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   735,   736,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1476,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1478,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,   735,   736,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   735,   736,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1480,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,   735,   736,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   735,   736,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,   735,   736,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   735,   736,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1556,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1563,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,   735,   736,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,   735,   736,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1566,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1576,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   745,   746,   747,   748,   749,
       0,     0,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,   735,   736,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   735,   736,     0,     0,     0,     0,     0,     0,     0,
     786,   787,     0,     0,  1581,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1646,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   767,   768,
     769,     0,     0,     0,   773,   745,   746,   747,   748,   749,
     735,   736,   752,   753,   754,   755,     0,   757,   758,   759,
     760,     0,     0,     0,     0,   761,     0,   763,   764,     0,
       0,     0,     0,   767,   768,   769,     0,     0,     0,   773,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,  1647,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   775,
       0,   776,   777,   778,   779,   780,   781,   782,   783,   784,
     785,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,   787,   830,     0,   745,   746,   747,   748,   749,   735,
     736,   752,   753,   754,   755,     0,   757,   758,   759,   760,
       0,     0,     0,     0,   761,     0,   763,   764,     0,     0,
       0,     0,   767,   768,   769,     0,     0,     0,   773,     0,
       0,     0,     0,     0,   735,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   775,     0,
     776,   777,   778,   779,   780,   781,   782,   783,   784,   785,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
     787,  1096,     0,   745,   746,   747,   748,   749,     0,     0,
     752,   753,   754,   755,     0,   757,   758,   759,   760,     0,
       0,     0,     0,   761,     0,   763,   764,     0,     0,     0,
       0,   767,   768,   769,     0,     0,     0,   773,   745,   746,
     747,   748,   749,   735,   736,   752,   753,   754,   755,     0,
     757,   758,   759,   760,     0,     0,     0,     0,   761,     0,
     763,   764,     0,     0,     0,     0,   767,   768,   769,     0,
       0,     0,   773,     0,     0,     0,     0,   775,     0,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   785,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,   787,
    1243,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   775,     0,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,   974,     0,     0,     0,     0,     0,
       0,     0,     0,   786,   787,  1258,     0,   745,   746,   747,
     748,   749,   735,   736,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,     0,     0,
       0,   773,     0,     0,     0,     0,   317,   318,   319,     0,
     321,   322,   323,   324,   325,   464,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,     0,   339,   340,
     341,     0,     0,   344,   345,   346,   347,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,  1391,     0,   745,   746,   747,   748,
     749,     0,     0,   752,   753,   754,   755,     0,   757,   758,
     759,   760,   231,   232,     0,     0,   761,     0,   763,   764,
       0,     0,   975,     0,   767,   768,   769,     0,     0,   233,
     773,     0,     0,     0,   976,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   735,   736,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     775,     0,   776,   777,   778,   779,   780,   781,   782,   783,
     784,   785,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   786,   787,  1396,     0,     0,     0,     0,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,     0,   252,   253,
     254,     0,     0,     0,     0,     0,     0,   255,   256,   257,
     258,   259,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   745,   746,   747,   748,   749,   735,   736,   752,   753,
     754,   755,     0,   757,   758,   759,   760,     0,     0,     0,
       0,   761,     0,   763,   764,     0,     0,     0,     0,   767,
     768,   769,     0,     0,     0,   773,     0,     0,     0,     0,
     267,     0,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,     0,     0,   278,   279,     0,     0,     0,     0,
       0,   280,   281,     0,     0,   286,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   775,     0,   776,   777,   778,
     779,   780,   781,   782,   783,   784,   785,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   786,   787,     0,     0,
     745,   746,   747,   748,   749,   735,   736,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,   934,     0,   767,   768,
     769,     0,     0,     0,   773,     0,     0,     0,     0,     0,
     735,   736,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   775,     0,   776,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,   745,
     746,   747,   748,   749,     0,     0,   752,   753,   754,   755,
       0,   757,   758,   759,   760,     0,     0,     0,     0,   761,
       0,   763,   764,     0,     0,  1175,     0,   767,   768,   769,
     735,   736,     0,   773,   745,   746,   747,   748,   749,     0,
       0,   752,   753,   754,   755,     0,   757,   758,   759,   760,
       0,     0,     0,     0,   761,     0,   763,   764,     0,     0,
       0,     0,   767,   768,   769,   735,   736,     0,   773,     0,
       0,     0,     0,   775,     0,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,   787,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   775,  1248,
     776,   777,   778,   779,   780,   781,   782,   783,   784,   785,
       0,     0,     0,     0,   745,   746,   747,   748,   749,   786,
     787,   752,   753,   754,   755,     0,   757,   758,   759,   760,
       0,     0,     0,     0,   761,     0,   763,   764,     0,     0,
       0,     0,   767,   768,   769,   735,   736,     0,   773,   745,
     746,   747,   748,   749,     0,     0,   752,   753,   754,   755,
       0,   757,   758,   759,   760,     0,     0,     0,     0,   761,
       0,   763,   764,     0,     0,     0,     0,   767,   768,   769,
    1260,     0,     0,   773,   735,   736,     0,     0,   775,     0,
     776,   777,   778,   779,   780,   781,   782,   783,   784,   785,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   786,
     787,  1354,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   775,     0,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,     0,     0,     0,     0,   745,
     746,   747,   748,   749,   786,   787,   752,   753,   754,   755,
       0,   757,   758,   759,   760,     0,     0,     0,     0,   761,
       0,   763,   764,     0,     0,     0,     0,   767,   768,   769,
       0,     0,     0,   773,   735,   736,     0,     0,   745,   746,
     747,   748,   749,     0,     0,   752,   753,   754,   755,     0,
     757,   758,   759,   760,     0,     0,     0,     0,   761,     0,
     763,   764,     0,     0,     0,  1577,   767,   768,   769,   735,
     736,     0,   773,   775,     0,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   786,   787,     0,     0,     0,     0,
       0,     0,     0,     0,  1637,     0,     0,     0,     0,     0,
       0,     0,   775,     0,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,     0,     0,     0,     0,   745,   746,
     747,   748,   749,   786,   787,   752,   753,   754,   755,     0,
     757,   758,   759,   760,     0,     0,     0,     0,   761,     0,
     763,   764,     0,     0,     0,     0,   767,   768,   769,   735,
     736,     0,   773,   745,   746,   747,   748,   749,     0,     0,
     752,   753,   754,   755,     0,   757,   758,   759,   760,     0,
       0,     0,     0,   761,     0,   763,   764,     0,     0,     0,
       0,   767,   768,   769,  1662,     0,     0,   773,   735,   736,
       0,     0,   775,     0,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   786,   787,     0,     0,     0,     0,  1663,
       0,     0,     0,     0,     0,     0,     0,   775,     0,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   785,     0,
       0,     0,     0,   745,   746,   747,   748,   749,   786,   787,
     752,   753,   754,   755,     0,   757,   758,   759,   760,     0,
       0,     0,     0,   761,     0,   763,   764,     0,     0,     0,
       0,   767,   768,   769,     0,     0,     0,   773,   735,   736,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,  1669,
     767,   768,   769,   735,   736,     0,   773,   775,     0,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   785,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,   787,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,   745,   746,   747,   748,   749,   786,   787,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,     0,     0,     0,     0,
     767,   768,   769,   735,   736,     0,  -751,   745,   746,   747,
     748,   749,     0,     0,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,   768,   769,   735,   736,
       0,     0,     0,     0,     0,     0,   775,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   775,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,   745,   746,   747,
     748,   749,   786,   787,   752,   753,   754,   755,     0,   757,
     758,   759,   760,     0,     0,     0,     0,   761,     0,   763,
     764,     0,     0,     0,     0,   767,     0,   769,   735,   736,
       0,     0,   745,   746,   747,   748,   749,     0,     0,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,   735,   736,     0,     0,
     767,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   776,   777,   778,   779,   780,   781,   782,
     783,   784,   785,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   786,   787,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,   745,   746,   747,   748,   749,   786,   787,   752,
     753,   754,   755,     0,   757,   758,   759,   760,     0,     0,
       0,     0,   761,     0,   763,   764,   735,   736,     0,     0,
     745,   746,   747,   748,   749,     0,     0,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,   735,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   776,   777,
     778,   779,   780,   781,   782,   783,   784,   785,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   786,   787,     0,
       0,     0,     0,     0,     0,     0,     0,   777,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
     745,   746,   747,   748,   749,   786,   787,   752,   753,   754,
     755,     0,   757,   758,   759,   760,     0,     0,     0,     0,
     761,     0,   763,   764,     0,     0,     0,     0,   745,   746,
     747,   748,   749,     0,     0,   752,   753,   754,   755,     0,
     757,   758,   759,   760,     0,     0,     0,     0,   761,     0,
     763,   764,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   978,     0,     0,     0,     0,   778,   779,
     780,   781,   782,   783,   784,   785,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   787,     0,     0,     0,
       0,     0,     0,     0,  1244,     0,     0,   779,   780,   781,
     782,   783,   784,   785,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   786,   787,   317,   318,   319,     0,   321,
     322,   323,   324,   325,   464,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,   339,   340,   341,
       0,     0,   344,   345,   346,   347,   317,   318,   319,     0,
     321,   322,   323,   324,   325,   464,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,     0,   339,   340,
     341,     0,     0,   344,   345,   346,   347,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   181,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   979,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   980,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   182,     0,   183,     0,   184,   185,   186,
     187,   188,  1245,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,  1246,   200,   201,   202,     0,     0,
     203,   204,   205,   206,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   207,
     208,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   209
};

static const yytype_int16 yycheck[] =
{
      14,    15,   211,   572,   521,   389,   691,   161,   693,   483,
     376,   909,   713,   383,   715,   376,   591,   625,   854,   420,
     421,   444,   592,   538,    80,   586,   227,   588,     7,   590,
      21,   472,   473,   816,   418,   818,   925,   820,   693,   724,
    1178,  1314,   931,     4,    52,   468,    32,   562,   630,    63,
      64,    65,    56,    18,    19,    19,    33,    19,    62,   103,
      19,    32,    14,    15,    52,    56,    19,   158,  1532,    19,
      19,    45,  1372,  1408,    19,   142,   143,   144,   227,    49,
    1523,  1557,  1525,   444,   141,    62,  1529,  1530,   127,   128,
     104,   105,   106,   107,   127,   128,   178,   481,   482,   168,
     193,   161,   444,     0,   471,   472,   473,   468,   690,     6,
     692,   168,   694,     6,   186,   168,   125,   172,   173,   212,
     702,    91,   125,   156,   215,  1589,   135,   828,  1604,   711,
     212,   105,    29,   125,    31,   168,    33,    14,    15,   211,
     101,   102,    39,   135,  1587,   214,   213,   301,   130,   141,
      20,    21,    49,   576,   521,   212,    49,   134,    55,   212,
     141,   503,   863,   219,   141,   680,  1501,  1467,   156,   854,
     209,   210,   180,   374,   218,   179,   209,   210,   187,   178,
     168,   548,    79,   192,   187,   140,   887,   190,    61,   178,
     124,   168,  1527,   127,   128,   187,  1334,   186,   180,   900,
     179,   141,   903,   168,   101,   102,   192,   198,   189,   161,
      20,    21,   613,   227,   166,   576,   168,   618,   209,   171,
     643,   192,   186,   178,   186,   374,   203,   186,   168,   180,
    1503,   210,   193,   186,   125,   212,   186,   186,   447,   210,
     389,   186,   186,   153,   135,   216,   116,   117,   158,   305,
     673,   212,   461,   595,   124,   180,   126,   127,   128,   129,
     212,  1159,   178,   150,   134,   649,  1432,   153,   125,   418,
     187,   420,   158,  1109,   125,   209,   210,   962,   135,   180,
     647,   168,   643,   180,   161,  1174,   186,   125,   672,   166,
     215,   168,   733,   194,   171,    32,   187,   135,   214,   215,
     642,   643,   199,   213,  1442,   215,   116,   117,   675,   180,
     125,   211,   673,   210,   124,   214,   215,   127,   128,   129,
     135,  1219,    59,    60,   134,   667,  1492,  1493,   670,   215,
     187,   180,   481,   482,   183,   186,   187,   351,   189,   209,
     210,   192,   213,   125,  1510,   186,   150,   156,   193,   187,
     189,   168,   189,   135,   158,   192,   168,   169,   170,   168,
     374,   731,   211,   180,   168,   379,   733,   212,   950,    32,
     211,  1269,   187,   186,   141,   389,   577,   578,   579,   802,
     581,   582,   724,  1562,   585,   122,   587,   199,   589,   126,
     183,   184,   186,    46,  1573,   186,    59,    60,   211,   209,
     210,  1567,  1568,   212,   418,   187,   420,   808,   212,   180,
     186,   972,   186,    66,   798,   168,   187,   180,   211,   213,
     629,   822,   213,   824,  1109,   848,   440,   441,   577,   578,
     579,   194,   581,   582,  1613,  1614,   585,   804,   587,   213,
     589,   802,   591,   180,  1333,   654,   961,   789,   212,   186,
    1233,   188,   186,   161,   186,   192,  1345,   194,   195,   122,
     186,   141,   936,   126,   186,   849,   850,   481,   482,   209,
     150,   141,   946,   210,   186,   155,   917,   211,   862,   216,
     150,   213,   186,   193,   885,   211,   193,   848,   168,  1174,
     699,   213,  1177,   168,  1102,    63,    64,    65,   168,   186,
     649,   213,   212,   189,  1189,   212,   192,   211,   892,   893,
     719,   895,   854,   193,  1089,   899,   168,   901,   902,  1174,
     904,  1091,   186,   672,   186,   188,   213,     4,     5,   192,
       7,   194,   195,    32,  1189,   186,   104,   105,   106,   107,
     907,   982,   983,   984,  1327,   186,   178,   210,   190,   213,
     917,   213,   194,   216,   142,   193,   144,   189,    35,   180,
      59,    60,   213,   577,   578,   579,    74,   581,   582,   168,
      78,   585,   213,   587,   212,   589,   150,   591,   189,  1362,
     922,   180,   180,   194,    92,    93,   735,   736,   930,    97,
      98,    99,   100,    11,   168,   189,   180,   156,   168,  1160,
     194,   212,   186,   168,    22,    23,   189,   156,   189,   168,
     194,   194,   761,   194,   168,   982,   983,   984,   212,   168,
     987,   635,   989,   122,   991,   826,   993,   126,   995,   212,
     997,   212,   999,   834,  1001,   649,   837,   168,   156,  1006,
      56,  1008,   168,   797,   190,   846,    62,  1014,   194,   798,
     168,  1212,   853,   172,   180,   156,   376,   858,   672,  1026,
     186,  1028,   156,    56,   178,   180,  1033,   168,  1035,    62,
    1037,   186,    56,  1040,   168,   884,   180,   826,    62,   194,
    1463,    56,   186,   168,  1369,   834,    56,    62,   837,   188,
     194,   180,   180,   192,   172,   194,   195,   846,   186,   190,
     849,   850,   716,   194,   853,  1072,   194,   142,   180,   858,
     180,   210,   180,   862,   186,    56,   186,   216,   186,   190,
     190,    62,   194,   194,   444,   186,   194,   179,   189,  1337,
     182,   192,   105,   185,  1118,  1119,  1120,  1104,   939,    56,
     941,   180,   187,   892,   893,    62,   895,   186,   468,   178,
     899,    56,   901,   902,   180,   904,   180,    62,   180,  1143,
     186,    32,   186,   180,   186,  1107,   180,   180,   178,   186,
     190,   190,   186,   186,   194,   194,   178,     4,     5,   190,
      34,   795,   190,   194,   798,    34,   194,   190,    59,    60,
     939,   194,   941,   178,   190,   190,   168,    24,   194,   194,
     212,   179,   180,    30,   182,   212,  1367,   214,   215,  1326,
     172,   173,   826,   172,   173,   174,   175,   142,   143,   144,
     834,   179,   939,   837,   941,  1167,  1511,  1211,   168,   169,
     170,   168,   846,  1441,   168,   849,   850,   168,  1204,   853,
      67,    68,   211,  1204,   858,    21,  1188,   168,   862,    86,
      87,   122,  1194,   189,   125,   126,   576,   211,   178,  1201,
     172,   173,   174,  1205,   135,   179,   180,   181,    10,    11,
      12,  1572,   178,   189,   101,   102,  1077,   212,   892,   893,
      42,   895,   193,  1584,  1569,   899,   193,   901,   902,   193,
     904,   193,  1041,   212,   193,   193,   193,   212,   193,  1241,
    1469,   193,  1303,   193,   193,   190,  1607,   168,   211,   180,
     135,  1253,   212,   168,   141,   168,   187,   188,   212,   210,
    1605,   192,   168,   643,   195,   939,   193,   941,  1077,   193,
     193,   158,   159,   160,   212,   168,   193,   193,   212,   210,
    1089,   168,   180,   193,   161,   216,   193,   212,   193,   193,
     212,   178,   212,   673,   212,    36,   215,   193,   180,  1326,
     212,   212,   180,     9,   180,   180,   193,   180,   180,  1118,
    1119,  1120,   180,  1581,    65,   210,  1450,  1546,  1547,  1453,
     212,  1666,   179,   210,   212,    42,  1328,   193,   193,    20,
      21,   168,   213,   168,  1143,   180,  1380,   211,   179,   179,
     189,   179,   212,    42,  1213,   212,   193,   193,   130,   193,
      13,   161,    20,    21,   186,   189,  1358,   737,   738,   739,
     740,   741,   742,   743,   744,   215,  1595,   187,    20,    21,
     750,   751,   186,   178,    12,   186,   756,   158,   168,     7,
     168,   194,   213,   168,   168,   765,   766,   213,   168,   212,
     770,   771,   772,   186,   774,   186,   179,   213,   213,   211,
     179,   168,  1211,  1077,   212,   212,  1408,   193,   212,   212,
       1,   212,   212,   212,   212,  1089,   193,   179,    66,   180,
     168,   194,   802,    32,  1468,   116,   117,  1296,   194,  1290,
     194,    42,   168,   124,   212,   126,   127,   128,   129,   213,
     168,   213,   213,   134,  1118,  1119,  1120,   212,   116,   117,
      59,    60,   168,   213,   213,   211,   124,   211,   126,   127,
     128,   129,   114,   115,   116,   117,   134,   211,   848,  1143,
     168,   168,   124,   193,   126,   127,   128,   129,   212,   212,
     212,  1290,   134,   212,   136,   137,   180,   168,   213,   168,
     212,   212,   211,   180,   168,  1364,    32,   211,   213,  1501,
     211,  1545,   212,   212,   212,  1507,   197,   198,   199,   168,
    1379,   194,   212,   122,    69,   180,   125,   126,   209,   210,
     213,  1382,   212,   212,   212,  1527,   135,   195,   196,   197,
     198,   199,   212,  1535,   212,   212,   194,  1211,   213,   194,
     212,   209,   210,   195,   196,   197,   198,   199,   212,    52,
     212,   186,   213,   213,   179,   376,   213,   209,   210,   213,
     211,   179,   179,   186,   211,   386,   180,   213,  1437,   213,
     213,  1380,   213,  1382,   395,   211,   213,   211,   187,   188,
     179,   213,   211,   192,   405,   212,   195,   213,  1590,   213,
     701,    79,     1,    44,   415,   133,    82,  1460,   795,  1461,
     948,   210,  1461,   424,    32,   225,  1461,   216,  1461,  1461,
       1,   968,  1437,   434,     9,  1377,  1290,  1400,  1489,  1403,
    1481,  1265,   428,   444,  1490,    20,    21,    52,   449,  1569,
     451,    59,    60,  1448,   560,  1184,   693,    -1,   494,   460,
      -1,    -1,    -1,   440,   440,    -1,    -1,   468,   469,   470,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1468,
      -1,    -1,   483,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     491,    -1,  1481,   494,  1543,    32,   497,   498,   499,   500,
     501,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   510,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    59,    60,    -1,    -1,  1380,    -1,  1382,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,  1545,    -1,    -1,   134,
     135,   136,   137,   138,   139,    -1,    -1,   142,   143,   144,
     145,   146,   147,   148,    -1,   576,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,   192,   122,   194,   195,    -1,   126,
      -1,  1640,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   210,   604,    -1,    -1,    -1,    -1,   216,    -1,
      -1,    -1,   187,   188,  1468,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,  1481,    -1,    -1,
      -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,   214,
     215,    -1,   643,    -1,  1204,    -1,    -1,    -1,    -1,   650,
     651,   188,   653,    -1,    -1,   192,    -1,   658,   195,    -1,
      -1,    -1,    -1,   664,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   673,   210,    -1,    -1,    -1,    -1,  1532,   216,
      -1,    -1,    -1,   684,   685,   686,   687,   688,   689,    -1,
     691,  1545,   693,    -1,    -1,   738,   739,   740,   741,   742,
     743,   744,    -1,    -1,    -1,    -1,    11,   750,   751,    -1,
      -1,    -1,    -1,   756,    -1,    20,    21,    -1,    -1,    -1,
      -1,    -1,   765,   766,    -1,    -1,    -1,   770,   771,   772,
      -1,   774,    -1,    -1,    -1,  1589,   737,   738,   739,   740,
     741,   742,   743,   744,   745,   746,    -1,    -1,   749,   750,
     751,   752,   753,   754,    -1,   756,   757,    -1,   759,   760,
     761,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   778,   779,   780,
     781,   782,   783,   784,   785,    -1,   787,    -1,    -1,    -1,
     791,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   802,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,   118,    -1,    -1,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
     841,    -1,    -1,   148,    -1,    -1,    -1,   848,    -1,    -1,
      -1,    -1,    -1,    -1,   855,    -1,   857,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   866,   867,   868,   869,   870,
     871,   872,   873,   874,   875,   876,   877,   878,   879,   880,
     881,   882,   883,   188,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   920,
     921,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,   934,    -1,   936,    -1,    59,    60,    -1,
      -1,    -1,   943,    -1,    -1,   946,    -1,    -1,    -1,     1,
     951,    -1,     4,     5,     6,    -1,     8,     9,    10,   960,
      12,    -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,    41,
     122,    43,    44,    -1,   126,    47,    -1,    49,    50,    51,
     122,    53,    54,    -1,   126,    57,    58,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,   104,  1054,    -1,    -1,   188,  1058,    -1,    -1,
     192,    -1,   194,   195,   116,   117,   188,    -1,    -1,    -1,
     192,    -1,   194,   195,    -1,    -1,    -1,    -1,   210,   131,
     132,   133,    -1,    -1,   216,    -1,    -1,    -1,   210,   141,
      -1,    -1,    -1,    -1,   216,    -1,    -1,   149,   150,   151,
     152,   153,    -1,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   179,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,  1147,   199,    -1,    -1,
     202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,
     212,    -1,   214,   215,    -1,    -1,    -1,    -1,  1169,  1170,
    1171,    -1,    -1,    -1,  1175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1183,  1184,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    12,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,  1204,    -1,    -1,    24,    -1,  1209,  1210,
      -1,    -1,    30,    -1,    -1,  1216,    -1,    -1,    -1,    -1,
    1221,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    20,    21,    -1,  1235,    -1,  1237,    -1,    -1,    -1,
      -1,  1242,    -1,    -1,    -1,    63,    -1,  1248,    -1,    -1,
      -1,  1252,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   114,   115,   116,   117,
     118,    -1,  1293,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,  1314,   142,   143,   144,    -1,    -1,    -1,
     148,    -1,    -1,   141,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,   154,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
     168,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,    20,    21,   213,    -1,  1388,    -1,  1390,
      -1,    -1,    -1,    -1,  1395,    -1,   214,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,  1417,    -1,    -1,  1420,
     209,   210,    -1,    -1,    -1,   214,   215,  1428,  1429,  1430,
      -1,    -1,    -1,    -1,  1435,    -1,    -1,    -1,    -1,  1440,
      -1,    -1,    -1,  1444,  1445,  1446,  1447,    -1,    -1,  1450,
    1451,    -1,  1453,  1454,    -1,    -1,    -1,    -1,    -1,    -1,
    1461,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1472,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,  1503,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,  1517,  1518,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1531,    -1,    -1,    -1,    -1,  1536,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1548,  1549,    -1,
      -1,   188,    32,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,  1564,  1565,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,   213,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1591,    -1,    -1,    -1,    -1,  1596,  1597,    -1,    -1,    -1,
      -1,     1,    -1,    -1,     4,     5,     6,    -1,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    -1,
    1621,    -1,  1623,  1624,    24,    25,    26,    27,    28,    -1,
      30,    -1,    -1,  1634,    -1,    -1,    -1,    37,    38,    -1,
      -1,    41,   122,    43,    44,    -1,   126,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,   188,    -1,
      -1,    -1,   192,    -1,   194,   195,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     210,   131,   132,   133,    -1,    -1,   216,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,   151,   152,   153,    32,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,   179,
     180,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,
     210,    -1,   212,    -1,   214,   215,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,
      -1,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   122,    -1,    44,    -1,   126,    47,
      -1,    -1,    50,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    59,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,   192,    -1,   194,   195,   116,   117,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,   126,    -1,
      -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,   216,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,    32,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,   192,    -1,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,   203,    -1,    -1,    -1,     4,
       5,    -1,   210,    -1,   212,    -1,   214,   215,   216,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,   122,    -1,    -1,    44,
     126,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,   188,    -1,    -1,    -1,   192,    -1,   194,   195,
      -1,   116,   117,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,   126,    -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,
     216,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    32,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,    -1,     4,     5,    -1,   210,    -1,   212,    -1,   214,
     215,   216,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   122,
      -1,    -1,    44,   126,    -1,    47,    -1,    -1,    50,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,   103,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,
      -1,   194,   195,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,
     152,   153,    32,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     192,    -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,   203,    -1,    -1,    -1,     4,     5,    -1,   210,    -1,
     212,    -1,   214,   215,   216,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,   122,    -1,    -1,    44,   126,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,   188,    -1,
      -1,    -1,   192,    -1,   194,   195,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
     210,    -1,   131,   132,   133,    -1,   216,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    32,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,
      -1,   210,    -1,   212,    -1,   214,   215,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   122,    -1,    44,    -1,   126,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,   192,    -1,   194,   195,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,   210,   131,   132,   133,    -1,    -1,   216,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    32,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,
       5,    -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      25,    26,    27,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   122,    -1,    44,
      -1,   126,    47,    -1,    -1,    50,    51,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,   188,    -1,    -1,    -1,   192,    -1,   194,
     195,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    32,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,
     215,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   122,
      -1,    44,    -1,   126,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,
      -1,   194,   195,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,   131,   132,
     133,    -1,    -1,   216,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    32,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,
      -1,   214,   215,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    32,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   122,    -1,    44,    -1,   126,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,   192,    -1,   194,   195,   116,   117,    -1,   122,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,   210,
     131,   132,   133,    -1,    -1,   216,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    32,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    59,    60,   188,    -1,    -1,    -1,   192,    -1,
     194,   195,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,    -1,   210,     4,     5,   210,
      -1,   212,   216,   214,   215,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    32,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   122,    -1,    44,    -1,   126,
      47,    48,    -1,    50,    -1,    -1,    -1,    54,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,   192,    -1,   194,   195,   116,
     117,    -1,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,   216,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    -1,   155,    32,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    59,    60,   188,    -1,
      -1,    -1,   192,    -1,   194,   195,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,    -1,
     210,     4,     5,   210,   211,   212,   216,   214,   215,    12,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   122,
      -1,    44,    -1,   126,    47,    48,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,   192,
      -1,   194,   195,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,
      -1,    -1,    -1,   216,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,    -1,    -1,     4,     5,   210,    -1,   212,
      -1,   214,   215,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    48,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,     4,
       5,   210,   211,   212,    -1,   214,   215,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    48,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,
     215,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,
     213,   214,   215,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,
      -1,   212,    -1,   214,   215,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,
      -1,   210,   211,   212,    -1,   214,   215,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,
       5,    -1,    -1,   210,    -1,   212,    -1,   214,   215,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,    -1,    -1,     4,     5,   210,    -1,   212,    -1,   214,
     215,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,
      -1,   212,    -1,   214,   215,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,     4,
       5,   210,    -1,   212,    -1,   214,   215,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,
     215,    14,    15,    16,    17,    18,    -1,    -1,    21,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,
      -1,   214,   215,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,
      -1,   212,   213,   214,   215,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,
      -1,   210,    -1,   212,    -1,   214,   215,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,
       5,    -1,    -1,   210,    -1,   212,   213,   214,   215,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,     4,     5,    -1,    -1,   210,    -1,   212,   213,   214,
     215,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,
     213,   214,   215,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,
      -1,   212,   213,   214,   215,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,
      -1,   210,    -1,   212,   213,   214,   215,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,   202,   203,    -1,    -1,     4,
       5,    -1,    -1,   210,    -1,   212,   213,   214,   215,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,    -1,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,    -1,    -1,   202,   203,    -1,
      -1,     4,     5,    -1,    -1,   210,    -1,   212,    -1,   214,
     215,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    24,    -1,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,    -1,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,
     203,    -1,    -1,     4,     5,    -1,    -1,   210,    -1,   212,
      -1,   214,   215,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,   152,   153,    -1,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,   202,   203,    -1,    -1,     4,     5,    -1,    -1,   210,
      -1,   212,    -1,   214,   215,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,   151,   152,   153,    -1,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,   202,   203,    -1,    -1,     4,     5,    -1,
      -1,   210,    -1,   212,    -1,   214,   215,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,   151,   152,   153,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    12,    -1,    -1,    -1,
      -1,   178,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,   195,   196,
     197,    -1,   199,    -1,    40,   202,   203,    -1,    -1,    -1,
      -1,    -1,    48,   210,    -1,   212,    -1,   214,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,   168,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,
      18,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,   168,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
     168,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,   180,    -1,    -1,    -1,    -1,    40,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,   214,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    20,    21,    -1,    -1,   168,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   168,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     214,    20,    21,   134,   135,   136,   137,   138,   139,    -1,
      -1,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   187,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,    -1,   214,   215,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,   135,   136,   137,   138,
     139,    -1,    -1,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,    -1,   214,   215,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
     139,    -1,    -1,   142,   143,   144,   145,   146,   147,   148,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    20,    21,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,   180,    -1,    -1,   148,    -1,    -1,    -1,   187,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,    -1,   214,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    20,    21,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    20,    21,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    20,    21,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    20,    21,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    20,    21,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    20,    21,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    20,    21,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    20,    21,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,   114,   115,   116,   117,   118,
      20,    21,   121,   122,   123,   124,    -1,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   213,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     209,   210,   211,    -1,   114,   115,   116,   117,   118,    20,
      21,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
     210,   211,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,   114,   115,
     116,   117,   118,    20,    21,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
     211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,   211,    -1,   114,   115,   116,
     117,   118,    20,    21,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,   211,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,    20,    21,    -1,    -1,   134,    -1,   136,   137,
      -1,    -1,   156,    -1,   142,   143,   144,    -1,    -1,    37,
     148,    -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   209,   210,   211,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    -1,   126,   127,
     128,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,   137,
     138,   139,    -1,    -1,   142,   143,   144,   145,   146,   147,
     148,   114,   115,   116,   117,   118,    20,    21,   121,   122,
     123,   124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,   142,
     143,   144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
     188,    -1,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,   202,   203,    -1,    -1,    -1,    -1,
      -1,   209,   210,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,
     114,   115,   116,   117,   118,    20,    21,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,   140,    -1,   142,   143,
     144,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,   114,
     115,   116,   117,   118,    -1,    -1,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,   140,    -1,   142,   143,   144,
      20,    21,    -1,   148,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    20,    21,    -1,   148,    -1,
      -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,   114,   115,   116,   117,   118,   209,
     210,   121,   122,   123,   124,    -1,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,
      -1,    -1,   142,   143,   144,    20,    21,    -1,   148,   114,
     115,   116,   117,   118,    -1,    -1,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
     180,    -1,    -1,   148,    20,    21,    -1,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,
     210,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,   118,   209,   210,   121,   122,   123,   124,
      -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,
      -1,    -1,    -1,   148,    20,    21,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,   180,   142,   143,   144,    20,
      21,    -1,   148,   188,    -1,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,   209,   210,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,   142,   143,   144,    20,
      21,    -1,   148,   114,   115,   116,   117,   118,    -1,    -1,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,   180,    -1,    -1,   148,    20,    21,
      -1,    -1,   188,    -1,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,   209,   210,
     121,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      -1,   142,   143,   144,    -1,    -1,    -1,   148,    20,    21,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,   180,
     142,   143,   144,    20,    21,    -1,   148,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   209,   210,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    -1,
     142,   143,   144,    20,    21,    -1,   148,   114,   115,   116,
     117,   118,    -1,    -1,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,   143,   144,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   209,   210,   121,   122,   123,   124,    -1,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    -1,   142,    -1,   144,    20,    21,
      -1,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    20,    21,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,   114,   115,   116,   117,   118,   209,   210,   121,
     122,   123,   124,    -1,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    20,    21,    -1,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,   210,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,   209,   210,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,   124,    -1,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   209,   210,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    -1,    -1,   193,   194,   195,
     196,   197,   198,   199,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   209,   210,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    -1,    97,    98,    99,   100,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    72,    -1,    74,    75,    76,
      77,    78,   156,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,   168,    92,    93,    94,    -1,    -1,
      97,    98,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   218,     0,     6,    29,    31,    33,    39,    49,    55,
      79,   101,   102,   180,   199,   210,   219,   222,   228,   230,
     231,   236,   266,   270,   304,   382,   389,   393,   404,   449,
     454,   459,    18,    19,   168,   258,   259,   260,   161,   237,
     238,   168,   169,   170,   199,   232,   233,   234,   178,   390,
     168,   214,   221,    56,    62,   385,   385,   385,   141,   168,
     290,    33,    62,   134,   203,   212,   262,   263,   264,   265,
     290,   180,     4,     5,     7,    35,   401,    61,   380,   187,
     186,   189,   186,   233,    21,    56,   198,   209,   235,   385,
     386,   388,   386,   380,   460,   450,   455,   168,   141,   229,
     264,   264,   264,   212,   142,   143,   144,   186,   211,    56,
      62,   271,   273,    56,    62,   391,    56,    62,   402,    56,
      62,   381,    14,    15,   161,   166,   168,   171,   212,   224,
     259,   161,   238,   168,   232,   232,   168,   180,   179,   386,
     180,    56,    62,   220,   168,   168,   168,   168,   172,   227,
     213,   260,   264,   264,   264,   264,   274,   168,   392,   405,
     178,   383,   172,   173,   223,    14,    15,   161,   166,   168,
     224,   256,   257,   235,   387,   180,   461,   451,   456,   172,
     213,    34,    70,    72,    74,    75,    76,    77,    78,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      92,    93,    94,    97,    98,    99,   100,   116,   117,   168,
     269,   272,   178,   189,   105,   399,   400,   378,   158,   215,
     261,   354,   172,   173,   174,   186,   213,   187,   178,   178,
     178,    20,    21,    37,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   126,   127,   128,   135,   136,   137,   138,   139,
     142,   143,   144,   145,   146,   147,   148,   188,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   202,   203,
     209,   210,    34,    34,   212,   267,   178,   275,   394,    74,
      78,    92,    93,    97,    98,    99,   100,   409,   168,   406,
     179,   379,   260,   259,   180,   215,   150,   168,   376,   377,
     256,    18,    24,    30,    40,    48,    63,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     154,   214,   290,   408,   410,   411,   414,   420,   448,   462,
     452,   457,   168,   168,   168,   211,    21,   168,   211,   153,
     213,   354,   364,   365,   189,   268,   278,   384,   178,   189,
     398,   178,   403,   354,   211,   259,   212,    42,   186,   189,
     192,   375,   193,   193,   193,   212,   193,   193,   212,   193,
     193,   193,   193,   193,   193,   212,   290,    32,    59,    60,
     122,   126,   188,   192,   195,   210,   216,   419,   190,   413,
     368,   371,   168,   135,   212,     6,    49,   303,   180,   213,
     448,     1,     4,     5,     8,     9,    10,    12,    14,    15,
      16,    17,    18,    24,    25,    26,    27,    28,    30,    37,
      38,    41,    43,    44,    47,    50,    51,    53,    54,    57,
      58,    64,    67,    68,    79,   101,   102,   103,   104,   116,
     117,   131,   132,   133,   149,   150,   151,   152,   153,   155,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   169,   170,   171,   178,   179,   180,   195,   196,   197,
     202,   203,   210,   212,   214,   215,   226,   228,   239,   240,
     243,   246,   247,   249,   251,   252,   253,   254,   276,   277,
     279,   284,   289,   290,   291,   295,   296,   297,   298,   299,
     300,   301,   302,   304,   308,   309,   316,   319,   322,   327,
     330,   331,   333,   334,   335,   337,   342,   345,   346,   353,
     408,   464,   479,   490,   494,   507,   510,   168,   180,   395,
     396,   290,   360,   377,   211,    64,   103,   169,   284,   346,
     168,   168,   420,   125,   135,   187,   374,   421,   426,   428,
     346,   430,   424,   168,   415,   432,   434,   436,   438,   440,
     442,   444,   446,   346,   193,   212,    32,   192,    32,   192,
     210,   216,   211,   346,   210,   216,   420,   168,   180,   463,
     168,   180,   186,   366,   417,   448,   453,   168,   369,   417,
     458,   346,   150,   168,   373,   407,   364,   193,   193,   346,
     250,   193,   292,   410,   464,   212,   290,   193,     4,   101,
     102,   193,   212,   125,   289,   320,   331,   346,   275,   193,
     212,    60,   346,   212,   346,   168,   193,   193,   212,   180,
     193,   161,    57,   346,   212,   275,   193,   212,   193,   193,
     212,   193,   193,   125,   289,   346,   346,   346,   215,   275,
     322,   326,   326,   326,   212,   212,   212,   212,   212,   212,
      12,   420,    12,   420,    12,   346,   489,   505,   193,   346,
     193,   225,    12,   489,   506,    36,   346,   346,   346,   346,
     346,    12,    48,   320,   346,   320,   215,   180,   180,   346,
       9,   322,   328,   168,   212,   180,   180,   180,   180,   180,
      65,   305,   266,   130,   180,    20,    21,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   134,   135,   136,   137,   138,   139,   142,   143,   144,
     145,   146,   147,   148,   187,   188,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   209,   210,   328,   212,
     180,   187,   179,   384,   179,   210,   266,   361,   193,   213,
      42,   180,   374,   289,   346,   448,   448,   418,   448,   213,
     448,   448,   213,   168,   412,   448,   267,   448,   267,   448,
     267,   366,   367,   369,   370,   213,   423,   280,   320,   211,
     211,   346,   180,   179,   189,   417,   179,   189,   417,   179,
     213,   212,    42,   125,   186,   187,   189,   192,   372,   480,
     482,   275,   407,   293,   212,   290,   193,   212,   317,   193,
     193,   193,   501,   320,   289,   320,   186,   106,   107,   108,
     109,   110,   111,   112,   113,   119,   120,   125,   138,   139,
     145,   146,   147,   187,    13,   420,   282,   506,   346,   346,
     275,   187,   310,   312,   346,   314,   189,   161,   346,   503,
     320,   486,   491,   320,   484,   420,   289,   346,   215,   266,
     346,   346,   346,   346,   346,   346,   407,    52,   156,   168,
     195,   210,   212,   346,   465,   468,   472,   488,   493,   407,
     212,   468,   493,   407,   140,   179,   180,   181,   473,   285,
     275,   287,   174,   175,   223,   407,   186,   509,   178,   407,
      12,   186,   509,   509,   150,   155,   193,   290,   336,   275,
     248,   329,   210,   213,   320,   465,   467,   158,   212,   307,
     377,   158,   325,   326,    18,   156,   168,   408,    18,   156,
     168,   408,   131,   132,   133,   276,   332,   346,   332,   346,
     332,   346,   332,   346,   332,   346,   332,   346,   332,   346,
     332,   346,   346,   346,   346,   332,   346,   332,   346,   346,
     346,   346,   168,   332,   346,   346,   156,   168,   346,   346,
     346,   408,   346,   346,   346,   332,   346,   332,   346,   346,
     346,   346,   332,   346,   332,   346,   332,   346,   346,   332,
     346,    21,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   127,   128,   156,   168,   209,   210,   343,
     346,   213,   320,   346,   397,   265,     7,   354,   359,   420,
     168,   289,   346,   180,   194,   194,   194,   417,   194,   194,
     180,   194,   194,   268,   194,   268,   194,   268,   194,   417,
     194,   417,   283,   448,   213,   509,   211,   448,   448,   346,
     168,   168,   448,   289,   346,   420,   420,    19,   448,    69,
     320,   467,   478,   193,   346,   168,   346,   448,   495,   497,
     499,   420,   509,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   275,   194,   417,   213,   509,   213,   255,   420,   420,
     213,   420,   213,   420,   509,   420,   420,   509,   420,   194,
     325,   213,   213,   213,   213,   213,   213,    19,   326,   212,
     135,   372,   210,   346,   213,   140,   186,   180,   472,   183,
     184,   211,   476,   186,   180,   183,   211,   475,    19,   213,
     472,   179,   182,   474,    19,   346,   179,   489,   283,   283,
     346,    19,   489,   179,   278,    19,   407,   211,   213,   212,
     212,   338,   340,    11,    22,    23,   241,   242,   346,   266,
     465,   186,   213,   213,   168,   306,   306,   125,   135,   187,
     192,   323,   324,   267,   193,   212,   193,   212,   326,   326,
     326,   212,   212,   211,    18,   156,   168,   408,   189,   156,
     168,   346,   212,   212,   156,   168,   346,     1,   211,   213,
     180,   179,   211,    56,    62,   357,    66,   358,   180,   194,
     180,   422,   427,   429,   448,   431,   425,   168,   416,   433,
     194,   437,   194,   441,   194,   445,   366,   447,   369,   194,
     417,   213,    42,   372,   194,   194,   320,   194,   168,   467,
     213,   213,   213,   168,   213,   180,   194,   213,   194,   420,
     420,   420,   194,   213,   212,   420,   346,   194,   194,   194,
     194,   213,   194,   194,   213,   194,   325,   267,   212,   320,
     346,   346,   346,   468,   472,   346,   156,   168,   465,   476,
     211,   346,   488,   211,   320,   468,   179,   182,   185,   477,
     211,   320,   194,   194,   176,   320,   179,   320,    19,   346,
     346,   420,   267,   275,   346,    11,   244,   325,   211,   210,
     186,   211,   213,   168,   168,   168,   168,   186,   211,   268,
     347,   346,   349,   346,   213,   320,   346,   193,   212,   346,
     212,   211,   346,   213,   320,   212,   211,   344,   180,    46,
     358,    45,   105,   355,   325,   435,   439,   443,   212,   448,
     168,   346,   481,   483,   275,   294,   213,   194,   417,   168,
     212,   318,   194,   194,   194,   502,   282,   194,   311,   313,
     315,   504,   487,   492,   485,   212,   328,   268,   213,   320,
     180,   213,   472,   476,   212,   135,   372,   180,   472,   211,
     180,   286,   288,   180,   180,   320,   213,   213,   194,   268,
     275,   245,   180,   267,   213,   465,   168,   211,   189,   375,
     323,   211,   140,   275,   321,   420,   213,   448,   213,   213,
     213,   351,   346,   346,   213,   213,   346,    32,   356,   355,
     357,   280,   212,   212,   346,   168,   194,   346,   496,   498,
     500,   212,   213,   212,   346,   346,   346,   212,    69,   478,
     212,   212,   213,   346,   321,   213,   346,   135,   372,   476,
     346,   346,   346,   346,   477,   489,   346,   212,   281,   489,
     346,   180,   339,   194,   242,    25,   104,   246,   296,   297,
     298,   300,   346,   268,   211,   189,   375,   420,   374,   125,
     346,   194,   194,   448,   213,   213,   213,   362,   356,   373,
     213,   478,   478,   213,   194,   212,   213,   212,   212,   212,
     280,   282,   320,   478,   465,   466,   213,   180,   508,   346,
     346,   213,   508,   508,   280,   508,   508,   346,   336,   341,
     125,   125,   346,   275,   213,   420,   374,   374,   346,   346,
     348,   350,   194,   272,   363,   212,   465,   469,   470,   471,
     471,   346,   346,   478,   478,   466,   213,   213,   509,   471,
     213,    52,   211,   135,   372,   179,   179,   186,   509,   179,
     211,   508,   336,   346,   374,   346,   346,   180,   352,   180,
     272,   465,   186,   509,   213,   213,   213,   213,   471,   471,
     213,   213,   213,   346,   211,   346,   346,   211,   179,   213,
     211,   346,   180,   180,   275,   213,   212,   213,   213,   180,
     465,   213
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   217,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   219,   220,   220,
     220,   221,   221,   222,   223,   223,   223,   223,   224,   225,
     225,   225,   226,   227,   227,   229,   228,   230,   231,   232,
     232,   232,   233,   233,   233,   233,   234,   234,   235,   235,
     236,   237,   237,   238,   238,   239,   240,   240,   241,   241,
     242,   242,   242,   243,   243,   244,   245,   244,   246,   246,
     246,   246,   246,   247,   248,   247,   250,   249,   251,   252,
     253,   255,   254,   256,   256,   256,   256,   256,   256,   257,
     257,   258,   258,   258,   259,   259,   259,   259,   259,   259,
     259,   259,   260,   260,   261,   261,   261,   262,   262,   262,
     263,   263,   264,   264,   264,   264,   264,   264,   264,   265,
     265,   266,   266,   267,   267,   267,   268,   268,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   270,   271,   271,   271,   272,   274,   273,
     275,   275,   276,   276,   276,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   278,   278,   278,   279,
     280,   280,   281,   281,   282,   282,   283,   283,   285,   286,
     284,   287,   288,   284,   289,   289,   289,   289,   289,   290,
     290,   290,   291,   291,   293,   294,   292,   292,   295,   295,
     295,   295,   295,   295,   296,   297,   298,   298,   298,   299,
     299,   299,   300,   300,   301,   301,   301,   302,   303,   303,
     303,   304,   304,   305,   305,   306,   306,   307,   307,   307,
     307,   308,   308,   310,   311,   309,   312,   313,   309,   314,
     315,   309,   317,   318,   316,   319,   319,   319,   319,   319,
     319,   320,   320,   321,   321,   321,   322,   322,   322,   323,
     323,   323,   323,   324,   324,   325,   325,   326,   326,   327,
     329,   328,   330,   330,   330,   330,   330,   330,   330,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   332,   332,
     332,   332,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   334,
     334,   335,   335,   336,   336,   337,   338,   339,   337,   340,
     341,   337,   342,   342,   342,   342,   343,   344,   342,   345,
     345,   345,   345,   345,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     347,   348,   346,   346,   346,   346,   349,   350,   346,   346,
     346,   351,   352,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   353,   353,
     353,   353,   353,   353,   353,   353,   353,   353,   353,   353,
     353,   353,   353,   353,   354,   354,   354,   355,   355,   355,
     356,   356,   357,   357,   357,   358,   358,   359,   360,   361,
     360,   362,   360,   363,   360,   360,   364,   364,   365,   365,
     366,   366,   367,   367,   368,   368,   368,   369,   370,   370,
     371,   371,   371,   372,   372,   373,   373,   373,   373,   373,
     373,   374,   374,   374,   375,   375,   376,   376,   376,   376,
     376,   377,   377,   377,   377,   377,   378,   379,   378,   380,
     380,   381,   381,   381,   382,   383,   382,   384,   384,   384,
     384,   385,   385,   385,   387,   386,   388,   388,   389,   390,
     389,   391,   391,   391,   392,   394,   395,   393,   396,   397,
     393,   398,   398,   399,   399,   400,   401,   401,   402,   402,
     402,   403,   403,   405,   406,   404,   407,   407,   407,   407,
     407,   408,   408,   408,   408,   408,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   408,   408,   408,   408,
     408,   408,   408,   408,   408,   408,   408,   408,   409,   409,
     409,   409,   409,   409,   409,   409,   410,   411,   411,   411,
     412,   412,   413,   413,   413,   415,   416,   414,   417,   417,
     418,   418,   419,   419,   420,   420,   420,   420,   420,   420,
     421,   422,   420,   420,   420,   423,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   424,
     425,   420,   420,   426,   427,   420,   428,   429,   420,   430,
     431,   420,   420,   432,   433,   420,   434,   435,   420,   420,
     436,   437,   420,   438,   439,   420,   420,   440,   441,   420,
     442,   443,   420,   444,   445,   420,   446,   447,   420,   448,
     448,   448,   450,   451,   452,   453,   449,   455,   456,   457,
     458,   454,   460,   461,   462,   463,   459,   464,   464,   464,
     464,   464,   465,   465,   465,   465,   465,   465,   465,   465,
     466,   467,   468,   468,   469,   469,   470,   470,   471,   471,
     472,   472,   473,   473,   474,   474,   475,   475,   476,   476,
     476,   477,   477,   477,   478,   478,   479,   479,   479,   479,
     479,   479,   480,   481,   479,   482,   483,   479,   484,   485,
     479,   486,   487,   479,   488,   488,   488,   489,   489,   490,
     491,   492,   490,   493,   493,   494,   494,   494,   495,   496,
     494,   497,   498,   494,   499,   500,   494,   494,   501,   502,
     494,   494,   503,   504,   494,   505,   505,   506,   506,   507,
     507,   507,   507,   507,   508,   508,   509,   509,   510,   510,
     510,   510,   510,   510
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     1,
       1,     1,     1,     4,     1,     1,     2,     2,     3,     0,
       2,     4,     3,     1,     2,     0,     4,     2,     2,     1,
       1,     1,     1,     2,     3,     3,     2,     4,     0,     1,
       2,     1,     3,     1,     3,     3,     3,     2,     1,     1,
       0,     2,     4,     1,     1,     0,     0,     3,     1,     1,
       1,     1,     1,     4,     0,     6,     0,     6,     2,     3,
       3,     0,     5,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       1,     5,     1,     3,     2,     3,     2,     1,     1,     1,
       1,     4,     1,     2,     3,     3,     3,     3,     2,     1,
       3,     0,     3,     0,     2,     3,     0,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     2,     2,     3,     4,     3,     2,
       2,     2,     2,     2,     3,     3,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     1,     3,     0,     4,
       3,     7,     2,     2,     6,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     0,     2,     2,     3,
       0,     2,     0,     4,     0,     2,     1,     3,     0,     0,
       7,     0,     0,     7,     3,     2,     2,     2,     1,     1,
       3,     2,     2,     3,     0,     0,     5,     1,     2,     5,
       5,     5,     6,     2,     1,     1,     1,     2,     3,     2,
       2,     3,     2,     3,     2,     2,     3,     4,     1,     1,
       0,     1,     1,     1,     0,     1,     3,     9,     8,     8,
       7,     3,     3,     0,     0,     7,     0,     0,     7,     0,
       0,     7,     0,     0,     6,     5,     8,    10,     5,     8,
      10,     1,     3,     1,     2,     3,     1,     1,     2,     2,
       2,     2,     2,     1,     3,     0,     4,     1,     6,     6,
       0,     7,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       8,     5,     6,     1,     4,     3,     0,     0,     8,     0,
       0,     9,     3,     4,     5,     6,     0,     0,     5,     3,
       4,     4,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     4,     4,     5,     4,     5,     3,
       4,     1,     1,     2,     4,     4,     7,     8,     3,     5,
       0,     0,     8,     3,     3,     3,     0,     0,     8,     3,
       4,     0,     0,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     2,     4,     1,     4,     4,
       4,     4,     4,     1,     6,     7,     6,     6,     7,     7,
       6,     7,     6,     6,     0,     4,     1,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     1,     5,     0,     0,
       4,     0,     9,     0,    10,     5,     3,     4,     1,     3,
       1,     3,     1,     3,     0,     2,     3,     3,     1,     3,
       0,     2,     3,     1,     1,     1,     2,     3,     5,     3,
       3,     1,     1,     1,     0,     1,     1,     4,     3,     3,
       5,     4,     6,     5,     5,     4,     0,     0,     4,     0,
       1,     0,     1,     1,     6,     0,     6,     0,     2,     3,
       5,     0,     1,     1,     0,     5,     2,     3,     4,     0,
       4,     0,     1,     1,     1,     0,     0,     9,     0,     0,
      11,     0,     2,     0,     1,     3,     1,     1,     0,     1,
       1,     0,     3,     0,     0,     7,     1,     4,     3,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     3,     0,     2,     3,     0,     0,     6,     1,     1,
       1,     3,     3,     4,     1,     1,     1,     1,     2,     3,
       0,     0,     6,     4,     5,     0,     9,     4,     2,     2,
       3,     2,     3,     2,     2,     3,     3,     3,     2,     0,
       0,     6,     2,     0,     0,     6,     0,     0,     6,     0,
       0,     6,     1,     0,     0,     6,     0,     0,     7,     1,
       0,     0,     6,     0,     0,     7,     1,     0,     0,     6,
       0,     0,     7,     0,     0,     6,     0,     0,     6,     1,
       3,     3,     0,     0,     0,     0,    10,     0,     0,     0,
       0,    10,     0,     0,     0,     0,    10,     1,     1,     1,
       1,     1,     3,     3,     5,     5,     6,     6,     8,     8,
       1,     1,     1,     3,     3,     5,     1,     2,     1,     0,
       0,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     2,     1,     1,     0,     1,     5,     4,     6,     7,
       5,     7,     0,     0,    10,     0,     0,    10,     0,     0,
       9,     0,     0,     7,     1,     3,     3,     3,     1,     5,
       0,     0,    10,     1,     3,     4,     4,     4,     0,     0,
      11,     0,     0,    11,     0,     0,    10,     5,     0,     0,
       9,     5,     0,     0,    10,     1,     3,     1,     3,     4,
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

  case 17: /* top_level_reader_macro: expr_reader "end of expression"  */
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
            yyextra->g_Program->thisModule->name = *(yyvsp[-2].s);
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

  case 29: /* string_builder_body: %empty  */
        {
        (yyval.pExpression) = new ExprStringBuilder();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 30: /* string_builder_body: string_builder_body character_sequence  */
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

  case 31: /* string_builder_body: string_builder_body "{" expr "}"  */
                                                                                                        {
        auto se = ExpressionPtr((yyvsp[-1].pExpression));
        static_cast<ExprStringBuilder *>((yyvsp[-3].pExpression))->elements.push_back(se);
        (yyval.pExpression) = (yyvsp[-3].pExpression);
    }
    break;

  case 32: /* string_builder: "start of the string" string_builder_body "end of the string"  */
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

  case 33: /* reader_character_sequence: STRING_CHARACTER  */
                               {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 34: /* reader_character_sequence: reader_character_sequence STRING_CHARACTER  */
                                                                {
        if ( !yyextra->g_ReaderMacro->accept(yyextra->g_Program.get(), yyextra->g_Program->thisModule.get(), yyextra->g_ReaderExpr, (yyvsp[0].ch), tokAt(scanner,(yylsp[0]))) ) {
            das_yyend_reader(scanner);
        }
    }
    break;

  case 35: /* $@1: %empty  */
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

  case 36: /* expr_reader: '%' name_in_namespace $@1 reader_character_sequence  */
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

  case 37: /* options_declaration: "options" annotation_argument_list  */
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

  case 39: /* keyword_or_name: "name"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 40: /* keyword_or_name: "keyword"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 41: /* keyword_or_name: "type function"  */
                            { (yyval.s) = (yyvsp[0].s); }
    break;

  case 42: /* require_module_name: keyword_or_name  */
                              {
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 43: /* require_module_name: '%' require_module_name  */
                                     {
        *(yyvsp[0].s) = "%" + *(yyvsp[0].s);
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 44: /* require_module_name: require_module_name '.' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += ".";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 45: /* require_module_name: require_module_name '/' keyword_or_name  */
                                                           {
        *(yyvsp[-2].s) += "/";
        *(yyvsp[-2].s) += *(yyvsp[0].s);
        delete (yyvsp[0].s);
        (yyval.s) = (yyvsp[-2].s);
    }
    break;

  case 46: /* require_module: require_module_name is_public_module  */
                                                         {
        ast_requireModule(scanner,(yyvsp[-1].s),nullptr,(yyvsp[0].b),tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 47: /* require_module: require_module_name "as" "name" is_public_module  */
                                                                              {
        ast_requireModule(scanner,(yyvsp[-3].s),(yyvsp[-1].s),(yyvsp[0].b),tokAt(scanner,(yylsp[-3])));
    }
    break;

  case 48: /* is_public_module: %empty  */
                    { (yyval.b) = false; }
    break;

  case 49: /* is_public_module: "public"  */
                    { (yyval.b) = true; }
    break;

  case 53: /* expect_error: "integer constant"  */
                   {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[0].i))] ++;
    }
    break;

  case 54: /* expect_error: "integer constant" ':' "integer constant"  */
                                      {
        yyextra->g_Program->expectErrors[CompilationError((yyvsp[-2].i))] += (yyvsp[0].i);
    }
    break;

  case 55: /* expression_label: "label" "integer constant" ':'  */
                                          {
        (yyval.pExpression) = new ExprLabel(tokAt(scanner,(yylsp[-2])),(yyvsp[-1].i));
    }
    break;

  case 56: /* expression_goto: "goto" "label" "integer constant"  */
                                                {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-2])),(yyvsp[0].i));
    }
    break;

  case 57: /* expression_goto: "goto" expr  */
                               {
        (yyval.pExpression) = new ExprGoto(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 58: /* elif_or_static_elif: "elif"  */
                          { (yyval.b) = false; }
    break;

  case 59: /* elif_or_static_elif: "static_elif"  */
                          { (yyval.b) = true; }
    break;

  case 60: /* expression_else: %empty  */
                                                           { (yyval.pExpression) = nullptr; }
    break;

  case 61: /* expression_else: "else" expression_block  */
                                                           { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 62: /* expression_else: elif_or_static_elif expr expression_block expression_else  */
                                                                                          {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 63: /* if_or_static_if: "if"  */
                        { (yyval.b) = false; }
    break;

  case 64: /* if_or_static_if: "static_if"  */
                        { (yyval.b) = true; }
    break;

  case 65: /* expression_else_one_liner: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 66: /* $@2: %empty  */
                      { yyextra->das_need_oxford_comma = true; }
    break;

  case 67: /* expression_else_one_liner: "else" $@2 expression_if_one_liner  */
                                                                                                 {
            (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 68: /* expression_if_one_liner: expr  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 69: /* expression_if_one_liner: expression_return_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 70: /* expression_if_one_liner: expression_yield_no_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 71: /* expression_if_one_liner: expression_break  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 72: /* expression_if_one_liner: expression_continue  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 73: /* expression_if_then_else: if_or_static_if expr expression_block expression_else  */
                                                                                      {
        auto eite = new ExprIfThenElse(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),
            ExpressionPtr((yyvsp[-1].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        eite->isStatic = (yyvsp[-3].b);
        (yyval.pExpression) = eite;
    }
    break;

  case 74: /* $@3: %empty  */
                                                     { yyextra->das_need_oxford_comma = true; }
    break;

  case 75: /* expression_if_then_else: expression_if_one_liner "if" $@3 expr expression_else_one_liner "end of expression"  */
                                                                                                                                                   {
        (yyval.pExpression) = new ExprIfThenElse(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr(ast_wrapInBlock((yyvsp[-5].pExpression))),(yyvsp[-1].pExpression) ? ExpressionPtr(ast_wrapInBlock((yyvsp[-1].pExpression))) : nullptr);
    }
    break;

  case 76: /* $@4: %empty  */
                     { yyextra->das_need_oxford_comma=false; }
    break;

  case 77: /* expression_for_loop: "for" $@4 variable_name_with_pos_list "in" expr_list expression_block  */
                                                                                                                                                 {
        (yyval.pExpression) = ast_forLoop(scanner,(yyvsp[-3].pNameWithPosList),(yyvsp[-1].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 78: /* expression_unsafe: "unsafe" expression_block  */
                                                 {
        auto pUnsafe = new ExprUnsafe(tokAt(scanner,(yylsp[-1])));
        pUnsafe->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pUnsafe;
    }
    break;

  case 79: /* expression_while_loop: "while" expr expression_block  */
                                                               {
        auto pWhile = new ExprWhile(tokAt(scanner,(yylsp[-2])));
        pWhile->cond = ExpressionPtr((yyvsp[-1].pExpression));
        pWhile->body = ExpressionPtr((yyvsp[0].pExpression));
        ((ExprBlock *)(yyvsp[0].pExpression))->inTheLoop = true;
        (yyval.pExpression) = pWhile;
    }
    break;

  case 80: /* expression_with: "with" expr expression_block  */
                                                         {
        auto pWith = new ExprWith(tokAt(scanner,(yylsp[-2])));
        pWith->with = ExpressionPtr((yyvsp[-1].pExpression));
        pWith->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyval.pExpression) = pWith;
    }
    break;

  case 81: /* $@5: %empty  */
                                        { yyextra->das_need_oxford_comma=true; }
    break;

  case 82: /* expression_with_alias: "assume" "name" '=' $@5 expr  */
                                                                                               {
        (yyval.pExpression) = new ExprAssume(tokAt(scanner,(yylsp[-4])), *(yyvsp[-3].s), (yyvsp[0].pExpression) );
        delete (yyvsp[-3].s);
    }
    break;

  case 83: /* annotation_argument_value: string_constant  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 84: /* annotation_argument_value: "name"  */
                                 { (yyval.aa) = new AnnotationArgument("",*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 85: /* annotation_argument_value: "integer constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",(yyvsp[0].i)); }
    break;

  case 86: /* annotation_argument_value: "floating point constant"  */
                                 { (yyval.aa) = new AnnotationArgument("",float((yyvsp[0].fd))); }
    break;

  case 87: /* annotation_argument_value: "true"  */
                                 { (yyval.aa) = new AnnotationArgument("",true); }
    break;

  case 88: /* annotation_argument_value: "false"  */
                                 { (yyval.aa) = new AnnotationArgument("",false); }
    break;

  case 89: /* annotation_argument_value_list: annotation_argument_value  */
                                       {
        (yyval.aaList) = new AnnotationArgumentList();
        (yyval.aaList)->push_back(*(yyvsp[0].aa));
        delete (yyvsp[0].aa);
    }
    break;

  case 90: /* annotation_argument_value_list: annotation_argument_value_list ',' annotation_argument_value  */
                                                                                {
            (yyval.aaList) = (yyvsp[-2].aaList);
            (yyval.aaList)->push_back(*(yyvsp[0].aa));
            delete (yyvsp[0].aa);
    }
    break;

  case 91: /* annotation_argument_name: "name"  */
                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 92: /* annotation_argument_name: "type"  */
                    { (yyval.s) = new string("type"); }
    break;

  case 93: /* annotation_argument_name: "in"  */
                    { (yyval.s) = new string("in"); }
    break;

  case 94: /* annotation_argument: annotation_argument_name '=' string_constant  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 95: /* annotation_argument: annotation_argument_name '=' "name"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[0].s); delete (yyvsp[-2].s); }
    break;

  case 96: /* annotation_argument: annotation_argument_name '=' "integer constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),(yyvsp[0].i),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 97: /* annotation_argument: annotation_argument_name '=' "floating point constant"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),float((yyvsp[0].fd)),tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 98: /* annotation_argument: annotation_argument_name '=' "true"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),true,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 99: /* annotation_argument: annotation_argument_name '=' "false"  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[-2].s),false,tokAt(scanner,(yylsp[-2]))); delete (yyvsp[-2].s); }
    break;

  case 100: /* annotation_argument: annotation_argument_name  */
                                                                    { (yyval.aa) = new AnnotationArgument(*(yyvsp[0].s),true,tokAt(scanner,(yylsp[0]))); delete (yyvsp[0].s); }
    break;

  case 101: /* annotation_argument: annotation_argument_name '=' '(' annotation_argument_value_list ')'  */
                                                                                          {
        { (yyval.aa) = new AnnotationArgument(*(yyvsp[-4].s),(yyvsp[-1].aaList),tokAt(scanner,(yylsp[-4]))); delete (yyvsp[-4].s); }
    }
    break;

  case 102: /* annotation_argument_list: annotation_argument  */
                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 103: /* annotation_argument_list: annotation_argument_list ',' annotation_argument  */
                                                                    {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 104: /* metadata_argument_list: '@' annotation_argument  */
                                      {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,new AnnotationArgumentList(),(yyvsp[0].aa));
    }
    break;

  case 105: /* metadata_argument_list: metadata_argument_list '@' annotation_argument  */
                                                                  {
        (yyval.aaList) = ast_annotationArgumentListEntry(scanner,(yyvsp[-2].aaList),(yyvsp[0].aa));
    }
    break;

  case 106: /* metadata_argument_list: metadata_argument_list "end of expression"  */
                                         {
        (yyval.aaList) = (yyvsp[-1].aaList);
    }
    break;

  case 107: /* annotation_declaration_name: name_in_namespace  */
                                    { (yyval.s) = (yyvsp[0].s); }
    break;

  case 108: /* annotation_declaration_name: "require"  */
                                    { (yyval.s) = new string("require"); }
    break;

  case 109: /* annotation_declaration_name: "private"  */
                                    { (yyval.s) = new string("private"); }
    break;

  case 110: /* annotation_declaration_basic: annotation_declaration_name  */
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

  case 111: /* annotation_declaration_basic: annotation_declaration_name '(' annotation_argument_list ')'  */
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

  case 112: /* annotation_declaration: annotation_declaration_basic  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
    }
    break;

  case 113: /* annotation_declaration: '!' annotation_declaration  */
                                              {
        if ( !(yyvsp[0].fa)->annotation->rtti_isFunctionAnnotation() || !((FunctionAnnotation *)((yyvsp[0].fa)->annotation.get()))->isSpecialized() ) {
            das_yyerror(scanner,"can only run logical operations on contracts", tokAt(scanner, (yylsp[0])),
                CompilationError::invalid_annotation); }
        (yyval.fa) = new AnnotationDeclaration();
        (yyval.fa)->at = tokAt(scanner, (yylsp[-1]));
        (yyval.fa)->annotation = newLogicAnnotation(LogicAnnotationOp::Not,(yyvsp[0].fa),nullptr);
    }
    break;

  case 114: /* annotation_declaration: annotation_declaration "&&" annotation_declaration  */
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

  case 115: /* annotation_declaration: annotation_declaration "||" annotation_declaration  */
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

  case 116: /* annotation_declaration: annotation_declaration "^^" annotation_declaration  */
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

  case 117: /* annotation_declaration: '(' annotation_declaration ')'  */
                                            {
        (yyval.fa) = (yyvsp[-1].fa);
    }
    break;

  case 118: /* annotation_declaration: "|>" annotation_declaration  */
                                          {
        (yyval.fa) = (yyvsp[0].fa);
        (yyvsp[0].fa)->inherited = true;
    }
    break;

  case 119: /* annotation_list: annotation_declaration  */
                                    {
            (yyval.faList) = new AnnotationList();
            (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 120: /* annotation_list: annotation_list ',' annotation_declaration  */
                                                              {
        (yyval.faList) = (yyvsp[-2].faList);
        (yyval.faList)->push_back(AnnotationDeclarationPtr((yyvsp[0].fa)));
    }
    break;

  case 121: /* optional_annotation_list: %empty  */
                                        { (yyval.faList) = nullptr; }
    break;

  case 122: /* optional_annotation_list: '[' annotation_list ']'  */
                                        { (yyval.faList) = (yyvsp[-1].faList); }
    break;

  case 123: /* optional_function_argument_list: %empty  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 124: /* optional_function_argument_list: '(' ')'  */
                                                { (yyval.pVarDeclList) = nullptr; }
    break;

  case 125: /* optional_function_argument_list: '(' function_argument_list ')'  */
                                                { (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList); }
    break;

  case 126: /* optional_function_type: %empty  */
        {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
    }
    break;

  case 127: /* optional_function_type: ':' type_declaration  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 128: /* function_name: "name"  */
                          {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyval.s) = (yyvsp[0].s);
    }
    break;

  case 129: /* function_name: "operator" '!'  */
                             { (yyval.s) = new string("!"); }
    break;

  case 130: /* function_name: "operator" '~'  */
                             { (yyval.s) = new string("~"); }
    break;

  case 131: /* function_name: "operator" "+="  */
                             { (yyval.s) = new string("+="); }
    break;

  case 132: /* function_name: "operator" "-="  */
                             { (yyval.s) = new string("-="); }
    break;

  case 133: /* function_name: "operator" "*="  */
                             { (yyval.s) = new string("*="); }
    break;

  case 134: /* function_name: "operator" "/="  */
                             { (yyval.s) = new string("/="); }
    break;

  case 135: /* function_name: "operator" "%="  */
                             { (yyval.s) = new string("%="); }
    break;

  case 136: /* function_name: "operator" "&="  */
                             { (yyval.s) = new string("&="); }
    break;

  case 137: /* function_name: "operator" "|="  */
                             { (yyval.s) = new string("|="); }
    break;

  case 138: /* function_name: "operator" "^="  */
                             { (yyval.s) = new string("^="); }
    break;

  case 139: /* function_name: "operator" "&&="  */
                                { (yyval.s) = new string("&&="); }
    break;

  case 140: /* function_name: "operator" "||="  */
                                { (yyval.s) = new string("||="); }
    break;

  case 141: /* function_name: "operator" "^^="  */
                                { (yyval.s) = new string("^^="); }
    break;

  case 142: /* function_name: "operator" "&&"  */
                             { (yyval.s) = new string("&&"); }
    break;

  case 143: /* function_name: "operator" "||"  */
                             { (yyval.s) = new string("||"); }
    break;

  case 144: /* function_name: "operator" "^^"  */
                             { (yyval.s) = new string("^^"); }
    break;

  case 145: /* function_name: "operator" '+'  */
                             { (yyval.s) = new string("+"); }
    break;

  case 146: /* function_name: "operator" '-'  */
                             { (yyval.s) = new string("-"); }
    break;

  case 147: /* function_name: "operator" '*'  */
                             { (yyval.s) = new string("*"); }
    break;

  case 148: /* function_name: "operator" '/'  */
                             { (yyval.s) = new string("/"); }
    break;

  case 149: /* function_name: "operator" '%'  */
                             { (yyval.s) = new string("%"); }
    break;

  case 150: /* function_name: "operator" '<'  */
                             { (yyval.s) = new string("<"); }
    break;

  case 151: /* function_name: "operator" '>'  */
                             { (yyval.s) = new string(">"); }
    break;

  case 152: /* function_name: "operator" ".."  */
                             { (yyval.s) = new string("interval"); }
    break;

  case 153: /* function_name: "operator" "=="  */
                             { (yyval.s) = new string("=="); }
    break;

  case 154: /* function_name: "operator" "!="  */
                             { (yyval.s) = new string("!="); }
    break;

  case 155: /* function_name: "operator" "<="  */
                             { (yyval.s) = new string("<="); }
    break;

  case 156: /* function_name: "operator" ">="  */
                             { (yyval.s) = new string(">="); }
    break;

  case 157: /* function_name: "operator" '&'  */
                             { (yyval.s) = new string("&"); }
    break;

  case 158: /* function_name: "operator" '|'  */
                             { (yyval.s) = new string("|"); }
    break;

  case 159: /* function_name: "operator" '^'  */
                             { (yyval.s) = new string("^"); }
    break;

  case 160: /* function_name: "++" "operator"  */
                             { (yyval.s) = new string("++"); }
    break;

  case 161: /* function_name: "--" "operator"  */
                             { (yyval.s) = new string("--"); }
    break;

  case 162: /* function_name: "operator" "++"  */
                             { (yyval.s) = new string("+++"); }
    break;

  case 163: /* function_name: "operator" "--"  */
                             { (yyval.s) = new string("---"); }
    break;

  case 164: /* function_name: "operator" "<<"  */
                             { (yyval.s) = new string("<<"); }
    break;

  case 165: /* function_name: "operator" ">>"  */
                             { (yyval.s) = new string(">>"); }
    break;

  case 166: /* function_name: "operator" "<<="  */
                             { (yyval.s) = new string("<<="); }
    break;

  case 167: /* function_name: "operator" ">>="  */
                             { (yyval.s) = new string(">>="); }
    break;

  case 168: /* function_name: "operator" "<<<"  */
                             { (yyval.s) = new string("<<<"); }
    break;

  case 169: /* function_name: "operator" ">>>"  */
                             { (yyval.s) = new string(">>>"); }
    break;

  case 170: /* function_name: "operator" "<<<="  */
                             { (yyval.s) = new string("<<<="); }
    break;

  case 171: /* function_name: "operator" ">>>="  */
                             { (yyval.s) = new string(">>>="); }
    break;

  case 172: /* function_name: "operator" '[' ']'  */
                             { (yyval.s) = new string("[]"); }
    break;

  case 173: /* function_name: "operator" "?[" ']'  */
                                { (yyval.s) = new string("?[]"); }
    break;

  case 174: /* function_name: "operator" '.'  */
                             { (yyval.s) = new string("."); }
    break;

  case 175: /* function_name: "operator" "?."  */
                             { (yyval.s) = new string("?."); }
    break;

  case 176: /* function_name: "operator" '.' "name"  */
                                       { (yyval.s) = new string(".`"+*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 177: /* function_name: "operator" '.' "name" ":="  */
                                             { (yyval.s) = new string(".`"+*(yyvsp[-1].s)+"`clone"); delete (yyvsp[-1].s); }
    break;

  case 178: /* function_name: "operator" "?." "name"  */
                                       { (yyval.s) = new string("?.`"+*(yyvsp[0].s)); delete (yyvsp[0].s);}
    break;

  case 179: /* function_name: "operator" ":="  */
                                { (yyval.s) = new string("clone"); }
    break;

  case 180: /* function_name: "operator" "delete"  */
                                { (yyval.s) = new string("finalize"); }
    break;

  case 181: /* function_name: "operator" "??"  */
                           { (yyval.s) = new string("??"); }
    break;

  case 182: /* function_name: "operator" "is"  */
                            { (yyval.s) = new string("`is"); }
    break;

  case 183: /* function_name: "operator" "as"  */
                            { (yyval.s) = new string("`as"); }
    break;

  case 184: /* function_name: "operator" "is" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`is`" + *(yyvsp[0].s); }
    break;

  case 185: /* function_name: "operator" "as" "name"  */
                                       { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "`as`" + *(yyvsp[0].s); }
    break;

  case 186: /* function_name: "operator" '?' "as"  */
                                { (yyval.s) = new string("?as"); }
    break;

  case 187: /* function_name: "operator" '?' "as" "name"  */
                                           { (yyval.s) = (yyvsp[0].s); *(yyvsp[0].s) = "?as`" + *(yyvsp[0].s); }
    break;

  case 188: /* function_name: "bool"  */
                     { (yyval.s) = new string("bool"); }
    break;

  case 189: /* function_name: "string"  */
                     { (yyval.s) = new string("string"); }
    break;

  case 190: /* function_name: "int"  */
                     { (yyval.s) = new string("int"); }
    break;

  case 191: /* function_name: "int2"  */
                     { (yyval.s) = new string("int2"); }
    break;

  case 192: /* function_name: "int3"  */
                     { (yyval.s) = new string("int3"); }
    break;

  case 193: /* function_name: "int4"  */
                     { (yyval.s) = new string("int4"); }
    break;

  case 194: /* function_name: "uint"  */
                     { (yyval.s) = new string("uint"); }
    break;

  case 195: /* function_name: "uint2"  */
                     { (yyval.s) = new string("uint2"); }
    break;

  case 196: /* function_name: "uint3"  */
                     { (yyval.s) = new string("uint3"); }
    break;

  case 197: /* function_name: "uint4"  */
                     { (yyval.s) = new string("uint4"); }
    break;

  case 198: /* function_name: "float"  */
                     { (yyval.s) = new string("float"); }
    break;

  case 199: /* function_name: "float2"  */
                     { (yyval.s) = new string("float2"); }
    break;

  case 200: /* function_name: "float3"  */
                     { (yyval.s) = new string("float3"); }
    break;

  case 201: /* function_name: "float4"  */
                     { (yyval.s) = new string("float4"); }
    break;

  case 202: /* function_name: "range"  */
                     { (yyval.s) = new string("range"); }
    break;

  case 203: /* function_name: "urange"  */
                     { (yyval.s) = new string("urange"); }
    break;

  case 204: /* function_name: "range64"  */
                     { (yyval.s) = new string("range64"); }
    break;

  case 205: /* function_name: "urange64"  */
                     { (yyval.s) = new string("urange64"); }
    break;

  case 206: /* function_name: "int64"  */
                     { (yyval.s) = new string("int64"); }
    break;

  case 207: /* function_name: "uint64"  */
                     { (yyval.s) = new string("uint64"); }
    break;

  case 208: /* function_name: "double"  */
                     { (yyval.s) = new string("double"); }
    break;

  case 209: /* function_name: "int8"  */
                     { (yyval.s) = new string("int8"); }
    break;

  case 210: /* function_name: "uint8"  */
                     { (yyval.s) = new string("uint8"); }
    break;

  case 211: /* function_name: "int16"  */
                     { (yyval.s) = new string("int16"); }
    break;

  case 212: /* function_name: "uint16"  */
                     { (yyval.s) = new string("uint16"); }
    break;

  case 213: /* global_function_declaration: optional_annotation_list "def" function_declaration  */
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

  case 214: /* optional_public_or_private_function: %empty  */
                        { (yyval.b) = yyextra->g_thisStructure ? !yyextra->g_thisStructure->privateStructure : yyextra->g_Program->thisModule->isPublic; }
    break;

  case 215: /* optional_public_or_private_function: "private"  */
                        { (yyval.b) = false; }
    break;

  case 216: /* optional_public_or_private_function: "public"  */
                        { (yyval.b) = true; }
    break;

  case 217: /* function_declaration_header: function_name optional_function_argument_list optional_function_type  */
                                                                                                {
        (yyval.pFuncDecl) = ast_functionDeclarationHeader(scanner,(yyvsp[-2].s),(yyvsp[-1].pVarDeclList),(yyvsp[0].pTypeDecl),tokAt(scanner,(yylsp[-2])));
    }
    break;

  case 218: /* $@6: %empty  */
                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
        }
    }
    break;

  case 219: /* function_declaration: optional_public_or_private_function $@6 function_declaration_header expression_block  */
                                                                {
        (yyvsp[-1].pFuncDecl)->body = ExpressionPtr((yyvsp[0].pExpression));
        (yyvsp[-1].pFuncDecl)->privateFunction = !(yyvsp[-3].b);
        (yyval.pFuncDecl) = (yyvsp[-1].pFuncDecl);
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
        }
    }
    break;

  case 220: /* expression_block: "begin of code block" expressions "end of code block"  */
                                                   {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-2]),(yylsp[0]));
    }
    break;

  case 221: /* expression_block: "begin of code block" expressions "end of code block" "finally" "begin of code block" expressions "end of code block"  */
                                                                                          {
        auto pB = (ExprBlock *) (yyvsp[-5].pExpression);
        auto pF = (ExprBlock *) (yyvsp[-1].pExpression);
        swap ( pB->finalList, pF->list );
        (yyval.pExpression) = (yyvsp[-5].pExpression);
        (yyval.pExpression)->at = tokRangeAt(scanner,(yylsp[-6]),(yylsp[0]));
        delete (yyvsp[-1].pExpression);
    }
    break;

  case 222: /* expr_call_pipe: expr expr_full_block_assumed_piped  */
                                                      {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 223: /* expr_call_pipe: expression_keyword expr_full_block_assumed_piped  */
                                                                    {
        if ( (yyvsp[-1].pExpression)->rtti_isCallLikeExpr() ) {
            ((ExprLooksLikeCall *)(yyvsp[-1].pExpression))->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-1].pExpression);
        } else {
            (yyval.pExpression) = (yyvsp[-1].pExpression);
            delete (yyvsp[0].pExpression);
        }
    }
    break;

  case 224: /* expr_call_pipe: "generator" '<' type_declaration_no_options '>' optional_capture_list expr_full_block_assumed_piped  */
                                                                                                                                             {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-3].pTypeDecl),(yyvsp[-1].pCaptList),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-5])));
    }
    break;

  case 225: /* expression_any: "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 226: /* expression_any: expr_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 227: /* expression_any: expr_keyword  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 228: /* expression_any: expr_assign_pipe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 229: /* expression_any: expr_assign "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 230: /* expression_any: expression_delete "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 231: /* expression_any: expression_let  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 232: /* expression_any: expression_while_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 233: /* expression_any: expression_unsafe  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 234: /* expression_any: expression_with  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 235: /* expression_any: expression_with_alias  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 236: /* expression_any: expression_for_loop  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 237: /* expression_any: expression_break "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 238: /* expression_any: expression_continue "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 239: /* expression_any: expression_return  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 240: /* expression_any: expression_yield  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 241: /* expression_any: expression_if_then_else  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 242: /* expression_any: expression_try_catch  */
                                            { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 243: /* expression_any: expression_label "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 244: /* expression_any: expression_goto "end of expression"  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 245: /* expression_any: "pass" "end of expression"  */
                                            { (yyval.pExpression) = nullptr; }
    break;

  case 246: /* expressions: %empty  */
        {
        (yyval.pExpression) = new ExprBlock();
        (yyval.pExpression)->at = LineInfo(yyextra->g_FileAccessStack.back(),
            yylloc.first_column,yylloc.first_line,yylloc.last_column,yylloc.last_line);
    }
    break;

  case 247: /* expressions: expressions expression_any  */
                                                        {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
        if ( (yyvsp[0].pExpression) ) {
            static_cast<ExprBlock*>((yyvsp[-1].pExpression))->list.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        }
    }
    break;

  case 248: /* expressions: expressions error  */
                                 {
        delete (yyvsp[-1].pExpression); (yyval.pExpression) = nullptr; YYABORT;
    }
    break;

  case 249: /* expr_keyword: "keyword" expr expression_block  */
                                                           {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s));
        pCall->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        auto resT = new TypeDecl(Type::autoinfer);
        auto blk = ast_makeBlock(scanner,0,nullptr,nullptr,nullptr,resT,(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])));
        pCall->arguments.push_back(ExpressionPtr(blk));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 250: /* optional_expr_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 251: /* optional_expr_list: expr_list optional_comma  */
                                            { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 252: /* optional_expr_list_in_braces: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 253: /* optional_expr_list_in_braces: '(' optional_expr_list optional_comma ')'  */
                                                             { (yyval.pExpression) = (yyvsp[-2].pExpression); }
    break;

  case 254: /* optional_expr_map_tuple_list: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 255: /* optional_expr_map_tuple_list: expr_map_tuple_list optional_comma  */
                                                      { (yyval.pExpression) = (yyvsp[-1].pExpression); }
    break;

  case 256: /* type_declaration_no_options_list: type_declaration  */
                               {
        (yyval.pTypeDeclList) = new vector<Expression *>();
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 257: /* type_declaration_no_options_list: type_declaration_no_options_list c_or_s type_declaration  */
                                                                              {
        (yyval.pTypeDeclList) = (yyvsp[-2].pTypeDeclList);
        (yyval.pTypeDeclList)->push_back(new ExprTypeDecl(tokAt(scanner,(yylsp[0])),(yyvsp[0].pTypeDecl)));
    }
    break;

  case 258: /* $@7: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 259: /* $@8: %empty  */
                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 260: /* expression_keyword: "keyword" '<' $@7 type_declaration_no_options_list '>' $@8 expr  */
                                                                                                                                                     {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 261: /* $@9: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 262: /* $@10: %empty  */
                                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 263: /* expression_keyword: "type function" '<' $@9 type_declaration_no_options_list '>' $@10 optional_expr_list_in_braces  */
                                                                                                                                                                                   {
        auto pCall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),*(yyvsp[-6].s));
        pCall->arguments = typesAndSequenceToList((yyvsp[-3].pTypeDeclList),(yyvsp[0].pExpression));
        delete (yyvsp[-6].s);
        (yyval.pExpression) = pCall;
    }
    break;

  case 264: /* expr_pipe: expr_assign " <|" expr_block  */
                                                        {
        Expression * pipeCall = (yyvsp[-2].pExpression)->tail();
        if ( pipeCall->rtti_isCallLikeExpr() ) {
            auto pCall = (ExprLooksLikeCall *) pipeCall;
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
            (yyval.pExpression) = (yyvsp[-2].pExpression);
        } else if ( pipeCall->rtti_isVar() ) {
            // a += b <| c
            auto pVar = (ExprVar *) pipeCall;
            auto pCall = yyextra->g_Program->makeCall(pVar->at,pVar->name);
            pCall->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
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

  case 265: /* expr_pipe: "@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 266: /* expr_pipe: "@@ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 267: /* expr_pipe: "$ <|" expr_block  */
                               {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 268: /* expr_pipe: expr_call_pipe  */
                             {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 269: /* name_in_namespace: "name"  */
                                               { (yyval.s) = (yyvsp[0].s); }
    break;

  case 270: /* name_in_namespace: "name" "::" "name"  */
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

  case 271: /* name_in_namespace: "::" "name"  */
                                               { *(yyvsp[0].s) = "::" + *(yyvsp[0].s); (yyval.s) = (yyvsp[0].s); }
    break;

  case 272: /* expression_delete: "delete" expr  */
                                      {
        (yyval.pExpression) = new ExprDelete(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 273: /* expression_delete: "delete" "explicit" expr  */
                                                   {
        auto delExpr = new ExprDelete(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
        delExpr->native = true;
        (yyval.pExpression) = delExpr;
    }
    break;

  case 274: /* $@11: %empty  */
           { yyextra->das_arrow_depth ++; }
    break;

  case 275: /* $@12: %empty  */
                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 276: /* new_type_declaration: '<' $@11 type_declaration '>' $@12  */
                                                                                                            {
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 277: /* new_type_declaration: structure_type_declaration  */
                                               {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 278: /* expr_new: "new" new_type_declaration  */
                                                       {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-1])),TypeDeclPtr((yyvsp[0].pTypeDecl)),false);
    }
    break;

  case 279: /* expr_new: "new" new_type_declaration '(' use_initializer ')'  */
                                                                                     {
        (yyval.pExpression) = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        ((ExprNew *)(yyval.pExpression))->initializer = (yyvsp[-1].b);
    }
    break;

  case 280: /* expr_new: "new" new_type_declaration '(' expr_list ')'  */
                                                                                    {
        auto pNew = new ExprNew(tokAt(scanner,(yylsp[-4])),TypeDeclPtr((yyvsp[-3].pTypeDecl)),true);
        (yyval.pExpression) = parseFunctionArguments(pNew,(yyvsp[-1].pExpression));
    }
    break;

  case 281: /* expr_new: "new" new_type_declaration '(' make_struct_single ')'  */
                                                                                      {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-3]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-3].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 282: /* expr_new: "new" new_type_declaration '(' "uninitialized" make_struct_single ')'  */
                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->at = tokAt(scanner,(yylsp[-4]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = (yyvsp[-4].pTypeDecl);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = false; // $init;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-5])),ExpressionPtr((yyvsp[-1].pExpression)));
    }
    break;

  case 283: /* expr_new: "new" make_decl  */
                                    {
        (yyval.pExpression) = new ExprAscend(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 284: /* expression_break: "break"  */
                       { (yyval.pExpression) = new ExprBreak(tokAt(scanner,(yylsp[0]))); }
    break;

  case 285: /* expression_continue: "continue"  */
                          { (yyval.pExpression) = new ExprContinue(tokAt(scanner,(yylsp[0]))); }
    break;

  case 286: /* expression_return_no_pipe: "return"  */
                        {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[0])),nullptr);
    }
    break;

  case 287: /* expression_return_no_pipe: "return" expr_list  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),sequenceToTuple((yyvsp[0].pExpression)));
    }
    break;

  case 288: /* expression_return_no_pipe: "return" "<-" expr_list  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),sequenceToTuple((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 289: /* expression_return: expression_return_no_pipe "end of expression"  */
                                              {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 290: /* expression_return: "return" expr_pipe  */
                                           {
        (yyval.pExpression) = new ExprReturn(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 291: /* expression_return: "return" "<-" expr_pipe  */
                                                  {
        auto pRet = new ExprReturn(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 292: /* expression_yield_no_pipe: "yield" expr  */
                                     {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 293: /* expression_yield_no_pipe: "yield" "<-" expr  */
                                            {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 294: /* expression_yield: expression_yield_no_pipe "end of expression"  */
                                             {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 295: /* expression_yield: "yield" expr_pipe  */
                                          {
        (yyval.pExpression) = new ExprYield(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 296: /* expression_yield: "yield" "<-" expr_pipe  */
                                                 {
        auto pRet = new ExprYield(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[0].pExpression)));
        pRet->moveSemantics = true;
        (yyval.pExpression) = pRet;
    }
    break;

  case 297: /* expression_try_catch: "try" expression_block "recover" expression_block  */
                                                                                       {
        (yyval.pExpression) = new ExprTryCatch(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 298: /* kwd_let_var_or_nothing: "let"  */
                 { (yyval.b) = true; }
    break;

  case 299: /* kwd_let_var_or_nothing: "var"  */
                 { (yyval.b) = false; }
    break;

  case 300: /* kwd_let_var_or_nothing: %empty  */
                    { (yyval.b) = true; }
    break;

  case 301: /* kwd_let: "let"  */
                 { (yyval.b) = true; }
    break;

  case 302: /* kwd_let: "var"  */
                 { (yyval.b) = false; }
    break;

  case 303: /* optional_in_scope: "inscope"  */
                    { (yyval.b) = true; }
    break;

  case 304: /* optional_in_scope: %empty  */
                     { (yyval.b) = false; }
    break;

  case 305: /* tuple_expansion: "name"  */
                    {
        (yyval.pNameList) = new vector<string>();
        (yyval.pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 306: /* tuple_expansion: tuple_expansion ',' "name"  */
                                             {
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        delete (yyvsp[0].s);
        (yyval.pNameList) = (yyvsp[-2].pNameList);
    }
    break;

  case 307: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-7].pNameList),tokAt(scanner,(yylsp[-7])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 308: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-6].pNameList),tokAt(scanner,(yylsp[-6])),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
        (yyval.pVarDecl)->isTupleExpansion = true;
    }
    break;

  case 309: /* tuple_expansion_variable_declaration: "[[" tuple_expansion ']' ']' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 310: /* tuple_expansion_variable_declaration: '(' tuple_expansion ')' optional_ref copy_or_move_or_clone expr "end of expression"  */
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

  case 311: /* expression_let: kwd_let optional_in_scope let_variable_declaration  */
                                                                 {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 312: /* expression_let: kwd_let optional_in_scope tuple_expansion_variable_declaration  */
                                                                             {
        (yyval.pExpression) = ast_Let(scanner,(yyvsp[-2].b),(yyvsp[-1].b),(yyvsp[0].pVarDecl),tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 313: /* $@13: %empty  */
                          { yyextra->das_arrow_depth ++; }
    break;

  case 314: /* $@14: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 315: /* expr_cast: "cast" '<' $@13 type_declaration_no_options '>' $@14 expr  */
                                                                                                                                                {
        (yyval.pExpression) = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
    }
    break;

  case 316: /* $@15: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 317: /* $@16: %empty  */
                                                                                                   { yyextra->das_arrow_depth --; }
    break;

  case 318: /* expr_cast: "upcast" '<' $@15 type_declaration_no_options '>' $@16 expr  */
                                                                                                                                                  {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->upcast = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 319: /* $@17: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 320: /* $@18: %empty  */
                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 321: /* expr_cast: "reinterpret" '<' $@17 type_declaration_no_options '>' $@18 expr  */
                                                                                                                                                       {
        auto pCast = new ExprCast(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[0].pExpression)),TypeDeclPtr((yyvsp[-3].pTypeDecl)));
        pCast->reinterpret = true;
        (yyval.pExpression) = pCast;
    }
    break;

  case 322: /* $@19: %empty  */
                         { yyextra->das_arrow_depth ++; }
    break;

  case 323: /* $@20: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 324: /* expr_type_decl: "type" '<' $@19 type_declaration '>' $@20  */
                                                                                                                      {
        (yyval.pExpression) = new ExprTypeDecl(tokAt(scanner,(yylsp[-5])),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 325: /* expr_type_info: "typeinfo" '(' name_in_namespace expr ')'  */
                                                                         {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[-1].pExpression)));
            }
            delete (yyvsp[-2].s);
    }
    break;

  case 326: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" '>' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ptd->typeexpr,*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-5].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-3].s));
            }
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 327: /* expr_type_info: "typeinfo" '(' name_in_namespace '<' "name" c_or_s "name" '>' expr ')'  */
                                                                                                                        {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ptd->typeexpr,*(yyvsp[-5].s),*(yyvsp[-3].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-7].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-5].s),*(yyvsp[-3].s));
            }
            delete (yyvsp[-7].s);
            delete (yyvsp[-5].s);
            delete (yyvsp[-3].s);
    }
    break;

  case 328: /* expr_type_info: "typeinfo" name_in_namespace '(' expr ')'  */
                                                                          {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ptd->typeexpr);
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-4])),*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)));
            }
            delete (yyvsp[-3].s);
    }
    break;

  case 329: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" '>' '(' expr ')'  */
                                                                                                {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ptd->typeexpr,*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-7])),*(yyvsp[-6].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-4].s));
            }
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 330: /* expr_type_info: "typeinfo" name_in_namespace '<' "name" "end of expression" "name" '>' '(' expr ')'  */
                                                                                                                     {
            if ( (yyvsp[-1].pExpression)->rtti_isTypeDecl() ) {
                auto ptd = (ExprTypeDecl *)(yyvsp[-1].pExpression);
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ptd->typeexpr,*(yyvsp[-6].s),*(yyvsp[-4].s));
                delete (yyvsp[-1].pExpression);
            } else {
                (yyval.pExpression) = new ExprTypeInfo(tokAt(scanner,(yylsp[-9])),*(yyvsp[-8].s),ExpressionPtr((yyvsp[-1].pExpression)),*(yyvsp[-6].s),*(yyvsp[-4].s));
            }
            delete (yyvsp[-8].s);
            delete (yyvsp[-6].s);
            delete (yyvsp[-4].s);
    }
    break;

  case 331: /* expr_list: expr  */
                      {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 332: /* expr_list: expr_list ',' expr  */
                                            {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 333: /* block_or_simple_block: expression_block  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 334: /* block_or_simple_block: "=>" expr  */
                                        {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[0].pExpression)));
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-1]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 335: /* block_or_simple_block: "=>" "<-" expr  */
                                               {
            auto retE = make_smart<ExprReturn>(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[0].pExpression)));
            retE->moveSemantics = true;
            auto blkE = new ExprBlock();
            blkE->at = tokAt(scanner,(yylsp[-2]));
            blkE->list.push_back(retE);
            (yyval.pExpression) = blkE;
    }
    break;

  case 336: /* block_or_lambda: '$'  */
                { (yyval.i) = 0;   /* block */  }
    break;

  case 337: /* block_or_lambda: '@'  */
                { (yyval.i) = 1;   /* lambda */ }
    break;

  case 338: /* block_or_lambda: '@' '@'  */
                { (yyval.i) = 2;   /* local function */ }
    break;

  case 339: /* capture_entry: '&' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_reference); delete (yyvsp[0].s); }
    break;

  case 340: /* capture_entry: '=' "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_copy); delete (yyvsp[0].s); }
    break;

  case 341: /* capture_entry: "<-" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_move); delete (yyvsp[0].s); }
    break;

  case 342: /* capture_entry: ":=" "name"  */
                               { (yyval.pCapt) = new CaptureEntry(*(yyvsp[0].s),CaptureMode::capture_by_clone); delete (yyvsp[0].s); }
    break;

  case 343: /* capture_list: capture_entry  */
                         {
        (yyval.pCaptList) = new vector<CaptureEntry>();
        (yyval.pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
    }
    break;

  case 344: /* capture_list: capture_list ',' capture_entry  */
                                               {
        (yyvsp[-2].pCaptList)->push_back(*(yyvsp[0].pCapt));
        delete (yyvsp[0].pCapt);
        (yyval.pCaptList) = (yyvsp[-2].pCaptList);
    }
    break;

  case 345: /* optional_capture_list: %empty  */
        { (yyval.pCaptList) = nullptr; }
    break;

  case 346: /* optional_capture_list: "[[" capture_list ']' ']'  */
                                         { (yyval.pCaptList) = (yyvsp[-2].pCaptList); }
    break;

  case 347: /* expr_block: expression_block  */
                                            {
        ExprBlock * closure = (ExprBlock *) (yyvsp[0].pExpression);
        (yyval.pExpression) = new ExprMakeBlock(tokAt(scanner,(yylsp[0])),ExpressionPtr((yyvsp[0].pExpression)));
        closure->returnType = make_smart<TypeDecl>(Type::autoinfer);
    }
    break;

  case 348: /* expr_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 349: /* expr_full_block: block_or_lambda optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type block_or_simple_block  */
                                                                                            {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-5].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 350: /* $@21: %empty  */
                             {  yyextra->das_need_oxford_comma = false; }
    break;

  case 351: /* expr_full_block_assumed_piped: block_or_lambda $@21 optional_annotation_list optional_capture_list optional_function_argument_list optional_function_type expression_block  */
                                                                                       {
        (yyval.pExpression) = ast_makeBlock(scanner,(yyvsp[-6].i),(yyvsp[-4].faList),(yyvsp[-3].pCaptList),(yyvsp[-2].pVarDeclList),(yyvsp[-1].pTypeDecl),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[-4])));
    }
    break;

  case 352: /* expr_numeric_const: "integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt(tokAt(scanner,(yylsp[0])),(int32_t)(yyvsp[0].i)); }
    break;

  case 353: /* expr_numeric_const: "unsigned integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt(tokAt(scanner,(yylsp[0])),(uint32_t)(yyvsp[0].ui)); }
    break;

  case 354: /* expr_numeric_const: "long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstInt64(tokAt(scanner,(yylsp[0])),(int64_t)(yyvsp[0].i64)); }
    break;

  case 355: /* expr_numeric_const: "unsigned long integer constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt64(tokAt(scanner,(yylsp[0])),(uint64_t)(yyvsp[0].ui64)); }
    break;

  case 356: /* expr_numeric_const: "unsigned int8 constant"  */
                                              { (yyval.pExpression) = new ExprConstUInt8(tokAt(scanner,(yylsp[0])),(uint8_t)(yyvsp[0].ui)); }
    break;

  case 357: /* expr_numeric_const: "floating point constant"  */
                                              { (yyval.pExpression) = new ExprConstFloat(tokAt(scanner,(yylsp[0])),(float)(yyvsp[0].fd)); }
    break;

  case 358: /* expr_numeric_const: "double constant"  */
                                              { (yyval.pExpression) = new ExprConstDouble(tokAt(scanner,(yylsp[0])),(double)(yyvsp[0].d)); }
    break;

  case 359: /* expr_assign: expr  */
                                             { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 360: /* expr_assign: expr '=' expr  */
                                             { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 361: /* expr_assign: expr "<-" expr  */
                                             { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 362: /* expr_assign: expr ":=" expr  */
                                             { (yyval.pExpression) = new ExprClone(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 363: /* expr_assign: expr "&=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 364: /* expr_assign: expr "|=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 365: /* expr_assign: expr "^=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 366: /* expr_assign: expr "&&=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 367: /* expr_assign: expr "||=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 368: /* expr_assign: expr "^^=" expr  */
                                                { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 369: /* expr_assign: expr "+=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 370: /* expr_assign: expr "-=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 371: /* expr_assign: expr "*=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 372: /* expr_assign: expr "/=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 373: /* expr_assign: expr "%=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 374: /* expr_assign: expr "<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 375: /* expr_assign: expr ">>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 376: /* expr_assign: expr "<<<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 377: /* expr_assign: expr ">>>=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 378: /* expr_assign_pipe_right: "@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 379: /* expr_assign_pipe_right: "@@ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 380: /* expr_assign_pipe_right: "$ <|" expr_block  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 381: /* expr_assign_pipe_right: expr_call_pipe  */
                                   { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 382: /* expr_assign_pipe: expr '=' expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprCopy(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 383: /* expr_assign_pipe: expr "<-" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprMove(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 384: /* expr_assign_pipe: expr "&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 385: /* expr_assign_pipe: expr "|=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 386: /* expr_assign_pipe: expr "^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 387: /* expr_assign_pipe: expr "&&=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 388: /* expr_assign_pipe: expr "||=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 389: /* expr_assign_pipe: expr "^^=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 390: /* expr_assign_pipe: expr "+=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 391: /* expr_assign_pipe: expr "-=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 392: /* expr_assign_pipe: expr "*=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 393: /* expr_assign_pipe: expr "/=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 394: /* expr_assign_pipe: expr "%=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 395: /* expr_assign_pipe: expr "<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 396: /* expr_assign_pipe: expr ">>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 397: /* expr_assign_pipe: expr "<<<=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 398: /* expr_assign_pipe: expr ">>>=" expr_assign_pipe_right  */
                                                                  { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 399: /* expr_named_call: name_in_namespace '(' '[' make_struct_fields ']' ')'  */
                                                                         {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-5])),*(yyvsp[-5].s));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-5].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 400: /* expr_named_call: name_in_namespace '(' expr_list ',' '[' make_struct_fields ']' ')'  */
                                                                                                  {
        auto nc = new ExprNamedCall(tokAt(scanner,(yylsp[-7])),*(yyvsp[-7].s));
        nc->nonNamedArguments = sequenceToList((yyvsp[-5].pExpression));
        nc->arguments = *(yyvsp[-2].pMakeStruct);
        delete (yyvsp[-2].pMakeStruct);
        delete (yyvsp[-7].s);
        (yyval.pExpression) = nc;
    }
    break;

  case 401: /* expr_method_call: expr "->" "name" '(' ')'  */
                                                         {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 402: /* expr_method_call: expr "->" "name" '(' expr_list ')'  */
                                                                              {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 403: /* func_addr_name: name_in_namespace  */
                                    {
        (yyval.pExpression) = new ExprAddr(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 404: /* func_addr_name: "$i" '(' expr ')'  */
                                          {
        auto expr = new ExprAddr(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression), expr, "i");
    }
    break;

  case 405: /* func_addr_expr: '@' '@' func_addr_name  */
                                          {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 406: /* $@22: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 407: /* $@23: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 408: /* func_addr_expr: '@' '@' '<' $@22 type_declaration_no_options '>' $@23 func_addr_name  */
                                                                                                                                                       {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 409: /* $@24: %empty  */
                    { yyextra->das_arrow_depth ++; }
    break;

  case 410: /* $@25: %empty  */
                                                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 411: /* func_addr_expr: '@' '@' '<' $@24 optional_function_argument_list optional_function_type '>' $@25 func_addr_name  */
                                                                                                                                                                                     {
        auto expr = (ExprAddr *) ((yyvsp[0].pExpression)->rtti_isAddr() ? (yyvsp[0].pExpression) : (((ExprTag *) (yyvsp[0].pExpression))->value.get()));
        expr->funcType = make_smart<TypeDecl>(Type::tFunction);
        expr->funcType->firstType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        if ( (yyvsp[-4].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, expr->funcType.get(), (yyvsp[-4].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-4].pVarDeclList));
        }
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 412: /* expr_field: expr '.' "name"  */
                                              {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 413: /* expr_field: expr '.' '.' "name"  */
                                                  {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true);
        delete (yyvsp[0].s);
    }
    break;

  case 414: /* expr_field: expr '.' "name" '(' ')'  */
                                                      {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-3])), (yyvsp[-4].pExpression), *(yyvsp[-2].s));
        delete (yyvsp[-2].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 415: /* expr_field: expr '.' "name" '(' expr_list ')'  */
                                                                           {
        auto pInvoke = makeInvokeMethod(tokAt(scanner,(yylsp[-4])), (yyvsp[-5].pExpression), *(yyvsp[-3].s));
        auto callArgs = sequenceToList((yyvsp[-1].pExpression));
        pInvoke->arguments.insert ( pInvoke->arguments.end(), callArgs.begin(), callArgs.end() );
        delete (yyvsp[-3].s);
        (yyval.pExpression) = pInvoke;
    }
    break;

  case 416: /* $@26: %empty  */
                               { yyextra->das_suppress_errors=true; }
    break;

  case 417: /* $@27: %empty  */
                                                                            { yyextra->das_suppress_errors=false; }
    break;

  case 418: /* expr_field: expr '.' $@26 error $@27  */
                                                                                                                    {
        (yyval.pExpression) = new ExprField(tokAt(scanner,(yylsp[-3])), tokAt(scanner,(yylsp[-3])), ExpressionPtr((yyvsp[-4].pExpression)), "");
        yyerrok;
    }
    break;

  case 419: /* expr_call: name_in_namespace '(' ')'  */
                                               {
            (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),*(yyvsp[-2].s));
            delete (yyvsp[-2].s);
    }
    break;

  case 420: /* expr_call: name_in_namespace '(' make_struct_single ')'  */
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

  case 421: /* expr_call: name_in_namespace '(' expr_list ')'  */
                                                                    {
            (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),*(yyvsp[-3].s)),(yyvsp[-1].pExpression));
            delete (yyvsp[-3].s);
    }
    break;

  case 422: /* expr_call: basic_type_declaration '(' ')'  */
                                                    {
        (yyval.pExpression) = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-2].type)));
    }
    break;

  case 423: /* expr_call: basic_type_declaration '(' expr_list ')'  */
                                                                         {
        (yyval.pExpression) = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[-3].type))),(yyvsp[-1].pExpression));
    }
    break;

  case 424: /* expr: "null"  */
                                              { (yyval.pExpression) = new ExprConstPtr(tokAt(scanner,(yylsp[0])),nullptr); }
    break;

  case 425: /* expr: name_in_namespace  */
                                              { (yyval.pExpression) = new ExprVar(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 426: /* expr: expr_numeric_const  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 427: /* expr: expr_reader  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 428: /* expr: string_builder  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 429: /* expr: make_decl  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 430: /* expr: "true"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),true); }
    break;

  case 431: /* expr: "false"  */
                                              { (yyval.pExpression) = new ExprConstBool(tokAt(scanner,(yylsp[0])),false); }
    break;

  case 432: /* expr: expr_field  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 433: /* expr: expr_mtag  */
                                              { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 434: /* expr: '!' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"!",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 435: /* expr: '~' expr  */
                                              { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"~",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 436: /* expr: '+' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"+",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 437: /* expr: '-' expr  */
                                                  { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"-",ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 438: /* expr: expr "<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 439: /* expr: expr ">>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 440: /* expr: expr "<<<" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<<<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 441: /* expr: expr ">>>" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">>>", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 442: /* expr: expr '+' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"+", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 443: /* expr: expr '-' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"-", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 444: /* expr: expr '*' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"*", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 445: /* expr: expr '/' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"/", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 446: /* expr: expr '%' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"%", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 447: /* expr: expr '<' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 448: /* expr: expr '>' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 449: /* expr: expr "==" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"==", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 450: /* expr: expr "!=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"!=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 451: /* expr: expr "<=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"<=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 452: /* expr: expr ">=" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),">=", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 453: /* expr: expr '&' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 454: /* expr: expr '|' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"|", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 455: /* expr: expr '^' expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 456: /* expr: expr "&&" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"&&", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 457: /* expr: expr "||" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"||", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 458: /* expr: expr "^^" expr  */
                                             { (yyval.pExpression) = new ExprOp2(tokAt(scanner,(yylsp[-1])),"^^", ExpressionPtr((yyvsp[-2].pExpression)), ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 459: /* expr: expr ".." expr  */
                                             {
        auto itv = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-1])),"interval");
        itv->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        itv->arguments.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = itv;
    }
    break;

  case 460: /* expr: "++" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"++", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 461: /* expr: "--" expr  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[-1])),"--", ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 462: /* expr: expr "++"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"+++", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 463: /* expr: expr "--"  */
                                                 { (yyval.pExpression) = new ExprOp1(tokAt(scanner,(yylsp[0])),"---", ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 464: /* expr: '(' expr_list optional_comma ')'  */
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

  case 465: /* expr: expr '[' expr ']'  */
                                                 { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 466: /* expr: expr '.' '[' expr ']'  */
                                                     { (yyval.pExpression) = new ExprAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 467: /* expr: expr "?[" expr ']'  */
                                                 { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-3].pExpression)), ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 468: /* expr: expr '.' "?[" expr ']'  */
                                                     { (yyval.pExpression) = new ExprSafeAt(tokAt(scanner,(yylsp[-2])), ExpressionPtr((yyvsp[-4].pExpression)), ExpressionPtr((yyvsp[-1].pExpression)), true); }
    break;

  case 469: /* expr: expr "?." "name"  */
                                                 { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-2].pExpression)), *(yyvsp[0].s)); delete (yyvsp[0].s); }
    break;

  case 470: /* expr: expr '.' "?." "name"  */
                                                     { (yyval.pExpression) = new ExprSafeField(tokAt(scanner,(yylsp[-1])), tokAt(scanner,(yylsp[0])), ExpressionPtr((yyvsp[-3].pExpression)), *(yyvsp[0].s), true); delete (yyvsp[0].s); }
    break;

  case 471: /* expr: func_addr_expr  */
                                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 472: /* expr: expr_call  */
                        { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 473: /* expr: '*' expr  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 474: /* expr: "deref" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprPtr2Ref(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 475: /* expr: "addr" '(' expr ')'  */
                                                   { (yyval.pExpression) = new ExprRef2Ptr(tokAt(scanner,(yylsp[-3])),ExpressionPtr((yyvsp[-1].pExpression))); }
    break;

  case 476: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' ')'  */
                                                                                                              {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-4].pTypeDecl),(yyvsp[-2].pCaptList),nullptr,tokAt(scanner,(yylsp[-6])));
    }
    break;

  case 477: /* expr: "generator" '<' type_declaration_no_options '>' optional_capture_list '(' expr ')'  */
                                                                                                                            {
        (yyval.pExpression) = ast_makeGenerator(scanner,(yyvsp[-5].pTypeDecl),(yyvsp[-3].pCaptList),(yyvsp[-1].pExpression),tokAt(scanner,(yylsp[-7])));
    }
    break;

  case 478: /* expr: expr "??" expr  */
                                                   { (yyval.pExpression) = new ExprNullCoalescing(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression))); }
    break;

  case 479: /* expr: expr '?' expr ':' expr  */
                                                          {
            (yyval.pExpression) = new ExprOp3(tokAt(scanner,(yylsp[-3])),"?",ExpressionPtr((yyvsp[-4].pExpression)),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
        }
    break;

  case 480: /* $@28: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 481: /* $@29: %empty  */
                                                                                                                      { yyextra->das_arrow_depth --; }
    break;

  case 482: /* expr: expr "is" "type" '<' $@28 type_declaration_no_options '>' $@29  */
                                                                                                                                                       {
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),TypeDeclPtr((yyvsp[-2].pTypeDecl)));
    }
    break;

  case 483: /* expr: expr "is" basic_type_declaration  */
                                                               {
        auto vdecl = new TypeDecl((yyvsp[0].type));
        vdecl->at = tokAt(scanner,(yylsp[0]));
        (yyval.pExpression) = new ExprIs(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),vdecl);
    }
    break;

  case 484: /* expr: expr "is" "name"  */
                                              {
        (yyval.pExpression) = new ExprIsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 485: /* expr: expr "as" "name"  */
                                              {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 486: /* $@30: %empty  */
                                               { yyextra->das_arrow_depth ++; }
    break;

  case 487: /* $@31: %empty  */
                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 488: /* expr: expr "as" "type" '<' $@30 type_declaration '>' $@31  */
                                                                                                                                            {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-7].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 489: /* expr: expr "as" basic_type_declaration  */
                                                               {
        (yyval.pExpression) = new ExprAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-2].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 490: /* expr: expr '?' "as" "name"  */
                                                  {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),*(yyvsp[0].s));
        delete (yyvsp[0].s);
    }
    break;

  case 491: /* $@32: %empty  */
                                                   { yyextra->das_arrow_depth ++; }
    break;

  case 492: /* $@33: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 493: /* expr: expr '?' "as" "type" '<' $@32 type_declaration '>' $@33  */
                                                                                                                                                {
        auto vname = (yyvsp[-2].pTypeDecl)->describe();
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-6])),ExpressionPtr((yyvsp[-8].pExpression)),vname);
        delete (yyvsp[-2].pTypeDecl);
    }
    break;

  case 494: /* expr: expr '?' "as" basic_type_declaration  */
                                                                   {
        (yyval.pExpression) = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-3].pExpression)),das_to_string((yyvsp[0].type)));
    }
    break;

  case 495: /* expr: expr_type_info  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 496: /* expr: expr_type_decl  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 497: /* expr: expr_cast  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 498: /* expr: expr_new  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 499: /* expr: expr_method_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 500: /* expr: expr_named_call  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 501: /* expr: expr_full_block  */
                                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 502: /* expr: expr "<|" expr  */
                                                { (yyval.pExpression) = ast_lpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 503: /* expr: expr "|>" expr  */
                                                { (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),(yyvsp[0].pExpression),tokAt(scanner,(yylsp[-1]))); }
    break;

  case 504: /* expr: expr "|>" basic_type_declaration  */
                                                          {
        auto fncall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[0])),tokAt(scanner,(yylsp[0])),das_to_string((yyvsp[0].type)));
        (yyval.pExpression) = ast_rpipe(scanner,(yyvsp[-2].pExpression),fncall,tokAt(scanner,(yylsp[-1])));
    }
    break;

  case 505: /* expr: name_in_namespace "name"  */
                                                { (yyval.pExpression) = ast_NameName(scanner,(yyvsp[-1].s),(yyvsp[0].s),tokAt(scanner,(yylsp[-1])),tokAt(scanner,(yylsp[0]))); }
    break;

  case 506: /* expr: "unsafe" '(' expr ')'  */
                                         {
        (yyvsp[-1].pExpression)->alwaysSafe = true;
        (yyvsp[-1].pExpression)->userSaidItsSafe = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 507: /* expr: expression_keyword  */
                                { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 508: /* expr_mtag: "$$" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"e"); }
    break;

  case 509: /* expr_mtag: "$i" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"i"); }
    break;

  case 510: /* expr_mtag: "$v" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"v"); }
    break;

  case 511: /* expr_mtag: "$b" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"b"); }
    break;

  case 512: /* expr_mtag: "$a" '(' expr ')'  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),"a"); }
    break;

  case 513: /* expr_mtag: "..."  */
                                                     { (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[0])),nullptr,"..."); }
    break;

  case 514: /* expr_mtag: "$c" '(' expr ')' '(' ')'  */
                                                            {
            auto ccall = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-5])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``");
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-5])),(yyvsp[-3].pExpression),ccall,"c");
        }
    break;

  case 515: /* expr_mtag: "$c" '(' expr ')' '(' expr_list ')'  */
                                                                                {
            auto ccall = parseFunctionArguments(yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),tokAt(scanner,(yylsp[0])),"``MACRO``TAG``CALL``"),(yyvsp[-1].pExpression));
            (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-6])),(yyvsp[-4].pExpression),ccall,"c");
        }
    break;

  case 516: /* expr_mtag: expr '.' "$f" '(' expr ')'  */
                                                                {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 517: /* expr_mtag: expr "?." "$f" '(' expr ')'  */
                                                                 {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-5].pExpression)), "``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 518: /* expr_mtag: expr '.' '.' "$f" '(' expr ')'  */
                                                                    {
        auto cfield = new ExprField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 519: /* expr_mtag: expr '.' "?." "$f" '(' expr ')'  */
                                                                     {
        auto cfield = new ExprSafeField(tokAt(scanner,(yylsp[-4])), tokAt(scanner,(yylsp[-1])), ExpressionPtr((yyvsp[-6].pExpression)), "``MACRO``TAG``FIELD``", true);
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 520: /* expr_mtag: expr "as" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 521: /* expr_mtag: expr '?' "as" "$f" '(' expr ')'  */
                                                                       {
        auto cfield = new ExprSafeAsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-6].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 522: /* expr_mtag: expr "is" "$f" '(' expr ')'  */
                                                                   {
        auto cfield = new ExprIsVariant(tokAt(scanner,(yylsp[-4])),ExpressionPtr((yyvsp[-5].pExpression)),"``MACRO``TAG``FIELD``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression),cfield,"f");
    }
    break;

  case 523: /* expr_mtag: '@' '@' "$c" '(' expr ')'  */
                                                         {
        auto ccall = new ExprAddr(tokAt(scanner,(yylsp[-4])),"``MACRO``TAG``ADDR``");
        (yyval.pExpression) = new ExprTag(tokAt(scanner,(yylsp[-3])),(yyvsp[-1].pExpression),ccall,"c");
    }
    break;

  case 524: /* optional_field_annotation: %empty  */
                                                        { (yyval.aaList) = nullptr; }
    break;

  case 525: /* optional_field_annotation: "[[" annotation_argument_list ']' ']'  */
                                                        { (yyval.aaList) = (yyvsp[-2].aaList); /*this one is gone when BRABRA is disabled*/ }
    break;

  case 526: /* optional_field_annotation: metadata_argument_list  */
                                                        { (yyval.aaList) = (yyvsp[0].aaList); }
    break;

  case 527: /* optional_override: %empty  */
                      { (yyval.i) = OVERRIDE_NONE; }
    break;

  case 528: /* optional_override: "override"  */
                      { (yyval.i) = OVERRIDE_OVERRIDE; }
    break;

  case 529: /* optional_override: "sealed"  */
                      { (yyval.i) = OVERRIDE_SEALED; }
    break;

  case 530: /* optional_constant: %empty  */
                        { (yyval.b) = false; }
    break;

  case 531: /* optional_constant: "const"  */
                        { (yyval.b) = true; }
    break;

  case 532: /* optional_public_or_private_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 533: /* optional_public_or_private_member_variable: "public"  */
                        { (yyval.b) = false; }
    break;

  case 534: /* optional_public_or_private_member_variable: "private"  */
                        { (yyval.b) = true; }
    break;

  case 535: /* optional_static_member_variable: %empty  */
                        { (yyval.b) = false; }
    break;

  case 536: /* optional_static_member_variable: "static"  */
                        { (yyval.b) = true; }
    break;

  case 537: /* structure_variable_declaration: optional_field_annotation optional_static_member_variable optional_override optional_public_or_private_member_variable variable_declaration  */
                                                                                                                                                                                      {
        (yyvsp[0].pVarDecl)->override = (yyvsp[-2].i) == OVERRIDE_OVERRIDE;
        (yyvsp[0].pVarDecl)->sealed = (yyvsp[-2].i) == OVERRIDE_SEALED;
        (yyvsp[0].pVarDecl)->annotation = (yyvsp[-4].aaList);
        (yyvsp[0].pVarDecl)->isPrivate = (yyvsp[-1].b);
        (yyvsp[0].pVarDecl)->isStatic = (yyvsp[-3].b);
        (yyval.pVarDecl) = (yyvsp[0].pVarDecl);
    }
    break;

  case 538: /* struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 539: /* $@34: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructureFields(tak);
        }
    }
    break;

  case 540: /* struct_variable_declaration_list: struct_variable_declaration_list $@34 structure_variable_declaration "end of expression"  */
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

  case 541: /* $@35: %empty  */
                                                                                                                     {
                yyextra->das_force_oxford_comma=true;
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-2]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 542: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable "abstract" optional_constant $@35 function_declaration_header "end of expression"  */
                                                    {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[-1]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDefAbstract(scanner,(yyvsp[-8].pVarDeclList),(yyvsp[-7].faList),(yyvsp[-5].b),(yyvsp[-3].b), (yyvsp[-1].pFuncDecl));
            }
    break;

  case 543: /* $@36: %empty  */
                                                                                                                                                                         {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeFunction(tak);
                }
            }
    break;

  case 544: /* struct_variable_declaration_list: struct_variable_declaration_list optional_annotation_list "def" optional_public_or_private_member_variable optional_static_member_variable optional_override optional_constant $@36 function_declaration_header expression_block  */
                                                                        {
                if ( !yyextra->g_CommentReaders.empty() ) {
                    auto tak = tokAt(scanner,(yylsp[0]));
                    for ( auto & crd : yyextra->g_CommentReaders ) crd->afterFunction((yyvsp[-1].pFuncDecl),tak);
                }
                (yyval.pVarDeclList) = ast_structVarDef(scanner,(yyvsp[-9].pVarDeclList),(yyvsp[-8].faList),(yyvsp[-5].b),(yyvsp[-6].b),(yyvsp[-4].i),(yyvsp[-3].b),(yyvsp[-1].pFuncDecl),(yyvsp[0].pExpression),tokRangeAt(scanner,(yylsp[-7]),(yylsp[0])),tokAt(scanner,(yylsp[-8])));
            }
    break;

  case 545: /* struct_variable_declaration_list: struct_variable_declaration_list '[' annotation_list ']' "end of expression"  */
                                                                                 {
        das_yyerror(scanner,"structure field or class method annotation expected to remain on the same line with the field or the class",
            tokAt(scanner,(yylsp[-2])), CompilationError::syntax_error);
        delete (yyvsp[-2].faList);
        (yyval.pVarDeclList) = (yyvsp[-4].pVarDeclList);
    }
    break;

  case 546: /* function_argument_declaration: optional_field_annotation kwd_let_var_or_nothing variable_declaration  */
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

  case 547: /* function_argument_declaration: "$a" '(' expr ')'  */
                                     {
            auto na = new vector<VariableNameAndPosition>();
            na->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1]))});
            auto decl = new VariableDeclaration(na, new TypeDecl(Type::none), (yyvsp[-1].pExpression));
            decl->pTypeDecl->isTag = true;
            (yyval.pVarDecl) = decl;
        }
    break;

  case 548: /* function_argument_list: function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 549: /* function_argument_list: function_argument_list "end of expression" function_argument_declaration  */
                                                                                 { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 550: /* tuple_type: type_declaration  */
                                    {
        (yyval.pVarDecl) = new VariableDeclaration(nullptr,(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 551: /* tuple_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 552: /* tuple_type_list: tuple_type  */
                                                       { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 553: /* tuple_type_list: tuple_type_list c_or_s tuple_type  */
                                                          { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 554: /* tuple_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 555: /* tuple_alias_type_list: tuple_alias_type_list c_or_s  */
                                         {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 556: /* tuple_alias_type_list: tuple_alias_type_list tuple_type c_or_s  */
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

  case 557: /* variant_type: "name" ':' type_declaration  */
                                                   {
        auto na = new vector<VariableNameAndPosition>();
        na->push_back(VariableNameAndPosition{*(yyvsp[-2].s),"",tokAt(scanner,(yylsp[-2]))});
        (yyval.pVarDecl) = new VariableDeclaration(na,(yyvsp[0].pTypeDecl),nullptr);
        delete (yyvsp[-2].s);
    }
    break;

  case 558: /* variant_type_list: variant_type  */
                                                         { (yyval.pVarDeclList) = new vector<VariableDeclaration*>(); (yyval.pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 559: /* variant_type_list: variant_type_list c_or_s variant_type  */
                                                            { (yyval.pVarDeclList) = (yyvsp[-2].pVarDeclList); (yyvsp[-2].pVarDeclList)->push_back((yyvsp[0].pVarDecl)); }
    break;

  case 560: /* variant_alias_type_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 561: /* variant_alias_type_list: variant_alias_type_list c_or_s  */
                                           {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 562: /* variant_alias_type_list: variant_alias_type_list variant_type c_or_s  */
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

  case 563: /* copy_or_move: '='  */
                    { (yyval.b) = false; }
    break;

  case 564: /* copy_or_move: "<-"  */
                    { (yyval.b) = true; }
    break;

  case 565: /* variable_declaration: variable_name_with_pos_list  */
                                          {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[0]));
        autoT->ref = false;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[0].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 566: /* variable_declaration: variable_name_with_pos_list '&'  */
                                              {
        auto autoT = new TypeDecl(Type::autoinfer);
        autoT->at = tokAt(scanner,(yylsp[-1]));
        autoT->ref = true;
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-1].pNameWithPosList),autoT,nullptr);
    }
    break;

  case 567: /* variable_declaration: variable_name_with_pos_list ':' type_declaration  */
                                                                          {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),(yyvsp[0].pTypeDecl),nullptr);
    }
    break;

  case 568: /* variable_declaration: variable_name_with_pos_list ':' type_declaration copy_or_move expr  */
                                                                                                      {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 569: /* variable_declaration: variable_name_with_pos_list copy_or_move expr  */
                                                                       {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 570: /* variable_declaration: variable_name_with_pos_list copy_or_move expr_pipe  */
                                                                            {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-2]));
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-2].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move = (yyvsp[-1].b);
    }
    break;

  case 571: /* copy_or_move_or_clone: '='  */
                    { (yyval.i) = CorM_COPY; }
    break;

  case 572: /* copy_or_move_or_clone: "<-"  */
                    { (yyval.i) = CorM_MOVE; }
    break;

  case 573: /* copy_or_move_or_clone: ":="  */
                    { (yyval.i) = CorM_CLONE; }
    break;

  case 574: /* optional_ref: %empty  */
            { (yyval.b) = false; }
    break;

  case 575: /* optional_ref: '&'  */
            { (yyval.b) = true; }
    break;

  case 576: /* let_variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 577: /* let_variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),ExpressionPtr((yyvsp[-1].pExpression))});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 578: /* let_variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 579: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name"  */
                                                             {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 580: /* let_variable_name_with_pos_list: let_variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                                   {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 581: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options "end of expression"  */
                                                                                            {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),(yyvsp[-1].pTypeDecl),nullptr);
    }
    break;

  case 582: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr "end of expression"  */
                                                                                                                                  {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-5].pNameWithPosList),(yyvsp[-3].pTypeDecl),(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 583: /* let_variable_declaration: let_variable_name_with_pos_list ':' type_declaration_no_options copy_or_move_or_clone expr_pipe  */
                                                                                                                                   {
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),(yyvsp[-2].pTypeDecl),(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 584: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr "end of expression"  */
                                                                                                          {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-4]));
        typeDecl->ref = (yyvsp[-3].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-4].pNameWithPosList),typeDecl,(yyvsp[-1].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-2].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-2].i) & CorM_CLONE) !=0;
    }
    break;

  case 585: /* let_variable_declaration: let_variable_name_with_pos_list optional_ref copy_or_move_or_clone expr_pipe  */
                                                                                                           {
        auto typeDecl = new TypeDecl(Type::autoinfer);
        typeDecl->at = tokAt(scanner,(yylsp[-3]));
        typeDecl->ref = (yyvsp[-2].b);
        (yyval.pVarDecl) = new VariableDeclaration((yyvsp[-3].pNameWithPosList),typeDecl,(yyvsp[0].pExpression));
        (yyval.pVarDecl)->init_via_move  = ((yyvsp[-1].i) & CorM_MOVE) !=0;
        (yyval.pVarDecl)->init_via_clone = ((yyvsp[-1].i) & CorM_CLONE) !=0;
    }
    break;

  case 586: /* global_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 587: /* $@37: %empty  */
                                               {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 588: /* global_variable_declaration_list: global_variable_declaration_list $@37 optional_field_annotation let_variable_declaration  */
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

  case 589: /* optional_shared: %empty  */
                     { (yyval.b) = false; }
    break;

  case 590: /* optional_shared: "shared"  */
                     { (yyval.b) = true; }
    break;

  case 591: /* optional_public_or_private_variable: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 592: /* optional_public_or_private_variable: "private"  */
                     { (yyval.b) = false; }
    break;

  case 593: /* optional_public_or_private_variable: "public"  */
                     { (yyval.b) = true; }
    break;

  case 594: /* global_let: kwd_let optional_shared optional_public_or_private_variable "begin of code block" global_variable_declaration_list "end of code block"  */
                                                                                                                                       {
        ast_globalLetList(scanner,(yyvsp[-5].b),(yyvsp[-4].b),(yyvsp[-3].b),(yyvsp[-1].pVarDeclList));
    }
    break;

  case 595: /* $@38: %empty  */
                                                                                        {
        yyextra->das_force_oxford_comma=true;
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeGlobalVariables(tak);
        }
    }
    break;

  case 596: /* global_let: kwd_let optional_shared optional_public_or_private_variable $@38 optional_field_annotation let_variable_declaration  */
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

  case 597: /* enum_list: %empty  */
        {
        (yyval.pEnum) = new Enumeration();
    }
    break;

  case 598: /* enum_list: enum_list "end of expression"  */
                          {
        (yyval.pEnum) = (yyvsp[-1].pEnum);
    }
    break;

  case 599: /* enum_list: enum_list "name" "end of expression"  */
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

  case 600: /* enum_list: enum_list "name" '=' expr "end of expression"  */
                                                     {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        if ( !(yyvsp[-4].pEnum)->add(*(yyvsp[-3].s),ExpressionPtr((yyvsp[-1].pExpression)),tokAt(scanner,(yylsp[-3]))) ) {
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

  case 601: /* optional_public_or_private_alias: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 602: /* optional_public_or_private_alias: "private"  */
                     { (yyval.b) = false; }
    break;

  case 603: /* optional_public_or_private_alias: "public"  */
                     { (yyval.b) = true; }
    break;

  case 604: /* $@39: %empty  */
                                                         {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeAlias(pubename);
        }
    }
    break;

  case 605: /* single_alias: optional_public_or_private_alias "name" $@39 '=' type_declaration  */
                                  {
        das_checkName(scanner,*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])));
        (yyvsp[0].pTypeDecl)->isPrivateAlias = !(yyvsp[-4].b);
        if ( (yyvsp[0].pTypeDecl)->baseType == Type::alias ) {
            das_yyerror(scanner,"alias cannot be defined in terms of another alias "+*(yyvsp[-3].s),tokAt(scanner,(yylsp[-3])),
                CompilationError::invalid_type);
        }
        (yyvsp[0].pTypeDecl)->alias = *(yyvsp[-3].s);
        if ( !yyextra->g_Program->addAlias(TypeDeclPtr((yyvsp[0].pTypeDecl))) ) {
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

  case 609: /* $@40: %empty  */
                    { yyextra->das_force_oxford_comma=true;}
    break;

  case 611: /* optional_public_or_private_enum: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 612: /* optional_public_or_private_enum: "private"  */
                     { (yyval.b) = false; }
    break;

  case 613: /* optional_public_or_private_enum: "public"  */
                     { (yyval.b) = true; }
    break;

  case 614: /* enum_name: "name"  */
                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumeration(pubename);
        }
        (yyval.pEnum) = ast_addEmptyEnum(scanner, (yyvsp[0].s), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 615: /* $@41: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 616: /* $@42: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 617: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name "begin of code block" $@41 enum_list $@42 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-5].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-8].faList),tokAt(scanner,(yylsp[-8])),(yyvsp[-6].b),(yyvsp[-5].pEnum),(yyvsp[-2].pEnum),Type::tInt);
    }
    break;

  case 618: /* $@43: %empty  */
                                                                                                                                                     {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-3]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeEnumerationEntries(tak);
        }
    }
    break;

  case 619: /* $@44: %empty  */
                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumerationEntries(tak);
        }
    }
    break;

  case 620: /* enum_declaration: optional_annotation_list "enum" optional_public_or_private_enum enum_name ':' enum_basic_type_declaration "begin of code block" $@43 enum_list $@44 "end of code block"  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto pubename = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterEnumeration((yyvsp[-7].pEnum)->name.c_str(),pubename);
        }
        ast_enumDeclaration(scanner,(yyvsp[-10].faList),tokAt(scanner,(yylsp[-10])),(yyvsp[-8].b),(yyvsp[-7].pEnum),(yyvsp[-2].pEnum),(yyvsp[-5].type));
    }
    break;

  case 621: /* optional_structure_parent: %empty  */
                                        { (yyval.s) = nullptr; }
    break;

  case 622: /* optional_structure_parent: ':' name_in_namespace  */
                                        { (yyval.s) = (yyvsp[0].s); }
    break;

  case 623: /* optional_sealed: %empty  */
                        { (yyval.b) = false; }
    break;

  case 624: /* optional_sealed: "sealed"  */
                        { (yyval.b) = true; }
    break;

  case 625: /* structure_name: optional_sealed "name" optional_structure_parent  */
                                                                           {
        (yyval.pStructure) = ast_structureName(scanner,(yyvsp[-2].b),(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])),(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
    }
    break;

  case 626: /* class_or_struct: "class"  */
                    { (yyval.b) = true; }
    break;

  case 627: /* class_or_struct: "struct"  */
                    { (yyval.b) = false; }
    break;

  case 628: /* optional_public_or_private_structure: %empty  */
                     { (yyval.b) = yyextra->g_Program->thisModule->isPublic; }
    break;

  case 629: /* optional_public_or_private_structure: "private"  */
                     { (yyval.b) = false; }
    break;

  case 630: /* optional_public_or_private_structure: "public"  */
                     { (yyval.b) = true; }
    break;

  case 631: /* optional_struct_variable_declaration_list: %empty  */
        {
        (yyval.pVarDeclList) = new vector<VariableDeclaration*>();
    }
    break;

  case 632: /* optional_struct_variable_declaration_list: "begin of code block" struct_variable_declaration_list "end of code block"  */
                                                       {
        (yyval.pVarDeclList) = (yyvsp[-1].pVarDeclList);
    }
    break;

  case 633: /* $@45: %empty  */
                                                                                                        {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto tak = tokAt(scanner,(yylsp[-1]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeStructure(tak);
        }
    }
    break;

  case 634: /* $@46: %empty  */
                         { if ( (yyvsp[0].pStructure) ) { (yyvsp[0].pStructure)->isClass = (yyvsp[-3].b); (yyvsp[0].pStructure)->privateStructure = !(yyvsp[-2].b); } }
    break;

  case 635: /* structure_declaration: optional_annotation_list class_or_struct optional_public_or_private_structure $@45 structure_name $@46 optional_struct_variable_declaration_list  */
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

  case 636: /* variable_name_with_pos_list: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 637: /* variable_name_with_pos_list: "$i" '(' expr ')'  */
                                     {
        auto pSL = new vector<VariableNameAndPosition>();
        pSL->push_back(VariableNameAndPosition{"``MACRO``TAG``","",tokAt(scanner,(yylsp[-1])),(yyvsp[-1].pExpression)});
        (yyval.pNameWithPosList) = pSL;
    }
    break;

  case 638: /* variable_name_with_pos_list: "name" "aka" "name"  */
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

  case 639: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name"  */
                                                         {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[0].s),"",tokAt(scanner,(yylsp[0]))});
        (yyval.pNameWithPosList) = (yyvsp[-2].pNameWithPosList);
        delete (yyvsp[0].s);
    }
    break;

  case 640: /* variable_name_with_pos_list: variable_name_with_pos_list ',' "name" "aka" "name"  */
                                                                               {
        das_checkName(scanner,*(yyvsp[-2].s),tokAt(scanner,(yylsp[-2])));
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-4].pNameWithPosList)->push_back(VariableNameAndPosition{*(yyvsp[-2].s),*(yyvsp[0].s),tokAt(scanner,(yylsp[-2]))});
        (yyval.pNameWithPosList) = (yyvsp[-4].pNameWithPosList);
        delete (yyvsp[-2].s);
        delete (yyvsp[0].s);
    }
    break;

  case 641: /* basic_type_declaration: "bool"  */
                        { (yyval.type) = Type::tBool; }
    break;

  case 642: /* basic_type_declaration: "string"  */
                        { (yyval.type) = Type::tString; }
    break;

  case 643: /* basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 644: /* basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 645: /* basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 646: /* basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 647: /* basic_type_declaration: "int2"  */
                        { (yyval.type) = Type::tInt2; }
    break;

  case 648: /* basic_type_declaration: "int3"  */
                        { (yyval.type) = Type::tInt3; }
    break;

  case 649: /* basic_type_declaration: "int4"  */
                        { (yyval.type) = Type::tInt4; }
    break;

  case 650: /* basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 651: /* basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 652: /* basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 653: /* basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 654: /* basic_type_declaration: "uint2"  */
                        { (yyval.type) = Type::tUInt2; }
    break;

  case 655: /* basic_type_declaration: "uint3"  */
                        { (yyval.type) = Type::tUInt3; }
    break;

  case 656: /* basic_type_declaration: "uint4"  */
                        { (yyval.type) = Type::tUInt4; }
    break;

  case 657: /* basic_type_declaration: "float"  */
                        { (yyval.type) = Type::tFloat; }
    break;

  case 658: /* basic_type_declaration: "float2"  */
                        { (yyval.type) = Type::tFloat2; }
    break;

  case 659: /* basic_type_declaration: "float3"  */
                        { (yyval.type) = Type::tFloat3; }
    break;

  case 660: /* basic_type_declaration: "float4"  */
                        { (yyval.type) = Type::tFloat4; }
    break;

  case 661: /* basic_type_declaration: "void"  */
                        { (yyval.type) = Type::tVoid; }
    break;

  case 662: /* basic_type_declaration: "range"  */
                        { (yyval.type) = Type::tRange; }
    break;

  case 663: /* basic_type_declaration: "urange"  */
                        { (yyval.type) = Type::tURange; }
    break;

  case 664: /* basic_type_declaration: "range64"  */
                        { (yyval.type) = Type::tRange64; }
    break;

  case 665: /* basic_type_declaration: "urange64"  */
                        { (yyval.type) = Type::tURange64; }
    break;

  case 666: /* basic_type_declaration: "double"  */
                        { (yyval.type) = Type::tDouble; }
    break;

  case 667: /* basic_type_declaration: "bitfield"  */
                        { (yyval.type) = Type::tBitfield; }
    break;

  case 668: /* enum_basic_type_declaration: "int"  */
                        { (yyval.type) = Type::tInt; }
    break;

  case 669: /* enum_basic_type_declaration: "int8"  */
                        { (yyval.type) = Type::tInt8; }
    break;

  case 670: /* enum_basic_type_declaration: "int16"  */
                        { (yyval.type) = Type::tInt16; }
    break;

  case 671: /* enum_basic_type_declaration: "uint"  */
                        { (yyval.type) = Type::tUInt; }
    break;

  case 672: /* enum_basic_type_declaration: "uint8"  */
                        { (yyval.type) = Type::tUInt8; }
    break;

  case 673: /* enum_basic_type_declaration: "uint16"  */
                        { (yyval.type) = Type::tUInt16; }
    break;

  case 674: /* enum_basic_type_declaration: "int64"  */
                        { (yyval.type) = Type::tInt64; }
    break;

  case 675: /* enum_basic_type_declaration: "uint64"  */
                        { (yyval.type) = Type::tUInt64; }
    break;

  case 676: /* structure_type_declaration: name_in_namespace  */
                                 {
        (yyval.pTypeDecl) = yyextra->g_Program->makeTypeDeclaration(tokAt(scanner,(yylsp[0])),*(yyvsp[0].s));
        if ( !(yyval.pTypeDecl) ) {
            (yyval.pTypeDecl) = new TypeDecl(Type::tVoid);
            (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
        }
        delete (yyvsp[0].s);
    }
    break;

  case 677: /* auto_type_declaration: "auto"  */
                       {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 678: /* auto_type_declaration: "auto" '(' "name" ')'  */
                                            {
        das_checkName(scanner,*(yyvsp[-1].s),tokAt(scanner,(yylsp[-1])));
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pTypeDecl)->alias = *(yyvsp[-1].s);
        delete (yyvsp[-1].s);
    }
    break;

  case 679: /* auto_type_declaration: "$t" '(' expr ')'  */
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

  case 680: /* bitfield_bits: "name"  */
                    {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        auto pSL = new vector<string>();
        pSL->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = pSL;
        delete (yyvsp[0].s);
    }
    break;

  case 681: /* bitfield_bits: bitfield_bits "end of expression" "name"  */
                                           {
        das_checkName(scanner,*(yyvsp[0].s),tokAt(scanner,(yylsp[0])));
        (yyvsp[-2].pNameList)->push_back(*(yyvsp[0].s));
        (yyval.pNameList) = (yyvsp[-2].pNameList);
        delete (yyvsp[0].s);
    }
    break;

  case 682: /* bitfield_alias_bits: %empty  */
        {
        auto pSL = new vector<string>();
        (yyval.pNameList) = pSL;

    }
    break;

  case 683: /* bitfield_alias_bits: bitfield_alias_bits "end of expression"  */
                                      {
        (yyval.pNameList) = (yyvsp[-1].pNameList);
    }
    break;

  case 684: /* bitfield_alias_bits: bitfield_alias_bits "name" "end of expression"  */
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

  case 685: /* $@47: %empty  */
                                     { yyextra->das_arrow_depth ++; }
    break;

  case 686: /* $@48: %empty  */
                                                                                            { yyextra->das_arrow_depth --; }
    break;

  case 687: /* bitfield_type_declaration: "bitfield" '<' $@47 bitfield_bits '>' $@48  */
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

  case 690: /* table_type_pair: type_declaration  */
                                      {
        (yyval.aTypePair).firstType = (yyvsp[0].pTypeDecl);
        (yyval.aTypePair).secondType = new TypeDecl(Type::tVoid);
    }
    break;

  case 691: /* table_type_pair: type_declaration c_or_s type_declaration  */
                                                                             {
        (yyval.aTypePair).firstType = (yyvsp[-2].pTypeDecl);
        (yyval.aTypePair).secondType = (yyvsp[0].pTypeDecl);
    }
    break;

  case 692: /* dim_list: '[' expr ']'  */
                             {
        (yyval.pTypeDecl) = new TypeDecl(Type::autoinfer);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 693: /* dim_list: dim_list '[' expr ']'  */
                                            {
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
        appendDimExpr((yyval.pTypeDecl), (yyvsp[-1].pExpression));
    }
    break;

  case 694: /* type_declaration_no_options: basic_type_declaration  */
                                                            { (yyval.pTypeDecl) = new TypeDecl((yyvsp[0].type)); (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0])); }
    break;

  case 695: /* type_declaration_no_options: auto_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 696: /* type_declaration_no_options: bitfield_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 697: /* type_declaration_no_options: structure_type_declaration  */
                                                            { (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl); }
    break;

  case 698: /* type_declaration_no_options: type_declaration_no_options dim_list  */
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

  case 699: /* type_declaration_no_options: type_declaration_no_options '[' ']'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->dim.push_back(TypeDecl::dimAuto);
        (yyvsp[-2].pTypeDecl)->dimExpr.push_back(nullptr);
        (yyvsp[-2].pTypeDecl)->removeDim = false;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 700: /* $@49: %empty  */
                     { yyextra->das_arrow_depth ++; }
    break;

  case 701: /* $@50: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 702: /* type_declaration_no_options: "type" '<' $@49 type_declaration '>' $@50  */
                                                                                                                      {
        (yyvsp[-2].pTypeDecl)->autoToAlias = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 703: /* type_declaration_no_options: "typedecl" '(' expr ')'  */
                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeDecl);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]),(yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr.push_back((yyvsp[-1].pExpression));
    }
    break;

  case 704: /* type_declaration_no_options: '$' name_in_namespace '(' optional_expr_list ')'  */
                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-3]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = sequenceToList((yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-3])), *(yyvsp[-3].s)));
        delete (yyvsp[-3].s);
    }
    break;

  case 705: /* $@51: %empty  */
                                        { yyextra->das_arrow_depth ++; }
    break;

  case 706: /* type_declaration_no_options: '$' name_in_namespace '<' $@51 type_declaration_no_options_list '>' '(' optional_expr_list ')'  */
                                                                                                                                                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::typeMacro);
        (yyval.pTypeDecl)->at = tokRangeAt(scanner,(yylsp[-7]), (yylsp[-1]));
        (yyval.pTypeDecl)->dimExpr = typesAndSequenceToList((yyvsp[-4].pTypeDeclList),(yyvsp[-1].pExpression));
        (yyval.pTypeDecl)->dimExpr.insert((yyval.pTypeDecl)->dimExpr.begin(), new ExprConstString(tokAt(scanner,(yylsp[-7])), *(yyvsp[-7].s)));
        delete (yyvsp[-7].s);
    }
    break;

  case 707: /* type_declaration_no_options: type_declaration_no_options '-' '[' ']'  */
                                                          {
        (yyvsp[-3].pTypeDecl)->removeDim = true;
        (yyval.pTypeDecl) = (yyvsp[-3].pTypeDecl);
    }
    break;

  case 708: /* type_declaration_no_options: type_declaration_no_options "explicit"  */
                                                           {
        (yyvsp[-1].pTypeDecl)->isExplicit = true;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 709: /* type_declaration_no_options: type_declaration_no_options "const"  */
                                                        {
        (yyvsp[-1].pTypeDecl)->constant = true;
        (yyvsp[-1].pTypeDecl)->removeConstant = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 710: /* type_declaration_no_options: type_declaration_no_options '-' "const"  */
                                                            {
        (yyvsp[-2].pTypeDecl)->constant = false;
        (yyvsp[-2].pTypeDecl)->removeConstant = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 711: /* type_declaration_no_options: type_declaration_no_options '&'  */
                                                  {
        (yyvsp[-1].pTypeDecl)->ref = true;
        (yyvsp[-1].pTypeDecl)->removeRef = false;
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
    }
    break;

  case 712: /* type_declaration_no_options: type_declaration_no_options '-' '&'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->ref = false;
        (yyvsp[-2].pTypeDecl)->removeRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 713: /* type_declaration_no_options: type_declaration_no_options '#'  */
                                                  {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->temporary = true;
    }
    break;

  case 714: /* type_declaration_no_options: type_declaration_no_options "implicit"  */
                                                           {
        (yyval.pTypeDecl) = (yyvsp[-1].pTypeDecl);
        (yyval.pTypeDecl)->implicit = true;
    }
    break;

  case 715: /* type_declaration_no_options: type_declaration_no_options '-' '#'  */
                                                      {
        (yyvsp[-2].pTypeDecl)->temporary = false;
        (yyvsp[-2].pTypeDecl)->removeTemporary = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 716: /* type_declaration_no_options: type_declaration_no_options "==" "const"  */
                                                               {
        (yyvsp[-2].pTypeDecl)->explicitConst = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 717: /* type_declaration_no_options: type_declaration_no_options "==" '&'  */
                                                         {
        (yyvsp[-2].pTypeDecl)->explicitRef = true;
        (yyval.pTypeDecl) = (yyvsp[-2].pTypeDecl);
    }
    break;

  case 718: /* type_declaration_no_options: type_declaration_no_options '?'  */
                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 719: /* $@52: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 720: /* $@53: %empty  */
                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 721: /* type_declaration_no_options: "smart_ptr" '<' $@52 type_declaration '>' $@53  */
                                                                                                                                {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->smartPtr = true;
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 722: /* type_declaration_no_options: type_declaration_no_options "??"  */
                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tPointer);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType = make_smart<TypeDecl>(Type::tPointer);
        (yyval.pTypeDecl)->firstType->at = tokAt(scanner,(yylsp[-1]));
        (yyval.pTypeDecl)->firstType->firstType = TypeDeclPtr((yyvsp[-1].pTypeDecl));
    }
    break;

  case 723: /* $@54: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 724: /* $@55: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 725: /* type_declaration_no_options: "array" '<' $@54 type_declaration '>' $@55  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tArray);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 726: /* $@56: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 727: /* $@57: %empty  */
                                                                                     { yyextra->das_arrow_depth --; }
    break;

  case 728: /* type_declaration_no_options: "table" '<' $@56 table_type_pair '>' $@57  */
                                                                                                                      {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTable);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].aTypePair).firstType);
        (yyval.pTypeDecl)->secondType = TypeDeclPtr((yyvsp[-2].aTypePair).secondType);
    }
    break;

  case 729: /* $@58: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 730: /* $@59: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 731: /* type_declaration_no_options: "iterator" '<' $@58 type_declaration '>' $@59  */
                                                                                                                                  {
        (yyval.pTypeDecl) = new TypeDecl(Type::tIterator);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 732: /* type_declaration_no_options: "block"  */
                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 733: /* $@60: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 734: /* $@61: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 735: /* type_declaration_no_options: "block" '<' $@60 type_declaration '>' $@61  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 736: /* $@62: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 737: /* $@63: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 738: /* type_declaration_no_options: "block" '<' $@62 optional_function_argument_list optional_function_type '>' $@63  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tBlock);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 739: /* type_declaration_no_options: "function"  */
                           {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 740: /* $@64: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 741: /* $@65: %empty  */
                                                                                                { yyextra->das_arrow_depth --; }
    break;

  case 742: /* type_declaration_no_options: "function" '<' $@64 type_declaration '>' $@65  */
                                                                                                                                 {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 743: /* $@66: %empty  */
                               { yyextra->das_arrow_depth ++; }
    break;

  case 744: /* $@67: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 745: /* type_declaration_no_options: "function" '<' $@66 optional_function_argument_list optional_function_type '>' $@67  */
                                                                                                                                                                          {
        (yyval.pTypeDecl) = new TypeDecl(Type::tFunction);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 746: /* type_declaration_no_options: "lambda"  */
                         {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[0]));
    }
    break;

  case 747: /* $@68: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 748: /* $@69: %empty  */
                                                                                              { yyextra->das_arrow_depth --; }
    break;

  case 749: /* type_declaration_no_options: "lambda" '<' $@68 type_declaration '>' $@69  */
                                                                                                                               {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
    }
    break;

  case 750: /* $@70: %empty  */
                             { yyextra->das_arrow_depth ++; }
    break;

  case 751: /* $@71: %empty  */
                                                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 752: /* type_declaration_no_options: "lambda" '<' $@70 optional_function_argument_list optional_function_type '>' $@71  */
                                                                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tLambda);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pTypeDecl)->firstType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        if ( (yyvsp[-3].pVarDeclList) ) {
            varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-3].pVarDeclList));
            deleteVariableDeclarationList((yyvsp[-3].pVarDeclList));
        }
    }
    break;

  case 753: /* $@72: %empty  */
                            { yyextra->das_arrow_depth ++; }
    break;

  case 754: /* $@73: %empty  */
                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 755: /* type_declaration_no_options: "tuple" '<' $@72 tuple_type_list '>' $@73  */
                                                                                                                        {
        (yyval.pTypeDecl) = new TypeDecl(Type::tTuple);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 756: /* $@74: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 757: /* $@75: %empty  */
                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 758: /* type_declaration_no_options: "variant" '<' $@74 variant_type_list '>' $@75  */
                                                                                                                            {
        (yyval.pTypeDecl) = new TypeDecl(Type::tVariant);
        (yyval.pTypeDecl)->at = tokAt(scanner,(yylsp[-5]));
        varDeclToTypeDecl(scanner, (yyval.pTypeDecl), (yyvsp[-2].pVarDeclList), true);
        deleteVariableDeclarationList((yyvsp[-2].pVarDeclList));
    }
    break;

  case 759: /* type_declaration: type_declaration_no_options  */
                                        {
        (yyval.pTypeDecl) = (yyvsp[0].pTypeDecl);
    }
    break;

  case 760: /* type_declaration: type_declaration '|' type_declaration_no_options  */
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

  case 761: /* type_declaration: type_declaration '|' '#'  */
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

  case 762: /* $@76: %empty  */
                                                          { yyextra->das_need_oxford_comma=false; }
    break;

  case 763: /* $@77: %empty  */
                                                                                                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTuple(atvname);
        }
    }
    break;

  case 764: /* $@78: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeTupleEntries(atvname);
        }
    }
    break;

  case 765: /* $@79: %empty  */
                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterTupleEntries(atvname);
        }
    }
    break;

  case 766: /* tuple_alias_declaration: "tuple" optional_public_or_private_alias $@76 "name" $@77 "begin of code block" $@78 tuple_alias_type_list $@79 "end of code block"  */
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

  case 767: /* $@80: %empty  */
                                                            { yyextra->das_need_oxford_comma=false; }
    break;

  case 768: /* $@81: %empty  */
                                                                                                                  {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariant(atvname);
        }
    }
    break;

  case 769: /* $@82: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeVariantEntries(atvname);
        }

    }
    break;

  case 770: /* $@83: %empty  */
                                    {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterVariantEntries(atvname);
        }
    }
    break;

  case 771: /* variant_alias_declaration: "variant" optional_public_or_private_alias $@80 "name" $@81 "begin of code block" $@82 variant_alias_type_list $@83 "end of code block"  */
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

  case 772: /* $@84: %empty  */
                                                             { yyextra->das_need_oxford_comma=false; }
    break;

  case 773: /* $@85: %empty  */
                                                                                                                   {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[0]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfield(atvname);
        }
    }
    break;

  case 774: /* $@86: %empty  */
          {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-2]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->beforeBitfieldEntries(atvname);
        }
    }
    break;

  case 775: /* $@87: %empty  */
                                {
        if ( !yyextra->g_CommentReaders.empty() ) {
            auto atvname = tokAt(scanner,(yylsp[-4]));
            for ( auto & crd : yyextra->g_CommentReaders ) crd->afterBitfieldEntries(atvname);
        }
    }
    break;

  case 776: /* bitfield_alias_declaration: "bitfield" optional_public_or_private_alias $@84 "name" $@85 "begin of code block" $@86 bitfield_alias_bits $@87 "end of code block"  */
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

  case 777: /* make_decl: make_struct_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 778: /* make_decl: make_dim_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 779: /* make_decl: make_table_decl  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 780: /* make_decl: array_comprehension  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 781: /* make_decl: make_tuple_call  */
                                 { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 782: /* make_struct_fields: "name" copy_or_move expr  */
                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 783: /* make_struct_fields: "name" ":=" expr  */
                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 784: /* make_struct_fields: make_struct_fields ',' "name" copy_or_move expr  */
                                                                           {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 785: /* make_struct_fields: make_struct_fields ',' "name" ":=" expr  */
                                                                  {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-2])),*(yyvsp[-2].s),ExpressionPtr((yyvsp[0].pExpression)),false,true);
        delete (yyvsp[-2].s);
        ((MakeStruct *)(yyvsp[-4].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-4].pMakeStruct);
    }
    break;

  case 786: /* make_struct_fields: "$f" '(' expr ')' copy_or_move expr  */
                                                                   {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 787: /* make_struct_fields: "$f" '(' expr ')' ":=" expr  */
                                                          {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        auto msd = new MakeStruct();
        msd->push_back(mfd);
        (yyval.pMakeStruct) = msd;
    }
    break;

  case 788: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' copy_or_move expr  */
                                                                                               {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),(yyvsp[-1].b),false);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 789: /* make_struct_fields: make_struct_fields ',' "$f" '(' expr ')' ":=" expr  */
                                                                                      {
        auto mfd = make_smart<MakeFieldDecl>(tokAt(scanner,(yylsp[-3])),"``MACRO``TAG``FIELD``",ExpressionPtr((yyvsp[0].pExpression)),false,true);
        mfd->tag = ExpressionPtr((yyvsp[-3].pExpression));
        ((MakeStruct *)(yyvsp[-7].pMakeStruct))->push_back(mfd);
        (yyval.pMakeStruct) = (yyvsp[-7].pMakeStruct);
    }
    break;

  case 790: /* make_variant_dim: make_struct_fields  */
                                {
        (yyval.pExpression) = ast_makeStructToMakeVariant((yyvsp[0].pMakeStruct), tokAt(scanner,(yylsp[0])));
    }
    break;

  case 791: /* make_struct_single: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 792: /* make_struct_dim: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 793: /* make_struct_dim: make_struct_dim "end of expression" make_struct_fields  */
                                                         {
        ((ExprMakeStruct *) (yyvsp[-2].pExpression))->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 794: /* make_struct_dim_list: '(' make_struct_fields ')'  */
                                        {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 795: /* make_struct_dim_list: make_struct_dim_list ',' '(' make_struct_fields ')'  */
                                                                     {
        ((ExprMakeStruct *) (yyvsp[-4].pExpression))->structs.push_back(MakeStructPtr((yyvsp[-1].pMakeStruct)));
        (yyval.pExpression) = (yyvsp[-4].pExpression);
    }
    break;

  case 796: /* make_struct_dim_decl: make_struct_fields  */
                                {
        auto msd = new ExprMakeStruct();
        msd->structs.push_back(MakeStructPtr((yyvsp[0].pMakeStruct)));
        (yyval.pExpression) = msd;
    }
    break;

  case 797: /* make_struct_dim_decl: make_struct_dim_list optional_comma  */
                                                 {
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 798: /* optional_make_struct_dim_decl: make_struct_dim_decl  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression);  }
    break;

  case 799: /* optional_make_struct_dim_decl: %empty  */
        {   (yyval.pExpression) = new ExprMakeStruct(); }
    break;

  case 800: /* optional_block: %empty  */
        { (yyval.pExpression) = nullptr; }
    break;

  case 801: /* optional_block: "where" expr_block  */
                                  { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 814: /* use_initializer: %empty  */
                            { (yyval.b) = true; }
    break;

  case 815: /* use_initializer: "uninitialized"  */
                            { (yyval.b) = false; }
    break;

  case 816: /* make_struct_decl: "[[" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 817: /* make_struct_decl: "[[" type_declaration_no_options optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = msd;
    }
    break;

  case 818: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                   {
        auto msd = new ExprMakeStruct();
        msd->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
        msd->useInitializer = true;
        msd->block = (yyvsp[-1].pExpression);
        msd->at = tokAt(scanner,(yylsp[-5]));
        (yyval.pExpression) = msd;
    }
    break;

  case 819: /* make_struct_decl: "[[" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_sqr_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 820: /* make_struct_decl: "[{" type_declaration_no_options make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-4]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 821: /* make_struct_decl: "[{" type_declaration_no_options '(' ')' make_struct_dim optional_block optional_trailing_delim_cur_sqr  */
                                                                                                                                        {
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-2].pExpression))->block = (yyvsp[-1].pExpression);
        (yyvsp[-2].pExpression)->at = tokAt(scanner,(yylsp[-6]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 822: /* $@88: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 823: /* $@89: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 824: /* make_struct_decl: "struct" '<' $@88 type_declaration_no_options '>' $@89 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 825: /* $@90: %empty  */
                            { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 826: /* $@91: %empty  */
                                                                                                                                        { yyextra->das_arrow_depth --; }
    break;

  case 827: /* make_struct_decl: "class" '<' $@90 type_declaration_no_options '>' $@91 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                          {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceClass = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 828: /* $@92: %empty  */
                               { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 829: /* $@93: %empty  */
                                                                                                                                           { yyextra->das_arrow_depth --; }
    break;

  case 830: /* make_struct_decl: "variant" '<' $@92 type_declaration_no_options '>' $@93 '(' make_variant_dim ')'  */
                                                                                                                                                                                                           {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-8]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 831: /* $@94: %empty  */
                              { yyextra->das_arrow_depth ++; }
    break;

  case 832: /* $@95: %empty  */
                                                                                                    { yyextra->das_arrow_depth --; }
    break;

  case 833: /* make_struct_decl: "default" '<' $@94 type_declaration_no_options '>' $@95 use_initializer  */
                                                                                                                                                           {
        auto msd = new ExprMakeStruct();
        msd->at = tokAt(scanner,(yylsp[-6]));
        msd->makeType = TypeDeclPtr((yyvsp[-3].pTypeDecl));
        msd->useInitializer = (yyvsp[0].b);
        msd->alwaysUseInitializer = true;
        (yyval.pExpression) = msd;
    }
    break;

  case 834: /* make_tuple: expr  */
                  {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 835: /* make_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 836: /* make_tuple: make_tuple ',' expr  */
                                      {
        ExprMakeTuple * mt;
        if ( (yyvsp[-2].pExpression)->rtti_isMakeTuple() ) {
            mt = static_cast<ExprMakeTuple *>((yyvsp[-2].pExpression));
        } else {
            mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-2])));
            mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        }
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 837: /* make_map_tuple: expr "=>" expr  */
                                         {
        ExprMakeTuple * mt = new ExprMakeTuple(tokAt(scanner,(yylsp[-1])));
        mt->values.push_back(ExpressionPtr((yyvsp[-2].pExpression)));
        mt->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mt;
    }
    break;

  case 838: /* make_map_tuple: expr  */
                 {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 839: /* make_tuple_call: "tuple" '(' expr_list optional_comma ')'  */
                                                                    {
        auto mkt = new ExprMakeTuple(tokAt(scanner,(yylsp[-4])));
        mkt->values = sequenceToList((yyvsp[-2].pExpression));
        mkt->makeType = make_smart<TypeDecl>(Type::autoinfer);
        (yyval.pExpression) = mkt;
    }
    break;

  case 840: /* $@96: %empty  */
                             { yyextra->das_force_oxford_comma=true; yyextra->das_arrow_depth ++; }
    break;

  case 841: /* $@97: %empty  */
                                                                                                                                         { yyextra->das_arrow_depth --; }
    break;

  case 842: /* make_tuple_call: "tuple" '<' $@96 type_declaration_no_options '>' $@97 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                                            {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 843: /* make_dim: make_tuple  */
                        {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 844: /* make_dim: make_dim "end of expression" make_tuple  */
                                          {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 845: /* make_dim_decl: '[' expr_list optional_comma ']'  */
                                                          {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 846: /* make_dim_decl: "[[" type_declaration_no_options make_dim optional_trailing_semicolon_sqr_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        (yyval.pExpression) = (yyvsp[-1].pExpression);
    }
    break;

  case 847: /* make_dim_decl: "[{" type_declaration_no_options make_dim optional_trailing_semicolon_cur_sqr  */
                                                                                                         {
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-2].pTypeDecl));
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-3]));
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_array_move");
        tam->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = tam;
    }
    break;

  case 848: /* $@98: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 849: /* $@99: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 850: /* make_dim_decl: "array" "struct" '<' $@98 type_declaration_no_options '>' $@99 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceStruct = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 851: /* $@100: %empty  */
                                       { yyextra->das_arrow_depth ++; }
    break;

  case 852: /* $@101: %empty  */
                                                                                                             { yyextra->das_arrow_depth --; }
    break;

  case 853: /* make_dim_decl: "array" "tuple" '<' $@100 type_declaration_no_options '>' $@101 '(' use_initializer optional_make_struct_dim_decl ')'  */
                                                                                                                                                                                                                {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-10]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = (yyvsp[-2].b);
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceTuple = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-10])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 854: /* $@102: %empty  */
                                         { yyextra->das_arrow_depth ++; }
    break;

  case 855: /* $@103: %empty  */
                                                                                                               { yyextra->das_arrow_depth --; }
    break;

  case 856: /* make_dim_decl: "array" "variant" '<' $@102 type_declaration_no_options '>' $@103 '(' make_variant_dim ')'  */
                                                                                                                                                                               {
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-9]));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->useInitializer = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->forceVariant = true;
        ((ExprMakeStruct *)(yyvsp[-1].pExpression))->alwaysUseInitializer = true;
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-9])),"to_array_move");
        tam->arguments.push_back((yyvsp[-1].pExpression));
        (yyval.pExpression) = tam;
    }
    break;

  case 857: /* make_dim_decl: "array" '(' expr_list optional_comma ')'  */
                                                                   {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-4])),"to_array_move");
        tam->arguments.push_back(mka);
        (yyval.pExpression) = tam;
    }
    break;

  case 858: /* $@104: %empty  */
                           { yyextra->das_arrow_depth ++; }
    break;

  case 859: /* $@105: %empty  */
                                                                                                 { yyextra->das_arrow_depth --; }
    break;

  case 860: /* make_dim_decl: "array" '<' $@104 type_declaration_no_options '>' $@105 '(' optional_expr_list ')'  */
                                                                                                                                                                        {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-8])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
            auto tam = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-8])),"to_array_move");
            tam->arguments.push_back(mka);
            (yyval.pExpression) = tam;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->makeType = make_smart<TypeDecl>(Type::tArray);
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-5].pTypeDecl));
            msd->at = tokAt(scanner,(yylsp[-5]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 861: /* make_dim_decl: "fixed_array" '(' expr_list optional_comma ')'  */
                                                                         {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-4])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 862: /* $@106: %empty  */
                                 { yyextra->das_arrow_depth ++; }
    break;

  case 863: /* $@107: %empty  */
                                                                                                       { yyextra->das_arrow_depth --; }
    break;

  case 864: /* make_dim_decl: "fixed_array" '<' $@106 type_declaration_no_options '>' $@107 '(' expr_list optional_comma ')'  */
                                                                                                                                                                                    {
        auto mka = new ExprMakeArray(tokAt(scanner,(yylsp[-9])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
        if ( !mka->makeType->dim.size() ) mka->makeType->dim.push_back(TypeDecl::dimAuto);
        (yyval.pExpression) = mka;
    }
    break;

  case 865: /* make_table: make_map_tuple  */
                            {
        auto mka = new ExprMakeArray();
        mka->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = mka;
    }
    break;

  case 866: /* make_table: make_table "end of expression" make_map_tuple  */
                                                {
        ((ExprMakeArray *) (yyvsp[-2].pExpression))->values.push_back(ExpressionPtr((yyvsp[0].pExpression)));
        (yyval.pExpression) = (yyvsp[-2].pExpression);
    }
    break;

  case 867: /* expr_map_tuple_list: make_map_tuple  */
                                {
        (yyval.pExpression) = (yyvsp[0].pExpression);
    }
    break;

  case 868: /* expr_map_tuple_list: expr_map_tuple_list ',' make_map_tuple  */
                                                                {
            (yyval.pExpression) = new ExprSequence(tokAt(scanner,(yylsp[-2])),ExpressionPtr((yyvsp[-2].pExpression)),ExpressionPtr((yyvsp[0].pExpression)));
    }
    break;

  case 869: /* make_table_decl: "begin of code block" expr_map_tuple_list optional_comma "end of code block"  */
                                                                    {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-2].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 870: /* make_table_decl: "{{" make_table optional_trailing_semicolon_cur_cur  */
                                                                          {
        auto mkt = make_smart<TypeDecl>(Type::autoinfer);
        mkt->dim.push_back(TypeDecl::dimAuto);
        ((ExprMakeArray *)(yyvsp[-1].pExpression))->makeType = mkt;
        (yyvsp[-1].pExpression)->at = tokAt(scanner,(yylsp[-2]));
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-2])),"to_table_move");
        ttm->arguments.push_back(ExpressionPtr((yyvsp[-1].pExpression)));
        (yyval.pExpression) = ttm;
    }
    break;

  case 871: /* make_table_decl: "table" '(' optional_expr_map_tuple_list ')'  */
                                                                       {
        auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-3])));
        mka->values = sequenceToList((yyvsp[-1].pExpression));
        mka->makeType = make_smart<TypeDecl>(Type::autoinfer);
        auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-3])),"to_table_move");
        ttm->arguments.push_back(mka);
        (yyval.pExpression) = ttm;
    }
    break;

  case 872: /* make_table_decl: "table" '<' type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
                                                                                                                 {
        if ( (yyvsp[-1].pExpression) ) {
            auto mka = make_smart<ExprMakeArray>(tokAt(scanner,(yylsp[-6])));
            mka->values = sequenceToList((yyvsp[-1].pExpression));
            mka->makeType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            auto ttm = yyextra->g_Program->makeCall(tokAt(scanner,(yylsp[-6])),"to_table_move");
            ttm->arguments.push_back(mka);
            (yyval.pExpression) = ttm;
        } else {
            auto msd = new ExprMakeStruct();
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->makeType = make_smart<TypeDecl>(Type::tTable);
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            msd->makeType->secondType = make_smart<TypeDecl>(Type::tVoid);
            msd->at = tokAt(scanner,(yylsp[-6]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 873: /* make_table_decl: "table" '<' type_declaration_no_options c_or_s type_declaration_no_options '>' '(' optional_expr_map_tuple_list ')'  */
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
            msd->makeType->firstType = TypeDeclPtr((yyvsp[-6].pTypeDecl));
            msd->makeType->secondType = TypeDeclPtr((yyvsp[-4].pTypeDecl));
            msd->at = tokAt(scanner,(yylsp[-8]));
            msd->useInitializer = true;
            msd->alwaysUseInitializer = true;
            (yyval.pExpression) = msd;
        }
    }
    break;

  case 874: /* array_comprehension_where: %empty  */
                                    { (yyval.pExpression) = nullptr; }
    break;

  case 875: /* array_comprehension_where: "end of expression" "where" expr  */
                                    { (yyval.pExpression) = (yyvsp[0].pExpression); }
    break;

  case 876: /* optional_comma: %empty  */
                { (yyval.b) = false; }
    break;

  case 877: /* optional_comma: ','  */
                { (yyval.b) = true; }
    break;

  case 878: /* array_comprehension: '[' "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                    {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,false);
    }
    break;

  case 879: /* array_comprehension: '[' "iterator" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']'  */
                                                                                                                                                                 {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),true,false);
    }
    break;

  case 880: /* array_comprehension: "[[" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where ']' ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),true,false);
    }
    break;

  case 881: /* array_comprehension: "[{" "for" variable_name_with_pos_list "in" expr_list "end of expression" expr array_comprehension_where "end of code block" ']'  */
                                                                                                                                                            {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-8])),(yyvsp[-7].pNameWithPosList),(yyvsp[-5].pExpression),(yyvsp[-3].pExpression),(yyvsp[-2].pExpression),tokRangeAt(scanner,(yylsp[-3]),(yylsp[0])),false,false);
    }
    break;

  case 882: /* array_comprehension: "begin of code block" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block"  */
                                                                                                                                                              {
        (yyval.pExpression) = ast_arrayComprehension(scanner,tokAt(scanner,(yylsp[-7])),(yyvsp[-6].pNameWithPosList),(yyvsp[-4].pExpression),(yyvsp[-2].pExpression),(yyvsp[-1].pExpression),tokRangeAt(scanner,(yylsp[-2]),(yylsp[0])),false,true);
    }
    break;

  case 883: /* array_comprehension: "{{" "for" variable_name_with_pos_list "in" expr_list "end of expression" make_map_tuple array_comprehension_where "end of code block" "end of code block"  */
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


